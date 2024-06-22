# Employee Data Processor

This project parses employee data from JSON and XML files, calculates average salary, finds the highest paid employee, and sorts employees by ID.

## Dependencies

This project includes the necessary header files for RapidJSON and TinyXML-2. No external installation is required.

## Build Instructions

### Windows (MinGW)

1. Open a terminal (e.g., Git Bash).
2. Navigate to the project directory.
3. Run the following command to compile the project:

    ```sh
    g++ -std=c++17 -Iinclude src/main.cpp src/tinyxml2.cpp -o EmployeeDataProcessor.exe
    ```

### Linux

1. Open a terminal.
2. Navigate to the project directory.
3. Run the following command to compile the project:

    ```sh
    g++ -std=c++17 -Iinclude src/main.cpp src/tinyxml2.cpp -o EmployeeDataProcessor
    ```

## Run the Program

After compiling, you can run the program with:

```sh
./EmployeeDataProcessor <input_file>
