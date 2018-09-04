# pplib - Elevate processes to Protected Processes

This project utilizes executing arbitrary code in the kernel by using libcapcom: https://github.com/notscimmy/libcapcom

## Protected Processes
Windows' security model defines that a process running with debug privileges, such as under an administrative account, can request access to any right to other processes running on the machine.  Such rights include the ability to:  
* Read/write arbitrary process memory
* Inject/execute code
* Control thread states and execution
* Query information about other processes

However, this model conflicts with security requirements for software that requires digital rights management. Protected processes are a solution to this requirement, adding many constraints to the rights that other processes can attain from the operating system. A process becomes a protected process when it's image on disk is signed by a specific Microsoft certificate, or is a process that is part of the **Windows Trusted Computer Base (WinTCB)**. Now the question is, can we trick the operating system to think a process is a protected process, without it being backed by a signed image on disk?

## Implementation
The answer is **yes**, we will achieve this via **Direct Kernel Object Manipulation (DKOM)**. Since behavior of the Win32 API changes based on whether or not rights are being asked for a protected process, it makes sense for the data that distinguishes normal processes from protected processes to live somewhere in the kernel. A look into the ```EPROCESS``` structure in WinDbg will reveal a member called ```PS_PROTECTION Protection``` which is in fact the data that we want to manipulate.  

```cpp
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
} PS_PROTECTED_TYPE
```

The structure is quite simple, and elevating a process to a protected process requires setting the **Protection** member in the corresponding process' ```EPROCESS``` structure to the appropriate bits as defined above.  

## How to use this library
1. Build the project
2. Link against **pplib.lib**
3. Include **pplib.h**
4. Call ```elevate_ppl()```

Inside the **testpplib** project:
```cpp
#include <Windows.h>
#include "pplib.h"

int main()
{
    elevate_ppl();
    MessageBoxA(NULL, "Use ProcessHacker to check PPL status", "Paused", NULL);
    return 0;
}
```
![](https://puu.sh/BpqSi/711eea8686.png")
