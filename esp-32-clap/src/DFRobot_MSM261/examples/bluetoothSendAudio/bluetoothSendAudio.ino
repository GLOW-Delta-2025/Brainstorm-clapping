/*!
 * @file bluetoothSendAudio.ino
 * @brief This is a Bluetooth microphone example that only transmits left channel data. 
 * @n Running this example enables the Bluetooth microphone to transmit data.
 * @copyright  Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-02-18
 * @url https://github.com/DFRobot/DFRobot_MSM261
 */

#include "DFrobot_MSM261.h"
#include "DFRobot_BluetoothA2DPSource.h"

#define SAMPLE_RATE     (44100)
#define I2S_SCK_IO      (25)
#define I2S_WS_IO       (16)
#define I2S_DI_IO       (26)
#define DATA_BIT        (16)
#define MODE_PIN        (4)

DFRobot_Microphone microphone(I2S_SCK_IO, I2S_WS_IO, I2S_DI_IO);
DFRobot_BluetoothA2DPSource a2dp;
const char* bleName = "KFC"; // The name of the Bluetooth slave device to connect to
char communicationData[512];

static int32_t btAppA2dDataCb(uint8_t *data, int32_t len)
{   
    if (len < 0 || data == NULL) {
        return 0;
    }
    microphone.read(communicationData, len);
    /**
     * @brief Mono data processing. When using mono, it is necessary to process the other channel's data
     * to prevent noise interference in the effective channel. This step can be skipped 
     * if two microphones are used to form stereo.
     */
    for (uint32_t i = 0; i < (len >> 2); i++) {
      communicationData[(i << 2)] = 0;
      communicationData[(i << 2) + 1] = 0;
      //communicationData[(i << 2) + 2] = 0;
      //communicationData[(i << 2) + 3] = 0;
    }
    for (int i = 0; i < (len >> 1); i++) {
        data[(i << 1)] = communicationData[i << 1];
        data[(i << 1) + 1] = communicationData[(i << 1) + 1];
    }
    return len;
}

void setup() {
  Serial.begin(115200);
  pinMode(MODE_PIN, OUTPUT);
  digitalWrite(MODE_PIN, LOW); // Configure the microphone to receive left channel data
  //digitalWrite(MODE_PIN, HIGH); // Configure the microphone to receive right channel data
  while (microphone.begin(SAMPLE_RATE, DATA_BIT) != 0) {
      Serial.println("I2S initialization failed");
  }
  Serial.println("I2S initialization successful");
  while (!a2dp.begin(bleName, btAppA2dDataCb, 50)) {
    Serial.println("A2DP initialization failed");
  }
}

void loop() {
}
