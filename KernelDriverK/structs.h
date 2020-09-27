#pragma once

#include <ntifs.h>

NTSTATUS NTAPI MmCopyVirtualMemory(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);


typedef struct KERNEL_READ_REQUEST {
	ULONG PID;
	ULONG Address;
	PVOID pBuffer;
	ULONG Size;
} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct KERNEL_WRITE_REQUEST {
	ULONG PID;
	ULONG Address;
	PVOID pBuffer;
	ULONG Size;
} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

