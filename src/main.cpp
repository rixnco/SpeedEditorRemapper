#include <Arduino.h>


#include "Adafruit_TinyUSB.h"
#include <bluefruit.h>
#include "HardwareSerial.h"
#include "BLEClientHIDReportCharacteristic.h"

#include"hid_key.h"


#define PROPS_BCAST         (0b00000001)
#define PROPS_READ          (0b00000010)
#define PROPS_WRITE_NO_RSP  (0b00000100)
#define PROPS_WRITE         (0b00001000)
#define PROPS_NOTFY         (0b00010000)
#define PROPS_INDICATE      (0b00100000)
#define PROPS_AUTHS_WR      (0b01000000)

#define KEY_SE_NONE			 0x00 //NO KEY

#define KEY_SE_SMART_INSRT		 0x01 //SMART INSRT [CLIP]
#define KEY_SE_APPND			 0x02 //APPND [CLIP]
#define KEY_SE_RIPL_OWR		 0x03 //RIPL O/WR
#define KEY_SE_CLOSE_UP		 0x04 //CLOSE UP [YPOS]
#define KEY_SE_PLACE_ON_TOP	 0x05 //PLACE ON TOP
#define KEY_SE_SRC_OWR			 0x06 //SRC O/WR

#define KEY_SE_IN				 0x07 //IN [CLR]
#define KEY_SE_OUT				 0x08 //OUT [CLR]
#define KEY_SE_TRIM_IN			 0x09 //TRIM IN
#define KEY_SE_TRIM_OUT		 0x0a //TRIM OUT
#define KEY_SE_ROLL			 0x0b //ROLL [SLIDE]
#define KEY_SE_SLIP_SRC		 0x0c //SLIP SRC
#define KEY_SE_SLIP_DEST		 0x0d //SLIP DEST
#define KEY_SE_TRANS_DUR		 0x0e //TRANS DUR [SET]
#define KEY_SE_CUT				 0x0f //CUT
#define KEY_SE_DIS				 0x10 //DIS
#define KEY_SE_SMTH_CUT		 0x11 //SMTH CUT

#define KEY_SE_SOURCE			 0x1a //SOURCE
#define KEY_SE_TIMELINE		 0x1b //TIMELINE

#define KEY_SE_SHTL			 0x1c //SHTL
#define KEY_SE_JOG				 0x1d //JOG
#define KEY_SE_SCRL			 0x1e //SCRL

#define KEY_SE_ESC				 0x31 //ESC [UNDO]
#define KEY_SE_SYNC_BIN		 0x1f //SYNC BIN
#define KEY_SE_AUDIO_LEVEL		 0x2c //AUDIO LEVEL [MARK]
#define KEY_SE_FULL_VIEW		 0x2d //FULL VIEW [RVW]
#define KEY_SE_TRANS			 0x22 //TRANS [TITLE]
#define KEY_SE_SPLIT			 0x2f //SPLIT [MOVE]
#define KEY_SE_SNAP			 0x2e //SNAP [=]
#define KEY_SE_RIPL_DEL		 0x2b //RIPL DEL

#define KEY_SE_CAM1			 0x33 //CAM1
#define KEY_SE_CAM2			 0x34 //CAM2
#define KEY_SE_CAM3			 0x35 //CAM3
#define KEY_SE_CAM4			 0x36 //CAM4
#define KEY_SE_CAM5			 0x37 //CAM5
#define KEY_SE_CAM6			 0x38 //CAM6
#define KEY_SE_CAM7			 0x39 //CAM7
#define KEY_SE_CAM8			 0x3a //CAM8
#define KEY_SE_CAM9			 0x3b //CAM9
#define KEY_SE_LIVE_OWR		 0x30 //LIVE O/WR [RND]
#define KEY_SE_VIDEO_ONLY		 0x25 //VIDEO ONLY
#define KEY_SE_AUDIO_ONLY		 0x26 //AUDIO ONLY
#define KEY_SE_STOP_PLAY		 0x3c //STOP/PLAY


typedef struct {
  uint16_t se_key;
  uint8_t kbd_mod;
  uint8_t kbd_key;
} keymap_t;


// keymap_t keymap[] = {
//   { .se_key=KEY_SE_CAM1, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP1 },
//   { .se_key=KEY_SE_CAM2, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP2 },
//   { .se_key=KEY_SE_CAM3, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP3 },
//   { .se_key=KEY_SE_CAM4, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP4 },
//   { .se_key=KEY_SE_CAM5, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP5 },
//   { .se_key=KEY_SE_CAM6, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP6 },
//   { .se_key=KEY_SE_CAM7, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP7 },
//   { .se_key=KEY_SE_CAM8, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP8 },
//   { .se_key=KEY_SE_CAM9, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_KP9 }
// };
keymap_t keymap[] = {
  { .se_key=KEY_SE_CAM1, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_1 },
  { .se_key=KEY_SE_CAM2, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_2 },
  { .se_key=KEY_SE_CAM3, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_3 },
  { .se_key=KEY_SE_CAM4, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_4 },
  { .se_key=KEY_SE_CAM5, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_5 },
  { .se_key=KEY_SE_CAM6, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_6 },
  { .se_key=KEY_SE_CAM7, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_7 },
  { .se_key=KEY_SE_CAM8, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_8 },
  { .se_key=KEY_SE_CAM9, .kbd_mod=KEY_MOD_LALT, .kbd_key=KEY_9 }
};


#define KEYMAP_SIZE (sizeof(keymap)/sizeof(keymap_t))



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
Adafruit_USBD_HID usb_se(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, true);
DavinciResolve_USBD_Vendor usb_vendor;


// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_kbd_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD()
};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_kbd(desc_kbd_report, sizeof(desc_kbd_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);


uint16_t usb_get_report_callback (uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
void usb_set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);



//--------------------------------------------------------
// BLEClientDevise
//--------------------------------------------------------


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

BLEClientHIDReportCharacteristic* identity_feature_report;
BLEClientHIDReportCharacteristic* leds_output_report;
BLEClientHIDReportCharacteristic* jog_input_report;
BLEClientHIDReportCharacteristic* jog_mode_output_report;
BLEClientHIDReportCharacteristic* jog_led_output_report;
BLEClientHIDReportCharacteristic* key_input_report;
BLEClientHIDReportCharacteristic* unknown_feature_report;
BLEClientHIDReportCharacteristic* auth_feature_report;
BLEClientHIDReportCharacteristic* batt_input_report;
BLEClientHIDReportCharacteristic* batt_feature_report;
BLEClientHIDReportCharacteristic* serial_feature_report;

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

BLEClientHIDReportCharacteristic* report_chrs[] ={
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

void ble_scan_callback(ble_gap_evt_adv_report_t* report);
void ble_connect_callback(uint16_t conn_handle);
void ble_pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);
void ble_connection_secured_callback(uint16_t conn_handle);
void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason);
// bool pair_passkey_callback(uint16_t conn_hdl, uint8_t const passkey[6], bool match_request);
// void pair_passkey_req_callback(uint16_t conn_hdl, uint8_t passkey[6]);
void ble_discovery(uint16_t conn_handle);

void ble_reportCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len);

typedef struct __attribute__((__packed__)) {
  uint8_t mod;
  uint8_t reserved;
  uint8_t key[6];
} hid_kbd_report_t;

#define MAX_REPORT_PAYLOAD 32
typedef struct {
  uint8_t id;
  uint8_t type;
  uint8_t len;
  uint8_t payload[MAX_REPORT_PAYLOAD];
} hid_report_t;


#define MAX_QUEUE_MESSAGE 80
QueueHandle_t msg_queue;

#define SERIAL_NUMBER_LEN 32
uint8_t SERIAL_NUMBER[SERIAL_NUMBER_LEN+1]={0};

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
  AUTHENTICATED,
  RUNNING
} state_t;




void task_running(void);


#define MAX_AUTHENTICATION_RETRY       (3u)
#define AUTHENTICATION_RETRY_DELAY_MS  (5000u)

static bool sed_authenticate(void);
bool authenticated;

state_t state;

void setup() {
  // Configure GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);

  state = UNKNOWN;
  authenticated= false;

  //TODO Check creation...
  msg_queue= xQueueCreate(MAX_QUEUE_MESSAGE, sizeof(hid_report_t));
  

  bond_init();

  // TinyUSBDevice.setVersion(0x0102);  
  TinyUSBDevice.setDeviceVersion(0x0102);
  //TinyUSBDevice.setLanguageDescriptor();
  // Declare SpeedEditor Private Interfaces
  usb_vendor.begin();

  // Declare SpeedEditor HID interface
  usb_se.enableOutEndpoint(true);
  usb_se.setPollInterval(2);
  usb_se.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_se.setStringDescriptor("SpeedEditorRemapper");
  usb_se.setReportCallback(usb_get_report_callback, usb_set_report_callback);
  usb_se.begin();

  usb_kbd.setStringDescriptor("Remapped Keyboard");
  usb_kbd.begin();


  // Start Debug Serial Interface
  Serial1.begin(115200);

  PRINTF("\nSpeedEditorRemapper\n");


  // clear bonds if BUTTON A is pressed
  uint32_t time= millis();
  while(millis()-time<2000)
  {
    if (0 == digitalRead(PIN_BUTTON1))
    {
      PRINTF("[BLE] Clearing bonds\n");
      Bluefruit.Central.clearBonds();
      for(int t=0; t<4; ++t)
      {
        digitalWrite(LED_BUILTIN, 0);
        delay(250);
        digitalWrite(LED_BUILTIN, 1);
        delay(250);
      }
      break;
    }
  }
  Bluefruit.configCentralBandwidth(BANDWIDTH_MAX);


  Bluefruit.Security.setIOCaps(false,false,false);
  // // Bluefruit.Security.setMITM(true);
  // Bluefruit.Security.setPairPasskeyCallback(pair_passkey_callback);
  // Set complete callback to print the pairing result
  Bluefruit.Security.setPairCompleteCallback(ble_pairing_complete_callback);
  // Set connection secured callback, invoked when connection is encrypted
  Bluefruit.Security.setSecuredCallback(ble_connection_secured_callback);
  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(ble_connect_callback);
  Bluefruit.Central.setDisconnectCallback(ble_disconnect_callback);


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
  Bluefruit.Scanner.setRxCallback(ble_scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(BLEUuid(0x1812));   // only report HID service  
  Bluefruit.Scanner.useActiveScan(false);
  
  state = SCANNING;

  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds
  PRINTF("[BLE] Scanning...\n");
  TinyUSBDevice.detach();

}

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


static bool led_state = false;
static uint32_t last_heartbeat_time=0;
void loop()
{

  uint32_t now= millis();
  if(now-last_heartbeat_time>250)
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
    PRINTF("[USB] Detaching...\n");
    state = DETACHING;
    break;
  case DETACHING:
    PRINTF("[USB] Detached...\n");
    state = DETACHED;
    break;
  case DETACHED:
    PRINTF("[USB] Resetting MCU...\n");
    delay(500);
    reset_mcu();
    break;
  case READY:
  {
    delay(500);

    // Resetting msg queue;
    xQueueReset(msg_queue);

    // Trying to authenticate SpeedEditor
    PRINTF("[SED] SpeedEditor Authentication...\n");

    uint8_t retry=MAX_AUTHENTICATION_RETRY;
    do
    {
      if(retry<MAX_AUTHENTICATION_RETRY)
      {
        PRINTF("[SED] Retrying...\n");
        delay(AUTHENTICATION_RETRY_DELAY_MS);
      }
      --retry;
      authenticated = sed_authenticate();
      if(authenticated){
        PRINTF("[SED] Authentication...OK\n");
      } else {
        PRINTF("[SED] Authentication...FAILED\n");
      }
    } while(retry>0 && !authenticated);
        
    if(serial_feature_report->read(SERIAL_NUMBER, SERIAL_NUMBER_LEN)==SERIAL_NUMBER_LEN) {
      PRINTF("[SED] SERIAL: %s\n\n", SERIAL_NUMBER);
      TinyUSBDevice.setSerialDescriptor((const char*)SERIAL_NUMBER);
    } else {
      TinyUSBDevice.setSerialDescriptor(nullptr);
    }

    PRINTF("[USB] Attaching...");
    if(TinyUSBDevice.attach()) {
      PRINTF("OK\n");
    }
    else
    {
      PRINTF("FAILED\n");
    }
    state = ATTACHING;
    break;
  }
  case ATTACHING:
  {
    if(TinyUSBDevice.mounted()) {
      PRINTF("[USB] Enumerated...\n");
      state = ATTACHED;
    }
    break;
  }
  case ATTACHED:
    if(authenticated) {
      state = AUTHENTICATED;
    }
    break;
  case AUTHENTICATED:
  {
    if(key_input_report->canNotify()) {
      key_input_report->setNotifyCallback(ble_reportCallback, false);
      PRINTF("[BLE] Enabling Key notifications (report %d-%d)\n", key_input_report->reportId(), key_input_report->reportType());
      key_input_report->enableNotify();
      delay(30);
    }
    if(jog_input_report->canNotify()) {
      jog_input_report->setNotifyCallback(ble_reportCallback, false);
      PRINTF("[BLE] Enabling Jog notifications (report %d-%d)\n", jog_input_report->reportId(), jog_input_report->reportType());
      jog_input_report->enableNotify();
      delay(30);
    }
    if(batt_input_report->canNotify()) {
      batt_input_report->setNotifyCallback(ble_reportCallback, false);
      PRINTF("[BLE] Enabling Batt notifications (report %d-%d)\n", batt_input_report->reportId(), batt_input_report->reportType());
      batt_input_report->enableNotify();
      delay(30);
    }
    PRINTF("\nAll set - ready to go !!\n");
    delay(100);
    state = RUNNING;
    break;
  }
  case RUNNING:
  {
    task_running();
    break;
  }
  default:
    break;
  }
  yield();
}



uint16_t  key_se[2][6] = {0};
keymap_t* key_kbd[2][6] = {0};

uint16_t* pcur_key_se=key_se[0];
uint16_t* pprv_key_se=key_se[1];

keymap_t** pcur_key_kbd=key_kbd[0];  
keymap_t** pprv_key_kbd=key_kbd[1];

hid_report_t report;

void task_running(void)
{
  // Read pending message from queue
  if(xQueueReceive(msg_queue, &report, 0) == pdFALSE) return;

  if(report.id==4 && report.type==1 && report.len!=0 && usb_se.ready())
  {
    // Process key report:
    // report_id=4
    // payload  =6*uint16_t (keycodes in little-endian format)
    // Filter keys based on remapping configuration.

    // Swap SE key buffers and clear the new current buffer;
    uint16_t *tmp_se;
    tmp_se= pprv_key_se;
    pprv_key_se= pcur_key_se;
    pcur_key_se= tmp_se;
    memclr(pcur_key_se,6*sizeof(uint16_t));

    // Swap the KBD key buffers and clear the new current buffer;
    keymap_t** tmp_kbd= pprv_key_kbd;
    pprv_key_kbd= pcur_key_kbd;
    pcur_key_kbd= tmp_kbd;
    memclr(pcur_key_kbd,6*sizeof(keymap_t*));
      
    // Fill current buffers with new keys.
    uint16_t* pkey_se= pcur_key_se;
    keymap_t** pkey_kbd= pcur_key_kbd;
    uint16_t key;
    uint16_t *pkey=(uint16_t*)report.payload;
    for(size_t t=0; t<6; ++t,++pkey)
    {
      key=*pkey;
      if(!key) continue;
      keymap_t* remapped=0;
      for(size_t u=0; !remapped && u<KEYMAP_SIZE; ++u)
      {
        if(key == keymap[u].se_key) remapped= &keymap[u];
      }
      if(remapped) {
        *pkey_kbd++=remapped;
      } else {
        *pkey_se++=key;
      }
    }
    bool comm_led=led_state;
    // Send SpeedEditor key report
    if(usb_se.ready() && memcmp(pcur_key_se, pprv_key_se, 6*sizeof(uint16_t))!=0)
    {
      comm_led=!led_state;
      digitalWrite(LED_BUILTIN, comm_led);
      last_heartbeat_time=millis()-50;
      usb_se.sendReport(report.id, pcur_key_se, 6*sizeof(uint16_t));
      PRINTF("[USB] SED: ");
      for(int t=0; t<6; ++t) PRINTF("%04X ",pcur_key_se[t]);
      PRINTF("\n");
    }
    // Send kbd report
    if(usb_kbd.ready() && memcmp(pcur_key_kbd, pprv_key_kbd, 6*sizeof(keymap_t*))!=0)
    {
      comm_led=!led_state;
      digitalWrite(LED_BUILTIN, comm_led);
      last_heartbeat_time=millis()-50;
      hid_kbd_report_t kbd_report= {0};
      pkey_kbd= pcur_key_kbd;
      int u=0;
      for(int t=0; t<6; ++t, ++pkey_kbd)
      {
        if(!(*pkey_kbd)) continue;
        kbd_report.mod |= (*pkey_kbd)->kbd_mod;
        kbd_report.key[u++]= (*pkey_kbd)->kbd_key;
      }
      usb_kbd.sendReport(0, &kbd_report, sizeof(hid_kbd_report_t));
      PRINTF("[USB] KBD: %02X - ",kbd_report.mod);
      for(int t=0; t<6; ++t) PRINTF("0x%02X ",kbd_report.key[t]);
      PRINTF("\n");
    }
    led_state = comm_led;
    return;
  }

  // Forward other reports
  // jog input reports:
  //   report.id=3
  //   report.type=1
  //   report.len=6
  // battery input reports:
  //   report.id=7
  //   report.type=1 
  //   report.len=2

  last_heartbeat_time= millis()-50;
  led_state=!led_state;
  digitalWrite(LED_BUILTIN, led_state);
  usb_se.sendReport(report.id, report.payload, report.len);
}


void ble_discovery(uint16_t conn_handle)
{
  PRINTF("[BLE] Discovery...\n\n");

  PRINTF("[BLE] Device Information Service...");
  if ( clientDis.discover(conn_handle) )
  {
    PRINTF("FOUND\n");
    char buffer[64+1];
    
    memset(buffer, 0, sizeof(buffer));
    uint16_t count;
    if ( (count=clientDis.getChars(UUID16_CHR_PNP_ID, buffer, sizeof(buffer)))!=0 )
    {
      PRINTF("[DIS] PNP_ID:\n");
      PRINTF("[DIS]   src: %d\n",   *(uint8_t*)(&buffer[0]));
      PRINTF("[DIS]   vid: %04X\n", *(uint16_t*)(&buffer[1]));
      PRINTF("[DIS]   pid: %04X\n", *(uint16_t*)(&buffer[3]));
      PRINTF("[DIS]   ver: %04X\n", *(uint16_t*)(&buffer[5]));
      // TODO Check VID/PID
    }

    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      PRINTF("[DIS] Manufacturer: %s\n", buffer);
    }
    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      PRINTF("[DIS] Model: %s\n", buffer);
    }
  }else
  {
    PRINTF("...NOT FOUND\n");
  }
  PRINTF("\n");

  PRINTF("[BLE] HID Service...");
  if ( clientHid.discover(conn_handle) )
  {
    PRINTF("FOUND\n");

    uint16_t count;

    count = Bluefruit.Discovery.discoverCharacteristic(conn_handle, chrs, sizeof(chrs)/sizeof(chrs[0]));
    PRINTF("[HID] Found %d characteristics\n", count);
    PRINTF("[HID] -HIDInfo\n");
    PRINTF("[HID] -HIDReportMap\n");
    for(int t=0; t<11; ++t)
    {
      if(chrs[t]->discovered())
      {
        delay(10); // Needed to give some time to the stack to finilized its internal processing.
        BLEClientHIDReportCharacteristic* pchr= (BLEClientHIDReportCharacteristic*)chrs[t];
        PRINTF("[HID] -HIDReport[%d]: %d - %d\n", t, pchr->reportId(), pchr->reportType());
        if(pchr->reportId()==1 && pchr->reportType()==3) identity_feature_report= pchr;
        if(pchr->reportId()==2 && pchr->reportType()==2) leds_output_report= pchr;
        if(pchr->reportId()==3 && pchr->reportType()==1) jog_input_report= pchr;
        if(pchr->reportId()==3 && pchr->reportType()==2) jog_mode_output_report= pchr;
        if(pchr->reportId()==4 && pchr->reportType()==1) key_input_report= pchr;
        if(pchr->reportId()==4 && pchr->reportType()==2) jog_led_output_report= pchr;
        if(pchr->reportId()==5 && pchr->reportType()==3) unknown_feature_report= pchr;
        if(pchr->reportId()==6 && pchr->reportType()==3) auth_feature_report= pchr;
        if(pchr->reportId()==7 && pchr->reportType()==1) batt_input_report= pchr;
        if(pchr->reportId()==7 && pchr->reportType()==3) batt_feature_report= pchr;
        if(pchr->reportId()==8 && pchr->reportType()==3) serial_feature_report= pchr;
      }
    }
    delay(10); // Needed to give some time to the stack to finilized its internal processing.
  }else
  {
    PRINTF("FAILED\n");
  }
  PRINTF("\n");
  

  PRINTF("[BLE] Battery Service...");
  if ( clientBas.discover(conn_handle) ) {
    PRINTF("FOUND\n");
  } else {
    PRINTF("NOT FOUND\n");
  }
  PRINTF("\n");

  delay(100);
  BLEConnection* conn = Bluefruit.Connection( conn_handle );
  uint16_t mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL);
  PRINTF("[BLE] Requesting MTU=%d  ...", mtu);
  if(!conn->requestMtuExchange(mtu)) {
    PRINTF("FAILED\n");
  } else {
    PRINTF("OK\n");
  }
  delay(100);
  PRINTF("[BLE] Requesting Connection parameters update...");
  if(!conn->requestConnectionParameter(6)) {
    PRINTF("FAILED\n");
  } else {
    PRINTF("OK\n");
  }
  PRINTF("\n");

  state = READY;
  PRINTF("[BLE] SpeedEditor Client configured...\n\n");
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t usb_get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{

  if(state < READY) return 0;

  uint16_t len=0;
  for(int t=0; t<11; ++t)
  {
    if(report_chrs[t]->reportId() == report_id && report_chrs[t]->reportType() == report_type )
    {
      delay(15);
      len = report_chrs[t]->read(buffer, reqlen);
      break;
    }
  }
  PRINTF("[USB] getReport: %d - %d: ", report_id, report_type, reqlen);
  for(int t=0; t<len; ++t)
  {
    PRINTF("%02X ", buffer[t]);
  }
  PRINTF("\n");
 
  if(state!=RUNNING && report_id==6 && report_type==3 && buffer[0]==0x04 && buffer[1]==0x58 && buffer[2]==0x02)
  {
    state = AUTHENTICATED;
  }

  return len;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void usb_set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
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
    if(report_chrs[t]->reportId() == report_id && report_chrs[t]->reportType() == report_type )
    {
      delay(15);
      report_chrs[t]->write_resp(buffer, bufsize);
      break;
    }
  }

  PRINTF("[USB] setReport: %d - %d: ", report_id, report_type, bufsize);
  for(int t=0; t<bufsize; ++t) {
    PRINTF("%02X ", buffer[t]);
  }
  PRINTF("\n");
}




/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void ble_scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Connect to device with bleuart service in advertising
  Bluefruit.Central.connect(report);
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void ble_connect_callback(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  PRINTF("[BLE] Connected\n");

  // HID device mostly require pairing/bonding
  if(!conn->bonded()) {
    PRINTF("[BLE] Request pairing...\n");
    conn->requestPairing();
  }
  else
  {
    PRINTF("[BLE] Bonded...\n");
  }
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  PRINTF("[BLE] Disconnected, reason = 0x%02X\n",reason);
  state = DISCONNECTED;
}


void ble_pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if (auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
  {
    PRINTF("[BLE] Pairing...FAILED\n");

    // disconnect
    conn->disconnect();
    return;
  }

  PRINTF("[BLE] Pairing...OK\n\n");

  ble_discovery(conn_handle);
}



void ble_connection_secured_callback(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if ( !conn->secured() )
  {
    // It is possible that connection is still not secured by this time.
    // This happens (central only) when we try to encrypt connection using stored bond keys
    // but peer reject it (probably it remove its stored key).
    // Therefore we will request an pairing again --> callback again when encrypted
    PRINTF("[BLE] Not secured - request pairing");  
    conn->requestPairing();
  }
  else
  {
    PRINTF("[BLE] Secured...\n");
    if(conn->bonded())
    {

      BLEConnection* conn = Bluefruit.Connection( conn_handle );
      bond_keys_t key;
      conn->loadBondKey(&key);

      uint8_t klen = key.own_enc.enc_info.ltk_len;
      uint8_t kauth= key.own_enc.enc_info.auth;
      uint8_t klesc= key.own_enc.enc_info.lesc;
      PRINTF("[BLE] -LTK[%d:%d:%d]: 0x",klen,kauth,klesc);
      for(int t=klen-1; t>=0; --t)
      {
        PRINTF("%02X",key.own_enc.enc_info.ltk[t]);
      }
      PRINTF("\n\n");


      ble_discovery(conn_handle);
    }
  }
}



void ble_reportCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  BLEClientHIDReportCharacteristic* report_chr= (BLEClientHIDReportCharacteristic*)chr;
  hid_report_t report;
  //TODO filter reports
  PRINTF("[BLE] notif: %d - %d: ", report_chr->reportId(), report_chr->reportType());
  for(int t=0; t<len; ++t) PRINTF("%02X ", data[t]);
  PRINTF("\n");

  if(state!=RUNNING || len>=MAX_REPORT_PAYLOAD) {
    PRINTF("[BLE] notif ignored\n");
    return;
  }

  report.id = report_chr->reportId();
  report.type = report_chr->reportType();
  report.len = len;
  memcpy(report.payload, data, len);
  xQueueSend(msg_queue, &report, 10*portTICK_PERIOD_MS);
}



static const uint64_t	AUTH_EVEN_TBL[] = {
		0x3ae1206f97c10bc8,
		0x2a9ab32bebf244c6,
		0x20a6f8b8df9adf0a,
		0xaf80ece52cfc1719,
		0xec2ee2f7414fd151,
		0xb055adfd73344a15,
		0xa63d2e3059001187,
		0x751bf623f42e0dde
};

static const uint64_t AUTH_ODD_TBL[] = {
		0x3e22b34f502e7fde,
		0x24656b981875ab1c,
		0xa17f3456df7bf8c3,
		0x6df72e1941aef698,
		0x72226f011e66ab94,
		0x3831a3c606296b42,
		0xfd7ff81881332c89,
		0x61a3f6474ff236c6
};

static const uint64_t MASK = 0xa79a63f585d37bf0;


static uint64_t rol8(uint64_t v)
{
	return ((v << 56) | (v >> 8)) & 0xffffffffffffffff;
}

static uint64_t rol8n(uint64_t v, uint16_t n)
{
	for(uint16_t i=0; i<n; ++i) v = rol8(v);
	return v;
}


static uint64_t compute_auth(uint64_t challenge)
{
	uint16_t n = challenge & 7;
	uint64_t v = rol8n(challenge, n);
  uint64_t k;

	if( (v & 1) == ((0x78 >> n) & 1) )
  {
		k = AUTH_EVEN_TBL[n];
  }
	else
  {
		v = v ^ rol8(v);
		k = AUTH_ODD_TBL[n];
  }
	return v ^ (rol8(v) & MASK) ^ k;
}


typedef struct __attribute__((packed)) {
  uint8_t  id;
  union {
    uint64_t value;
    uint8_t  bytes[8];
    uint16_t words[4];
    uint32_t dwords[2];
  };
} challenge_t;


static bool sed_authenticate(void)
{
  challenge_t token;
  challenge_t challenge;

  // Clear challenge FSM
  
  token.id=0;
  token.value=0;
  PRINTF("[SED] Resetting Auth FSM   : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&token, sizeof(challenge_t));

  
  delay(40);
  auth_feature_report->read(&challenge, sizeof(challenge_t));
  PRINTF("[SED] Reading kbd challenge: %02X - %08lX%08lX\n", challenge.id, challenge.dwords[1], challenge.dwords[0]);
  if(challenge.id!=0)
  {
    return false;
  }

  token.id= 1;
  token.value=0;
  PRINTF("[SED] Sending our challenge: %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&token, sizeof(challenge_t));

  
  delay(40);
  auth_feature_report->read(&token, sizeof(challenge_t));
  PRINTF("[SED] Reading kbd response : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  if(token.id!=2)
  {
    return false;
  }

  challenge.id = 3;
  challenge.value = compute_auth(challenge.value);
  PRINTF("[SED] Sending our response : %02X - %08lX%08lX\n", challenge.id, challenge.dwords[1], challenge.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&challenge, sizeof(challenge_t));

  delay(40);
  auth_feature_report->read(&token, sizeof(challenge_t));
  PRINTF("[SED] Reading Auth result  : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  if(token.id!=4)
  {
    return false;
  }
  return true;
}


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
