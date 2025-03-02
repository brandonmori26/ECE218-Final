//=====[Libraries]=============================================================

#include "parking_system.h"

#include "collision_detection_subsystem.h"
#include "entrance_subsystem.h"
#include "exit_subsystem.h"
#include "reset_code.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

void smartCarSystemInit()
{
    collisionDetectionSubsystemInit();
    entranceSubsystemInit();
    exitSubsystemInit();
}

void smartCarSystemUpdate()
{
    collisionDetectionSubsystemUpdate();
    entranceSubsystemInit();
    exitSubsystemInit();
    delay(TIME_INCREMENT_MS);
}

//=====[Implementations of private functions]==================================