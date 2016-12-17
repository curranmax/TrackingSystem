
#include "gm.h"

#include "cbw.h"

#include <string>
#include <iostream>

GM::GM(const std::string& serial_number_, int board_number_, int channel_) :
		channel(channel_),
		serial_number(serial_number_),
		board_number(board_number_),
		current_value(0),
		is_connected(false),
		dummy_data(false),
		verbose(false) {}

GM::~GM() {
	if (is_connected) {
		disconnectDevice();
	}
}

bool GM::setValue(int new_value) {
	if (MIN_GM_VALUE <= new_value && new_value <= MAX_GM_VALUE) {
		if (verbose) {
			std::cout << "(" << serial_number << ", " << channel << ") set to " << new_value << std::endl;
		}
		if (!dummy_data) {
			// 0 is because the range value is ignored with the USB-3103 device
			cbAOut(board_number, channel, 0, unsigned short(new_value));
		}
		current_value = new_value;
		return true;
	} else {
		if (verbose) {
			std::cout << "(" << serial_number << ", " << channel << ") out of range request of " << new_value << std::endl;
		}
		return false;
	}
}

bool GM::connectDevice() {	
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

bool GM::disconnectDevice() {
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
