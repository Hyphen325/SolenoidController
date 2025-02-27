#include <iostream>
#include <windows.h>

void processCommand(const std::string& command) {
    int solenoidId;
    int setpoint;
    if (sscanf(command.c_str(), "SET %d %d", &solenoidId, &setpoint) == 2) {
        std::cout << "Simulated Arduino: Received command to set solenoid " << solenoidId << " to " << (setpoint ? "open" : "close") << std::endl;
        std::cout << "OK" << std::endl;
    } else {
        std::cerr << "Simulated Arduino: ERROR: Invalid command format" << std::endl;
    }
}

int main() {
    const WCHAR* portName = L"COM13"; // Change this to the correct virtual COM port name
    HANDLE hSerial = CreateFileW(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open COM port " << portName << std::endl;
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to get current serial parameters" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to set serial parameters" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    char buffer[256];
    DWORD bytesRead;
    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            std::string command(buffer);
            processCommand(command);
        }
    }

    CloseHandle(hSerial);
    return 0;
}