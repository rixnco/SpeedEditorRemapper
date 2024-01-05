#include "DavinciResolve_USBD_Vendor.h"

//-----------------------------------------------------------
// DavinciResolve_USBD_Vendor class definition
//-----------------------------------------------------------

uint8_t DavinciResolve_USBD_Vendor::_instance_count = 0;

// Starting endpoints; adjusted elsewhere as needed
#define EPOUT 0x00
#define EPIN 0x80


void DavinciResolve_USBD_Vendor::begin() {
  // already called begin()
  if (isValid()) {
    return;
  }

  // too many instances
  if (!(_instance_count < 1)) {
    return;
  }

  _instance = _instance_count++;
  this->setStringDescriptor("Blackmagic DaVinci Resolve Speed Editor");
  TinyUSBDevice.addInterface(*this);
}

uint16_t DavinciResolve_USBD_Vendor::getInterfaceDescriptor(uint8_t itfnum, uint8_t *buf, uint16_t bufsize)
{
  uint8_t desc[] = {TUD_VENDOR_DESCRIPTOR(itfnum, 0, EPOUT, EPIN, 1)};
  uint16_t const len = sizeof(desc);

  if (bufsize < len) {
    return 0;
  }

  memcpy(buf, desc, len);
  return len;
}
