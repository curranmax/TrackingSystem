
#ifndef _TRACKING_SYSTEM_H_
#define _TRACKING_SYSTEM_H_

#include "gm.h"
#include "light_meter.h"

#include <Windows.h>

class TrackingSystem {
public:
	TrackingSystem(float k_proportional_,
					GM* horizontal_gm_, GM* vertical_gm_,
					LightMeter* positive_horizontal_light_meter_,
					LightMeter* negative_horizontal_light_meter_,
					LightMeter* positive_vertical_light_meter_,
					LightMeter* negative_vertical_light_meter_);
	~TrackingSystem();

	bool connectAllDevices();
	bool disconnectAllDevices();

	void run();
	void manualAlignment();
	void autoTracking();

	void makeAllDevicesDummy();
	void toggleVerboseMode();
	void basicVerboseMode() { verbose = true; }
private:
	float adjust(GM* gm, LightMeter* pos_lm, LightMeter* neg_lm, int& pos_val, int& neg_val);

	bool verbose;

	float k_proportional;

	// TrackingSystem owns both GMs
	GM* horizontal_gm;
	GM* vertical_gm;

	// TrackingSystem owns the four LightMeters
	LightMeter* positive_horizontal_light_meter;
	LightMeter* negative_horizontal_light_meter;
	LightMeter* positive_vertical_light_meter;
	LightMeter* negative_vertical_light_meter;
};

#endif
