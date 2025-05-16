/*!
 * @file DFRobot_Microphone.h
 * @brief This is the implementation of the microphone library methods.
 * @copyright Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-2-18
 * @url https://github.com/DFRobot/DFRobot_MSM261
 */
#ifndef _DFROBOT_MICROPHONE_H_
#define _DFROBOT_MICROPHONE_H_

#include "Arduino.h"
#include <driver/i2s.h>
#include "stdio.h"

#define I2S_NUM         (i2s_port_t)(0)

//#define ENABLE_DBG //!< Enable this macro to view detailed program execution logs
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
class DFRobot_Microphone
{
    
public:
    /**
     * @fn DFRobot_Microphone
     * @brief Microphone constructor
     * @param bckIoNum Clock pin configuration
     * @param wsIoNum  WS pin configuration
     * @param dInNum   Data acquisition pin
     */
    DFRobot_Microphone(uint8_t bckIoNum, uint8_t wsIoNum, uint8_t dInNum);
    /**
     * @fn begin
     * @brief Initialize the device
     * @param baudRate Communication rate
     * @param bit Data bit width
     * @return Initialization status
     */
    uint8_t begin(uint16_t baudRate,uint8_t bit);

    /**
     * @fn read
     * @brief Obtain data returned by the microphone
     * @param buffer Buffer to store the acquired data
     * @param len    Length of the data to acquire
     * @return Length of the acquired data
     */
    uint32_t read(char* buffer,size_t len);

    /**
     * @fn createWavHeader
     * @brief Construct the header for a WAVE file
     * @param header The WAVE file header to construct
     * @param totalDataLen Length of the data to write
     * @param longSampleRate Sampling rate
     * @param channels Number of channels
     * @param byteRate Byte rate, calculated as: sampling rate × number of channels × data bit width ÷ 8
     * @param blockAlign Block alignment, calculated as: number of channels × data bit width ÷ 8
     * @return None
     */
    void createWavHeader(byte* header, int totalDataLen, int longSampleRate, uint8_t channels, int byteRate, uint8_t blockAlign);
   
private:
    uint8_t _bckIoNum;
    uint8_t _wsIoNum;
    uint8_t _dInNum;
    uint8_t _mode;

};
#endif