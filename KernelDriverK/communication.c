
#pragma warning (disable : 4022)

#include "communication.h"
#include "debug.h"
#include "entry.h"
#include "data.h"
#include "memory.h"

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	ULONG ByteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	if (ControlCode == IO_GET_CLIENTADDRESS) {
		PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer; // cast this to whatever you want to give back to usermode
		*Output = csgoClientDLL; // set ouput to equal to content

		DbgMsg("Client Address requested\n");
		Status = STATUS_SUCCESS;
		ByteIO = sizeof(*Output);
	}
	else if (ControlCode == IO_GET_ENGINEADDRESS) {
		PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*Output = csgoEngineDLL;

		DbgMsg("Engine Address requested\n");

		Status = STATUS_SUCCESS;
		ByteIO = sizeof(*Output);
	}
	else if (ControlCode == IO_READVIRTUALMEMORY) {
		PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;

		if (NT_SUCCESS(PsLookupProcessByProcessId(ReadInput->PID, &Process))) {
			KernelReadVirtualMemory(Process, ReadInput->Address, ReadInput->pBuffer, ReadInput->Size);
			Status = STATUS_SUCCESS;
			ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	else if (ControlCode == IO_WRITEVIRTUALMEMORY) {
		PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;

		if (NT_SUCCESS(PsLookupProcessByProcessId(WriteInput->PID, &Process))) {
			KernelWriteVirtualMemory(Process, WriteInput->pBuffer, WriteInput->Address, WriteInput->Size);
			Status = STATUS_SUCCESS;
			ByteIO = sizeof(KERNEL_WRITE_REQUEST);
		}
	}
	else {

		DbgMsg("Invalid IO Code called: %d", ControlCode);

		ByteIO = 0;
	}

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = ByteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DbgMsg("Connection Terminated \n");

	return STATUS_SUCCESS;
}

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DbgMsg("CreateCall was called. Connection Established\n");

	return STATUS_SUCCESS;
}

