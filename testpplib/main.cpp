#include <Windows.h>
#include "pplib.h"

int main()
{
	elevate_ppl();
	MessageBoxA(NULL, "Use ProcessHacker to check PPL status", "Paused", NULL);
	return 0;
}
