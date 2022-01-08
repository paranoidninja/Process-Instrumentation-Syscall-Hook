#include <windows.h>
#include <winternl.h>

#define NtCurrentPeb() ((_EtwPTEB)(NtCurrentTeb()->ProcessEnvironmentBlock))

typedef struct _EtwPEB {
  BYTE InheritedAddressSpace;
  BYTE ReadImageFileExecOptions;
  BYTE BeingDebugged;
  BYTE _SYSTEM_DEPENDENT_01;
  LPVOID Mutant;
  LPVOID ImageBaseAddress;
} EtwPEB, *EtwPPEB;

typedef struct _EtwTEB {
    NT_TIB NtTib;
    PVOID EnvironmentPointer;
    CLIENT_ID ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    EtwPPEB ProcessEnvironmentBlock;
} _EtwTEB, *_EtwPTEB;