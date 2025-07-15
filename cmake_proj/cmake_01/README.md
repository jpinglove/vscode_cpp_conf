# README for Bubble Sort Project

## Project Overview
This project implements the Bubble Sort algorithm in C++. It allows users to input a list of numbers, which are then sorted using the Bubble Sort technique. The sorted array is displayed as output.

## Files
- `src/main.cpp`: The entry point of the application, containing the main function and logic to execute the bubble sort algorithm.
- `src/bubble_sort.cpp`: Implements the Bubble Sort algorithm, sorting an array of numbers and outputting the sorted array.
- `CMakeLists.txt`: Configuration file for CMake, specifying the project name, required CMake version, source files, and executable creation.

## Building the Project
To build the project, follow these steps:

1. Ensure you have CMake installed on your system.
2. Open a terminal and navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   cmake --build .
   ```

## Running the Application
After building the project, you can run the application using the following command:
```
./BubbleSort
```
Follow the prompts to enter the number of elements and the numbers to be sorted.

## License
This project is open-source and available for modification and distribution.