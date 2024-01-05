#ifndef __CONFIG_HID_DESC_H__
#define __CONFIG_HID_DESC_H__



#define TUD_HID_REPORT_DESC_CONFIG(id,size) \
    0x06, 0x00, 0xFF,       /* Usage Page (Vendor Defined 0xFF00)                                                    */ \
    0x09, 0x20,             /* Usage (0x20)                                                                          */ \
    0xA1, 0x01,             /* Collection (Application)                                                              */ \
    0x09, 0x20,             /*   Usage (0x20)                                                                        */ \
    0x85, id,               /*   Report ID (REPORT_ID_CONFIG)                                                        */ \
    0x75, 0x08,             /*   Report Size (8)                                                                     */ \
    0x95, size,             /*   Report Count (CONFIG_SIZE)                                                          */ \
    0xB1, 0x02,             /*   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile) */ \
    0xC0                    /* End Collection                                                                        */ \


#endif