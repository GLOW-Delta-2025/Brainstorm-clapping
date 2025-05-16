# Clap Detection System

This project implements a clap detection system using Python. It processes audio input in real-time to detect claps based on various audio features such as volume, frequency content, zero-crossing rate, and spectral centroid. The detected claps are then sent to an Arduino or processed directly on an ESP32, depending on the implementation.

## Features

- **Real-Time Audio Processing**: Captures audio input and processes it in real-time.
- **Clap Detection**: Detects claps based on:
  - Volume threshold
  - High-frequency content ratio
  - Zero-crossing rate (ZCR)
  - Spectral centroid
- **Debounce Mechanism**: Ensures claps are not detected too frequently.
- **BPM Calculation**: Calculates beats per minute (BPM) based on the time intervals between claps.
- **Arduino Communication**: Sends detected clap data to an Arduino via serial communication (for the Arduino implementation).
- **ESP32 Standalone Processing**: Processes clap detection directly on the ESP32 using a microphone module.

## File Structure

- `arduino-clap/arduino-audio.py`: Main Python script for clap detection and Arduino communication.
- `esp-32-clap/`: Folder containing the ESP32 implementation for clap detection.
- `audio.py`: Alternate implementation of the clap detection system using Python.
- `Readme.md`: Documentation for the project.

## Implementations

### Arduino Implementation (`arduino-clap/arduino-audio.py`)

- Uses your **laptop's microphone** to capture audio input in real-time.
- The audio is processed on your laptop using Python to detect claps.
- Detected claps are sent to the Arduino via a **serial connection**.

### ESP32 Implementation (`esp-32-clap`)

- Uses an **ESP32 microcontroller** connected to a **microphone module** for audio input.
- The clap detection logic is implemented directly on the ESP32, making it independent of your laptop.

## How It Works

1. **Audio Input**: The system uses the `sounddevice` library (for the Arduino implementation) or a microphone module (for the ESP32 implementation) to capture audio input.
2. **Clap Detection**:
   - **Volume Check**: Ensures the audio signal exceeds a minimum volume (`THRESHOLD`).
   - **Frequency Analysis**: Uses FFT to check if the high-frequency content is sufficient.
   - **Zero-Crossing Rate**: Measures the rate of sign changes in the audio signal.
   - **Spectral Centroid**: Calculates the "center of mass" of the audio spectrum.
3. **Debounce**: Ensures a minimum time gap between consecutive claps to avoid false positives.
4. **Arduino Communication**: Sends the detected clap data to an Arduino via a serial connection (Arduino implementation only).
5. **BPM Calculation**: Calculates the BPM based on the intervals between claps and checks for consistency.

## Parameters

The following parameters can be adjusted to fine-tune the clap detection:

- `THRESHOLD`: Minimum RMS volume to detect a clap.
- `HIGH_FREQ_CUTOFF`: Minimum frequency for high-frequency content analysis.
- `MIN_HIGH_FREQ_RATIO`: Minimum ratio of high-frequency content to total energy.
- `MIN_ZCR`: Minimum zero-crossing rate.
- `MIN_CENTROID`: Minimum spectral centroid.
- `DURATION`: Duration of each audio block in seconds.
- `MIN_CLAP_GAP`: Minimum time gap between consecutive claps.

## Setup

### Arduino Implementation

1. Install the required Python libraries:
   ```bash
   pip install sounddevice numpy pyserial