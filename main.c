#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_STR 255
#define SPNAV_VENDOR_ID 0x046d
#define SPNAV_PRODUCT_ID 0xc626

// enum {
//     TRANSLATION = 1,
//     ROTATION = 2,
//     BUTTON = 3
// };

// struct event_motion {
// 	int type;
// 	int x, y, z;
// 	int rx, ry, rz;
// 	unsigned int period;
// 	int *data;
// };

// struct event_button {
// 	int type;
// 	int press;
// 	int bnum;
// };

// typedef union spnav_event {
// 	int type;
// 	struct event_motion motion;
// 	struct event_button button;
// } spnav_event;

// static bool INTERRUPTED = false;

// int spnav_convert_input(int first, unsigned char val) {
//     switch (val) {
//         case 0:
//             return first;
//         case 1:
//             return first + 255;
//         case 254:
//             return -512 + first;
//         case 255:
//             return -255 + first;
//         default:
//             return 0;
//     }
// }

// spnav_event read_event(hid_device *device, int ms) {
//     spnav_event ev;
//     unsigned char buf[64];
//     int nbytes = hid_read_timeout(device, buf, sizeof(buf), ms);
//     if (nbytes < 0) {
//         // TODO : error handling
//         wprintf(L"error hid_read");
//     } else if (nbytes == 0) {
//         ev.type = 0;
//     }
//     ev.type = buf[0];

//     switch (ev.type) {
//         case TRANSLATION:
//             ev.type = 0;

//             ev.motion.x = spnav_convert_input((buf[1] & 0x0000ff), buf[2]);
//             ev.motion.y = spnav_convert_input((buf[3] & 0x0000ff), buf[4]);
//             ev.motion.z = spnav_convert_input((buf[5] & 0x0000ff), buf[6]);
//             //wprintf(L"Translation x=%d, y=%d, z=%d\n", ev.motion.x, ev.motion.y, ev.motion.z);
//             break;
//         case ROTATION:
//             ev.type = 1;
//             ev.motion.rx = spnav_convert_input((buf[1] & 0x0000ff), buf[2]);
//             ev.motion.ry = spnav_convert_input((buf[3] & 0x0000ff), buf[4]);
//             ev.motion.rz = spnav_convert_input((buf[5] & 0x0000ff), buf[6]);
//             //wprintf(L"Rotation rx=%d, ry=%d, rz=%d\n", ev.motion.rx, ev.motion.ry, ev.motion.rz);
//             break;
//         case BUTTON:
//             wprintf(L"Buttons: %d %d\n", /* btn 1 */buf[1] & 0x01, /* btn 2 */ buf[1] & 0x02);
//             break;
//     }
//     return ev;
// }

// void set_led(hid_device *dev, char state) {
//     const unsigned char led_data[2] = {0x04, state};
//     // led_data[0] = 0x04; 
//     // led_data[1] = state;
//     int nbytes = hid_write(dev, led_data, sizeof(led_data));
//     wprintf(L"Bytes written : %d\n", nbytes);
// }

// void spnav_stop(hid_device *dev) {
//     set_led(dev, 0);
//     hid_close(dev);
//     hid_exit();
// }

// void sighandler(int signo) {
//     if (signo == SIGINT) {
//         wprintf(L"Stopping...\n");
//         INTERRUPTED = true;
//     }
// }

#include <signal.h>
#include <stdbool.h>
#include "spnav.h"

static bool INTERRUPTED = false;

void sighandler(int signo) {
    if (signo == SIGINT) {
        INTERRUPTED = true;
    }  
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, sighandler);
    /*
	// Initialize the hidapi library
    hid_init();
    // Open the device using the VID, PID,
	// and optionally the Serial number.
    hid_device *handle = hid_open(SPNAV_VENDOR_ID, SPNAV_PRODUCT_ID, NULL);
    
    set_led(handle, 1);

    while (1) {
        spnav_event ev = read_event(handle, 400);
        if (ev.type == 0) {
            wprintf(L"x=%d y=%d z=%d rx=%d ry=%d rz=%d\n", 
                ev.motion.x, ev.motion.y, ev.motion.z, ev.motion.rx, ev.motion.ry, ev.motion.rz);
        } else {
            wprintf(L"ev.type : %d\n", ev.type);
        }

        if (INTERRUPTED) {
            break;
        }
    }
    spnav_stop(handle);
    return 0;
    */
   spnav_event ev;
   spnav_open();
   spnav_sensitivity(0.1);
   for (;;) {
       spnav_wait_event_timeout(&ev, 400);
       
       switch (ev.type) {
           case MOTION:
                printf("x=%d, y=%d, z=%d ", ev.motion.x, ev.motion.y, ev.motion.z);
                printf("rx=%d, ry=%d, rz=%d\n", ev.motion.rx, ev.motion.ry, ev.motion.rz);
                break;
            case BUTTON:
                printf("bnum=%d, pressed=%d\n", ev.button.bnum, ev.button.press);
                break;
       }

       if (INTERRUPTED) {
           break;
       }
   }
   spnav_close();
}
