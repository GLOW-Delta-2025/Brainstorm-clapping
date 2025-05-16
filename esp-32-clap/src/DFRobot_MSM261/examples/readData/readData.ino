/*!
 * @file readData.ino
 * @brief This retrieves microphone data and prints it out.
 * @copyright  Copyright (c) 2022 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-02-18
 * @url https://github.com/DFRobot/DFRobot_MSM261
 */

#include "DFrobot_MSM261.h"

#define SAMPLE_RATE     (44100)
#define I2S_SCK_IO      (25)
#define I2S_WS_IO       (16)
#define I2S_DI_IO       (26)
#define DATA_BIT        (16)
#define MODE_PIN        (4)
DFRobot_Microphone microphone(I2S_SCK_IO, I2S_WS_IO, I2S_DI_IO);
char i2sReadrawBuff[100];
void setup() {
  Serial.begin(115200);
  pinMode(MODE_PIN,OUTPUT);
  digitalWrite(MODE_PIN,LOW);//Configure the Microphone to Receive Left Channel Data
  //digitalWrite(MODE_PIN,HIGH);//Configure the Microphone to Receive Right Channel Data
  while(microphone.begin(SAMPLE_RATE, DATA_BIT) != 0){
      Serial.println(" I2S init failed");
  }
  Serial.println("I2S init success");
  
}

void loop() {
  microphone.read(i2sReadrawBuff,100);
  //Output Right Channel Data
  //Serial.println((int16_t)(i2sReadrawBuff[2]|i2sReadrawBuff[3]<<8));
  //Output Left Channel Data
  Serial.println((int16_t)(i2sReadrawBuff[0]|i2sReadrawBuff[1]<<8));
  delay(100);
}
