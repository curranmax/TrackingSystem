
#include "gm.h"
#include "light_meter.h"
#include "tracking_system.h"

#include <iostream>

int main() {
	float k_proportional = 1.0;
	int samples = 1;
	int rate = 25000;

	// Open GMs, and light_meters
	GM* hgm = new GM("106925", 1, 0);
	GM* vgm = new GM("106925", 1, 1);

	LightMeter* phlm = new LightMeter("1BCA997", 0, 0);
	LightMeter* nhlm = new LightMeter("1BCA997", 0, 1);
	LightMeter* pvlm = new LightMeter("1BCA997", 0, 2);
	LightMeter* nvlm = new LightMeter("1BCA997", 0, 3);
	
	phlm->setSamples(samples);
	nhlm->setSamples(samples);
	pvlm->setSamples(samples);
	nvlm->setSamples(samples);

	phlm->setRate(rate);
	nhlm->setRate(rate);
	pvlm->setRate(rate);
	nvlm->setRate(rate);

	// Create TrackingSystem object
	TrackingSystem* tracking_system = new TrackingSystem(k_proportional, hgm, vgm, phlm, nhlm, pvlm, nvlm);

	// Allow user to either manually adjust GMs or run tracking system (maybe for a set amount of time)
	tracking_system->run();

	delete tracking_system;
	return 0;
}
