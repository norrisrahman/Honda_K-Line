# Honda K-Line STM32

This repository contains a library for K-Line communication implemented in C. K-Line communication is commonly used in automotive diagnostics and is supported by various electronic control units (ECUs).

## Overview

The `kline.c` file provides the implementation of K-Line communication functionality, including initialization, sending messages, and handling responses. It utilizes UART communication via the `usbd_cdc_if.h` and `main.h` files. The `kline.h` header file defines the necessary structures and function prototypes.

## Files

- `kline.c`: Contains the implementation of K-Line communication functions.
- `kline.h`: Header file with structure definitions and function prototypes.
- `main.h`: Header file for main configurations.
- `usbd_cdc_if.h`: Header file for USB communication interface.

## Dependencies

- STM32 HAL Library
- STM32 USB Library

## Usage

To use this library, include `kline.h` in your project and ensure that the STM32 HAL and USB libraries are properly configured. Initialize the UART communication and call the appropriate functions from `kline.c` to send and receive messages over K-Line.

## Functions

- `calcChecksum`: Calculates the checksum of a given data array.
- `filterData`: Filters the received data based on a specified length.
- `initEcu`: Initializes the ECU for communication.
- `pingECU`: Sends a wakeup message to the ECU.
- `UART_TURN_OFF` / `UART_TURN_ON`: Functions to turn off/on UART communication.
- `resetGpio`: Resets the GPIO configuration for UART communication.
- `main_loop`: Main loop for handling K-Line communication.

## Contributing

Contributions to this library are welcome. Please fork the repository, make your changes, and submit a pull request for review.

## License

This library is licensed under the MIT License. See the `LICENSE` file for details.
