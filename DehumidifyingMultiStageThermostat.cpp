
#include <DehumidifyingMultiStageThermostat.h>
DehumidifyingMultiStageThermostat::DehumidifyingMultiStageThermostat(uint8_t pDHTSensorPin):MultiStageThermostat(pDHTSensorPin)
{
    lastStage = -1;
    dehuSettings.bottomTempLimit = 1.0;
    dehuSettings.bottomTempRange = 0.2;
    dehuSettings.HumidityLimit = 55;
    dehuSettings.HumidityRange = 5;
    Enabled = false;
    
}

int DehumidifyingMultiStageThermostat::getStage()
{
    int newStage = MultiStageThermostat::getStage();

    // if we're in cooling mode, and getStage() returns zero, go into dehumidify mode
    // if temperature is within 1C of setpoint and humidity is 55% or higher turn on mode 1 fan
    // switch off if temp drops below 1.2C below setpoint or humidity drops below 50%
    /*
    If temp set point is 25, humidityLimit is 55, bottom temp limit is 0.8, bottom temp range is 0.2, humidityrange is 5

    Humidity/Temp  Over 24.2 | 24.0-24.2 | Below 24.0
    Below 55        0             0          0 
    55-60           Sticky        Sticky     0
    Over 60         1             Sticky     0   
    
    */

    if (Enabled)

    {
        if (newStage == 0 && mode == MODE_COOL)
        {
            bool sticky = false;

            if (getTemp() > setTemp - dehuSettings.bottomTempLimit - dehuSettings.bottomTempRange)  // if temp > 24.0
            {
                if (getHumidity() > dehuSettings.HumidityLimit) // if humidity > 55
                {
                    if (getHumidity() > dehuSettings.HumidityLimit + dehuSettings.HumidityRange) // humidity > 60
                    {
                        if (getTemp() > setTemp - dehuSettings.bottomTempLimit) // if temp > 24.2
                        {
                            newStage = 1;
                        }
                        else
                        {
                            sticky = true;
                        }
                        
                    }
                    else
                    {
                        sticky = true;
                    }
                }
            }
            // sticky
            if (sticky && lastStage != 0)
            {
                newStage = 1;
            }

            
        }
    }
    lastStage = newStage;
    return newStage;
}

void DehumidifyingMultiStageThermostat::SetParameters(DehuSettings* newSettings)
{
    dehuSettings.bottomTempLimit=newSettings->bottomTempLimit;
    dehuSettings.bottomTempRange= newSettings->bottomTempRange;
    dehuSettings.HumidityLimit= newSettings->HumidityLimit;
    dehuSettings.HumidityRange= newSettings->HumidityRange;
    lastStage = -1;
}
