#ifndef SOLENOIDTRANSMITTER_H
#define SOLENOIDTRANSMITTER_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function to create a SolenoidTransmitter instance
__declspec(dllexport) uintptr_t __stdcall SolenoidTransmitter_create();

// Function to destroy a SolenoidTransmitter instance
__declspec(dllexport) void __stdcall SolenoidTransmitter_destroy(uintptr_t transmitter);

// Function to initialize a SolenoidTransmitter instance with a specified port name
__declspec(dllexport) bool __stdcall SolenoidTransmitter_initialize(uintptr_t transmitter, const char* portName);

// Function to set the state of a solenoid
__declspec(dllexport) bool __stdcall SolenoidTransmitter_set(uintptr_t transmitter, int solenoidId, bool setpoint);

// Function to close the SolenoidTransmitter instance
__declspec(dllexport) void __stdcall SolenoidTransmitter_close(uintptr_t transmitter);

// Function to send a command to the SolenoidTransmitter instance
__declspec(dllexport) bool __stdcall SolenoidTransmitter_sendCommand(uintptr_t transmitter, const char* command);

// Function to read data from the SolenoidTransmitter instance
__declspec(dllexport) bool __stdcall SolenoidTransmitter_read(uintptr_t transmitter, char* buffer, size_t bufferSize);

#ifdef __cplusplus
}
#endif

#endif // SOLENOIDTRANSMITTER_H