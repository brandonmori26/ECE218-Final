#include "arm_book_lib.h"
#include "mbed.h"
#define daylightMin 100
#define duskMax 50

// Driver seat occupancy sensor (provides contact closure when occupied);
// Passenger seat occupancy sensor (provides contact closure when occupied);
// Driver seatbelt sensor (provides contact closure when fastened);
// Passenger seatbelt sensor (provides contact closure when fastened);
// Ignition push button (provides contact closure to request ignition);
DigitalIn driveSeatUsed(D1);
DigitalIn passSeatUsed(D2);
DigitalIn driveBelt(D6);
DigitalIn passBelt(D7);
DigitalIn ignition(D5);
// Alarm buzzer (sounds when ignition is inhibited);
// Green indicator LED (lights when ignition is enabled);
//  Blue indicator LED (lights when the engine has started);
DigitalInOut alarm(PE_10);
DigitalOut green(LED1);
DigitalOut blue(LED2);
// Headlight mode selector (provides ON, OFF, or AUTO signals);
// Light sensor (provides ambient light level signal);
// Left low beam lamp, right low beam lamp
DigitalIn headlightSelect();
DigitalIn lightSensor();
DigitalIn leftLamp();
DigitalIn rightLamp();

driveSeatUsed.mode(PullDown);
passSeatUsed.mode(PullDown);
driveBelt.mode(PullDown);
passBelt.mode(PullDown);
ignition.mode(PullDown);

UnbufferedSerial system(USBTX, USBRX, 115200);
/*
Ignition Subsystem
When the driver sits down, display the message,
“Welcome to enhanced alarm system model 218-W25”.
Indicate ignition enabled with the green LED only when
both seats are occupied and both seatbelts are fastened.
 When the ignition button is pushed:
-        If the ignition enabled (green LED) is lit, then normal ignition
occurs. Light the blue LED and extinguish the green LED. Display the message,
“Engine started.”
-        If the ignition is not enabled (green LED not lit), then ignition is
inhibited. Sound the alarm buzzer; display the message, “Ignition inhibited” and
display all the reasons why the ignition was inhibited: “Passenger seat not
occupied,” “Driver seatbelt not fastened,” etc. Once the error messages are
displayed, the system allows additional start attempts. (note this change from
Project 1 requirements) Keep the engine running even if the driver or passenger
should remove their seat belts or exit the vehicle. When the engine is running,
stop the engine when the ignition button is pushed and then released. (note this
change from Project 1 requirements)


Headlight subsystem
If the engine is running, and the user selects: ON, turn on both low beam lamps;
if they select OFF, turn off all lamps.
If the user selects the AUTO position:
When the light level is above a “daylight” level, switch headlights off after
a delay of 2 seconds.
When the light level is below a “dusk” level, switch headlights on after a delay
of 1 second. When between these two levels, the headlights should hold their
previous state. Turn off all headlights when the engine is off.
*/

void greenUpdate() {
  // Light the green LED only when both seats are occupied and both
  // seatbelts are fastened.
  if (driveSeatUsed == ON && passSeatUsed == ON && driveBelt == ON &&
      passBelt == ON) {
    green = ON;
  } else {
    green = OFF;
  }
}

void ignitionStart() {
  // -        If the green LED is lit, then normal ignition occurs.
  //          Light the blue LED and extinguish the green LED. Display
  //          the message, “Engine started.”
  blue = ON;
  green = OFF;
  system.write("Engine Started\n", 16);

  while (ignition == false) {
    ignitionRun()
  }
  // Keep the engine running even if the driver or passenger should remove their
  // seat
  // belts or exit the vehicle.
  while (ignition == true) {
  }
  // When the engine is running, stop the engine when the ignition button is
  // pushed and then released. (note this change from Project 1 requirements)
  // Turn off all headlights when the engine is off.
  rightLamp == OFF leftLamp == OFF blue = OFF;
  green = OFF;
  system.write("Engine Stopped\n", 16);
}

void ignitionRun() {
  // If the engine is running, and the user selects: ON, turn on both low beam
  // lamps;
  if (headlightSelect == ON) {
    rightLamp == ON leftLamp == ON
  } else if (headlightSelect ==
             OFF) { // if they select OFF, turn off all lamps.
    rightLamp == OFF leftLamp == OFF
  } else if (headlightSelect ==
             AUTO) { // If the user selects the AUTO position:
    if (lightSensor >=
        daylightMin) { // When the light level is above a “daylight” level,
      // switch headlights off after a delay of 2 seconds.
      delay(2000);
      rightLamp == OFF;
      leftLamp == OFF;
    } else if (lightSensor <=
               duskMax) { // When the light level is below a “dusk” level,
      // switch headlights on after a delay of 1 second.
      delay(1000);
      rightLamp == ON leftLamp == ON
      // When between these two levels, the headlights should hold their
      // previous state.
    }
  }
}

void ignitionFail() {
  // -        If the green LED is not lit, then ignition is inhibited.
  // Sound the alarm buzzer;
  alarm.output();
  //          display the message, “Ignition inhibited” and
  system.write("Ignition inhibited\n", 20);
  //     display all
  //          the reasons why the ignition was inhibited: “Passenger
  //          seat not occupied,” “Driver seatbelt not fastened,” etc.
  if (driveSeatUsed == OFF) {
    system.write("Driver seat unoccupied\n", 24);
  }
  if (passSeatUsed == OFF) {
    system.write("Passenger seat unoccupied\n", 27);
  }
  if (driveBelt == OFF) {
    system.write("Driver seatbelt not fastened\n", 30);
  }
  if (passBelt == OFF) {
    system.write("Passenger seatbelt not fastened\n", 31);
  }
}

int main() {
  // Once the error messages are displayed, the system allows additional start
  // attempts. (note this change from Project 1 requirements)
  while (true) {
    //   When the driver sits down, display the message,
    // “Welcome to enhanced alarm system model 218-W25”.
    while (driveSeatUsed == OFF) {
    }
    system.write("Welcome to enhanced alarm system model 218-W24\n", 47);
    // Indicate ignition enabled with the green LED only when
    // both seats are occupied and both seatbelts are fastened
    while (ignition == OFF) {
      greenUpdate()
    }
    //  When the ignition button is pushed:
    // -        If the ignition enabled (green LED) is lit, then normal ignition
    // occurs. Light the blue LED and extinguish the green LED. Display the
    // message, “Engine started.”
    if (green == ON) {
      ignitionStart()
      // -        If the ignition is not enabled (green LED not lit), then
      // ignition is inhibited. Sound the alarm buzzer; display the message,
      // “Ignition inhibited” and display all the reasons why the ignition was
      // inhibited: “Passenger seat not occupied,” “Driver seatbelt not
      // fastened,” etc.
    } else {
      ignitionFail()
    }
  }
}
