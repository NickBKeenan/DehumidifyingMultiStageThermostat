//
#pragma once

#include <MultiStageThermostat.h>

struct DehuSettings
{
	float bottomTempLimit;
	float bottomTempRange;
	int HumidityLimit;
	int HumidityRange;

};

class DehumidifyingMultiStageThermostat:public MultiStageThermostat
{
	int lastStage;
	bool Enabled;
	DehuSettings dehuSettings;

public:
	DehumidifyingMultiStageThermostat(uint8_t pDHTSensorPin);
	int getStage();
	void EnableDehumidify() { Enabled = true; };
	void DisableDehumidify() { Enabled = false; };
	void SetParameters(DehuSettings * newSettings);


};
