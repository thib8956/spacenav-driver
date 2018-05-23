#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "spnav.h"
#include "hidapi.h"

#ifdef DEBUG
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define DEBUG_PRINT(...) do{ } while ( false )
#endif

enum {
    TRANSLATION = 1,
    ROTATION = 2,
    BTN = 3
};

static bool IS_OPEN = false;
/* HID device for SpaceNavigator mouse */ 
hid_device *device = NULL;

int convert_input(int first, unsigned char val) {
    switch (val) {
        case 0:
            return first;
        case 1:
            return first + 255;
        case 254:
            return -512 + first;
        case 255:
            return -255 + first;
        default:
            return 0;
    }
}

bool in_deadzone(unsigned char *data, int threshold) {
    /* data[0] is the event type */
    int i;
    for (i=1; i<SPNAV_NAXIS; i++) {
        if (data[i] > threshold) {
            return false;
        }
    }
    DEBUG_PRINT("in_deadzone\n");
    return true;
}

int read_event(hid_device *device, spnav_event* ev, int ms) {
    unsigned char buf[64];
    int nbytes = hid_read_timeout(device, buf, sizeof(buf), ms);
    if (nbytes < 0) {
        DEBUG_PRINT("hid_read_timeout() error");
        return -1;
    } else if (nbytes == 0) {
        ev->type = 0;
        return 0;
    }
    ev->type = buf[0];

    switch (ev->type) {
        case TRANSLATION:
             if (in_deadzone(buf, 4)) {
                ev->type = 0;
                return ev->type;
            }
            ev->motion.type = 1;
            ev->motion.x = convert_input((buf[1] & 0x0000ff), buf[2]);
            ev->motion.y = convert_input((buf[3] & 0x0000ff), buf[4]);
            ev->motion.z = convert_input((buf[5] & 0x0000ff), buf[6]);
            // DEBUG_PRINT("Translation x=%d, y=%d, z=%d\n", ev->motion.x, ev->motion.y, ev->motion.z);
            break;
        case ROTATION:
            if (in_deadzone(buf, 4)) {
                ev->type = 0;
                return ev->type;
            }
            ev->motion.type = 1;
            ev->motion.rx = convert_input((buf[1] & 0x0000ff), buf[2]);
            ev->motion.ry = convert_input((buf[3] & 0x0000ff), buf[4]);
            ev->motion.rz = convert_input((buf[5] & 0x0000ff), buf[6]);
            // DEBUG_PRINT("Rotation rx=%d, ry=%d, rz=%d\n", ev->motion.rx, ev->motion.ry, ev->motion.rz);
            break;
        case BTN:
            ev->button.type = 2;
            ev->button.press = buf[1] == 0x01;
            ev->button.bnum = buf[1];
            //DEBUG_PRINT("Buttons: %d %d\n", /* btn 1 */buf[1] & 0x01, /* btn 2 */ buf[1] & 0x02);
            break;
    }

    return ev->type;
}

int set_led(hid_device *dev, char state) {
    const unsigned char led_data[2] = {0x04, state};
    int nbytes = hid_write(dev, led_data, sizeof(led_data));
    if (nbytes != sizeof(led_data)) {
        DEBUG_PRINT("set_led(): hid_write() has written %d bytes (should be %ld)\n",
                     nbytes, sizeof(led_data));
        return -1;
    }
    return nbytes;
}

int spnav_open() {
    DEBUG_PRINT("spnav_open()\n");
    /* Connexion already opened */
    if (IS_OPEN) {
        DEBUG_PRINT("Connexion already opened!\n");
        return -1;
    }
    // Initialize the hidapi library
    hid_init();
    // Open the device using the VID, PID,
	// and optionally the Serial number.
    device = hid_open(SPNAV_VENDOR_ID, SPNAV_PRODUCT_ID, NULL);
    if (device == NULL) {
        DEBUG_PRINT("hid_open() failed!");
        return -1;
    }
    IS_OPEN = true;
    set_led(device, 1);
    return 0;
}

int spnav_close() {
    DEBUG_PRINT("spnav_close()\n");
    if (!IS_OPEN) {
        return -1;
    }

    set_led(device, 0);
    hid_close(device);
    hid_exit();
    IS_OPEN = false;
    return 0;
}

int spnav_wait_event(spnav_event *event) {
    if (device == NULL) {
        DEBUG_PRINT("spnav_wait_event(): device not connected.\n");
        return -1;
    }
    return read_event(device, event, -1);;
}

int spnav_wait_event_timeout(spnav_event *event, int milliseconds) {
    if (device == NULL) {
        DEBUG_PRINT("spnav_wait_event_timeout(): device not connected.\n");
        return -1;
    }
    return read_event(device, event, milliseconds);;
}
/* 
int spnav_wait_event(spnav_event *event);
int spnav_poll_event(spnav_event *event, int timeout); */