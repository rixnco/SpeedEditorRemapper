#ifndef __DAVINCIRRESOLVE_USBD_VENDOR__
#define __DAVINCIRRESOLVE_USBD_VENDOR__

#include <Adafruit_TinyUSB.h>

class DavinciResolve_USBD_Vendor : public Adafruit_USBD_Interface
{
public:
  void begin();

  DavinciResolve_USBD_Vendor(void) { _instance = INVALID_INSTANCE; }
  // Get Interface Descriptor
  // Fill the descriptor (if buf is not NULL) and return its length
  virtual uint16_t getInterfaceDescriptor(uint8_t itfnum, uint8_t *buf, uint16_t bufsize);
private:
  enum { INVALID_INSTANCE = 0xffu };
  static uint8_t _instance_count;

  uint8_t _instance;

  bool isValid(void) { return _instance != INVALID_INSTANCE; }
};

#endif // __DAVINCIRRESOLVE_USBD_VENDOR__