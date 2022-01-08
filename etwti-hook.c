#include "etwti-hook.h"
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <intrin.h>

extern void hookedCallback();

#define ProcessInstrumentationCallback 0x28
typedef struct _PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION
{
    ULONG Version;
    ULONG Reserved;
    PVOID Callback;
} PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION, *PPROCESS_INSTRUMENTATION_CALLBACK_INFORMATION;

VOID HuntSyscall(ULONG_PTR ReturnAddress, ULONG_PTR retSyscallPtr) {
    PVOID ImageBase = ((EtwPPEB)(((_EtwPTEB)(NtCurrentTeb()->ProcessEnvironmentBlock))))->ImageBaseAddress;
    PIMAGE_NT_HEADERS NtHeaders = RtlImageNtHeader(ImageBase);
    if (ReturnAddress >= (ULONG_PTR)ImageBase && ReturnAddress < (ULONG_PTR)ImageBase + NtHeaders->OptionalHeader.SizeOfImage) {
        printf("[+] Syscall detected:  Return address: 0x%X  Syscall value: 0x%X\n", ReturnAddress, retSyscallPtr);
    }
}

BOOL EtwTiMod() {
    PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION InstrumentationCallbackInfo;
    InstrumentationCallbackInfo.Version  = 0;
    InstrumentationCallbackInfo.Reserved = 0;
    InstrumentationCallbackInfo.Callback = hookedCallback;
    NTSTATUS Status = NtSetInformationProcess((HANDLE) -1, ProcessInstrumentationCallback, &InstrumentationCallbackInfo, sizeof(InstrumentationCallbackInfo));
    if (NT_SUCCESS(Status)) {
        printf("Callback added\n");
        return TRUE;
    }
    printf("Failed : %lx\n", Status);
    return FALSE;
}

int main() {
    EtwTiMod();
    OBJECT_ATTRIBUTES objAttr;
	InitializeObjectAttributes(&objAttr, NULL, 0, NULL, NULL);
	CLIENT_ID cID;
	cID.UniqueProcess = (HANDLE) ULongToHandle(2084);
	cID.UniqueThread = 0;
    HANDLE hProcess = INVALID_HANDLE_VALUE;
	NtOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &objAttr, &cID);
    return 0;
}