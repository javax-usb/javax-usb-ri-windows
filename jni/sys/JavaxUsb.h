
#include <wdm.h>
#include <usbioctl.h>
#include <usbdi.h>
#include <usbdlib.h>

#ifndef __JAVAXUSB_H
#define __JAVAXUSB_H

typedef struct _JAVAXUSB_DEVEXT {
  PDEVICE_OBJECT filterDO;
  PDEVICE_OBJECT nextDO;
  PDEVICE_OBJECT physicalDO;
  UCHAR PnP_MN;
  UCHAR last_PnP_MN;
} javaxusb_devext;

NTSTATUS JavaxUsb_PnP(IN PDEVICE_OBJECT dev, IN PIRP irp);
NTSTATUS JavaxUsb_Power(IN PDEVICE_OBJECT dev, IN PIRP irp);
NTSTATUS JavaxUsb_PassThru(IN PDEVICE_OBJECT dev, IN PIRP irp);

#define SET_PNP_MN(devExt, MN) do { \
  devExt->last_PnP_MN = devExt->PnP_MN; \
  devExt->PnP_MN = MN; \
  } while (0)

#define RESTORE_PNP_MN(devExt) do { devExt->PnP_MN = devExt->last_PnP_MN; } while (0)

#endif /* __JAVAXUSB_H */

#ifdef DEFINE_GUID

/* JavaxUsb GUID */
// {136E983A-096C-49bb-A6C6-E608A0A0CDBC}
DEFINE_GUID(GUID_DEVINTERFACE_JAVAXUSB, 
0x136e983a, 0x96c, 0x49bb, 0xa6, 0xc6, 0xe6, 0x8, 0xa0, 0xa0, 0xcd, 0xbc);

#endif /* DEFINE_GUID */

