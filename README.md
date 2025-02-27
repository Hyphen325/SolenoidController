# Solenoid Controller

This project is a solenoid controller that can be built as a shared library or with testing code for tests.

## Building the Project

### Prerequisites

Ensure you have the following installed:
- A C++ compiler (e.g., GCC, Clang, MSVC)
- Make (if using GCC or Clang)

### Building as a Shared Library

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/SolenoidController.git
    cd SolenoidController
    ```

2. Compile the shared library:
    ```sh
    g++ -shared -o libsolenoidcontroller.so *.cpp
    ```

### Building the Testing Code

1. Follow step 1 from the "Building as a Shared Library" section.

2. Compile the testing code:
    ```sh
    g++ -o test_solenoidcontroller tests/*.cpp *.cpp
    ```

3. Run the tests:
    ```sh
    ./test_solenoidcontroller
    ```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
