Kindly rate this project to help me further improve it !
Feel free to post a ticket in case of a bug or feature request :)


0.7.3
-----
	- Improved : Queue access functions dynamically select the fastest possible method for any situation now.
	- Improved : Fully optimized cache performance.
	- Improved : Read/Write performance fully optimized on assembly level. (See benchmarks for details)
	- Improved : Vectorized Load & Pointer handling, improving queue performance when using High Readpointer count.
	- Improved : ReWritten CPU info functions in Assembly now for extremely fast Queue creation & control
	- Improved : Seriously improved pointer aliasing, false sharing & boundary alignments for optimal caching & SSE2 performance.
	- Improved : Read/Writes smaller than 4kB are now handled by Agner Fog's "Asmlib" memcpy functions.
		     (Special Thanks to Agner Fog for the amazing optimization manuals & libs !! - please visit www.agner.org)
	- Updated : All benchmarks
	- Added : Comparison Benchmarks based on Queue sizes fitting in RAM, L2 & L1 cache.

0.7.2
-----
	- Critical Fix : Queue data could get corrupt after lots of reading/writing.
	- Improved : ~13% speed gain on small writes when using a lot of channels.
	- Improved : ~15% speed gain when writing data to a full FIFO queue.
	- Improved : ~60% speed gain when Flushing all ReadChannels at once (using "-1" in the command)
	- Changed : BufferGetOverflow_Wait(BufferNr) returns an Unsigned Int now i.s.o. a Bool. (compatibility with ANSI C)
	- Updated : Write Benchmarks & Code Example

0.7.1
-----
	- Fix : BufferRead() could never return "-3" (Queue is Empty), even if it was empty.
	- Improved : Updated max nr of Read Channels from 32 to 128.
		     keep in mind that every additional Read Channel costs a little bit extra CPU power.
		     See benchmarks for details.
		     It's advised to not make more ReadChannels than you need, in order to have max performance.

0.7.0
-----
	- Changed : Renamed project files from "Buffers.xxx" to "UltraQueue.xxx" :)
	- Fix : BufferCreate() could cause a mem. access violation in very specific low memory environments.
	- Added : BufferCreate() could return 0 for multiple reasons.
		  new param. (*Status) was added for error handling. (See "UltraQueue.h" for details)
	- Improved : Finetuned highly threaded access with small blocksizes (~280% speed improvement)
	- Improved : Improved Threaded performance for HyperThreading-enabled CPU's
	- Improved : CPU load decreased in high-threaded situations (~50%)
	- Improved : Queue access latency lowered by ~35% in specific high threaded situations.
	- Improved : Slightly optimized clean-up code on queue creating errors.
	- Changed : Code Example file has been updated with new parameter in BufferCreate().

0.6.9
-----
	- Critical Fix : DLL crashed when not enough memory could be allocated.
	- Improved : Faster treaded access speed for Single Core machines
	- Improved : Speed increased ~20% when R/W small blocksizes (see benchmarks)

0.6.8
-----
	- Added : Doc. -> Benchmarks
		  Helps the programmer to get the max. speed using UltraQueue
		  See "Benchmarks" folder

0.6.7
-----
	- Added : OverFlow/Wait flag functions (See "Code Example" for details)

0.6.6
-----
	- Added : 1 Application Example
		* Multi Data Copy/Processing
0.6.5
-----
	- Added : 2 Application Examples
		* RS232 Driver
		* High Speed Capturing

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