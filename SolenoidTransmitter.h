#ifndef SOLENOID_TRANSMITTER_H
#define SOLENOID_TRANSMITTER_H

#define SOLENOIDTRANSMITTER_API 


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Define the SolenoidTransmitter structure
typedef struct {
    void* serialHandle;
} SolenoidTransmitter;



// Constructor
__declspec(dllexport)  SolenoidTransmitter* __stdcall SolenoidTransmitter_create();

// Destructor
__declspec(dllexport)  void __stdcall SolenoidTransmitter_destroy(SolenoidTransmitter* transmitter);

// Initialize the connection to the Arduino
__declspec(dllexport) bool __stdcall SolenoidTransmitter_initialize(SolenoidTransmitter* transmitter, const char* portName);

// Send a signal to set a specific solenoid (0 : Close, 1 : Open)
// Returns true if the command was sent successfully
__declspec(dllexport) bool __stdcall SolenoidTransmitter_set(SolenoidTransmitter* transmitter, int solenoidId, bool setpoint);

// Close the connection to the Arduino
__declspec(dllexport) void __stdcall SolenoidTransmitter_close(SolenoidTransmitter* transmitter);

// Internal method to send a command to the Arduino
__declspec(dllexport) bool __stdcall SolenoidTransmitter_sendCommand(SolenoidTransmitter* transmitter, const char* command);

#ifdef __cplusplus
}
#endif

#endif 