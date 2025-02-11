//=====[#include guards - begin]===============================================

#ifndef _Ignition_H_
#define _Ignition_H_

//=====[Declarations (prototypes) of public functions]=========================
bool ignitionUpdate()
{
    welcomeMessage();
    ignitionEnable();
    if (!engineStarted && debounceIgnition()){
        uartUsb.write("Ignition attempted.\r\n\r\n", 23);
        if (!greenLED){
            sirenPin = BUZZER_ON;
            errorMessage();
            engineStarted = false;
        }
        else{
            sirenPin = BUZZER_OFF;
            greenLED = OFF;
            blueLED = ON;
            uartUsb.write("Engine started.\r\n\r\n", 19);
            engineStarted = true;
        }
    }
    else if (engineStarted && debounceIgnition()){
        uartUsb.write("Engine off.\r\n\r\n", 15);
        engineStarted = false;
        blueLED = OFF;
    }
    
    return engineStarted;
}



//=====[#include guards - end]=================================================

#endif //_Ignition_H_
