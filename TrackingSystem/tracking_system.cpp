
#include "tracking_system.h"

#include <iostream>
#include <Windows.h>
#include <time.h>

TrackingSystem::TrackingSystem(float k_proportional_,
								GM * horizontal_gm_, GM * vertical_gm_,
								LightMeter * positive_horizontal_light_meter_,
								LightMeter * negative_horizontal_light_meter_,
								LightMeter * positive_vertical_light_meter_,
								LightMeter * negative_vertical_light_meter_) : 
		k_proportional(k_proportional_),
		horizontal_gm(horizontal_gm_), vertical_gm(vertical_gm_),
		positive_horizontal_light_meter(positive_horizontal_light_meter_),
		negative_horizontal_light_meter(negative_horizontal_light_meter_),
		positive_vertical_light_meter(positive_vertical_light_meter_),
		negative_vertical_light_meter(negative_vertical_light_meter_),
		verbose(false) {}

TrackingSystem::~TrackingSystem() {
	delete horizontal_gm;
	delete vertical_gm;

	delete positive_horizontal_light_meter;
	delete negative_horizontal_light_meter;
	delete positive_vertical_light_meter;
	delete negative_vertical_light_meter;
}

bool TrackingSystem::connectAllDevices() {
	return horizontal_gm->connectDevice() && vertical_gm->connectDevice() &&
			positive_horizontal_light_meter->connectDevice() &&
			negative_horizontal_light_meter->connectDevice() &&
			positive_vertical_light_meter->connectDevice() &&
			negative_vertical_light_meter->connectDevice();
}

bool TrackingSystem::disconnectAllDevices() {
	return horizontal_gm->disconnectDevice() && vertical_gm->disconnectDevice() &&
		positive_horizontal_light_meter->disconnectDevice() &&
		negative_horizontal_light_meter->disconnectDevice() &&
		positive_vertical_light_meter->disconnectDevice() &&
		negative_vertical_light_meter->disconnectDevice();
}

void TrackingSystem::run() {
	if (verbose) {
		std::cout << "Starting TrackingSystem.run()" << std::endl;
	}
	if (!connectAllDevices()) {
		if (verbose) {
			std::cout << "Failed to connect all the devices" << std::endl;
		}
		return;
	}
	while (true) {
		std::string user_input = "";
		std::cout << "Enter option:" << std::endl <<
						"\tset number of (s)amples," << std::endl <<
						"\tset (r)ate of samples," << std::endl <<
						"\tset k_(p)roportional," << std::endl <<
						"\ttoggle (v)erbose mode," << std::endl <<
						"\t(d)isplay params" << std::endl <<
						"\t(m)anual alignment," << std::endl <<
						"\t(a)utomatic tracking," << std::endl <<
						"\tor (q)uit" << std::endl << ">> ";
		std::cin >> user_input;
		if (user_input == "m") {
			manualAlignment();
		} else if (user_input == "a") {
			autoTracking();
		} else if (user_input == "q") {
			if (verbose) {
				std::cout << "Quitting Tracking System" << std::endl;
			}
			break;
		} else if (user_input == "s") {
			int new_samples = 0;
			std::cin >> new_samples;
			positive_horizontal_light_meter->setSamples(new_samples);
			negative_horizontal_light_meter->setSamples(new_samples);
			positive_vertical_light_meter->setSamples(new_samples);
			negative_vertical_light_meter->setSamples(new_samples);
		} else if (user_input == "r") {
			int new_rate = 0;
			std::cin >> new_rate;
			positive_horizontal_light_meter->setRate(new_rate);
			negative_horizontal_light_meter->setRate(new_rate);
			positive_vertical_light_meter->setRate(new_rate);
			negative_vertical_light_meter->setRate(new_rate);
		} else if (user_input == "p") {
			float new_k_proportional = 1.0;
			std::cin >> new_k_proportional;
			k_proportional = new_k_proportional;
		} else if (user_input == "v") {
			toggleVerboseMode();
		} else if (user_input == "d") {
			std::cout << std::endl;
			std::cout << "Num Samples: " << positive_horizontal_light_meter->getSamples() << std::endl;
			std::cout << "Rate: " << positive_horizontal_light_meter->getRate() << std::endl;
			std::cout << "k_proportional: " << k_proportional << std::endl << std::endl;
		}
	}

	if (!disconnectAllDevices()) {
		if (verbose) {
			std::cout << "Failed to disconnect all devices" << std::endl;
		}
		return;
	}
	if (verbose) {
		std::cout << "Ending TrackingSystem.run()" << std::endl;
	}
}

void TrackingSystem::manualAlignment() {
	if (verbose) {
		std::cout << "Starting manual alignment" << std::endl;
	}
	while (true) {
		std::string user_input = "";
		std::cout << "Enter manual alignment option:" << std::endl <<
						"\tEnter gm choice ([h]orizontal, [v]ertical)," << std::endl <<
						"\t\taction ([a]bsolute change, [r]elative change)," << std::endl <<
						"\t\tand integer value separated by spaces," << std::endl <<
						"\t[c]heck light meters," << std::endl <<
						"\tor [q]uit" << std::endl << ">> ";
		std::cin >> user_input;
		if (user_input == "q") {
			break;
		} else if (user_input == "c") {
			// Horizontal
			int phlm_val = 0, nhlm_val = 0;
			float hr = adjust(nullptr, positive_horizontal_light_meter, negative_horizontal_light_meter, phlm_val, nhlm_val);

			// Vertical
			int pvlm_val = 0, nvlm_val = 0;
			float vr = adjust(nullptr, positive_vertical_light_meter, negative_vertical_light_meter, pvlm_val, nvlm_val);

			std::cout << "Response   (" << hr << ", " << vr << ")" << std::endl;
			std::cout << "Horizontal (+: " << phlm_val << ", -: " << nhlm_val << ")" << std::endl;
			std::cout << "Vertical   (+: " << pvlm_val << ", -: " << nvlm_val << ")" << std::endl;
		} else {
			std::string gm_description = user_input, action;
			int value;
			std::cin >> action >> value;
			GM* this_gm = nullptr;
			if (gm_description == "h") {
				this_gm = horizontal_gm;
			} else if (gm_description == "v") {
				this_gm = vertical_gm;
			}

			if (this_gm != nullptr) {
				if (action == "a") {
					this_gm->setValue(value);
				} else if (action == "r") {
					this_gm->setValue(this_gm->getCurrentValue() + value);
				} else {
					std::cerr << "Invalid Action" << std::endl;
				}
			} else {
				std::cerr << "Invalid GM choice" << std::endl;
			}
		}
	}
	if (verbose) {
		std::cout << "Ending manual alignment" << std::endl;
	}
}

// Definetly not the best way to do things
bool loop = true;
BOOL handleCtrlEvent(DWORD ctrl_type) {
	if (ctrl_type == CTRL_C_EVENT) {
		loop = false;
		return true;
	}
	return false;
}

void TrackingSystem::autoTracking() {
	std::cout << "Start auto tracking, press Ctrl+C to stop" << std::endl;

	bool first = true;
	float max_hr = 0.0, min_hr = 0.0, total_hr = 0.0, total_hr_mag = 0.0;
	float max_vr = 0.0, min_vr = 0.0, total_vr = 0.0, total_vr_mag = 0.0;
	int max_ph_val = 0, min_ph_val = 0, total_ph_val = 0;
	int max_nh_val = 0, min_nh_val = 0, total_nh_val = 0;
	int max_pv_val = 0, min_pv_val = 0, total_pv_val = 0;
	int max_nv_val = 0, min_nv_val = 0, total_nv_val = 0;
	int num_iters = 0;

	int print_every_k_iters = 50;

	time_t start_time = time(NULL);

	loop = true;
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)handleCtrlEvent, true);
	while (loop) {
		// Horizontal
		int phlm_val = 0, nhlm_val = 0;
		float hr = adjust(horizontal_gm, positive_horizontal_light_meter, negative_horizontal_light_meter, phlm_val, nhlm_val);
		
		// Vertical
		int pvlm_val = 0, nvlm_val = 0;
		float vr = adjust(vertical_gm, positive_vertical_light_meter, negative_vertical_light_meter, pvlm_val, nvlm_val);

		if (verbose) {
			std::cout << "TS: (" << hr << "," << vr << ")" << std::endl;
		}

		if (num_iters % print_every_k_iters == 0) {
			std::cout << "GM pos (" << horizontal_gm->getCurrentValue() << ", " << vertical_gm->getCurrentValue() << ")" << std::endl;
		}
		
		// Update stats
		if (first || hr > max_hr) {
			max_hr = hr;
		}
		if (first || hr < min_hr) {
			min_hr = hr;
		}
		if (first || vr > max_vr) {
			max_vr = vr;
		}
		if (first || vr < min_vr) {
			min_vr = vr;
		}
		total_hr += hr;
		total_vr += vr;
		total_hr_mag += abs(hr);
		total_vr_mag += abs(vr);
		num_iters++;

		if (first || phlm_val > max_ph_val) {
			max_ph_val = phlm_val;
		}
		if (first || phlm_val < min_ph_val) {
			min_ph_val = phlm_val;
		}
		if (first || nhlm_val > max_nh_val) {
			max_nh_val = nhlm_val;
		}
		if (first || nhlm_val < min_nh_val) {
			min_nh_val = nhlm_val;
		}
		if (first || pvlm_val > max_pv_val) {
			max_pv_val = pvlm_val;
		}
		if (first || pvlm_val < min_pv_val) {
			min_pv_val = pvlm_val;
		}
		if (first || nvlm_val > max_nv_val) {
			max_nv_val = nvlm_val;
		}
		if (first || nvlm_val < min_nv_val) {
			min_nv_val = nvlm_val;
		}
		total_ph_val += phlm_val;
		total_nh_val += nhlm_val;
		total_pv_val += pvlm_val;
		total_nv_val += nvlm_val;

		first = false;
	}
	SetConsoleCtrlHandler(NULL, false);

	time_t end_time = time(NULL);

	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "|                  Test Parameters                  |" << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "Number of Samples: " << positive_horizontal_light_meter->getSamples() << std::endl;
	std::cout << "Rate of Samples:   " << positive_horizontal_light_meter->getRate() << " samples per second" << std::endl;
	std::cout << "K_proportional:    " << k_proportional << std::endl << std::endl;

	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "|                Response Statistics                |" << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "Max hr:     " << max_hr << std::endl;
	std::cout << "Min hr:     " << min_hr << std::endl;
	std::cout << "Avg hr:     " << total_hr / num_iters << std::endl;
	std::cout << "Avg mag hr: " << total_hr_mag / num_iters << std::endl;
	std::cout << "Max vr:     " << max_vr << std::endl;
	std::cout << "Min vr:     " << min_vr << std::endl;
	std::cout << "Avg vr:     " << total_vr / num_iters << std::endl;
	std::cout << "Avg mag vr: " << total_vr_mag / num_iters << std::endl;
	std::cout << "Num iters:  " << num_iters << std::endl;
	std::cout << "Iters/sec:  " << float(num_iters) / float((end_time - start_time > 0) ? end_time - start_time : 0.1) << std::endl << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "|                 Value Statistics                  |" << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << "Max ph_val: " << max_ph_val << std::endl;
	std::cout << "Min ph_val: " << min_ph_val << std::endl;
	std::cout << "Avg ph_val: " << total_ph_val / num_iters << std::endl;
	std::cout << "Max nh_val: " << max_nh_val << std::endl;
	std::cout << "Min nh_val: " << min_nh_val << std::endl;
	std::cout << "Avg nh_val: " << total_nh_val / num_iters << std::endl;
	std::cout << "Max pv_val: " << max_pv_val << std::endl;
	std::cout << "Min pv_val: " << min_pv_val << std::endl;
	std::cout << "Avg pv_val: " << total_pv_val / num_iters << std::endl;
	std::cout << "Max nv_val: " << max_nv_val << std::endl;
	std::cout << "Min nv_val: " << min_nv_val << std::endl;
	std::cout << "Avg nv_val: " << total_nv_val / num_iters << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl << std::endl;

	std::cout << "Ending automatic tracking" << std::endl;
}

void TrackingSystem::makeAllDevicesDummy() {
	horizontal_gm->makeDummy();
	vertical_gm->makeDummy();

	positive_horizontal_light_meter->makeDummy();
	negative_horizontal_light_meter->makeDummy();
	positive_vertical_light_meter->makeDummy();
	negative_vertical_light_meter->makeDummy();

	verbose = true;
}

void TrackingSystem::toggleVerboseMode() {
	verbose = !verbose;
	
	horizontal_gm->toggleVerboseMode();
	vertical_gm->toggleVerboseMode();

	positive_horizontal_light_meter->toggleVerboseMode();
	negative_horizontal_light_meter->toggleVerboseMode();
	positive_vertical_light_meter->toggleVerboseMode();
	negative_vertical_light_meter->toggleVerboseMode();
}

float TrackingSystem::adjust(GM * gm, LightMeter * pos_lm, LightMeter * neg_lm, int& pos_val, int& neg_val) {
	pos_val = pos_lm->getValue();
	neg_val = neg_lm->getValue();
	float proportional_error = 0.0;
	if (pos_val + neg_val != 0) {
		// -1 factor is because we want to "move" away from the lm with the higher value
		proportional_error = float(-1.0 * float(pos_val - neg_val) / float(pos_val + neg_val));
	}

	// TODO add in integral and derivative elements
	float response = k_proportional * proportional_error;

	if (gm != nullptr) {
		gm->setValue(gm->getCurrentValue() + int(response + .5));
	}

	return response;
}
