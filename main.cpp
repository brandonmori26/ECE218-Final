//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"
#include "display.h"
#include "Ignition.h"
#include "Windshield.h"


int  main(){
    inputsInit();
    outputsInitIgnition();
    
    while(true)
        ignitionUpdate();
        delay(TIME_INCREMENT_MS);

}
