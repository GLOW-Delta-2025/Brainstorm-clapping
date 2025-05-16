/*!
 * @file microphoneRecordsToSDCard.ino
 * @brief This is an example for microphone recording. It uses 16-bit, 44.1kHz mono to record sound.
 * @n Running this example will record 20 seconds of audio to the SD card.
 * @copyright  Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-02-24
 * @url https://github.com/DFRobot/DFRobot_MSM261
 */

#include "DFrobot_MSM261.h"
#include "SPI.h"
#include <SD.h>
#include <FS.h>
#define SAMPLE_RATE     (44100)
#define I2S_SCK_IO      (25)
#define I2S_WS_IO       (16)
#define I2S_DI_IO       (26)
#define DATA_BIT        (16)
#define MODE_PIN        (4)

const int record_time = 20;  // Recording duration
const char filename[] = "/sound.wav"; // Name of the saved file

const int headerSize = 44;
const int byteRate = 176400; // Number of bytes collected per second: Sampling rate x Number of channels x Bits per sample / 8
const int waveDataSize = record_time * byteRate; // Bytes sampled in 20 seconds
const int numCommunicationData = 512; // Amount of data collected per read
byte header[headerSize]; // WAVE file header
char communicationData[numCommunicationData];

DFRobot_Microphone microphone(I2S_SCK_IO, I2S_WS_IO, I2S_DI_IO);
File file;

void setup() {
  Serial.begin(115200);
  pinMode(MODE_PIN, OUTPUT);
  digitalWrite(MODE_PIN, LOW); // Configure the microphone to receive left channel data
  //digitalWrite(MODE_PIN, HIGH); // Configure the microphone to receive right channel data
  while (microphone.begin(SAMPLE_RATE, DATA_BIT) != 0) {
      Serial.println("I2S initialization failed");
  }
  Serial.println("I2S initialization successful");
  if (!SD.begin()) Serial.println("SD initialization failed");
  while (!SD.begin()) {
      Serial.print(".");
      delay(500);
  }
  /*
   * @brief Create the header for a WAVE file
   * @param header Array to store the WAVE file header
   * @param totalDataLen Total length of the data to be written
   * @param longSampleRate Sampling rate
   * @param channels Number of channels
   * @param byteRate Byte rate: Sampling rate x Number of channels x Bits per sample / 8
   * @param blockAlign Block alignment: Number of channels x Bits per sample / 8
   */
  microphone.createWavHeader(header, waveDataSize, 44100, 2, byteRate, 4);
  SD.remove(filename);
  // Open the file, or create it if it doesn't exist
  file = SD.open(filename, FILE_WRITE);
  if (!file) return;
  // Write the WAVE file header to the file
  file.write(header, headerSize);
  Serial.println("Start");
  for (int j = 0; j < waveDataSize / numCommunicationData; ++j) {
    microphone.read(communicationData, numCommunicationData);
    /**
     * @brief Mono data processing. When using mono, it is necessary to process the other channel's data
     * to prevent noise interference in the effective channel. This step can be skipped
     * if two microphones are used to form stereo.
     */
    for (uint32_t i = 0; i < (numCommunicationData >> 2); i++) {
      communicationData[(i << 2)] = 0;
      communicationData[(i << 2) + 1] = 0;
      //communicationData[(i << 2) + 2] = 0;
      //communicationData[(i << 2) + 3] = 0;
    }
    file.write((uint8_t*)communicationData, numCommunicationData);
  }
  file.close();
  Serial.println("Finish");
}

void loop() {
}
