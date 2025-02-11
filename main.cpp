//=====[Libraries]=============================================================

#include "display.h"
#include "Ignition.h"
#include "Windshield.h"

int  main(){
    
    while(true)
        ignitionUpdate();
        delay(TIME_INCREMENT_MS);
    
}
