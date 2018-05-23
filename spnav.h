#ifndef SPNAV_H__
#define SPNAV_H__

#define SPNAV_VENDOR_ID 0x046d
#define SPNAV_PRODUCT_ID 0xc626
#define SPNAV_NAXIS 6

#ifdef _WIN32
      #define SPNAV_API_EXPORT __declspec(dllexport)
      #define SPNAV_API_CALL
#else
      #define SPNAV_API_EXPORT /**< API export macro */
      #define SPNAV_API_CALL /**< API call macro */
#endif

#define SPNAV_API_EXPORT_CALL SPNAV_API_EXPORT SPNAV_API_CALL /**< API export and call macro*/

enum event_type {
    ANY,
    MOTION,
    BUTTON
};

struct event_motion {
	int type;
	int x, y, z;
	int rx, ry, rz;
	unsigned int period;
	int *data;
};

struct event_button {
	int type;
	bool press;
	int bnum;
};

typedef union spnav_event {
	int type;
	struct event_motion motion;
	struct event_button button;
} spnav_event;

#ifdef __cplusplus
extern "C" {
#endif

int SPNAV_API_EXPORT_CALL spnav_open(void);
int SPNAV_API_EXPORT_CALL spnav_close(void);
int SPNAV_API_EXPORT_CALL spnav_wait_event(spnav_event *event);
int SPNAV_API_EXPORT_CALL spnav_wait_event_timeout(spnav_event *event, int timeout);
int SPNAV_API_EXPORT_CALL spnav_sensitivity(double sens);
int SPNAV_API_EXPORT_CALL spnav_deadzone(int value);

#ifdef __cplusplus
}
#endif

#endif /* SPNAV_H__ */