// Koen Vandeputte
// C++ Learning
// FIFO / RING buffer DLL
// MultiThreading supported

#if !defined(BUFFERS_H_INCLUDED)
#define BUFFERS_H_INCLUDED


#ifdef BUFFERS_EXPORTS
#define BUFFERS_API __declspec(dllexport)
#else
#define BUFFERS_API __declspec(dllimport)
#endif

#define CALLING_CONVENTION __stdcall
//#define CALLING_CONVENTION __cdecl
#ifdef __cplusplus
extern "C"{
#endif


BUFFERS_API unsigned int CALLING_CONVENTION BufferCreate (unsigned int size,unsigned int buffertype, unsigned short NrReadChannels);
	//Create a new Buffer
	// Parameters :
	//		- Size of the buffer in bytes (max 512MB)
	//		- Type of buffer : 0 = FIFO	; 1 = RING
	//		- Amount of ReadChannel wanted. (Max = 4;  Giving higher values will set it to the maximum)
	// Return : 
	//		- 0 : Unknown buffertype
	//		- >0 : Pointer to the buffer

// Additional info :
	// With a FIFO, when the buffer is full, writes are accepted, overwriting the oldest data first
	// With a RING, when the buffer is full (any of the ReadChannel), writes are denied, space must become available again first


BUFFERS_API void CALLING_CONVENTION BufferRelease(unsigned int buffer_nr);
	//Delete a Buffer
	//It is the users responsability to call this function for each buffer ID in order to clean up the memory !!
	// Parameters :
	//		- Pointer to the buffer

BUFFERS_API int CALLING_CONVENTION BufferRead (unsigned int buffer_nr, unsigned short ReadChannel, unsigned int NrOfBytes, char * Data);
	//Read from a buffer
	// Parameters :
	//		- Pointer to the buffer
	//		- Nr of bytes you wish to read
	//		- Pointer to the user's array (0-term not needed)
	// Return : status
	//		 0 : OK
	//		-1 : "Invalid ReadPointer"
	//		-2 : "Minimum of 1 byte must be requested"
	//		-3 : "Buffer is empty"
	//		-4 : "Numbers of requested bytes is higher than the number of available bytes"

BUFFERS_API int CALLING_CONVENTION BufferWrite (unsigned int buffer_nr, unsigned int NrOfBytes, char * Data);
	//Write to a buffer
	// Parameters :
	//		- Pointer to the buffer
	//		- Nr of bytes you wish to write
	//		- Pointer to the user's array containing the data (0-term not needed)
	// Return : status
	//		 0 : OK
	//		-1 : "Minimum of 1 byte must be requested"
	//		-2 : "RING only : Buffer is full"
	//		-3 : "RING only : Not enough  space in the buffer to write the number of requested bytes (No bytes were written)

BUFFERS_API void CALLING_CONVENTION BufferFlush (unsigned int buffer_nr, short ReadChannel);
	//Clear the buffer
	// Parameters :
	//		- Pointer to the buffer

BUFFERS_API unsigned int CALLING_CONVENTION BufferSpaceAvailable (unsigned int buffer_nr, unsigned short ReadChannel);
	//Request amount of Space available
	// Parameters :
	//		- Pointer to the buffer
	// Return : status
	//		- -1	: Invalid ReadChannel
	//		- FIFO	: Available space before data will be overwritten
	//		- RING	: Available space before write actions will be blocked

BUFFERS_API double CALLING_CONVENTION BufferSpaceAvailable_Percentage (unsigned int buffer_nr, unsigned short ReadChannel);
	//Request amount of Space available (in Percentage)
	// Parameters :
	//		- Pointer to the buffer
	// Return : status
	//		- Amount of used space (in Percentage)

BUFFERS_API unsigned int CALLING_CONVENTION BufferSpaceUsed (unsigned int buffer_nr, unsigned short ReadChannel);
	//Request amount of Space used
	// Parameters :
	//		- Pointer to the buffer
	// Return : status
	//		- Amount of used space (in bytes)

BUFFERS_API double CALLING_CONVENTION BufferSpaceUsed_Percentage (unsigned int buffer_nr, unsigned short ReadChannel);
	//Request amount of Space used
	// Parameters :
	//		- Pointer to the buffer
	// Return : status
	//		- Amount of used space (in bytes)

#ifdef __cplusplus
}
#endif


#endif