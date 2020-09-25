#pragma warning (disable: 4100)

#include "entry.h"
#include "debug.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	pDriverObject->DriverUnload = UnloadDriver;
	DbgPrintEx(0, 0, "Start message");
	DbgMsg("Loaded");
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	DbgMsg("Unloaded");
	return STATUS_SUCCESS;
}
