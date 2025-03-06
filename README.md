# Morse_Decoder_KL05Z
Morse code decoder on the FRDM-KL05Z development board, using an external keyboard for input. It processes Morse signals, decodes them into characters, and displays the output on an LCD screen.
# Morse Code Decoder

## Project Overview
This project implements a Morse code decoder using the FRDM-KL05Z development board. The decoder interprets Morse code input via a button, converts it into characters, and displays the decoded message on a 16x2 LCD screen.

## Features
- **Morse Code Input:** Uses a button to input Morse code (short press for dot, long press for dash).
- **Automatic Decoding:** Converts Morse code sequences into readable characters.
- **LCD Display:** Shows real-time Morse code input and decoded messages.
- **Reset Functionality:** Clears the current message using a reset button.

## Hardware Requirements
- FRDM-KL05Z development board
- 4x4 matrix keyboard (for Morse code input)
- 16x2 LCD display with I2C converter

## Software Requirements
- Keil uVision5
- CMSIS and peripheral libraries for MKL05Z4

## How It Works
1. The user inputs Morse code by pressing a button (short press for `.` and long press for `-`).
2. The decoder determines character boundaries based on time gaps between inputs.
3. The Morse sequence is matched against a predefined lookup table.
4. The decoded character is displayed on the LCD screen.
5. A reset button clears the message and resets the decoder.

## Code Structure
- `main.c` – Contains the main logic for Morse code decoding.
- `lcd1602.h/.c` – Handles LCD display operations.
- `frdm_bsp.h` – Board support package for handling buttons and interrupts.
- `klaw.h/.c` – Manages button presses and debouncing.
