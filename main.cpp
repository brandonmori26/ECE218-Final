//=====[Libraries]=============================================================

#include "parking_system.h"

//=====[Defines]===============================================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

#include "mbed.h"
#include "carAtEntrance.h"
#include "arm_book_lib.h"
int main()
{
    // Initialize the car detection system
    carAtEntranceInit();
    while (true) {
        carAtEntranceUpdate();
        
        delay(500);

    }
}
//=====[Implementations of public functions]===================================
