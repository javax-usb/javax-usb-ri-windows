
#include "JavaxUsb.h"

NTSTATUS JavaxUsb_Completion(IN PDEVICE_OBJECT dev, IN PIRP irp, IN PVOID context)
{
  PKEVENT event = context;

  DbgPrint("JavaxUsb_Completion dev %p irp %p context %p\n", dev, irp, context);

  KeSetEvent(event, 0, FALSE);

  return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS JavaxUsb_IoCallDriverSync(PDEVICE_OBJECT dev, PIRP irp)
{
  KEVENT event;
  NTSTATUS status;

  DbgPrint("JavaxUsb_IoCallDriverSync start dev %p, irp %p\n", dev, irp);

  KeInitializeEvent(&event, NotificationEvent, FALSE);

  IoSetCompletionRoutine(irp, JavaxUsb_Completion, &event, TRUE, TRUE, TRUE);

  status = IoCallDriver(dev, irp);

  KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);

  status = irp->IoStatus.Status;

  DbgPrint("JavaxUsb_IoCallDriverSync end status %d dev %p, irp %p\n", status, dev, irp);

  return status;
}
