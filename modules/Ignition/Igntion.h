//=====[#include guards - begin]===============================================

#ifndef _Ignition_H_
#define _Ignition_H_

//=====[Declarations (prototypes) of public functions]=========================
void handleIgnitionButton();
void displayInhibitReasons();

//=========[Declarations of Private Variables]=========================
bool engineRunning = false;           // Tracks if engine is running
bool welcomeDisplayed = false;        // Tracks if welcome message has been shown
bool previousButtonState = false;     // For detecting new button presses

//=====[#include guards - end]=================================================

#endif //_Ignition_H_