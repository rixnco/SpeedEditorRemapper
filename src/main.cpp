#include <Arduino.h>


#include "Adafruit_TinyUSB.h"
#include <bluefruit.h>
#include "HardwareSerial.h"
#include "BLEClientHIDReportCharacteristic.h"

#define PROPS_BCAST         (0b00000001)
#define PROPS_READ          (0b00000010)
#define PROPS_WRITE_NO_RSP  (0b00000100)
#define PROPS_WRITE         (0b00001000)
#define PROPS_NOTFY         (0b00010000)
#define PROPS_INDICATE      (0b00100000)
#define PROPS_AUTHS_WR      (0b01000000)


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


// HID report descriptor using TinyUSB's template
// Blackmagic Speed Editor Keyboard
uint8_t const desc_hid_report[] =
{
0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
0x09, 0x00,        // Usage (0x00)
0xA1, 0x01,        // Collection (Application)
0x09, 0x01,        //   Usage (0x01)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x01,        //     Report ID (1)
0x05, 0x06,        //     Usage Page (Generic Dev Ctrls)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x2A,        //     Usage (0x2A)
0x75, 0x08,        //     Report Size (8)
0x95, 0x04,        //     Report Count (4)
0xB1, 0x81,        //     Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x09, 0x2D,        //     Usage (0x2D)
0x09, 0x2E,        //     Usage (0x2E)
0x09, 0x2F,        //     Usage (0x2F)
0x75, 0x08,        //     Report Size (8)
0x95, 0x03,        //     Report Count (3)
0xB1, 0x81,        //     Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0xC0,              //   End Collection
0x09, 0x08,        //   Usage (0x08)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x08,        //     Report ID (8)
0x06, 0x07, 0xFF,  //     Usage Page (Vendor Defined 0xFF07)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x20,        //     Report Count (32)
0xB1, 0x81,        //     Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0xC0,              //   End Collection
0x09, 0x02,        //   Usage (0x02)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x03,        //     Report ID (3)
0x06, 0x01, 0xFF,  //     Usage Page (Vendor Defined 0xFF01)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x04,        //     Report Count (4)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x02,        //     Usage (0x02)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x04,        //     Report Count (4)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x09, 0x02,        //     Usage (0x02)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0x09, 0x03,        //   Usage (0x03)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x04,        //     Report ID (4)
0x06, 0x02, 0xFF,  //     Usage Page (Vendor Defined 0xFF02)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x19, 0x00,        //     Usage Minimum (0x00)
0x29, 0xFF,        //     Usage Maximum (0xFF)
0x75, 0x10,        //     Report Size (16)
0x95, 0x06,        //     Report Count (6)
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x08,        //     Usage Maximum (0x08)
0x75, 0x01,        //     Report Size (1)
0x95, 0x08,        //     Report Count (8)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0x09, 0x06,        //   Usage (0x06)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x02,        //     Report ID (2)
0x06, 0x05, 0xFF,  //     Usage Page (Vendor Defined 0xFF05)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x20,        //     Usage Maximum (0x20)
0x75, 0x01,        //     Report Size (1)
0x95, 0x20,        //     Report Count (32)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0x09, 0x04,        //   Usage (0x04)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x05,        //     Report ID (5)
0x06, 0x03, 0xFF,  //     Usage Page (Vendor Defined 0xFF03)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0xB1, 0x82,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x04,        //     Report Count (4)
0xB1, 0x82,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x09, 0x02,        //     Usage (0x02)
0x75, 0x08,        //     Report Size (8)
0x95, 0x02,        //     Report Count (2)
0xB1, 0x82,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0xC0,              //   End Collection
0x09, 0x05,        //   Usage (0x05)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x06,        //     Report ID (6)
0x06, 0x04, 0xFF,  //     Usage Page (Vendor Defined 0xFF04)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0xB1, 0x82,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x08,        //     Report Count (8)
0xB1, 0x82,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
0xC0,              //   End Collection
0x09, 0x07,        //   Usage (0x07)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x07,        //     Report ID (7)
0x06, 0x06, 0xFF,  //     Usage Page (Vendor Defined 0xFF06)
0x15, 0x00,        //     Logical Minimum (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x00,        //     Usage (0x00)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0xC0,              // End Collection

// 308 bytes
};


// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, true);

DavinciResolve_USBD_Vendor usb_vendor;

uint16_t get_report_callback (uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);


BLEClientBas  clientBas;  // battery client
BLEClientDis  clientDis;  // device information client
BLEClientService clientHid(UUID16_SVC_HUMAN_INTERFACE_DEVICE);
BLEClientCharacteristic hidInfo(UUID16_CHR_HID_INFORMATION);
BLEClientCharacteristic reportMap(UUID16_CHR_REPORT_MAP);
BLEClientHIDReportCharacteristic chr1;
BLEClientHIDReportCharacteristic chr2;
BLEClientHIDReportCharacteristic chr3;
BLEClientHIDReportCharacteristic chr4;
BLEClientHIDReportCharacteristic chr5;
BLEClientHIDReportCharacteristic chr6;
BLEClientHIDReportCharacteristic chr7;
BLEClientHIDReportCharacteristic chr8;
BLEClientHIDReportCharacteristic chr9;
BLEClientHIDReportCharacteristic chr10;
BLEClientHIDReportCharacteristic chr11;

BLEClientCharacteristic *chrs[] = {
  &chr1,
  &chr2,
  &chr3,
  &chr4,
  &chr5,
  &chr6,
  &chr7,
  &chr8,
  &chr9,
  &chr10,
  &chr11,
  &reportMap,
  &hidInfo
};

BLEClientHIDReportCharacteristic* reports[] ={
  &chr1,
  &chr2,
  &chr3,
  &chr4,
  &chr5,
  &chr6,
  &chr7,
  &chr8,
  &chr9,
  &chr10,
  &chr11
};

void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void connect_callback(uint16_t conn_handle);
void scan_callback(ble_gap_evt_adv_report_t* report);
void connection_secured_callback(uint16_t conn_handle);
bool pair_passkey_callback(uint16_t conn_hdl, uint8_t const passkey[6], bool match_request);
void pair_passkey_req_callback(uint16_t conn_hdl, uint8_t passkey[6]);
void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);
void discover(uint16_t conn_handle);


void ble_reportCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len);


typedef enum {
  UNKNOWN = 0,
  SCANNING,
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
  SECURED,
  READY,
  ATTACHING,
  ATTACHED,
  DETACHING,
  DETACHED,
  IDENTIFIED,
  RUNNING
} state_t;


state_t state = UNKNOWN;

void setup() {
  // Configure GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);

  bond_init();

                                        
  // Declare SpeedEditor Private Interfaces
  usb_vendor.begin();

  // Declare SpeedEditor HID interface
  usb_hid.enableOutEndpoint(true);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("SpeedEditorRemapper");
  usb_hid.setReportCallback(get_report_callback, set_report_callback);
  usb_hid.begin();


  // Declare CDC Serial Interface
  Serial1.begin(115200);

  Serial1.println("\nSpeedEditorRemapper");


  // // Increase Blink rate to different from PrPh advertising mode
  // DO NOT USE - FREEZE BOARD to be investigated !!
  // Bluefruit.setConnLedInterval(250);

  Bluefruit.configCentralBandwidth(BANDWIDTH_MAX);

  // clear bonds if BUTTON A is pressed
  if (0 == digitalRead(PIN_BUTTON1))
  {
    Serial1.println("Clearing all bonds");
    Bluefruit.Central.clearBonds();
  }


  // Set connection secured callback, invoked when connection is encrypted
  Bluefruit.Security.setSecuredCallback(connection_secured_callback);
  Bluefruit.Security.setIOCaps(false,false,false);
  // Bluefruit.Security.setMITM(true);
  Bluefruit.Security.setPairPasskeyCallback(pair_passkey_callback);
  // Set complete callback to print the pairing result
  Bluefruit.Security.setPairCompleteCallback(pairing_complete_callback);
  // Set connection secured callback, invoked when connection is encrypted
  Bluefruit.Security.setSecuredCallback(connection_secured_callback);
  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);


  Bluefruit.begin(0, 1);
  
  Bluefruit.setName("SpeedEditorRemapper");

  // Configure Battery client
  clientBas.begin();  

  // Configure DIS client
  clientDis.begin();

  clientHid.begin();
  chr1.begin(&clientHid);
  chr2.begin(&clientHid);
  chr3.begin(&clientHid);
  chr4.begin(&clientHid);
  chr5.begin(&clientHid);
  chr6.begin(&clientHid);
  chr7.begin(&clientHid);
  chr8.begin(&clientHid);
  chr9.begin(&clientHid);
  chr10.begin(&clientHid);
  chr11.begin(&clientHid);
  reportMap.begin(&clientHid);
  hidInfo.begin(&clientHid);




  /* Start Central Scanning
   * - Enable auto scan if disconnected
   * - Interval = 100 ms, window = 80 ms
   * - Don't use active scan
   * - Start(timeout) with timeout = 0 will scan forever (until connected)
   */
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(BLEUuid(0x1812));   // only report HID service  
  Bluefruit.Scanner.useActiveScan(false);
  
  state = SCANNING;

  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds
  Serial1.println("Scanning...");
  TinyUSBDevice.detach();

}

// void tud_umount_cb(void)
// {
//   Serial1.println("unmount");
// }

// void tud_mount_cb(void)
// {
//   Serial1.println("mount");
// }


void reset_mcu(uint32_t gpregret=0)
{
  // disable SD
  sd_softdevice_disable();

  // Disable all interrupts
  NVIC->ICER[0]=0xFFFFFFFF;
  NVIC->ICPR[0]=0xFFFFFFFF;
#if defined(__NRF_NVIC_ISER_COUNT) && __NRF_NVIC_ISER_COUNT == 2
  NVIC->ICER[1]=0xFFFFFFFF;
  NVIC->ICPR[1]=0xFFFFFFFF;
#endif

  NRF_POWER->GPREGRET = gpregret;

  NVIC_SystemReset();

  // maybe yield ?
  while(1) {}
}



uint8_t buffer[64][4]= {0};


void loop()
{
  static uint32_t last_heartbeat_time=0;
  static uint32_t last_report_time=0;
  static bool led_state = false;
  static uint16_t report_idx=0;

  uint32_t now= millis();
  if(now-last_heartbeat_time>500)
  {
    last_heartbeat_time=now;
    led_state=!led_state;
    digitalWrite(LED_BUILTIN, led_state);
  }

  switch(state)
  {
  case SCANNING:
    break;
  case DISCONNECTED:
    Serial1.println("Detaching...");
    // if(TinyUSBDevice.detach()) {
    //   Serial1.println("OK");
    // }
    // else
    // {
    //   Serial1.println("FAILED");
    // }
    state = DETACHING;
    break;
  case DETACHING:
    Serial1.println("Detached...");
    state = DETACHED;
    break;
  case DETACHED:
    Serial1.println("Resetting...");
    delay(500);
    reset_mcu();
    break;
  case READY:
    Serial1.print("Attaching...");
    if(TinyUSBDevice.attach()) {
      Serial1.println("OK");
    }
    else
    {
      Serial1.println("FAILED");
    }
    state = ATTACHING;
    break;
  case ATTACHING:
    if(TinyUSBDevice.mounted()) {
      Serial1.println("Enumerated...");
      state = ATTACHED;
    }
    break;
  case ATTACHED:
    break;
  case IDENTIFIED:
    // Serial1.println("enabling ble notifications");
    // for(int t=0; t<11; ++t)
    // {
    //   if(reports[t]->canNotify()) {
    //     Serial1.printf("Notify: %d-%d\n", reports[t]->reportId(),reports[t]->reportType());
    //     reports[t]->enableNotify();
    //     delay(10);
    //   }
    // }
    Serial1.println("Identified");
    delay(100);
    last_report_time = millis();
    state = RUNNING;
    break;
  case RUNNING:
  {
    if(now-last_report_time<25) break;
    last_report_time=now;
    if(reports[report_idx]->reportType() == 1) 
    {
      uint8_t _buffer[64];
      
      uint16_t len = reports[report_idx]->read(_buffer, 64);
      if(len && memcmp(_buffer, buffer[report_idx], len)!=0) 
      {
        digitalWrite(LED_BUILTIN, !led_state);
        memcpy(buffer[report_idx], _buffer, len);
        usb_hid.sendReport(reports[report_idx]->reportId(), buffer[report_idx], len);
      }
    }
    report_idx= (report_idx+1)%4;
    digitalWrite(LED_BUILTIN, led_state);
    break;
  }
  default:
    break;
  }
  yield();
}


void discover(uint16_t conn_handle)
{
  Serial1.println("Discovering");

  Serial1.print("Discovering Device Information ... ");
  if ( clientDis.discover(conn_handle) )
  {
    Serial1.println("Found");
    char buffer[64+1];
    
    memset(buffer, 0, sizeof(buffer));
    uint16_t count;
    if ( (count=clientDis.getChars(UUID16_CHR_PNP_ID, buffer, sizeof(buffer)))!=0 )
    {
      Serial1.println("PNP_ID: ");
      Serial1.printf ("  src: %d\n",   *(uint8_t*)(&buffer[0]));
      Serial1.printf ("  vid: %04X\n", *(uint16_t*)(&buffer[1]));
      Serial1.printf ("  pid: %04X\n", *(uint16_t*)(&buffer[3]));
      Serial1.printf ("  ver: %04X\n", *(uint16_t*)(&buffer[5]));
      // TODO Check VID/PID
    }

    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      Serial1.print("Manufacturer: ");
      Serial1.println(buffer);
    }
    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      Serial1.print("Model: ");
      Serial1.println(buffer);
    }

    Serial1.println();
  }else
  {
    Serial1.println("Found NONE");
  }

  Serial1.print("Discovering HID ... ");
  if ( clientHid.discover(conn_handle) )
  {
    Serial1.println("Found");

    uint16_t count;

    count = Bluefruit.Discovery.discoverCharacteristic(conn_handle, chrs, sizeof(chrs)/sizeof(chrs[0]));
    Serial1.printf("Found %d characteristics\n", count);
    for(int t=0; t<11; ++t)
    {
      if(chrs[t]->discovered())
      {
        delay(10); // Needed to give some time to the stack to finilized its internal processing.
        BLEClientHIDReportCharacteristic* pchr= (BLEClientHIDReportCharacteristic*)chrs[t];
        Serial1.printf("report ref[%d]: %d - %d\n", t, pchr->reportId(), pchr->reportType());
        if(chrs[t]->canNotify()) pchr->setNotifyCallback(ble_reportCallback);
      }
    }
    delay(10); // Needed to give some time to the stack to finilized its internal processing.
  }else
  {
    Serial1.println("Found NONE");
  }
  

  Serial1.print("Discovering Battery ... ");
  if ( clientBas.discover(conn_handle) )
  {
    Serial1.println("Found");
    delay(100);
    Serial1.printf("Battery level: %d%%\n",clientBas.read());

  }else
  {
    Serial1.println("Found NONE");
  }

  delay(100);
  BLEConnection* conn = Bluefruit.Connection( conn_handle );
  uint16_t mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL);
  Serial1.printf("Requesting MTU=%d\n", mtu);
  if(!conn->requestMtuExchange(mtu))
  {
    Serial1.println("MTU Request failed !!");
  }
  delay(100);
  Serial1.printf("Requesting Connection parameters update\n");
  if(!conn->requestConnectionParameter(6))
  {
    Serial1.println("Connection parameter update failed !!");
  }
  delay(100);


  state = READY;
  Serial1.println("Ready to receive from peripheral");
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t get_report_callback (uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // not used in this example
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  if(state < READY) return 0;

  uint16_t len=0;
  for(int t=0; t<11; ++t)
  {
    if(reports[t]->reportId() == report_id && reports[t]->reportType() == report_type )
    {
      // delay(5);
       len = reports[t]->read(buffer, reqlen);
       break;
    }
  }
  Serial1.printf("getReport: ID=%d, Type=%d, Len=%d: ", report_id, report_type, reqlen);
  for(int t=0; t<len; ++t)
  {
    Serial1.printf("%02X ", buffer[t]);
  }
  Serial1.println();
 
  if(report_id==6 && report_type==3 && buffer[0]==0x04 && buffer[1]==0x58 && buffer[2]==0x02)
  {
    state = IDENTIFIED;
  }

  return len;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // This example doesn't use multiple report and report ID
  (void) report_id;
  (void) report_type;

  if(state < READY) return;

  if(report_id==0 && report_type==0)
  {
    report_id=buffer[0];
    report_type=HID_REPORT_TYPE_OUTPUT;
    ++buffer;
    --bufsize;
  }
  for(int t=0; t<11; ++t)
  {
    if(reports[t]->reportId() == report_id && reports[t]->reportType() == report_type )
    {
      // delay(5);
      reports[t]->write_resp(buffer, bufsize);
      break;
    }
  }

  Serial1.printf("setReport: ID=%d, Type=%d, Len=%d: ", report_id, report_type, bufsize);
  for(int t=0; t<bufsize; ++t)
  {
    Serial1.printf("%02X ", buffer[t]);
  }
  Serial1.println();
  // echo back anything we received from host
  // usb_hid.sendReport(0, buffer, bufsize);
}



uint8_t DavinciResolve_USBD_Vendor::_instance_count = 0;

// Starting endpoints; adjusted elsewhere as needed
#define EPOUT 0x00
#define EPIN 0x80


// Baud and config is ignore in CDC
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
  // CDC is mostly always existed for DFU
  // usb core will automatically update endpoint number
  uint8_t desc[] = {TUD_VENDOR_DESCRIPTOR(itfnum, 0, EPOUT, EPIN, 1)};
  uint16_t const len = sizeof(desc);

  if (bufsize < len) {
    return 0;
  }

  memcpy(buf, desc, len);
  return len;
}

/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Connect to device with bleuart service in advertising
  Bluefruit.Central.connect(report);
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  Serial1.println("Connected");

  // Serial1.println("Request MTU exchange: 128");
  // conn->requestMtuExchange(128);
  // delay(100);
  // Serial1.printf("MTU=%d\n", conn->getMtu());

  // HID device mostly require pairing/bonding
  if(!conn->bonded()) {
    Serial1.println("Request pairing...");
    conn->requestPairing();
  }
  else
  {
    Serial1.println("Bonded...");
    // discover(conn_handle);
    // ready = true;
  }
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial1.print("Disconnected, reason = 0x"); Serial1.println(reason, HEX);
  state = DISCONNECTED;
}


bool pair_passkey_callback(uint16_t conn_hdl, uint8_t const passkey[6], bool match_request)
{
  Serial1.println("Pairing passkey");
  Serial1.printf("      %.3s %.3s\n\n", passkey, passkey+3);
  Serial1.println("pair_passkey_callback");
  return true;
}


void pair_passkey_req_callback(uint16_t conn_hdl, uint8_t passkey[6])
{
  Serial1.println("pair_passkey_req_callback");
}

void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if (auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
  {
    Serial1.println("Failed");

    // disconnect
    conn->disconnect();
    return;
  }

  Serial1.println("Succeeded");

  discover(conn_handle);
 
}



void connection_secured_callback(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if ( !conn->secured() )
  {
    // It is possible that connection is still not secured by this time.
    // This happens (central only) when we try to encrypt connection using stored bond keys
    // but peer reject it (probably it remove its stored key).
    // Therefore we will request an pairing again --> callback again when encrypted
    Serial1.println("Not secured - request pairing");  
    conn->requestPairing();
  }
  else
  {
    Serial1.println("Secured");
    if(conn->bonded())
    {
      discover(conn_handle);
    }
  }
}

void ble_reportCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  BLEClientHIDReportCharacteristic* report= (BLEClientHIDReportCharacteristic*)chr;
  Serial1.printf("notification: %d - %d: ", report->reportId(), report->reportType());
  for(int t=0; t<len; ++t) Serial1.printf("%02X ", data[t]);
  Serial1.println();
}