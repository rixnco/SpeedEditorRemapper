#include <Arduino.h>


#include "Adafruit_TinyUSB.h"
#include <bluefruit.h>
#include "HardwareSerial.h"
#include "BLEClientHIDReportCharacteristic.h"

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

using namespace Adafruit_LittleFS_Namespace;


#include "debug.h"

#include "hid_key.h"
#include "sed_key.h"
#include "sed_hid_desc.h"
#include "config_hid_desc.h"
#include "DavinciResolve_USBD_Vendor.h"
#include "sed_auth.h"

#include "Cli.h"

const char* banner =
"/  ___|                   | |  ___|  | (_) |            \r\n"
"\\ `--. _ __   ___  ___  __| | |__  __| |_| |_ ___  _ __ \r\n"
" `--. \\ '_ \\ / _ \\/ _ \\/ _` |  __|/ _` | | __/ _ \\| '__|\r\n"
"/\\__/ / |_) |  __/  __/ (_| | |__| (_| | | || (_) | |   \r\n"
"\\____/| .__/ \\___|\\___|\\__,_\\____/\\__,_|_|\\__\\___/|_|   \r\n"
"      | |                                               \r\n"
"      |_|                                               \r\n"
" _   _       _                _              _          \r\n"
"| | | |     | |              | |            | |         \r\n"
"| | | |_ __ | | ___  __ _ ___| |__   ___  __| |         \r\n"
"| | | | '_ \\| |/ _ \\/ _` / __| '_ \\ / _ \\/ _` |         \r\n"
"| |_| | | | | |  __/ (_| \\__ \\ | | |  __/ (_| |         \r\n"
" \\___/|_| |_|_|\\___|\\__,_|___/_| |_|\\___|\\__,_|         \r\n"
"\r\n";




typedef struct {
  uint32_t tap_hold_threshold_ms;
} settings_t;


typedef struct {
  uint32_t input;
  uint32_t output;
  uint16_t layers;
  uint16_t flags;
} mapping_t;

typedef struct {
  uint32_t MAGIC;
  uint16_t crc;
  uint16_t version;
  uint32_t len;
} config_t;



typedef struct {
  uint16_t se_key;
  uint8_t  kbd_mod;
  uint8_t  kbd_key;
} keymap_t;

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

#define SETTINGS_FILE "/settings.cfg"
File settings(InternalFS);


// HID report descriptor using TinyUSB's template
// Blackmagic Speed Editor Keyboard
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_SPEED_EDITOR()
};

// USB HID object. For ESP32 these values cannot be changed after this declaration
// desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_se(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, true);
DavinciResolve_USBD_Vendor usb_vendor;


// HID report descriptor using TinyUSB's template
// Keyboard Single Report (no ID) descriptor
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


#if defined(BOARD_ADAFRUIT_FEATHER_NRF52840)
#define LED_STATE_OFF (!LED_STATE_ON)
#elif defined(BOARD_MDK_NRF52840_DONGLE)

#else
#error Unsupported board
#endif



class MyCmdHandler : public CliCmdHandler{
public:
  void process(const char* cmd, Print *output, Cli* cli) {
    output->printf("Hello\n");
  };
};

MyCmdHandler cmd_handler;

Cli cli(6,255,&cmd_handler, "$ ",banner);


void reset_mcu(uint32_t gpregret=0);
void task_running(void);


#define MAX_AUTHENTICATION_RETRY       (3u)
#define AUTHENTICATION_RETRY_DELAY_MS  (5000u)

static bool sed_authentication(void);
bool authenticated;

state_t state;

void setup() {
  // Configure GPIOs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_STATE_ON);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);

  // Start Debug Serial Interface
  DBG_INIT();
  DBG_PRINT("\nSpeedEditor Unleashed\n");

  // Initialize Internal File System
  InternalFS.begin();

  settings.open(SETTINGS_FILE, FILE_O_READ);
  // file existed
  if ( settings )
  {
    DBG_PRINTLN("Settings found...");
    
    // uint32_t readlen;
    // char buffer[64] = { 0 };
    // readlen = settings.read(buffer, sizeof(buffer));

    // buffer[readlen] = 0;
    // DBG_PRINTLN(buffer);

    settings.close();
  }else
  {
    DBG_PRINTLN("Settings not found...\r\n   Using defaults...");

    if( settings.open(SETTINGS_FILE, FILE_O_WRITE) )
    {
      DBG_PRINTLN("OK");
      // settings.write(SETTINGS_FILE, strlen(SETTINGS_FILE));
      settings.close();
    }else
    {
      DBG_PRINTLN("Failed!");
    }
  }


  DBG_PRINT("[SED] Initialization...\r\n");
  state = UNKNOWN;
  authenticated= false;

  //TODO Check creation...
  msg_queue= xQueueCreate(MAX_QUEUE_MESSAGE, sizeof(hid_report_t));

  DBG_PRINT("[BLE] Checking bonds\r\n");
  bond_init();


  // clear bonds if BUTTON A is pressed
  uint32_t time= millis();
  while(millis()-time<2000)
  {
    if (0 == digitalRead(PIN_BUTTON1))
    {
      DBG_PRINTF("[BLE] Clearing bonds\n");
      Bluefruit.Central.clearBonds();
      for(int t=0; t<4; ++t)
      {
        digitalWrite(LED_BUILTIN, LED_STATE_OFF);
        delay(250);
        digitalWrite(LED_BUILTIN, LED_STATE_ON);
        delay(250);
      }
      break;
    }
    yield();
    if (serialEvent && serialEventRun) serialEventRun();
  }
  digitalWrite(LED_BUILTIN, LED_STATE_OFF);

  //********************************
  // Configure USB stuff
  //********************************
  
  TinyUSBDevice.begin();

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

  // Declare HID Keyboard
  usb_kbd.setStringDescriptor("Remapped Keyboard");
  usb_kbd.begin();

  // Declare Serial (CDC)
  Serial.begin(115200);


  //********************************
  // Configure BLE stuff
  //********************************

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
  DBG_PRINTF("[BLE] Scanning...\r\n");


  //********************************
  // Start cli
  //********************************
  cli.begin(&Serial);

  DBG_PRINT("[SED] Initialization...DONE\r\n");
}

static bool led_state = LED_STATE_OFF;
static uint32_t last_heartbeat_time=0;
void loop()
{

  if(Serial.dtr()) {
    cli.update();
  } else {
    cli.reset();
  }

  uint32_t now= millis();
  if((state >= READY) && (now-last_heartbeat_time > 250))
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
    DBG_PRINTF("[USB] Detaching...\n");
    state = DETACHING;
    break;
  case DETACHING:
    DBG_PRINTF("[USB] Detached...\n");
    state = DETACHED;
    break;
  case DETACHED:
    DBG_PRINTF("[USB] Resetting MCU...\n");
    delay(500);
    reset_mcu();
    break;
  case READY:
  {
    delay(500);

    // Resetting msg queue;
    xQueueReset(msg_queue);

    // Trying to authenticate SpeedEditor
    DBG_PRINTF("[SED] SpeedEditor Authentication...\n");

    uint8_t retry=MAX_AUTHENTICATION_RETRY;
    do
    {
      if(retry<MAX_AUTHENTICATION_RETRY)
      {
        DBG_PRINTF("[SED] Retrying...\n");
        delay(AUTHENTICATION_RETRY_DELAY_MS);
      }
      --retry;
      authenticated = sed_authentication();
      if(authenticated){
        DBG_PRINTF("[SED] Authentication...OK\n");
      } else {
        DBG_PRINTF("[SED] Authentication...FAILED\n");
      }
    } while(retry>0 && !authenticated);
        
    if(serial_feature_report->read(SERIAL_NUMBER, SERIAL_NUMBER_LEN)==SERIAL_NUMBER_LEN) {
      DBG_PRINTF("[SED] SERIAL: %s\n\n", SERIAL_NUMBER);
      TinyUSBDevice.setSerialDescriptor((const char*)SERIAL_NUMBER);
    } else {
      TinyUSBDevice.setSerialDescriptor(nullptr);
    }

    DBG_PRINTF("[USB] Attaching...");
    if(TinyUSBDevice.attach()) {
      DBG_PRINTF("OK\n");
    }
    else
    {
      DBG_PRINTF("FAILED\n");
    }
    state = ATTACHING;
    break;
  }
  case ATTACHING:
  {
    if(TinyUSBDevice.mounted()) {
      DBG_PRINTF("[USB] Enumerated...\n");
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
      DBG_PRINTF("[BLE] Enabling Key notifications (report %d-%d)\n", key_input_report->reportId(), key_input_report->reportType());
      key_input_report->enableNotify();
      delay(30);
    }
    if(jog_input_report->canNotify()) {
      jog_input_report->setNotifyCallback(ble_reportCallback, false);
      DBG_PRINTF("[BLE] Enabling Jog notifications (report %d-%d)\n", jog_input_report->reportId(), jog_input_report->reportType());
      jog_input_report->enableNotify();
      delay(30);
    }
    if(batt_input_report->canNotify()) {
      batt_input_report->setNotifyCallback(ble_reportCallback, false);
      DBG_PRINTF("[BLE] Enabling Batt notifications (report %d-%d)\n", batt_input_report->reportId(), batt_input_report->reportType());
      batt_input_report->enableNotify();
      delay(30);
    }
    DBG_PRINTF("\nAll set - ready to go !!\n");
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
      DBG_PRINTF("[USB] SED: ");
      for(int t=0; t<6; ++t) DBG_PRINTF("%04X ",pcur_key_se[t]);
      DBG_PRINTF("\n");
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
      DBG_PRINTF("[USB] KBD: %02X - ",kbd_report.mod);
      for(int t=0; t<6; ++t) DBG_PRINTF("0x%02X ",kbd_report.key[t]);
      DBG_PRINTF("\n");
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
  DBG_PRINTF("[BLE] Discovery...\n\n");

  DBG_PRINTF("[BLE] Device Information Service...");
  if ( clientDis.discover(conn_handle) )
  {
    DBG_PRINTF("FOUND\n");
    char buffer[64+1];
    
    memset(buffer, 0, sizeof(buffer));
    uint16_t count;
    if ( (count=clientDis.getChars(UUID16_CHR_PNP_ID, buffer, sizeof(buffer)))!=0 )
    {
      DBG_PRINTF("[DIS] PNP_ID:\n");
      DBG_PRINTF("[DIS]   src: %d\n",   *(uint8_t*)(&buffer[0]));
      DBG_PRINTF("[DIS]   vid: %04X\n", *(uint16_t*)(&buffer[1]));
      DBG_PRINTF("[DIS]   pid: %04X\n", *(uint16_t*)(&buffer[3]));
      DBG_PRINTF("[DIS]   ver: %04X\n", *(uint16_t*)(&buffer[5]));
      // TODO Check VID/PID
    }

    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      DBG_PRINTF("[DIS] Manufacturer: %s\n", buffer);
    }
    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      DBG_PRINTF("[DIS] Model: %s\n", buffer);
    }
  }else
  {
    DBG_PRINTF("...NOT FOUND\n");
  }
  DBG_PRINTF("\n");

  DBG_PRINTF("[BLE] HID Service...");
  if ( clientHid.discover(conn_handle) )
  {
    DBG_PRINTF("FOUND\n");

    uint16_t count;

    count = Bluefruit.Discovery.discoverCharacteristic(conn_handle, chrs, sizeof(chrs)/sizeof(chrs[0]));
    DBG_PRINTF("[HID] Found %d characteristics\n", count);
    DBG_PRINTF("[HID] -HIDInfo\n");
    DBG_PRINTF("[HID] -HIDReportMap\n");
    for(int t=0; t<11; ++t)
    {
      if(chrs[t]->discovered())
      {
        delay(10); // Needed to give some time to the stack to finilized its internal processing.
        BLEClientHIDReportCharacteristic* pchr= (BLEClientHIDReportCharacteristic*)chrs[t];
        DBG_PRINTF("[HID] -HIDReport[%d]: %d - %d\n", t, pchr->reportId(), pchr->reportType());
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
    DBG_PRINTF("FAILED\n");
  }
  DBG_PRINTF("\n");
  

  DBG_PRINTF("[BLE] Battery Service...");
  if ( clientBas.discover(conn_handle) ) {
    DBG_PRINTF("FOUND\n");
  } else {
    DBG_PRINTF("NOT FOUND\n");
  }
  DBG_PRINTF("\n");

  delay(100);
  BLEConnection* conn = Bluefruit.Connection( conn_handle );
  uint16_t mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_CENTRAL);
  DBG_PRINTF("[BLE] Requesting MTU=%d  ...", mtu);
  if(!conn->requestMtuExchange(mtu)) {
    DBG_PRINTF("FAILED\n");
  } else {
    DBG_PRINTF("OK\n");
  }
  delay(100);
  DBG_PRINTF("[BLE] Requesting Connection parameters update...");
  if(!conn->requestConnectionParameter(6)) {
    DBG_PRINTF("FAILED\n");
  } else {
    DBG_PRINTF("OK\n");
  }
  DBG_PRINTF("\n");

  state = READY;
  DBG_PRINTF("[BLE] SpeedEditor Client configured...\n\n");
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t usb_get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{

  DBG_PRINTF("[USB] getReport: %u - %u - %u \n", report_id, report_type, reqlen);
  if(state < READY) {
    DBG_PRINTF("[USB] getReport: %u - %u - %u - IGNORED\n", report_id, report_type, reqlen);
    return 0;
  }

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
  DBG_PRINTF("[USB] getReport: %d - %d: ", report_id, report_type);
  for(int t=0; t<len; ++t)
  {
    DBG_PRINTF("%02X ", buffer[t]);
  }
  DBG_PRINTF("\n");
 
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
  
  if(state < READY) 
  {
    DBG_PRINTF("[USB] setReport: %u - %u - %u - IGNORED\n", report_id, report_type, bufsize);
    return;
  }

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

  DBG_PRINTF("[USB] setReport: %d - %d: ", report_id, report_type);
  for(int t=0; t<bufsize; ++t) {
    DBG_PRINTF("%02X ", buffer[t]);
  }
  DBG_PRINTF("\n");
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

  DBG_PRINTF("[BLE] Connected\n");

  // HID device mostly require pairing/bonding
  if(!conn->bonded()) {
    DBG_PRINTF("[BLE] Request pairing...\n");
    conn->requestPairing();
  }
  else
  {
    DBG_PRINTF("[BLE] Bonded...\n");
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
  DBG_PRINTF("[BLE] Disconnected, reason = 0x%02X\n",reason);
  state = DISCONNECTED;
}


void ble_pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  if (auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
  {
    DBG_PRINTF("[BLE] Pairing...FAILED\n");

    // disconnect
    conn->disconnect();
    return;
  }

  DBG_PRINTF("[BLE] Pairing...OK\n\n");

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
    DBG_PRINTF("[BLE] Not secured - request pairing");  
    conn->requestPairing();
  }
  else
  {
    DBG_PRINTF("[BLE] Secured...\n");
    if(conn->bonded())
    {

      BLEConnection* conn = Bluefruit.Connection( conn_handle );
      bond_keys_t key;
      conn->loadBondKey(&key);

      uint8_t klen = key.own_enc.enc_info.ltk_len;
      uint8_t kauth= key.own_enc.enc_info.auth;
      uint8_t klesc= key.own_enc.enc_info.lesc;
      DBG_PRINTF("[BLE] -LTK[%d:%d:%d]: 0x",klen,kauth,klesc);
      for(int t=klen-1; t>=0; --t)
      {
        DBG_PRINTF("%02X",key.own_enc.enc_info.ltk[t]);
      }
      DBG_PRINTF("\n\n");


      ble_discovery(conn_handle);
    }
  }
}



void ble_reportCallback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  BLEClientHIDReportCharacteristic* report_chr= (BLEClientHIDReportCharacteristic*)chr;
  hid_report_t report;
  //TODO filter reports
  DBG_PRINTF("[BLE] notif: %d - %d: ", report_chr->reportId(), report_chr->reportType());
  for(int t=0; t<len; ++t) DBG_PRINTF("%02X ", data[t]);
  DBG_PRINTF("\n");

  if(state!=RUNNING || len>=MAX_REPORT_PAYLOAD) {
    DBG_PRINTF("[BLE] notif ignored\n");
    return;
  }

  report.id = report_chr->reportId();
  report.type = report_chr->reportType();
  report.len = len;
  memcpy(report.payload, data, len);
  xQueueSend(msg_queue, &report, 10*portTICK_PERIOD_MS);
}





static bool sed_authentication(void)
{
  challenge_t token;
  challenge_t challenge;

  // Clear challenge FSM
  
  token.id=0;
  token.value=0;
  DBG_PRINTF("[SED] Resetting Auth FSM   : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&token, sizeof(challenge_t));

  
  delay(40);
  auth_feature_report->read(&challenge, sizeof(challenge_t));
  DBG_PRINTF("[SED] Reading kbd challenge: %02X - %08lX%08lX\n", challenge.id, challenge.dwords[1], challenge.dwords[0]);
  if(challenge.id!=0)
  {
    return false;
  }

  token.id= 1;
  token.value=0;
  DBG_PRINTF("[SED] Sending our challenge: %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&token, sizeof(challenge_t));

  
  delay(40);
  auth_feature_report->read(&token, sizeof(challenge_t));
  DBG_PRINTF("[SED] Reading kbd response : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  if(token.id!=2)
  {
    return false;
  }

  challenge.id = 3;
  challenge.value = compute_auth(challenge.value);
  DBG_PRINTF("[SED] Sending our response : %02X - %08lX%08lX\n", challenge.id, challenge.dwords[1], challenge.dwords[0]);
  delay(40);
  auth_feature_report->write_resp(&challenge, sizeof(challenge_t));

  delay(40);
  auth_feature_report->read(&token, sizeof(challenge_t));
  DBG_PRINTF("[SED] Reading Auth result  : %02X - %08lX%08lX\n", token.id, token.dwords[1], token.dwords[0]);
  if(token.id!=4)
  {
    return false;
  }
  return true;
}

void reset_mcu(uint32_t gpregret)
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


extern "C"
{

// nanolib printf() retarget
int _write (int fd, const void *buf, size_t count)
{
  (void) fd;
  size_t ret = 0;
  if ( Serial )
  {
    ret = Serial.write((const uint8_t *) buf, count);
  }
  return (int) ret;
}

}


