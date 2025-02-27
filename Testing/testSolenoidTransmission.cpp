#include <iostream>
#include <windows.h>
#include "../SolenoidTransmitter.h"

int main() {
    const char* portName = "COM12"; // Change this to the correct port name for your Arduino Nano
    SolenoidTransmitter* transmitter = SolenoidTransmitter_create();

    if (!transmitter) {
        if (!SolenoidTransmitter_initialize(transmitter, portName)) {
            DWORD error = GetLastError();
            std::cerr << "Failed to initialize SolenoidTransmitter on port " << portName
                      << ". Error Code: " << error << std::endl;
            SolenoidTransmitter_destroy(transmitter);
            return 1;
        }
        return 1;
    }

    if (!SolenoidTransmitter_initialize(transmitter, portName)) {
        std::cerr << "Failed to initialize SolenoidTransmitter on port " << portName << std::endl;
        SolenoidTransmitter_destroy(transmitter);
        return 1;
    }

    // Test setting solenoids
    for (int i = 0; i < 5; i++) {
        if (SolenoidTransmitter_set(transmitter, i, true)) {
            std::cout << "Successfully set solenoid " << i << " to open" << std::endl;
        } else {
            std::cerr << "Failed to set solenoid " << i << " to open" << std::endl;
        }

        // Wait for 1 second
        Sleep(1000);

        if (SolenoidTransmitter_set(transmitter, i, false)) {
            std::cout << "Successfully set solenoid " << i << " to close" << std::endl;
        } else {
            std::cerr << "Failed to set solenoid " << i << " to close" << std::endl;
        }

        // Wait for 1 second
        Sleep(1000);
    }

    SolenoidTransmitter_close(transmitter);
    SolenoidTransmitter_destroy(transmitter);

    return 0;
}