void GetCPUInfo ();

void GetCpuBrand(char* CPUbrand);

int CheckCPUFeature(unsigned int Feature);

// Return
//	1 : Feature Available
//	0 : Feature Not Available
//	-1 : Invalid Feature Request

// Feature Options
//	 0 : MMX
//	 1 : SSE
//	 2 : SSE2
//	 3 : SSE3
//	 4 : SSE4.1
//	 5 : SSE4.2
//	 6 : AVX
//	 7 : AES
//	 8 : 64-Bit
//	 9 : SpeedStep
//	 10 : HyperThreading
//	 11 : Amount of logical CPU's
//	 12 : Amount of Physical CPU's
