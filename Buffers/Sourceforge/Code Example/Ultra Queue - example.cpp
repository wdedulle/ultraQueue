Ultraqueue 0.6 Example :

#include "Buffers.h"

int main ()
{

	unsigned int Buffer1 = 0;
	unsigned int Buffer2 = 0;

	int status=0;

	unsigned char TempBuffer[2048];
	unsigned char Small[128] = {0};
	unsigned int BytesAvailable =0;

	double UsedSpace = 0;
	bool Flag = false;

	//Creating Buffers
	Buffer1 = BufferCreate (1*1048576,FIFO, 2);	// Create a queue :  1MB in size , FIFO , 2 ReadChannels (Channels : 0 & 1)
	Buffer2 = BufferCreate (128,RING, 1);		// Create a queue :  128 Bytes in size , RING , 1 ReadChannels (Channel : 0)


	If (Buffer1 == 0) PrintError();	// When succeeded, "Buffer1" will be > 0
	If (Buffer2 == 0) PrintError();	// When succeeded, "Buffer2" will be > 0


	//Writing to buffers
	status = BufferWrite (Buffer1, 64, TempBuffer);	// Write First 64 bytes from TempBuffer to Buffer1
	if (status < 0) PrintError(status);	// If a negative value was returned, display it

	status = BufferWrite (Buffer2, 129, TempBuffer);	// Write First 129 bytes from TempBuffer to Buffer2
	if (status < 0) PrintError(status);	// In this example, return value will be -4 : "Number of requested bytes is higher than the number of available bytes"

	
	//Check OverFlow/Wait
	//These functions are usefull to indicate a user that : {FIFO : data was lost}	or	{RING : Data could not be written}
	Flag = BufferGetOverflow_Wait(Buffer1);	//FIFO queue
	if (Flag) PrintError(Flag);	// Data was lost for 1 of the readchannels, at it was full.	("False" in this example)
	
	Flag = BufferGetOverflow_Wait(Buffer2);	// RING queue
	if (Flag) PrintError(Flag);	// Write action failed, 1 of the readchannels was full or lacked space to write all requested data.	("True" in this example)

	BufferClearOverflow_Wait(Buffer2);	// Reset the flag back to "false"
	
	
	//Check capacity
	BytesAvailable = BufferSpaceUsed (Buffer1, 0);	// Bytes available in Buffer 1 - Channel 0	(64 in this example)
	BytesAvailable = BufferSpaceUsed (Buffer1, 1);	// Bytes available in Buffer 1 - Channel 1	(64 in this example)


	//Reading from buffers
	status = BufferRead (Buffer1, 0, 16, Small);	// Read 16 bytes from Buffer1 - Channel 0  into Small (starting from [0])


	// Each readchannel has it's own copy of the buffer, while only using the space of 1 (only 1MB of memory will be used in total in this example)
	BytesAvailable = BufferSpaceUsed (Buffer1, 0);	// Bytes available in Buffer 1 - Channel 0	("48" in this example)
	BytesAvailable = BufferSpaceUsed (Buffer1, 1);	// Bytes available in Buffer 1 - Channel 1	("64" in this example)

	
	// Check capacity in % (usefull for displaying Buffer status in your GUI using an indicator)
	UsedSpace = BufferSpaceUsed_Percentage(Buffer1, 0);	// read amount of occupied space in %	(0,0045% in this example)
	UsedSpace = BufferSpaceUsed_Percentage(Buffer1, 1);	// read amount of occupied space in %	(0,0061% in this example)

	
	//Clear Buffers
	BufferFlush(Buffer1,-1);	//Clear Buffer 1 : All Channels
	BufferFlush(Buffer2,0);		//Clear Buffer 2 : Channel 0
	
	//note: It is not needed to clear buffers before Deleting them


	// Delete Buffers before closing the application
	BufferRelease(Buffer1);	// Delete Buffer1
	BufferRelease(Buffer2); // Delete Buffer2


Return 0;
}