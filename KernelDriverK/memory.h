#pragma once
#pragma warning (disable : 4047 4024)

#include <ntdef.h>
#include <ntifs.h>

#include "structs.h"

#ifndef MEMORY_H
#define MEMORY_H

NTSTATUS KernelReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);

NTSTATUS KernelWriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);

#endif // !MEMORY_H

