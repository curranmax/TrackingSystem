
#include "light_meter.h"

#include "cbw.h"

#include <string>
#include <iostream>
#include <stdlib.h>

LightMeter::LightMeter(std::string serial_number_, int board_number_, int channel_) :
			channel(channel_),
			serial_number(serial_number_),
			board_number(board_number_),
			is_connected(false),
			dummy_data(false),
			verbose(false),
			samples(1),
			rate(25000) {}

LightMeter::~LightMeter() {
	if (is_connected) {
		disconnectDevice();
	}
}

int LightMeter::getValue() const {
	int fetched_value = 0;
	if (dummy_data) {
		fetched_value = 256;
	} else {
		if (samples == 1) {
			unsigned short val = 0;
			int err_code = cbAIn(board_number, channel, BIP10VOLTS, &val);
			if (err_code != 0) {
				std::cerr << "Error in cbAin: " << err_code << std::endl;
				exit(1);
			}
			fetched_value = int(val);
		} else {
			long this_rate = (long)rate;
			HGLOBAL buffer_handle = cbWinBufAlloc(long(samples));
			int err_code = cbAInScan(board_number, channel, channel, samples, &this_rate, BIP10VOLTS, buffer_handle, 0);

			unsigned short* data_array = (unsigned short*)buffer_handle;

			int sum_data = 0;
			for (int i = 0; i < samples; ++i) {
				sum_data += data_array[i];
			}
			fetched_value = int(float(sum_data) / float(samples));

			cbWinBufFree(buffer_handle);
		}
	}

	if (verbose) {
		std::cout << "(" << serial_number << ", " << channel << ") fetched value " << fetched_value << std::endl;
	}
	return fetched_value;
}

bool LightMeter::connectDevice() {
	if (!dummy_data) {
		// Nothing to actually do on Windows
		is_connected = true;
	} else {
		is_connected = true;
	}
	if (verbose) {
		std::cout << "(" << serial_number << ", " << channel << ") connect device" << std::endl;
	}
	return is_connected;
}

bool LightMeter::disconnectDevice() {
	if (!dummy_data) {
		// Nothing to actually do on Windows
		is_connected = false;
	}
	else {
		is_connected = false;
	}
	if (verbose) {
		std::cout << "(" << serial_number << ", " << channel << ") disconnect device" << std::endl;
	}
	return !is_connected;
}
