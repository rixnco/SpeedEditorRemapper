#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "stdint.h"
#include "stdbool.h"


#define MAPPING_FLAG_STICKY     (1<<0)
#define MAPPING_FLAG_TAG        (1<<1)
#define MAPPING_FLAG_HOLD       (1<<2)

#define MAPPING_USAGE_NONE      (0x00000000)

#define MAPPING_USAGE_KEYBOARD  (0x00070000)
#define MAPPING_USAGE_MEDIA     (0x000C0000)

#define MAPPING_USAGE_LAYER     (0xFFF10000)
#define MAPPING_USAGE_MACRO     (0xFFF20000)
#define MAPPING_USAGE_EXPR      (0xFFF30000)
#define MAPPING_USAGE_GPIO      (0xFFF40000)
#define MAPPING_USAGE_REGISTER  (0xFFF50000)
#define MAPPING_USAGE_PASSTHRU  (0xFFFE0000)



#define MAPPING_USAGE_SED_JOG_MODE  (0xFF010000)
#define MAPPING_USAGE_SED_KEY       (0xFF020000)
#define MAPPING_USAGE_SED_LED       (0xFF050000)




// #define SED_KEY_SMART_INSRT	  (MAPPING_USAGE_SED_KEY | 0x01) //SMART INSRT [CLIP]
// #define SED_KEY_APPND			    (MAPPING_USAGE_SED_KEY | 0x02) //APPND [CLIP]
// #define SED_KEY_RIPL_OWR		  (MAPPING_USAGE_SED_KEY | 0x03) //RIPL O/WR
// #define SED_KEY_CLOSE_UP		  (MAPPING_USAGE_SED_KEY | 0x04) //CLOSE UP [YPOS]
// #define SED_KEY_PLACE_ON_TOP	(MAPPING_USAGE_SED_KEY | 0x05) //PLACE ON TOP
// #define SED_KEY_SRC_OWR			  (MAPPING_USAGE_SED_KEY | 0x06) //SRC O/WR

// #define SED_KEY_IN				    (MAPPING_USAGE_SED_KEY | 0x07) //IN [CLR]
// #define SED_KEY_OUT				    (MAPPING_USAGE_SED_KEY | 0x08) //OUT [CLR]
// #define SED_KEY_TRIM_IN			  (MAPPING_USAGE_SED_KEY | 0x09) //TRIM IN
// #define SED_KEY_TRIM_OUT		  (MAPPING_USAGE_SED_KEY | 0x0a) //TRIM OUT
// #define SED_KEY_ROLL			    (MAPPING_USAGE_SED_KEY | 0x0b) //ROLL [SLIDE]
// #define SED_KEY_SLIP_SRC		  (MAPPING_USAGE_SED_KEY | 0x0c) //SLIP SRC
// #define SED_KEY_SLIP_DEST		  (MAPPING_USAGE_SED_KEY | 0x0d) //SLIP DEST
// #define SED_KEY_TRANS_DUR		  (MAPPING_USAGE_SED_KEY | 0x0e) //TRANS DUR [SET]
// #define SED_KEY_CUT				    (MAPPING_USAGE_SED_KEY | 0x0f) //CUT
// #define SED_KEY_DIS				    (MAPPING_USAGE_SED_KEY | 0x10) //DIS
// #define SED_KEY_SMTH_CUT		  (MAPPING_USAGE_SED_KEY | 0x11) //SMTH CUT

// #define SED_KEY_SOURCE			  (MAPPING_USAGE_SED_KEY | 0x1a) //SOURCE
// #define SED_KEY_TIMELINE		  (MAPPING_USAGE_SED_KEY | 0x1b) //TIMELINE

// #define SED_KEY_SHTL			    (MAPPING_USAGE_SED_KEY | 0x1c) //SHTL
// #define SED_KEY_JOG				    (MAPPING_USAGE_SED_KEY | 0x1d) //JOG
// #define SED_KEY_SCRL			    (MAPPING_USAGE_SED_KEY | 0x1e) //SCRL

// #define SED_KEY_ESC				    (MAPPING_USAGE_SED_KEY | 0x31) //ESC [UNDO]
// #define SED_KEY_SYNC_BIN		  (MAPPING_USAGE_SED_KEY | 0x1f) //SYNC BIN
// #define SED_KEY_AUDIO_LEVEL	  (MAPPING_USAGE_SED_KEY | 0x2c) //AUDIO LEVEL [MARK]
// #define SED_KEY_FULL_VIEW		  (MAPPING_USAGE_SED_KEY | 0x2d) //FULL VIEW [RVW]
// #define SED_KEY_TRANS			    (MAPPING_USAGE_SED_KEY | 0x22) //TRANS [TITLE]
// #define SED_KEY_SPLIT			    (MAPPING_USAGE_SED_KEY | 0x2f) //SPLIT [MOVE]
// #define SED_KEY_SNAP			    (MAPPING_USAGE_SED_KEY | 0x2e) //SNAP [=]
// #define SED_KEY_RIPL_DEL		  (MAPPING_USAGE_SED_KEY | 0x2b) //RIPL DEL

// #define SED_KEY_CAM1			    (MAPPING_USAGE_SED_KEY | 0x33) //CAM1
// #define SED_KEY_CAM2			    (MAPPING_USAGE_SED_KEY | 0x34) //CAM2
// #define SED_KEY_CAM3			    (MAPPING_USAGE_SED_KEY | 0x35) //CAM3
// #define SED_KEY_CAM4			    (MAPPING_USAGE_SED_KEY | 0x36) //CAM4
// #define SED_KEY_CAM5			    (MAPPING_USAGE_SED_KEY | 0x37) //CAM5
// #define SED_KEY_CAM6			    (MAPPING_USAGE_SED_KEY | 0x38) //CAM6
// #define SED_KEY_CAM7			    (MAPPING_USAGE_SED_KEY | 0x39) //CAM7
// #define SED_KEY_CAM8			    (MAPPING_USAGE_SED_KEY | 0x3a) //CAM8
// #define SED_KEY_CAM9			    (MAPPING_USAGE_SED_KEY | 0x3b) //CAM9
// #define SED_KEY_LIVE_OWR		  (MAPPING_USAGE_SED_KEY | 0x30) //LIVE O/WR [RND]
// #define SED_KEY_VIDEO_ONLY		(MAPPING_USAGE_SED_KEY | 0x25) //VIDEO ONLY
// #define SED_KEY_AUDIO_ONLY		(MAPPING_USAGE_SED_KEY | 0x26) //AUDIO ONLY
// #define SED_KEY_STOP_PLAY		  (MAPPING_USAGE_SED_KEY | 0x3c) //STOP/PLAY


// #define SED_LED_CLOSE_UP			(MAPPING_USAGE_SED_LED | (1 <<  0) )
// #define SED_LED_CUT				    (MAPPING_USAGE_SED_LED | (1 <<  1) )
// #define SED_LED_DIS				    (MAPPING_USAGE_SED_LED | (1 <<  2) )
// #define SED_LED_SMTH_CUT			(MAPPING_USAGE_SED_LED | (1 <<  3) )
// #define SED_LED_TRANS			    (MAPPING_USAGE_SED_LED | (1 <<  4) )
// #define SED_LED_SNAP				  (MAPPING_USAGE_SED_LED | (1 <<  5) )
// #define SED_LED_CAM_7			    (MAPPING_USAGE_SED_LED | (1 <<  6) )
// #define SED_LED_CAM_8			    (MAPPING_USAGE_SED_LED | (1 <<  7) )
// #define SED_LED_CAM_9			    (MAPPING_USAGE_SED_LED | (1 <<  8) )
// #define SED_LED_LIVE_OWR			(MAPPING_USAGE_SED_LED | (1 <<  9) )
// #define SED_LED_CAM_4			    (MAPPING_USAGE_SED_LED | (1 << 10) )
// #define SED_LED_CAM_5			    (MAPPING_USAGE_SED_LED | (1 << 11) )
// #define SED_LED_CAM_6			    (MAPPING_USAGE_SED_LED | (1 << 12) )
// #define SED_LED_VIDEO_ONLY		(MAPPING_USAGE_SED_LED | (1 << 13) )
// #define SED_LED_CAM_1			    (MAPPING_USAGE_SED_LED | (1 << 14) )
// #define SED_LED_CAM_2			    (MAPPING_USAGE_SED_LED | (1 << 15) )
// #define SED_LED_CAM_3			    (MAPPING_USAGE_SED_LED | (1 << 16) )
// #define SED_LED_AUDIO_ONLY		(MAPPING_USAGE_SED_LED | (1 << 17) )

// // #define SED_JOG_LED_JOG       ( (1 << 0))
// // #define SED_JOG_LED_SHTL      ( (1 << 1))
// // #define SED_JOG_LED_SCRL      ( (1 << 2))

// // #define SED_JOG_MODE_0         ( 0 )     // Same as mode 3
// // #define SED_JOG_MODE_1         ( 1 )     // 
// // #define SED_JOG_MODE_2         ( 2 )
// // #define SED_JOG_MODE_3         ( 3 )




#endif