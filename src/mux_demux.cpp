/**
 * @file mux_demux.cpp
 * @author Francisco Armenta (francisco.armenta@sistron.com.mx)
 * @brief Driver file for 74154 & 74150 multiplexor and demux 
 * @version 0.1
 * @date 2019-11-21
 * 
 * @copyright Copyright (c) 2019 SITRN SISTEMAS ELECTRONICOS S DE RL DE CV
 * 
 */

// Include files
#include "mux_demux.h"

// Debug process declaration
// #define MUX_DEMUX_DEBUG

// Macro declaration
const byte CNT_PINS = 4;                /* Control pins number in 74150 and 74154 */
const byte ADRSS[4] = {2, 3, 4, 5};     /* selector addres pins */
const byte STROBE = 6;                  /* enable pin for inputs multiplexor */
const byte DATA_IN = 7;                 /* serial data output for input multiplexor */
const byte OUT1_EN = 8;                 /* enable pins for demux */
const byte OUT2_EN = 9;


// Local functions
/**
 * @brief Select I/O address in 74150 and 74154 control pins
 * 
 * @param address addres to write
 */
void writeAddrss(uint8_t address)
{
    for (size_t i = 0; i < CNT_PINS; i++)
    {
        // read 4 less significant bits from address value
        if ((address >> i) & 1)
        {
            digitalWrite(ADRSS[i], HIGH);
        }
        else
        {
            digitalWrite(ADRSS[i], LOW);
        }
        
        // debug: Tag WriteAddrss
        #ifdef MUX_DEMUX_DEBUG
            Serial.print("adress[");
            Serial.print(i, DEC);
            Serial.println("]: ");
            Serial.print(digitalRead(ADRSS[i]), DEC);
            Serial.println("");
        #endif
    }
    
}

// Functions implementation
/**
 * @brief Define I/O pins and set initial conditions for 74150 and 74154 devices
 * 
 *  This function should be call before any other
 */
void muxDemuxBegin(void)
{
    //   define I/O pins
    pinMode(STROBE, OUTPUT);
    pinMode(OUT1_EN, OUTPUT);
    pinMode(OUT2_EN, OUTPUT);
    pinMode(DATA_IN, INPUT);

    for (size_t i = 0; i < CNT_PINS; i++)
    {
        pinMode(ADRSS[i], OUTPUT);
    }
  
    //  Disable Demux 74154
    digitalWrite(OUT1_EN, HIGH);
    digitalWrite(OUT2_EN, LOW);

    // Disable Multiplexor 74150
    digitalWrite(STROBE, HIGH);

    // Set initial addres to 0
    for (size_t i = 0; i < CNT_PINS; i++)
    {
        digitalWrite(ADRSS[i], LOW);
    }
}


/**
 * @brief Function to transmit at n address and receive from m address
 * 
 * @param addrXmt addres to transmit
 * @param dataXmt data to transmit
 * @return uint8_t value read
 */
uint8_t xmitRcv(uint8_t addrXmt, uint8_t dataXmt)
{
    uint8_t dataRcv = 0;    /* register to store data read */
    
    // write address to transmit
    writeAddrss(addrXmt);
    // transmit data bit per bit
    for (size_t i = 0; i < 7; i++)
    {
        if (!((dataXmt >> i) & 1))
        {
            digitalWrite(OUT1_EN, HIGH);
        }
        else
        {
            digitalWrite(OUT1_EN, LOW);
        }
        // enable read
        digitalWrite(STROBE, LOW);
        // read data and save in register
        dataRcv |= digitalRead(DATA_IN) << i; 

        // debug: Tag xmitRcv
        #ifdef MUX_DEMUX_DEBUG
            Serial.print("data sent: ");
            Serial.print(dataXmt, BIN);
            Serial.println("");
            Serial.print("G2 pin: ");
            Serial.print(digitalRead(OUT1_EN), DEC);
            Serial.println("");
            Serial.print("data in: ");
            Serial.print(dataRcv, BIN);
            Serial.println("");
        #endif
    }

    // disable read
    digitalWrite(STROBE, HIGH);
    // return data read
    return dataRcv;
}