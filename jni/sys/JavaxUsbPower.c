
#include "JavaxUsb.h"

NTSTATUS JavaxUsb_Power(IN PDEVICE_OBJECT dev, IN PIRP irp)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	javaxusb_devext *devExt = dev->DeviceExtension;
	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(irp);

	DbgPrint("JavaxUsb_Power start dev %p irp %p\n", dev, irp);

	PoStartNextPowerIrp(irp);
	IoSkipCurrentIrpStackLocation(irp);
	status = PoCallDriver(devExt->nextDO, irp);

	DbgPrint("JavaxUsb_Power end status %d\n", status);

	return status;
}

