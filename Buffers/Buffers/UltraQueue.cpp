// Buffers.cpp : Defines the exported functions for the DLL application.
//

// Koen Vandeputte
// C++ Learning
// FIFO / RING buffer DLL
// MultiThreading supported

#include "stdafx.h"
#include "UltraQueue.h"

#define MAXSIZE 512	// In MegaBytes
#define MINSIZE 16	// In Bytes

#define MAXPTRS 32

#define BLOCKSIZE_READ 8	//In Bytes (values >= BLOCKSIZE_READ will be written byte/Byte
#define BLOCKSIZE_WRITE 8	//In Bytes (values >= BLOCKSIZE_WRITE will be written byte/Byte

#define FIFO 0	// With a FIFO, when the buffer is full, writing are accepted, overwriting the oldest data first
#define RING 1	// With a RING, when the buffer is full, writes are denied, space must become available again first

class Buffers
{
public:
	Buffers(unsigned int _NrPtrs, unsigned int _size, unsigned int _BufferType);
	virtual ~Buffers();
	unsigned int QFree(unsigned int NrPtr);
	unsigned int QLoaded(unsigned int NrPtr);
	void Flush(int NrPtr);
	int Read (unsigned int NrPtr, unsigned int r, unsigned char * d);
	int Write (unsigned int w, unsigned char * d);
	unsigned int GetBuffertype();
	unsigned int CQsize();
	bool StatusGetOverFlow();
	void StatusClearOverFlow();
	bool QueueError;

private:
	int CreateQueue(unsigned int size);
	unsigned char * CQ;
	unsigned int QLoad[MAXPTRS];
	unsigned int size;
	unsigned int RdPtr[MAXPTRS];
	unsigned int WrPtr;
	unsigned int BufferType;
	unsigned int NrPtrs;
	volatile bool OverFlow;
	CRITICAL_SECTION Crit1;
};

//Constructor
Buffers::Buffers(unsigned int _NrPtrs, unsigned int _size, unsigned int _BufferType)
{
	this->QueueError = false;
	this->CreateQueue(_size);
	this->size = _size;
	this->WrPtr = 0;
	this->BufferType = _BufferType;
	this->NrPtrs = _NrPtrs;
	this->OverFlow = false;
	
	for (unsigned int i=0;i<_NrPtrs;i++)
		{
			this->RdPtr[i] = 0;
			this->QLoad[i] = 0;
		}

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	if (sysinfo.dwNumberOfProcessors > 1)
	{	 // MultiCore
		InitializeCriticalSectionAndSpinCount(&this->Crit1, 0x0000005F);	// 95 Spin is added (benchmarked using Vtune) so a thread does not use an expensive Sleep while the 
	}																// chance is high it will be able to continue within a few cycles
	else															// This improves the speed dramatically on very small Threaded Reads or Writes (<64 Bytes)
	{	// SingleCore
		InitializeCriticalSectionAndSpinCount(&this->Crit1, 0x00000000);
	}
}

//Destructor
Buffers::~Buffers()
{
	DeleteCriticalSection(&Crit1);
	if (this->CQ) delete []this->CQ;
	this->CQ = nullptr;
}

//Class Functions
unsigned int Buffers::QFree(unsigned int NrPtr)
{
	unsigned int k=0;

	if (NrPtr >= this->NrPtrs) return 0;	// Invalid pointer

	EnterCriticalSection(&Crit1);
	k= (this->size - this->QLoad[NrPtr]); // k is needed to give the function a chance to release the lock first before returning
	LeaveCriticalSection(&Crit1);
	
	return k;
}

unsigned int Buffers::QLoaded(unsigned int NrPtr)
{
	unsigned int k=0;

	if (NrPtr >= this->NrPtrs) return 0;	// Invalid pointer

	EnterCriticalSection(&Crit1);
	k = this->QLoad[NrPtr];	
	LeaveCriticalSection(&Crit1);
	
	return k;
}

void Buffers::Flush(int NrPtr)
{
	if (NrPtr >= (int)this->NrPtrs) return;	// Invalid pointer

	EnterCriticalSection(&Crit1);
	if (NrPtr == -1)	// If -1 is received, reset all pointers
		{
			for (unsigned int i=0;i<this->NrPtrs;i++)
			{
				this->RdPtr[i] = this->WrPtr;
				this->QLoad[i] = 0;
			}
		}
	else
		{
			this->RdPtr[NrPtr]=this->WrPtr;
			this->QLoad[NrPtr] = 0;
		}
		
	LeaveCriticalSection(&Crit1);
}

int Buffers::CreateQueue(unsigned int size)
{
	try
	{
		this->CQ = new unsigned char[size];
		if (!this->CQ) throw -1;
	}

	catch (...)
	{
		this->CQ = nullptr;
		this->QueueError = true;
		return -1;
	}

	return 0;
}

int Buffers::Read (unsigned int NrPtr, unsigned int r, unsigned char * d)
{
	if (NrPtr >= this->NrPtrs) return -1;	// Invalid pointer

	if (!r) return -2;	// A minimum of 1 byte should be requested

	EnterCriticalSection(&Crit1);

	if (!this->QLoad)
		{
			LeaveCriticalSection(&Crit1);
			return -3;	//Queue is empty
		}
	if (r > (this->QLoad[NrPtr]))
		{
			LeaveCriticalSection(&Crit1);
			return -4;	//Requested amount of bytes is higher than nr of available bytes
		}
	
	//copy class vars to locals for caching reasons (96% Speed increase)
	unsigned int temp_size = this->size;
	unsigned int temp_RdPtr = this->RdPtr[NrPtr];
	unsigned int temp_QLoad = this->QLoad[NrPtr];

	unsigned int block;	//Space to Wrap point
	unsigned int offset=0;	//Unfinished block indicator + starting point indicator

	//read from buff
	while(r)	// Loop until all bytes have been read
	{
		block = temp_size - (temp_RdPtr+1);	// Get distance to Wrap point

		if (r <= BLOCKSIZE_READ || block <= BLOCKSIZE_READ)	// Byte-by-Byte Read
		{	// When not much data to write, or we're close at the wrap point, write byte-per-byte
			unsigned int Small = r;

			if (Small > BLOCKSIZE_READ && (Small-block) > BLOCKSIZE_READ) Small = block+1;

			for (register unsigned int i=0;i<Small;i++) // Store counter in Cache if possible
			{
				temp_RdPtr++;
				if (temp_RdPtr >= temp_size) temp_RdPtr = 0;	// Make it circular
				d[i+offset] = this->CQ[temp_RdPtr];
				temp_QLoad--;
				r--;
			}
		}
		else	// Block Read		Max block read = size of requested bytes (if possible)
		{
			if (block > r) block = r;	// If more sequential space is available than requested amount, write amount left
			memcpy(d,&this->CQ[temp_RdPtr+1],block);
			temp_RdPtr += block;
			temp_QLoad -=block;
			r -= block;	// Decrease nr of BytesToRead
			offset = block;	// offset = position to start for the Wrap write
		}
	}

	//Save changes
	this->RdPtr[NrPtr] = temp_RdPtr;
	this->QLoad[NrPtr] = temp_QLoad;

	LeaveCriticalSection(&Crit1);

	return 0;
}

int Buffers::Write (unsigned int w, unsigned char * d)
{
	if (!w) return -1;	// A minimum of 1 byte should be requested
	EnterCriticalSection(&Crit1);
	
	//Copy class vars to local vars for caching purposes (~ +95% speed increase)
	unsigned int temp_NrPtrs = this->NrPtrs;
	unsigned int temp_size = this->size;
	unsigned int temp_WrPtr = this->WrPtr;
	unsigned int temp_QLoad[MAXPTRS];
	unsigned int temp_RdPtr[MAXPTRS];

	for (unsigned int b=0;b<this->NrPtrs;b++)
	{
		temp_QLoad[b] = this->QLoad[b];
		temp_RdPtr[b] = this->RdPtr[b];
	}

	//Buffer write actions

	if (this->BufferType == RING)	//ringbuffer
	{
		for (unsigned int i=0;i<temp_NrPtrs;i++)	//check space requirements
		{
			if(temp_QLoad[i] >= temp_size)
				{
					this->OverFlow = true;	// Set flag to inform that a write action could not be performed
					LeaveCriticalSection(&Crit1);
					return -2;	// One of the Ring buffers is completely full
				}
			if (w > (temp_size - temp_QLoad[i]))
				{
					this->OverFlow = true;	// Set flag to inform that a write action could not be performed
					LeaveCriticalSection(&Crit1);
					return -3;	// One of the Ring buffers lacks space to write all requested bytes
				}
		}

		unsigned int block;
		unsigned int offset=0;
		unsigned int w2 = w;	// Keep a copy of written bytes to update Read pointer(s) & Load(s)

		while (w)
		{
			block = temp_size - (temp_WrPtr+1);	// Get distance to Wrap point

			if (w <= BLOCKSIZE_WRITE || block <= BLOCKSIZE_WRITE)	// Byte-by-Byte Read
			{	// When not much data to write, or we're close at the wrap point, write byte-per-byte
				unsigned int Small = w;

				if (Small > BLOCKSIZE_READ && (Small-block) > BLOCKSIZE_WRITE) Small = block+1;

				for (register unsigned int i=0;i<Small;i++)	// Store counter in Cache if possible
				{
					temp_WrPtr++;
					if (temp_WrPtr == temp_size) temp_WrPtr = 0;	// inc + compare&set is 40% faster than modulus trick
					this->CQ[temp_WrPtr] = d[i+offset];
					w--;
				}
			}
			else	// Block Write		Max block write = size of requested bytes (if possible)
			{
				if (block > w) block = w;	// If more sequential space is available than requested amount, write requested amount (otherwise we have a memory (& pointer) overflow
				memcpy(&this->CQ[temp_WrPtr+1],d,block);
				temp_WrPtr += block;
				w -= block;	// Decrease nr of BytesToWrite
				offset = block;	// offset = position to start for the Wrap write
			}
		}
		//Update Loads
		for (unsigned int q=0;q<temp_NrPtrs;q++)
		{
			temp_QLoad[q] += (w2-w);	//Update loads
		}

	}

	else	//FIFO

	{
		unsigned int block;
		unsigned int offset=0;
		unsigned int w2 = w;	// Keep a copy of written bytes to update Read pointer(s) & Load(s)

			while (w)
			{
				block = temp_size - (temp_WrPtr+1);	// Get distance to Wrap point

				if (w <= BLOCKSIZE_WRITE || block <= BLOCKSIZE_WRITE)	// Byte-by-Byte Read
				{	// When not much data to write, or we're close at the wrap point, write byte-per-byte
					unsigned int Small = w;

					if (Small > BLOCKSIZE_READ && (Small-block) > BLOCKSIZE_WRITE) Small = block+1;

					for (register unsigned int i=0;i<Small;i++)	// Store counter in Cache if possible
					{
						temp_WrPtr++;
						if (temp_WrPtr == temp_size) temp_WrPtr = 0;	// inc + compare&set is 40% faster than modulus trick
						this->CQ[temp_WrPtr] = d[i+offset];
						w--;
					}
				}
				else	// Block Write		Max block write = size of requested bytes (if possible)
				{
					if (block > w) block = w;	// If more sequential space is available than requested amount, write requested amount (otherwise we have a memory (& pointer) overflow
					memcpy(&this->CQ[temp_WrPtr+1],d,block);
					temp_WrPtr += block;
					w -= block;	// Decrease nr of BytesToWrite
					offset = block;	// offset = position to start for the Wrap write
				}
			}
			//Update Loads & pointers
			for (unsigned int q=0;q<temp_NrPtrs;q++)
			{
				temp_QLoad[q] += (w2-w);	//Update loads

				if (temp_QLoad[q] >= temp_size)	// Load is larger than queue size
				{
					temp_QLoad[q] = temp_size; // Max. possible amount is queue size
					temp_RdPtr[q] = temp_WrPtr; // Pointers are equal when buffer is full
					this->OverFlow = true;	// Set flag to indicate that we lost data
				}
				else	// Load is within Queue limits
				{
					temp_RdPtr[q] = temp_WrPtr - temp_QLoad[q];
					if (temp_RdPtr[q] > temp_size) temp_RdPtr[q] = temp_size - (temp_size - temp_RdPtr[q]);
				}
			}
	}

	// Write back changed value's
	this->WrPtr = temp_WrPtr;	
	for (unsigned int c=0;c<this->NrPtrs;c++)
	{
		this->QLoad[c] = temp_QLoad[c];
		this->RdPtr[c] = temp_RdPtr[c];
	}

	// Exit
	LeaveCriticalSection(&Crit1);
	return 0;
}

unsigned int Buffers::GetBuffertype()
{
	return this->BufferType;
}

unsigned int Buffers::CQsize()
{
	return this->size;
}

bool Buffers::StatusGetOverFlow()
{
	bool a = false;
	
	EnterCriticalSection(&Crit1);
		a = this->OverFlow;
	LeaveCriticalSection(&Crit1);

	return a;
}

void Buffers::StatusClearOverFlow()
{
	EnterCriticalSection(&Crit1);
		this->OverFlow = false;
	LeaveCriticalSection(&Crit1);
}

//*********************
//*End Class Functions*
//*********************

unsigned int CALLING_CONVENTION BufferCreate (unsigned int size, unsigned int buffertype, unsigned int NrReadChannels, int * Status)	//Unsigned int is used for BufferType for future-proof reasons
{
	unsigned int i=0;
	*Status = 0;
	
	if (NrReadChannels > MAXPTRS) NrReadChannels = MAXPTRS;	// Check (and set) maximum allowed nr of Read Pointers
	if (!NrReadChannels) {*Status = -1 ;return 0;}	// ReadChannels must be at least 1

	if (size > 1048576*MAXSIZE) size = 1048576*MAXSIZE;	// Buffer Maximum Capacity
	if (size < MINSIZE) size = MINSIZE;	// Buffer Minimum Capacity

	if (buffertype > 1) {*Status = -2 ;return 0;}	// Unknown Buffertype
	
	Buffers * NewBuffer;

	try
	{
		NewBuffer = new Buffers(NrReadChannels, size, buffertype);
		if (!NewBuffer) throw -1;

		if (NewBuffer->QueueError)
			{
				delete NewBuffer;
				throw -1;
			}
	}
	catch (...)
	{
		NewBuffer = nullptr;
		*Status = -3;	// Out of memory
	}
	
	i = (unsigned int) NewBuffer;
	return i;
}

void CALLING_CONVENTION BufferRelease (unsigned int buffer_nr)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	delete incoming;
	incoming = nullptr;
	return;
}

int CALLING_CONVENTION BufferRead (unsigned int buffer_nr, unsigned int ReadChannel, unsigned int NrOfBytes, unsigned char * Data)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	return incoming->Read(ReadChannel, NrOfBytes,Data);
}

int CALLING_CONVENTION BufferWrite (unsigned int buffer_nr, unsigned int NrOfBytes, unsigned char * Data)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	return incoming->Write(NrOfBytes,Data);
}

BUFFERS_API void CALLING_CONVENTION BufferFlush (unsigned int buffer_nr, int ReadChannel)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	incoming->Flush(ReadChannel);
	return;
}

unsigned int CALLING_CONVENTION BufferSpaceAvailable (unsigned int buffer_nr, unsigned int ReadChannel)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	return incoming->QFree(ReadChannel);
}

double CALLING_CONVENTION BufferSpaceAvailable_Percentage (unsigned int buffer_nr, unsigned int ReadChannel)
{
	unsigned int a =0;
	double k =0;
	Buffers * incoming = (Buffers*) buffer_nr;
	a = incoming->QFree(ReadChannel);
	
	if (a < 0) return a;
	
	k = ((double)a / (double)incoming->CQsize())*100;

	return  k;
}

unsigned int CALLING_CONVENTION BufferSpaceUsed (unsigned int buffer_nr, unsigned int ReadChannel)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	return incoming->QLoaded(ReadChannel);
}

double CALLING_CONVENTION BufferSpaceUsed_Percentage (unsigned int buffer_nr, unsigned int ReadChannel)
{
	double k =0;
	Buffers * incoming = (Buffers*) buffer_nr;
	k = ((double)incoming->QLoaded(ReadChannel) / (double)incoming->CQsize())*100;
	return  k;
}

bool CALLING_CONVENTION BufferGetOverflow_Wait (unsigned int buffer_nr)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	return incoming->StatusGetOverFlow();
}

void CALLING_CONVENTION BufferClearOverflow_Wait (unsigned int buffer_nr)
{
	Buffers * incoming = (Buffers*) buffer_nr;
	incoming->StatusClearOverFlow();
}