#pragma once
#define WIN32_NO_STATUS
#include <Windows.h>
#include <Winternl.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

namespace native::structs
{
	typedef LARGE_INTEGER PHYSICAL_ADDRESS;

	typedef union _PS_PROTECTION
	{
		UCHAR Level;
		struct
		{
			int Type : 3;
			int Audit : 1;
			int Signer : 4;
		} Flags;
	} PS_PROTECTION, *PPS_PROTECTION;

	typedef enum _PS_PROTECTED_SIGNER
	{
		PsProtectedSignerNone = 0,
		PsProtectedSignerAuthenticode = 1,
		PsProtectedSignerCodeGen = 2,
		PsProtectedSignerAntimalware = 3,
		PsProtectedSignerLsa = 4,
		PsProtectedSignerWindows = 5,
		PsProtectedSignerWinTcb = 6,
		PsProtectedSignerMax = 7
	} PS_PROTECTED_SIGNER;

	typedef enum _PS_PROTECTED_TYPE
	{
		PsProtectedTypeNone = 0,
		PsProtectedTypeProtectedLight = 1,
		PsProtectedTypeProtected = 2,
		PsProtectedTypeMax = 3

	} PS_PROTECTED_TYPE;

	typedef struct _PHYSICAL_MEMORY_RANGE
	{
		PHYSICAL_ADDRESS BaseAddress;
		LARGE_INTEGER NumberOfBytes;
	} PHYSICAL_MEMORY_RANGE, *PPHYSICAL_MEMORY_RANGE;

	typedef enum _SECTION_INHERIT 
	{ 
		ViewShare = 1, 
		ViewUnmap = 2 
	} SECTION_INHERIT, *PSECTION_INHERIT;

	typedef CLIENT_ID* PCLIENT_ID;
	typedef ULONG (*DbgPrintEx_t)(_In_ ULONG, _In_ ULONG, _In_ PCSTR, ...);
	typedef PVOID (*PsGetCurrentProcess_t)();
	typedef VOID (*RtlInitUnicodeString_t)(PUNICODE_STRING, PCWSTR);
	typedef NTSTATUS (*ZwOpenSection_t)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
	typedef NTSTATUS (*ZwMapViewOfSection_t)(HANDLE, HANDLE, PVOID*, ULONG_PTR, SIZE_T, PLARGE_INTEGER, PSIZE_T, SECTION_INHERIT, ULONG,ULONG);
	typedef NTSTATUS (*ZwOpenProcess_t)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
	typedef PPHYSICAL_MEMORY_RANGE (*MmGetPhysicalMemoryRanges_t)(VOID);
	typedef PVOID (NTAPI* MmGetSystemRoutineAddress_t)(PUNICODE_STRING);
}
