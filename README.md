# Embedded Systems Laboratory - Crazy Car

This repository contains the project files and code developed as part of the **Embedded Systems Laboratory** course. The goal of this project is to implement software for the "Crazy Car," enabling it to navigate autonomously through a course. The software is designed using a **layered architecture**, ensuring modularity and maintainability.

## 📂 Project Structure

The project is organized into the following layers:

1. **HAL (Hardware Abstraction Layer)**  
   This layer handles direct interaction with the microcontroller's hardware, including initialization of peripherals and providing interfaces to higher layers.  
   - Example file naming: `hal_module.c`, `hal_module.h`.

2. **DL (Driver Layer)**  
   This optional layer uses HAL functions to implement low-level driver functionalities. Direct hardware register access is avoided here.

3. **AL (Application Layer)**  
   This layer contains the main application logic, responsible for high-level decision-making and behavior of the Crazy Car.

Each layer is represented by a dedicated folder:

## 🛠 Development Tools

- **Microcontroller**: MSP430
- **IDE**: Code Composer Studio (CCS)
- **Programming Interface**: Spy-by-wire JTAG
- **Programming Language**: C

## 🚀 Features

### Completed Modules
1. **Basic I/O Configuration**  
   - Input/Output pin setup.
   - Interrupt-driven I/O handling.

2. **Clock System and Timers**  
   - Unified Clock System configuration.  
   - Timer-based interrupts (e.g., TimerB0).  
   - PWM generation using TimerA1 for actuator control.

3. **Communication Protocols**  
   - SPI configuration (USCI B1).  
   - UART communication (optional).

4. **Peripheral Drivers**  
   - LC-Display driver implementation (basic and fast options).  
   - ADC configuration and DMA integration for sensor readings.

5. **Sensor Integration**  
   - Sharp distance sensors: calibration and response curve implementation.  

6. **Control Algorithms**  
   - Motor speed control.  
   - Path-following and obstacle-avoidance algorithms.

## 📖 Instructions

### Setting Up a Workspace
1. Create a workspace on your local machine.  
2. Add a new project for each lab session to maintain incremental development.  
3. Follow the folder structure and file naming conventions for consistency.  

### Adding New Files
1. Right-click on the project in CCS > `New > Folder` > Name it (`HAL`, `DL`, or `AL`).  
2. Add `.c` and `.h` files to the respective folders.  
3. Configure include paths:  
   - `Right-click Project > Properties > Build > MSP430 Compiler > Include Options`  
   - Add the folders (e.g., `HAL/`) to the include paths.

### Building and Flashing
1. Compile and link the project in CCS.  
2. Use the debug session and Spy-by-wire JTAG interface to upload the program.  

## 🌟 Key Concepts

- **Layered Architecture**: Facilitates swapping microcontroller types by only modifying the HAL layer.
- **Workspace Management**: Maintain separate projects for each lab task.
- **Interrupts**: Utilized for I/O operations, timers, and ADC for real-time performance.

## 📝 Notes

- Ensure a consistent include-path configuration to avoid header file errors.  
- Follow the lab manual step-by-step for incremental learning and development.  

## 🤝 Contribution

Feel free to contribute by:
- Improving existing modules.
- Adding documentation or comments for better clarity.
- Extending functionalities (e.g., implementing advanced algorithms or optimizing drivers).

---

Happy coding! 🚗✨

# Introduction to team "Totalschaden"
Meet Team Totalschaden from CrazyCar – where chaos meets the road! 🏎️💥 Our battle-scarred companion embodies resilience and humor, thriving against all odds. With every dent, crack, and misplaced traffic cone, we remind the world: it's not the destination, but the ride that counts! 🚦
![File not loading, obviously](Team Totalschaden Logo.jpg)
