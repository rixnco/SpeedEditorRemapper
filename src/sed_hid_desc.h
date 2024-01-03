#ifndef __SED_HID_REPORT_DESC_H__
#define __SED_HID_REPORT_DESC_H__


#define TUD_HID_REPORT_DESC_SPEED_EDITOR() \
0x06, 0x00, 0xFF,  /*  Usage Page (Vendor Defined 0xFF00)  */ \
0x09, 0x00,        /*  Usage (0x00)  */ \
0xA1, 0x01,        /*  Collection (Application)  */ \
0x09, 0x01,        /*    Usage (0x01)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x01,        /*      Report ID (1)  */ \
0x05, 0x06,        /*      Usage Page (Generic Dev Ctrls)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x2A,        /*      Usage (0x2A)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x04,        /*      Report Count (4)  */ \
0xB1, 0x81,        /*      Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0x09, 0x2D,        /*      Usage (0x2D)  */ \
0x09, 0x2E,        /*      Usage (0x2E)  */ \
0x09, 0x2F,        /*      Usage (0x2F)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x03,        /*      Report Count (3)  */ \
0xB1, 0x81,        /*      Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x08,        /*    Usage (0x08)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x08,        /*      Report ID (8)  */ \
0x06, 0x07, 0xFF,  /*      Usage Page (Vendor Defined 0xFF07)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x20,        /*      Report Count (32)  */ \
0xB1, 0x81,        /*      Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x02,        /*    Usage (0x02)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x03,        /*      Report ID (3)  */ \
0x06, 0x01, 0xFF,  /*      Usage Page (Vendor Defined 0xFF01)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x81, 0x02,        /*      Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x04,        /*      Report Count (4)  */ \
0x81, 0x02,        /*      Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x09, 0x02,        /*      Usage (0x02)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x81, 0x02,        /*      Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x91, 0x02,        /*      Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x04,        /*      Report Count (4)  */ \
0x91, 0x02,        /*      Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0x09, 0x02,        /*      Usage (0x02)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x91, 0x02,        /*      Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x03,        /*    Usage (0x03)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x04,        /*      Report ID (4)  */ \
0x06, 0x02, 0xFF,  /*      Usage Page (Vendor Defined 0xFF02)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x19, 0x00,        /*      Usage Minimum (0x00)  */ \
0x29, 0xFF,        /*      Usage Maximum (0xFF)  */ \
0x75, 0x10,        /*      Report Size (16)  */ \
0x95, 0x06,        /*      Report Count (6)  */ \
0x81, 0x00,        /*      Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x25, 0x01,        /*      Logical Maximum (1)  */ \
0x19, 0x01,        /*      Usage Minimum (0x01)  */ \
0x29, 0x08,        /*      Usage Maximum (0x08)  */ \
0x75, 0x01,        /*      Report Size (1)  */ \
0x95, 0x08,        /*      Report Count (8)  */ \
0x91, 0x02,        /*      Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x06,        /*    Usage (0x06)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x02,        /*      Report ID (2)  */ \
0x06, 0x05, 0xFF,  /*      Usage Page (Vendor Defined 0xFF05)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x25, 0x01,        /*      Logical Maximum (1)  */ \
0x19, 0x01,        /*      Usage Minimum (0x01)  */ \
0x29, 0x20,        /*      Usage Maximum (0x20)  */ \
0x75, 0x01,        /*      Report Size (1)  */ \
0x95, 0x20,        /*      Report Count (32)  */ \
0x91, 0x02,        /*      Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x04,        /*    Usage (0x04)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x05,        /*      Report ID (5)  */ \
0x06, 0x03, 0xFF,  /*      Usage Page (Vendor Defined 0xFF03)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0xB1, 0x82,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x04,        /*      Report Count (4)  */ \
0xB1, 0x82,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0x09, 0x02,        /*      Usage (0x02)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x02,        /*      Report Count (2)  */ \
0xB1, 0x82,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x05,        /*    Usage (0x05)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x06,        /*      Report ID (6)  */ \
0x06, 0x04, 0xFF,  /*      Usage Page (Vendor Defined 0xFF04)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0xB1, 0x82,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x08,        /*      Report Count (8)  */ \
0xB1, 0x82,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)  */ \
0xC0,              /*    End Collection  */ \
0x09, 0x07,        /*    Usage (0x07)  */ \
0xA1, 0x01,        /*    Collection (Application)  */ \
0x85, 0x07,        /*      Report ID (7)  */ \
0x06, 0x06, 0xFF,  /*      Usage Page (Vendor Defined 0xFF06)  */ \
0x15, 0x00,        /*      Logical Minimum (0)  */ \
0x26, 0xFF, 0x00,  /*      Logical Maximum (255)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x81, 0x02,        /*      Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0x81, 0x02,        /*      Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)  */ \
0x09, 0x00,        /*      Usage (0x00)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0xB1, 0x02,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0x09, 0x01,        /*      Usage (0x01)  */ \
0x75, 0x08,        /*      Report Size (8)  */ \
0x95, 0x01,        /*      Report Count (1)  */ \
0xB1, 0x02,        /*      Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)  */ \
0xC0,              /*    End Collection  */ \
0xC0,              /*  End Collection  */ \
/*  308 bytes  */ \

#endif