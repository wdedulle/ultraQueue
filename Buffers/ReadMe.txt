0.6.4
-----
	- Added : BufferFlush() now accepts "-1" as ChannelNr, clearing ALL queues
	- Fix : ChannelNr parameters was not present in the Header doc.
	- Example file has been updated with BufferFlush()

0.6.3
-----
	- Added : Dynamic Block Writing (RING)

0.6.2
-----
	- Added : Defines in Header (FIFO / RING)
	- Fix : Corrected an error in the header explanation
	- Fix : A minimum of 1 ReadChannel is now requested when creating a queue

0.6.1
-----
	- Added : Example file

0.6
---
	- Added : Dynamic Block Writing (FIFO only atm)
	- FIFO write speeds improved by 3968%
	- MultiRead supports up to 32 parallel Read members now
	- Each additional MultiRead copy requires 5% less cycles

0.5
---
	- Added : Dynamic Block Reading
	- Read speed improved by 1551%
	- Every additional MultiRead copy requires 60% less cycles

0.4
---
	- Improved caching methods.
	- Write speed improved with 205%
	- Read speed improved with 213%

0.3
---
	- Write speed improved with 93%
	- Read speed improved with 91%

0.2
---
	- Added : MultiRead 16

0.1
---
	Inital version


------

Multiread "x" : Up to x clients have their own copy of a queue, while only using the memory space of 1.

Dynamic Block Read/Write : Fastest possible execution method is chosen automatically for any nr of bytes. (In Blocks or Byte-by-Byte)