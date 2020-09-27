
#pragma warning (disable : 4100 4047 4024)

#include "entry.h"
#include "debug.h"
#include "memory.h"
#include "data.h"
#include "communication.h"

NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(
	_In_ HANDLE ProcessId,
	_Outptr_ PEPROCESS *Process
);

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallBack(PUNICODE_STRING FullImageName, HANDLE ProcessID, PIMAGE_INFO ImageInfo) { // get address of dll if it getse loaded.
	//DbgMsg("ImageLoaded: %ls \n", FullImageName->Buffer);

	if (wcsstr(FullImageName->Buffer, L"\\Counter-Strike Global Offensive\\csgo\\bin\\client.dll")){ // input dll (client.dll) path
		DbgMsg("found client.dll at %d", ImageInfo->ImageBase);
		csgoClientDLL = ImageInfo->ImageBase;

		DbgMsg("PID: %d \n", ProcessID);
	}
	else if (wcsstr(FullImageName->Buffer, L"\\Counter-Strike Global Offensive\\bin\\engine.dll")) { // input dll (engine.dll) path
		DbgMsg("found engine.dll at %d", ImageInfo->ImageBase);
		csgoEngineDLL = ImageInfo->ImageBase;

		DbgMsg("PID: %d \n", ProcessID);
	}

	return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);
	pDriverObject->DriverUnload = UnloadDriver;

	DbgMsg("Driver loaded");

	PsSetLoadImageNotifyRoutine(ImageLoadCallBack);

	RtlInitUnicodeString(&dev, L"\\Device\\KernelDriverK");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\KernelDriverK");

	IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&dos, &dev);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	UNREFERENCED_PARAMETER(pDriverObject);

	PsRemoveLoadImageNotifyRoutine(ImageLoadCallBack);

	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	DbgMsg("Driver Unloaded");

	return STATUS_SUCCESS;
}
