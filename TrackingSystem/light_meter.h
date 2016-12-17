
#ifndef _LIGHT_METER_H_
#define _LIGHT_METER_H_

#include <string>

class LightMeter {
public:
	LightMeter(std::string serial_number_, int board_number_, int channel_);
	~LightMeter();

	int getValue() const;

	bool connectDevice();
	bool disconnectDevice();

	void makeDummy() { dummy_data = true; verbose = true; }
	void toggleVerboseMode() { verbose = !verbose; }

	void setSamples(int samples_) { samples = samples_; }
	int getSamples() const { return samples; }

	void setRate(int rate_) { rate = rate_; }
	int getRate() const { return rate; }
private:
	bool dummy_data;
	bool verbose;

	bool is_connected;

	std::string serial_number;
	int board_number;
	int channel;

	int samples;
	int rate;
};

#endif
