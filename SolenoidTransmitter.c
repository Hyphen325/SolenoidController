#include "SolenoidTransmitter.h"
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    HANDLE serialHandle;
} SolenoidTransmitter;

__declspec(dllexport) uintptr_t __stdcall SolenoidTransmitter_create() {
    SolenoidTransmitter* transmitter = (SolenoidTransmitter*) malloc(sizeof(SolenoidTransmitter));
    if (transmitter) {
        transmitter->serialHandle = INVALID_HANDLE_VALUE;
    }
    return (uintptr_t)transmitter;
}

__declspec(dllexport) void __stdcall SolenoidTransmitter_destroy(uintptr_t transmitter) {
    if (transmitter) {
        SolenoidTransmitter_close(transmitter);
        free((SolenoidTransmitter*)transmitter);
    }
}

__declspec(dllexport) bool __stdcall SolenoidTransmitter_initialize(uintptr_t transmitter, const char* portName) {
    if (!transmitter || !portName) return false;

    SolenoidTransmitter* trans = (SolenoidTransmitter*)transmitter;
    int baudRate = 9600; //default we'll use on the arduino

    trans->serialHandle = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (trans->serialHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(trans->serialHandle, &dcbSerialParams)) {
        CloseHandle(trans->serialHandle);
        trans->serialHandle = INVALID_HANDLE_VALUE;
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(trans->serialHandle, &dcbSerialParams)) {
        CloseHandle(trans->serialHandle);
        trans->serialHandle = INVALID_HANDLE_VALUE;
        return false;
    }

    return true;
}

__declspec(dllexport) bool __stdcall SolenoidTransmitter_set(uintptr_t transmitter, int solenoidId, bool setpoint) {
    if (!transmitter) return false;

    SolenoidTransmitter* trans = (SolenoidTransmitter*)transmitter;
    if (trans->serialHandle == INVALID_HANDLE_VALUE) return false;

    char command[32];
    snprintf(command, sizeof(command), "SET %d %d\n", solenoidId, setpoint ? 1 : 0);
    return SolenoidTransmitter_sendCommand(transmitter, command);
}

__declspec(dllexport) void __stdcall SolenoidTransmitter_close(uintptr_t transmitter) {
    if (transmitter) {
        SolenoidTransmitter* trans = (SolenoidTransmitter*)transmitter;
        if (trans->serialHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(trans->serialHandle);
            trans->serialHandle = INVALID_HANDLE_VALUE;
        }
    }
}

__declspec(dllexport) bool __stdcall SolenoidTransmitter_sendCommand(uintptr_t transmitter, const char* command) {
    if (!transmitter || !command) return false;

    SolenoidTransmitter* trans = (SolenoidTransmitter*)transmitter;
    if (trans->serialHandle == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten;
    if (!WriteFile(trans->serialHandle, command, strlen(command), &bytesWritten, NULL)) {
        return false;
    }

    return bytesWritten == strlen(command);
}

__declspec(dllexport) bool __stdcall SolenoidTransmitter_read(uintptr_t transmitter, char* buffer, size_t bufferSize) {
    if (!transmitter || !buffer || bufferSize == 0) return false;

    SolenoidTransmitter* trans = (SolenoidTransmitter*)transmitter;
    if (trans->serialHandle == INVALID_HANDLE_VALUE) return false;

    DWORD bytesRead;
    if (!ReadFile(trans->serialHandle, buffer, bufferSize - 1, &bytesRead, NULL)) {
        return false;
    }

    buffer[bytesRead] = '\0'; // Null-terminate the string
    return true;
}