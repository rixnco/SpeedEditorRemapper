#include <Arduino.h>


#include "Adafruit_TinyUSB.h"
#include <bluefruit.h>
#include "HardwareSerial.h"
#include "BLEClientHIDReportCharacteristic.h"

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

void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void connect_callback(uint16_t conn_handle);
void scan_callback(ble_gap_evt_adv_report_t* report);
void connection_secured_callback(uint16_t conn_handle);
bool pair_passkey_callback(uint16_t conn_hdl, uint8_t const passkey[6], bool match_request);
void pair_passkey_req_callback(uint16_t conn_hdl, uint8_t passkey[6]);
void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);

void discover(uint16_t conn_handle);

bool ready = false;

void setup() {

  bond_init();
  // Clear TinyUSB USB interface
  //Serial.end();

  // TinyUSBDevice.setSerialDescriptor("12ECE81A21B347CE8B706886DC00A9E9");
  // TinyUSBDevice.setSerialDescriptor("12ECE81A21B");
  // TinyUSBDevice.begin();

  Serial1.begin(115200);


  // Configure GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);

  usb_vendor.begin();

  // Declare CDC Serial Interface
  Serial.begin(115200);

  // Declare SpeedEditor interface
  usb_hid.enableOutEndpoint(true);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  // usb_hid.setStringDescriptor("SpeedEditor");
  usb_hid.setReportCallback(get_report_callback, set_report_callback);
  usb_hid.begin();


  // wait until device mounted
  while( !TinyUSBDevice.mounted() ) delay(1);

  delay(1000);
  Serial.println("\nSpeedEditorRemapper");


  // clear bonds if BUTTON A is pressed
  if (0 == digitalRead(PIN_BUTTON1))
  {
    Serial.println("Clearing all bonds");
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

// Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);
  Bluefruit.configCentralBandwidth(BANDWIDTH_HIGH);

  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);


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
  
  ready = false;
  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds

}

void loop()
{
  static bool connected=false;

  if(ready && !connected)
  {
    // onConnect

    // for(uint16_t t=0; t<sizeof(chrs)/sizeof(chrs[0]); ++t) {
    //   if(chrs[t]->discovered() && (chrs[t]->uuid==UUID16_CHR_REPORT) )
    //   {
    //     delay(10); // Needed to give some time to the stack to finilized its internal processing.
    //     BLEClientHIDReportCharacteristic* pchr= (BLEClientHIDReportCharacteristic*)chrs[t];
    //     Serial.printf("report ref[%d]: %d - %d\n", t, pchr->reportId(), pchr->reportType());
    //   }
    // }
    // if(clientBas.discovered())
    // {
    //   delay(10);
    //   Serial.printf("Battery level: %d%%\n",clientBas.read());
    // }
    connected = ready;
  }
  else if(connected && !ready)
  {
    // onDisconnect


    connected = ready;
  }

  delay(500);
  digitalWrite(LED_BUILTIN, 0);
  delay(500);
  digitalWrite(LED_BUILTIN, 1);
}


void discover(uint16_t conn_handle)
{
  Serial.println("Discovering");

  // Serial.printf("Requested MTU=%d\n", Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL));
  // conn->requestMtuExchange(Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL));

  Serial.print("Discovering Device Information ... ");
  if ( clientDis.discover(conn_handle) )
  {
    Serial.println("Found");
    char buffer[64+1];
    
    memset(buffer, 0, sizeof(buffer));
    uint16_t count;
    if ( (count=clientDis.getChars(UUID16_CHR_PNP_ID, buffer, sizeof(buffer)))!=0 )
    {
      Serial.println("PNP_ID: ");
      Serial.printf ("  src: %d\n",   *(uint8_t*)(&buffer[0]));
      Serial.printf ("  vid: %04X\n", *(uint16_t*)(&buffer[1]));
      Serial.printf ("  pid: %04X\n", *(uint16_t*)(&buffer[3]));
      Serial.printf ("  ver: %04X\n", *(uint16_t*)(&buffer[5]));
      // TODO Check VID/PID
    }

    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      Serial.print("Manufacturer: ");
      Serial.println(buffer);
    }
    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      Serial.print("Model: ");
      Serial.println(buffer);
    }

    Serial.println();
  }else
  {
    Serial.println("Found NONE");
  }

  Serial.print("Discovering HID ... ");
  if ( clientHid.discover(conn_handle) )
  {
    Serial.println("Found");

    uint16_t count;

    count = Bluefruit.Discovery.discoverCharacteristic(conn_handle, chrs, sizeof(chrs)/sizeof(chrs[0]));
    Serial.printf("Found %d report characteristics\n", count);
    for(int t=0; t<11; ++t)
    {
      if(chrs[t]->discovered())
      {
        delay(10); // Needed to give some time to the stack to finilized its internal processing.
        BLEClientHIDReportCharacteristic* pchr= (BLEClientHIDReportCharacteristic*)chrs[t];
        Serial.printf("report ref[%d]: %d - %d\n", t, pchr->reportId(), pchr->reportType());
      }
    }
    delay(10); // Needed to give some time to the stack to finilized its internal processing.
  }else
  {
    Serial.println("Found NONE");
  }
  

  Serial.print("Discovering Battery ... ");
  if ( clientBas.discover(conn_handle) )
  {
    Serial.println("Found");
    delay(10);
    Serial.printf("Battery level: %d%%\n",clientBas.read());

  }else
  {
    Serial.println("Found NONE");
  }

  Serial.println("Ready to receive from peripheral");
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

  Serial.printf("getReport: ID=%d, Type=%d, Len=%d\n", report_id, report_type, reqlen);

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // This example doesn't use multiple report and report ID
  (void) report_id;
  (void) report_type;
  Serial.printf("setReport: ID=%d, Type=%d, Len=%d\n", report_id, report_type, bufsize);


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

  Serial.println("Connected");


 
  // HID device mostly require pairing/bonding
  if(!conn->bonded()) {
    Serial.println("Request pairing...");
    conn->requestPairing();
  }
  else
  {
    Serial.println("Bonded...");
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
  ready = false;
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}


bool pair_passkey_callback(uint16_t conn_hdl, uint8_t const passkey[6], bool match_request)
{
  Serial.println("Pairing passkey");
  Serial.printf("      %.3s %.3s\n\n", passkey, passkey+3);
  Serial.println("pair_passkey_callback");
  return true;
}


void pair_passkey_req_callback(uint16_t conn_hdl, uint8_t passkey[6])
{
  Serial.println("pair_passkey_req_callback");
}

void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if (auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
  {
    Serial.println("Failed");

    // disconnect
    conn->disconnect();
    return;
  }

  Serial.println("Succeeded");

  discover(conn_handle);
  ready = true;
  
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
    Serial.println("Not secured - request pairing");  
    conn->requestPairing();
  }
  else
  {
    Serial.println("Secured");
    if(conn->bonded())
    {
      discover(conn_handle);
      ready = true;
    }
  }
}

