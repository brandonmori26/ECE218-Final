//=====[#include guards - begin]===============================================

#ifndef _SIREN_H_
#define _SIREN_H_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void sirensInit();
bool internalSirenStateRead();
bool externalSirenStateRead();
void internalSirenStateWrite( bool state );
void externalSirenStateWrite( bool state );
void sirensUpdate();

//=====[#include guards - end]=================================================

#endif // _SIREN_H_