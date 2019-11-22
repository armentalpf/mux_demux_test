/**
 * @file main.cpp
 * @author Francisco Armenta (francisco.armenta@sistron.com.mx)
 * @brief Main file for test SmartLink Bennette dispensary
 * @version 0.1
 * @date 2019-11-21
 * 
 * @copyright Copyright (c) 2019 SITRN SISTEMAS ELECTRONICOS S DE RL DE CV
 * 
 */

// include files
#include <Arduino.h>
#include "mux_demux.h"

// macro declaration
// Comment for release version
// #define DEBUG

// global constants
const unsigned long BAUDS = 9600;
const uint8_t ADDRESS_REQUEST = 0;
const uint8_t WAIT_ADDRS_INPUT = 1;
const uint8_t DATA_REQUEST = 2;


/**
 * @brief System setup function
 * 
 */
void setup() 
{
  // Initialize mux and demux
  muxDemuxBegin();

  // Initialize serial port
  Serial.begin(BAUDS);
  delay(1000);

  // write Header message
  Serial.println("*****************************");
  Serial.println("74150 & 74154 Test system");
  Serial.println("*****************************");
}


/**
 * @brief Main routine
 * 
 */
void loop() 
{
  static uint8_t lastState = ADDRESS_REQUEST;   /* register for last state */
  static uint8_t deviceAddrs;                          /* device address register */
  static uint8_t deviceData;                           /* device address register */
  uint8_t dataRcv;                              /* serial reception data */ 
  static String commandRcv = "";                /* string to store commands */


  switch (lastState)
  {
  case ADDRESS_REQUEST:
    Serial.println("Type an address (0 to 15)");
    lastState = WAIT_ADDRS_INPUT;
    break;
  
  case WAIT_ADDRS_INPUT:
    // wait for buffer dont empty
    if (Serial.available() > 0)
    {
      // read dat from serial port and store in a string
      dataRcv = Serial.read();
      commandRcv += (char) dataRcv;
      // wait for end of command  
      if (dataRcv == '\n')
      {
        // one or two digits
        if (commandRcv.length() > 3)
        {
          // read 1st character
          dataRcv = commandRcv.charAt(0);
          // validate data
          if (isDigit((int) dataRcv))
          {
            // save value at decens position
            deviceAddrs = (dataRcv - 0x30) * 10;
            // read 2cnd character
            dataRcv = commandRcv.charAt(1);
            // validate character
            if (isDigit((int) dataRcv))
            {
              // save value at units position
              deviceAddrs += (dataRcv - 0x30);
              // write data request message
              Serial.println("write data");
              // change state
              lastState = DATA_REQUEST;
              // clear string
              commandRcv = "";
            }
            else
            {
              // clean result
              deviceAddrs = 0;
              // clear string
              commandRcv = "";
            }
          }
          else
          {
            // clear string
              commandRcv = "";
          }                   
        }
        else
        {
          // read 1st character
          dataRcv = commandRcv.charAt(0);
          // validate character
          if (isDigit((int) dataRcv))
          {
            // save value at units position
            deviceAddrs = (dataRcv - 0x30);
            // write data request message
            Serial.println("write data");
            // change state
            lastState = DATA_REQUEST;
            // clear string
            commandRcv = "";
          }
          else
          {
            // clear string
            commandRcv = "";
          }
        }
      }
    }
    break;

  case DATA_REQUEST:
    // wait for buffer dont empty
    if (Serial.available() > 0)
    {
      // read dat from serial port and store in a string
      dataRcv = Serial.read();
      commandRcv += (char) dataRcv;
      // wait for end of command  
      if (dataRcv == '\n')
      {
        // read 1st character
        deviceData = commandRcv.charAt(0);
        // write processing message
        Serial.println("process command...");
        // write address and data xmt
        Serial.print("address: ");
        Serial.println(deviceAddrs, DEC);
        Serial.print("data: ");
        Serial.write(deviceData);
        Serial.println("");
        // clear string
        commandRcv = "";
        // send data and read it back
        Serial.print("data read: ");
        Serial.write(xmitRcv(deviceAddrs, deviceData));
        Serial.println("");
        // change state
        lastState = ADDRESS_REQUEST;
      }
    }
    break;

  default:
    break;
  }
}