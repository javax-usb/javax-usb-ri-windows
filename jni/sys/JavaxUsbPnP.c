
#include "JavaxUsb.h"

NTSTATUS Irp_Completion(IN PDEVICE_OBJECT dev, IN PIRP irp, IN PVOID context)
{
	DbgPrint("Irp_Completion start dev %p irp %p context %p\n", dev, irp, context);

	if (irp->PendingReturned)
		IoMarkIrpPending(irp);

	KeSetEvent((PKEVENT)context, IO_NO_INCREMENT, FALSE);

	DbgPrint("Irp_Completion end\n");

	return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS JavaxUsb_PnP_START_DEVICE(PDEVICE_OBJECT dev, PIRP irp)
{
	NTSTATUS status;
	javaxusb_devext *devExt = dev->DeviceExtension;
	KEVENT event;

	DbgPrint("JavaxUsb_PnP_START_DEVICE start dev %p irp %p\n", dev, irp);

	IoCopyCurrentIrpStackLocationToNext(irp);
	KeInitializeEvent(&event, NotificationEvent, FALSE);
	IoSetCompletionRoutine(irp, Irp_Completion, &event, TRUE, TRUE, TRUE);
	status = IoCallDriver(devExt->nextDO, irp);

	if (STATUS_PENDING == status)
		KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);

	if (NT_SUCCESS(status) && NT_SUCCESS(irp->IoStatus.Status))
		SET_PNP_MN(devExt, IRP_MN_START_DEVICE);

	irp->IoStatus.Status = status;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	DbgPrint("JavaxUsb_PnP_START_DEVICE end status %d\n", status);

	return status;
}

NTSTATUS JavaxUsb_PnP_REMOVE_DEVICE(PDEVICE_OBJECT dev, PIRP irp)
{
	NTSTATUS status;
	javaxusb_devext *devExt = dev->DeviceExtension;

	DbgPrint("JavaxUsb_PnP_REMOVE_DEVICE start dev %p irp %p\n", dev, irp);

	IoSkipCurrentIrpStackLocation(irp);
	status = IoCallDriver(devExt->nextDO, irp);

	IoDetachDevice(devExt->nextDO);
	IoDeleteDevice(dev);

	SET_PNP_MN(devExt, IRP_MN_REMOVE_DEVICE);

	DbgPrint("JavaxUsb_PnP_REMOVE_DEVICE end status %d\n", status);

	return status;
}

NTSTATUS JavaxUsb_PnP(IN PDEVICE_OBJECT dev, IN PIRP irp)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	javaxusb_devext *devExt = dev->DeviceExtension;
	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(irp);

	DbgPrint("JavaxUsb_PnP start dev %p irp %p\n", dev, irp);

	switch (irpStack->MinorFunction) {
	case IRP_MN_START_DEVICE:
		status = JavaxUsb_PnP_START_DEVICE(dev, irp);
		break;
	case IRP_MN_REMOVE_DEVICE:
		status = JavaxUsb_PnP_REMOVE_DEVICE(dev, irp);
		break;
	default:
		status = JavaxUsb_PassThru(dev, irp);
		break;
	}

	DbgPrint("JavaxUsb_PnP end status %d\n", status);

	return status;
}

