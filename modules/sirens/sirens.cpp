//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "siren.h"

#include "smart_car_system.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut internalSirenPin(PE_10);
DigitalOut externalSirenPin(PE_12);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static bool internalSirenState = OFF;
static bool externalSirenState = OFF;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================

/**
* Initializes the siren to not output
*/
void sirensInit()
{
    internalSirenPin = ON;
    externalSirenPin = ON;
}

/**
* Reads the current state of the siren
* Returns the current state of the siren
*/
bool internalSirenStateRead()
{
    return internalSirenState;
}


bool externalSirenStateRead()
{
    return externalSirenState;
}

/**
* Sets the state of the siren 
* Parameter: The desired state of the siren
*/
void internalSirenStateWrite( bool state )
{
    internalSirenState = state;
}

void externalSirenStateWrite( bool state )
{
    externalSirenState = state;
}

/**
* Updates the siren's output based on its state.
*/
void sirensUpdate() {
    if ( internalSirenState == ON ) {
        internalSirenPin = ON;
    } else {
        internalSirenPin = OFF;
    }
    
    if ( externalSirenState == ON ) {
        externalSirenPin = ON;
    } else {
        externalSirenPin == OFF;
    }
}

//=====[Implementations of private functions]==================================