
#include "JavaxUsb.h"

static int devices = 0;

NTSTATUS JavaxUsb_AddDevice(IN PDRIVER_OBJECT driver, IN PDEVICE_OBJECT physDevObj)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PDEVICE_OBJECT dev = NULL;
	javaxusb_devext *ext = NULL;

	DbgPrint("JavaxUsb_AddDevice start devices %d\n", devices);
	DbgPrint("JavaxUsb_AddDevice driver name %s\n", driver->DriverName);

	if (!NT_SUCCESS(status = IoCreateDevice(driver, sizeof(*ext), NULL, FILE_DEVICE_USB, FILE_DEVICE_SECURE_OPEN, FALSE, &dev)))
		goto ADD_DEVICE_END;

	DbgPrint("JavaxUsb_AddDevice successfully created device\n");

	ext = dev->DeviceExtension;
	ext->filterDO = dev;
	ext->physicalDO = physDevObj;

	if (!(ext->nextDO = IoAttachDeviceToDeviceStack(dev, physDevObj))) {
		status = STATUS_NO_SUCH_DEVICE;
		goto ADD_DEVICE_END;
	}

	dev->Flags |= (ext->nextDO->Flags & (DO_BUFFERED_IO | DO_DIRECT_IO));
	dev->Flags |= (ext->nextDO->Flags & (DO_POWER_INRUSH | DO_POWER_PAGABLE));

	dev->Flags &= ~DO_DEVICE_INITIALIZING;

ADD_DEVICE_END:
	if (!NT_SUCCESS(status)) {
		if (ext && ext->nextDO) IoDetachDevice(ext->nextDO);
		if (dev) IoDeleteDevice(dev);
	} else {
		devices++;
	}

	DbgPrint("JavaxUsb_AddDevice end devices %d status %d\n", devices, status);

	return status;
}

VOID JavaxUsb_DriverUnload(IN PDRIVER_OBJECT driver)
{
	DbgPrint("JavaxUsb_DriverUnload\n");
}

NTSTATUS JavaxUsb_PassThru(IN PDEVICE_OBJECT dev, IN PIRP irp)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	javaxusb_devext *devExt = dev->DeviceExtension;

	DbgPrint("JavaxUsb_PassThru start\n");

	IoSkipCurrentIrpStackLocation(irp);
	status = IoCallDriver(devExt->nextDO, irp);

	DbgPrint("JavaxUsb_PassThru end status %d\n", status);

	return status;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	int i;

	DbgPrint("JavaxUsb_DriverEntry start\n");

	for (i=0; i<IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = JavaxUsb_PassThru;

	//DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = JavaxUsb_DevCtrl;
	DriverObject->MajorFunction[IRP_MJ_POWER] = JavaxUsb_Power;
	//DriverObject->MajorFunction[IRP_MJ_PNP] = JavaxUsb_PnP;
	//DriverObject->MajorFunction[IRP_MJ_CREATE] = JavaxUsb_Create;
	//DriverObject->MajorFunction[IRP_MJ_CLOSE] = JavaxUsb_Close;
	//DriverObject->MajorFunction[IRP_MJ_CLEANUP] = JavaxUsb_Clean;
	//DriverObject->MajorFunction[IRP_MJ_READ] = JavaxUsb_ReadWrite;
	//DriverObject->MajorFunction[IRP_MJ_WRITE] = JavaxUsb_ReadWrite;
	//DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = JavaxUsb_SysCtrl;
	DriverObject->DriverUnload = JavaxUsb_DriverUnload;
	DriverObject->DriverExtension->AddDevice = JavaxUsb_AddDevice;

	DbgPrint("JavaxUsb_DriverEntry end status %d\n", status);

	return status;
}

