#include "SolenoidTransmitter.h"

SolenoidTransmitter* SolenoidTransmitter_create() {
    SolenoidTransmitter* transmitter = (SolenoidTransmitter*) malloc(sizeof(SolenoidTransmitter));
    if (transmitter) {
        transmitter->serialHandle = INVALID_HANDLE_VALUE;
    }
    return transmitter;
}

void SolenoidTransmitter_destroy(SolenoidTransmitter* transmitter) {
    if (transmitter) {
        SolenoidTransmitter_close(transmitter);
        free(transmitter);
    }
}

//port name is specific to the computer, likely will end up as a constant in the labview code.
//Inside of device manager settings, you can find the port name for the arduino.
//Likely will be COM3 or COM4, at least thats what it is on mine
bool SolenoidTransmitter_initialize(SolenoidTransmitter* transmitter, const char* portName) {
    if (!transmitter || !portName) return false;

    int baudRate = 9600; //default we'll use on the arduino

    transmitter->serialHandle = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (transmitter->serialHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(transmitter->serialHandle, &dcbSerialParams)) {
        CloseHandle(transmitter->serialHandle);
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    //check things twice in case of failure
    if (!SetCommState(transmitter->serialHandle, &dcbSerialParams)) {
        CloseHandle(transmitter->serialHandle);
        return false;
    }

    return true;
}

//do we want to have a better way to set solenoids? Maybe a text based format?
//Ideally we use these as some sort of enum as it's kinda hard to do it with no numbers.
bool SolenoidTransmitter_set(SolenoidTransmitter* transmitter, int solenoidId, bool setpoint) {
    if (!transmitter || transmitter->serialHandle == INVALID_HANDLE_VALUE) return false;

    char command[32];
    snprintf(command, sizeof(command), "SET %d %d\n", solenoidId, setpoint ? 1 : 0);
    return SolenoidTransmitter_sendCommand(transmitter, command);
}

void SolenoidTransmitter_close(SolenoidTransmitter* transmitter) {
    if (transmitter && transmitter->serialHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(transmitter->serialHandle);
        transmitter->serialHandle = INVALID_HANDLE_VALUE;
    }
}

bool SolenoidTransmitter_sendCommand(SolenoidTransmitter* transmitter, const char* command) {
    if (!transmitter || transmitter->serialHandle == INVALID_HANDLE_VALUE || !command) return false;

    DWORD bytesWritten;
    if (!WriteFile(transmitter->serialHandle, command, strlen(command), &bytesWritten, NULL)) {
        return false;
    }

    return bytesWritten == strlen(command);
}

