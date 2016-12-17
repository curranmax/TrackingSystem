
#ifndef _GM_H_
#define _GM_H_

#include <string>

#define MIN_GM_VALUE 0x0
#define MAX_GM_VALUE 0xffff

class GM {
public:
	GM(const std::string& serial_number_, int board_number_, int channel_);
	~GM();

	// Changes the value of the GM, returns True if the GM is now set to new_value (essentially returns false if new_value is too small or too big)
	bool setValue(int new_value);
	int getCurrentValue() const { return current_value; }

	// These return true iff the status of the device (i.e. is it connected or not) ends at the appropriate value
	bool connectDevice();
	bool disconnectDevice();

	void makeDummy() { dummy_data = true; verbose = true; }
	void toggleVerboseMode() { verbose = !verbose; }
private:
	bool dummy_data;
	bool verbose;

	int current_value;

	bool is_connected;

	std::string serial_number;
	int board_number;
	int channel;
};

#endif
