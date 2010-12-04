// Buffers.cpp : Defines the exported functions for the DLL application.
//

// Koen Vandeputte
// C++ Learning
// FIFO / RING buffer DLL
// MultiThreading supported


#include "stdafx.h"
#include "Buffers.h"

#define MAXSIZE 256	// In MegaBytes
#define MINSIZE 16	// In Bytes

#define MAXPTRS 4

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

private:
	int CreateQueue(unsigned int size);
	unsigned char * CQ;
	unsigned int QLoad[MAXPTRS];
	unsigned int size;
	unsigned int RdPtr[MAXPTRS];
	unsigned int WrPtr;
	unsigned int BufferType;
	unsigned int NrPtrs;
	CRITICAL_SECTION Crit1;
};

//Constructor
Buffers::Buffers(unsigned int _NrPtrs, unsigned int _size, unsigned int _BufferType)
{
	this->CreateQueue(_size);
	this->size = _size;
	this->WrPtr = 0;
	this->BufferType = _BufferType;
	this->NrPtrs = _NrPtrs;
	
	for (unsigned int i=0;i<_NrPtrs;i++)
		{
			this->RdPtr[i] = 0;
			this->QLoad[i] = 0;
		}
	InitializeCriticalSectionAndSpinCount(&Crit1, 0x0001FFFF);		// 128k Spin is added so a thread does not use an expensive Sleep while the 
																	// chance is high it will be able to continue within a few cycles
																	// This improves the speed dramatically on very small Reads or Writes
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

	if (NrPtr >= this->NrPtrs) return -1;	// Invalid pointer

	EnterCriticalSection(&Crit1);
	k= (this->size - this->QLoad[NrPtr]); // k is needed to give the function a chance to release the lock first before returning
	LeaveCriticalSection(&Crit1);
	
	return k;
}

unsigned int Buffers::QLoaded(unsigned int NrPtr)
{
	unsigned int k=0;

	if (NrPtr >= this->NrPtrs) return -1;	// Invalid pointer

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
				this->RdPtr[i]=this->WrPtr;
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
	}

	catch (...)
	{
		return -1;
	}

	if (!this->CQ) return -1;	// Error while creating the buffer

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

		for (register unsigned int i=0;i<r;i++) // Store counter in Cache if possible
			{

				this->RdPtr[NrPtr]++;
				if (this->RdPtr[NrPtr] == this->size) this->RdPtr[NrPtr] = 0;	// Make it circular
				d[i] = this->CQ[this->RdPtr[NrPtr]];
				this->QLoad[NrPtr]--;

			}

	LeaveCriticalSection(&Crit1);

	return 0;
}

int Buffers::Write (unsigned int w, unsigned char * d)
{
	
	if (!w) return -1;	// A minimum of 1 byte should be requested
	EnterCriticalSection(&Crit1);
	
	if (this->BufferType == RING)	//ringbuffer
	{
		for (unsigned int i=0;i<this->NrPtrs;i++)	//check space requirements
		{
			if(this->QLoad[i] >= this->size)
				{
					LeaveCriticalSection(&Crit1);
					return -2;	// One of the Ring buffers is completely full
				}
			if (w > (this->size - this->QLoad[i]))
				{
					LeaveCriticalSection(&Crit1);
					return -3;	// One of the Ring buffers lacks space to write all requested bytes
				}
		}

		for (register unsigned int i=0;i<w;i++) // Store counter in Cache if possible
		{
			this->WrPtr++;
			if (this->WrPtr == this->size) this->WrPtr = 0;	//Make it circular
			this->CQ[this->WrPtr] = d[i];

			for (unsigned int j=0;j<this->NrPtrs;j++)
			{
				this->QLoad[j]++;
			}
		}
	}

	else	//FIFO
	
	{
		for (register unsigned int i=0;i<w;i++)	// Store counter in Cache if possible
		{
			this->WrPtr++;
			if (this->WrPtr == this->size) this->WrPtr = 0;	// inc + compare&set is 40% faster than modulus trick
			
			this->CQ[this->WrPtr] = d[i];

			for (register unsigned int j=0;j<this->NrPtrs;j++)
			{
				if (this->size - this->QLoad[j]) this->QLoad[j]++;	// Increment buffer usage as long as it's not full
				else 
					{
						this->RdPtr[j]++;	// If FIFO is full, also move the Read Pointer
						if (this->RdPtr[j] == this->size) this->RdPtr[j] = 0;	//Make it circular
					}
			}
		}
	}
	
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

//*********************
//*End Class Functions*
//*********************

unsigned int CALLING_CONVENTION BufferCreate (unsigned int size, unsigned int buffertype, unsigned int NrReadChannels)	//Unsigned int is used for BufferType for future-proof reasons
{
	unsigned int i=0;
	
	if (NrReadChannels > MAXPTRS) NrReadChannels = MAXPTRS;	// Check (and set) maximum allowed nr of Read Pointers

	if (size > 1048576*MAXSIZE) size = 1048576*MAXSIZE;	// Buffer Maximum Capacity
	if (size < MINSIZE) size = MINSIZE;	// Buffer Minimum Capacity

	if (buffertype > 1) return 0;	// Unknown Buffertype
	Buffers * NewBuffer = new Buffers(NrReadChannels, size, buffertype);
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


