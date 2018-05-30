#include <stdio.h>
#include <stdlib.h>

#define MAX_STR 255
#define SPNAV_VENDOR_ID 0x046d
#define SPNAV_PRODUCT_ID 0xc626
#define SPNAV_3D_EXPLORER_PRODUCT_ID 0x0c627

#include <signal.h>
#include <stdbool.h>
#include "spnav.h"

static bool INTERRUPTED = false;

void sighandler(int signo) {
    if (signo == SIGINT) {
        INTERRUPTED = true;
    }
}

int main(int argc, char const* argv[]) {
    signal(SIGINT, sighandler);
    spnav_event ev;
    spnav_open(SPNAV_VENDOR_ID, SPNAV_3D_EXPLORER_PRODUCT_ID);
    spnav_sensitivity(0.1);
    spnav_deadzone(10);
    spnav_set_nonblocking(true);
    for (;;) {
        // spnav_wait_event_timeout(&ev, 400);
        spnav_wait_event(&ev);

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
