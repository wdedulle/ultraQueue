; Listing generated by Microsoft (R) Optimizing Compiler Version 16.00.30319.01 

	TITLE	C:\Users\xback\Documents\Visual Studio 2010\Projects\Buffers\Buffers\dllmain.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	_DllMain@12
; Function compile flags: /Ogtpy
; File c:\users\xback\documents\visual studio 2010\projects\buffers\buffers\dllmain.cpp
;	COMDAT _DllMain@12
_TEXT	SEGMENT
_hModule$ = 8						; size = 4
_ul_reason_for_call$ = 12				; size = 4
_lpReserved$ = 16					; size = 4
_DllMain@12 PROC					; COMDAT

; 11   : 	switch (ul_reason_for_call)

	mov	eax, DWORD PTR _ul_reason_for_call$[esp-4]
	dec	eax
	jne	SHORT $LN7@DllMain

; 12   : 	{
; 13   : 	case DLL_PROCESS_ATTACH:
; 14   : 		GetCPUInfo();

	call	?GetCPUInfo@@YAXXZ			; GetCPUInfo
$LN7@DllMain:

; 15   : 		break;
; 16   : 	case DLL_THREAD_ATTACH:
; 17   : 	case DLL_THREAD_DETACH:
; 18   : 	case DLL_PROCESS_DETACH:
; 19   : 
; 20   : 		break;
; 21   : 	}
; 22   : 	return TRUE;

	mov	eax, 1

; 23   : }

	ret	12					; 0000000cH
_DllMain@12 ENDP
END