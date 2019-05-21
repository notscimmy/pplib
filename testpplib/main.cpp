#include <Windows.h>
#include "pplib.h"

int main()
{
	if (elevate_ppl())
		MessageBoxA(NULL, "Use ProcessHacker to check PPL status", "Paused", NULL);
	else
		MessageBoxA(NULL, "Something went wrong - probably unsuported windows version", "Error", MB_ICONERROR);
	return 0;
}
