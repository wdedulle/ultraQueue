#include "stdafx.h"

static bool CPU_Initialized = false;

static char CPUBrand[13] = {0};

static bool MMX = false;
static bool SSE = false;
static bool SSE2 = false;
static bool SSE3 = false;
static bool SSE41 = false;
static bool SSE42 = false;
static bool AVX = false;
static bool AES = false;
static bool _64_BIT = false;
static bool SpeedStep = false;
static bool HyperThreading = false;
static int AmountLogicalCPU = 0;
static int AmountPhysicalCPU = 0;

void GetCPUInfo ()
{
	int Flags[4];

	//Flags[0] = EAX
	//Flags[1] = EBX
	//Flags[2] = ECX
	//Flags[3] = EDX

	CPU_Initialized = false;

	//Get CPU Name
	__asm
	{
		XOR ECX,ECX;	//ECX must be 0
		MOV EAX,0;
		CPUID;
		MOV	Flags[0], EAX
		MOV	Flags[4], EBX
		MOV	Flags[8], ECX
		MOV	Flags[12], EDX
	}

	memcpy(&CPUBrand[0],&Flags[1],4);
	memcpy(&CPUBrand[4],&Flags[3],4);
	memcpy(&CPUBrand[8],&Flags[2],4);

	//Get CPU Features
	__asm
	{
		XOR ECX,ECX;
		MOV EAX,1;
		CPUID;
		MOV	Flags[0], EAX
		MOV	Flags[4], EBX
		MOV	Flags[8], ECX
		MOV	Flags[12], EDX
	}

	//HyperThreading (EDX - bit 28)
	if ((Flags[3] & 268435456)) HyperThreading = true;
	else HyperThreading = false;

	//MMX (EDX - bit 23)
	if (Flags[3] & 8388608) MMX = true;
	else MMX = false;

	//SSE (EDX - bit 25)
	if (Flags[3] & 33554432) SSE = true;
	else SSE = false;

	//SSE2 (EDX - bits 19 & 26)
	if ((Flags[3] & 67633152) == 67633152) SSE2 = true;
	else SSE2 = false;

	//SSE3 (ECX - bits 0 & 3 & 9)
	if ((Flags[2] & 521) == 521) SSE3 = true;
	else SSE3 = false;

	//SSE4.1 (ECX - bit 19)
	if (Flags[2] & 524288) SSE41 = true;
	else SSE41 = false;

	//SSE4.2 (ECX - bit 20)
	if (Flags[2] & 1048576) SSE42 = true;
	else SSE42 = false;

	//AES (ECX - bit 25)
	if (Flags[2] & 33554432) AES = true;
	else AES = false;

	//AVX (ECX - bit 28)
	if (Flags[2] & 268435456) AVX = true;
	else AVX = false;

	//_64_BIT (EDX - bit 30)
	if (Flags[3] & 1073741824) _64_BIT = false;
	else _64_BIT = true;

	//SpeedStep (ECX - bit 7)
	if (Flags[2] & 128) SpeedStep = true;
	else SpeedStep = false;

	//Nr Of CPU's
	AmountLogicalCPU = Flags[1];
	AmountLogicalCPU &= 16711680;	//Only save bits [23:16]
	AmountLogicalCPU >>= 17;

	if (!strcmp(CPUBrand,"GenuineIntel"))
	{
		__asm
		{
			XOR ECX,ECX;
			MOV EAX,4;
			CPUID;
			AND EAX, 0xFC000000;
			SHR EAX, 27;
			INC EAX;
			MOV AmountPhysicalCPU,EAX;
		}
	}
	else if (!strcmp(CPUBrand,"AuthenticAMD"))
	{
		__asm
		{
			XOR ECX,ECX;
			MOV EAX,0x80000008;
			CPUID;
			AND ECX,0xFF;
			MOV	AmountPhysicalCPU,ECX;
		}
	}
	
	CPU_Initialized = true;
}

void GetCpuBrand(char* CPUbrand)
{
	if (!CPU_Initialized) GetCPUInfo();
	memcpy(CPUbrand,CPUBrand,12);
}

int CheckCPUFeature(unsigned int Feature)
{
	if (!CPU_Initialized) GetCPUInfo();

	switch (Feature)
	{
		case 0 : if (MMX) return 1;
		case 1 : if (SSE) return 1;
		case 2 : if (SSE2) return 1;
		case 3 : if (SSE3) return 1;
		case 4 : if (SSE41) return 1;
		case 5 : if (SSE42) return 1;
		case 6 : if (AVX) return 1;
		case 7 : if (AES) return 1;
		case 8 : if (_64_BIT) return 1;
		case 9 : if (SpeedStep) return 1;
		case 10 : if (HyperThreading) return 1;
		case 11 : return AmountLogicalCPU;
		case 12 : return AmountPhysicalCPU;
		default : return -1;
	}

	return 0;
}