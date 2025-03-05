//=====[Libraries]=============================================================

//#include "parking_system.h"

//=====[Defines]===============================================================

//=====[Declaration of public data types]======================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

#include "mbed.h"
#include "collision_detection_subsystem.h"
#include "arm_book_lib.h"
int main() {
    collisionDetectionSubsystemInit();

    while(true) {
        collisionDetectionSubsystemUpdate();
        
    }
}
//=====[Implementations of public functions]===================================
