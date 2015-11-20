#ifndef _DWUSBHOSTDXE_H_
#define _DWUSBHOSTDXE_H_

#include <Uefi.h>

#include <Protocol/Usb2HostController.h>
#include <Protocol/PciIo.h>
//#include <Protocol/EmbeddedGpio.h>

#include <Guid/EventGroup.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UncachedMemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include <IndustryStandard/Pci.h>

#include "ComponentName.h"

#define MAX_DEVICE                      16
#define MAX_ENDPOINT                    16

UINT32 RhDevNum;

UINT32 BulkDataToggle[MAX_DEVICE][MAX_ENDPOINT];
UINT32 ControlDataToggle[MAX_DEVICE][MAX_ENDPOINT];

typedef struct _DWUSB_OTGHC_DEV DWUSB_OTGHC_DEV;
typedef struct _USB_DEV_CONTEXT	USB_DEV_CONTEXT;

extern EFI_DRIVER_BINDING_PROTOCOL      gDwUsbHostDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL      gDwUsbHostComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL	gDwUsbHostComponentName2;

#define DWUSB_OTGHC_DEV_SIGNATURE	SIGNATURE_32 ('d', 'w', 'h', 'c')
#define DWHC_FROM_THIS(a)		CR(a, DWUSB_OTGHC_DEV, DwUsbOtgHc, DWUSB_OTGHC_DEV_SIGNATURE)

#define min(x, y) ({                            \
        typeof(x) _min1 = (x);                  \
        typeof(y) _min2 = (y);                  \
        (void) (&_min1 == &_min2);              \
        _min1 < _min2 ? _min1 : _min2; })

#define max(x, y) ({                            \
        typeof(x) _max1 = (x);                  \
        typeof(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })
                        
#define min3(x, y, z) min((typeof(x))min(x, y), z)
#define max3(x, y, z) max((typeof(x))max(x, y), z)

//
// The RequestType in EFI_USB_DEVICE_REQUEST is composed of
// three fields: One bit direction, 2 bit type, and 5 bit
// target.
//
#define USB_REQUEST_TYPE(Dir, Type, Target) \
          ((UINT8)((((Dir) == EfiUsbDataIn ? 0x01 : 0) << 7) | (Type) | (Target)))

struct _USB_DEV_CONTEXT {
	EFI_USB_DEVICE_DESCRIPTOR	DevDesc;
	EFI_USB_CONFIG_DESCRIPTOR	**ConfDesc;
};

struct _DWUSB_OTGHC_DEV {
	UINTN				Signature;
	EFI_USB2_HC_PROTOCOL		DwUsbOtgHc;

	EFI_PCI_IO_PROTOCOL		*PciIo;

	EFI_DEVICE_PATH_PROTOCOL	*DevicePath;

	EFI_UNICODE_STRING_TABLE	*ControllerNameTable;
	EFI_EVENT			ExitBootServiceEvent;

	UINT64				OriginalPciAttributes;
	UINT64				DwUsbBase;
	UINT8				*StatusBuffer;
	UINT8				*AlignedBuffer;

	UINT16				PortStatus;
	UINT16				PortChangeStatus;

	UINT32				RhDevNum;

	USB_DEV_CONTEXT			UsbDevContext[256];
};

EFI_STATUS
EFIAPI
DwUsbHostDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
DwUsbHostDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
DwUsbHostDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN UINTN                       NumberOfChildren,
  IN EFI_HANDLE                  *ChildHandleBuffer
  );

#endif //_DWUSBHOSTDXE_H_
