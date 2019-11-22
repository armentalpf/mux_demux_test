/**
 * @file mux_demux.h
 * @author Francisco Armenta (francisco.armenta@sistron.com.mx)
 * @brief Header file for 74154 & 74150 multiplexor and demux driver
 * @version 0.1
 * @date 2019-11-21
 * 
 * @copyright Copyright (c) 2019 SITRN SISTEMAS ELECTRONICOS S DE RL DE CV
 * 
 */
#ifndef MUX_DEMUX_H
#define MUX_DEMUX_H

// include files 
#include <Arduino.h>


// Functions declaration 
/**
 * @brief Initialization funtion for demux and mux devices
 * 
 */
void muxDemuxBegin(void);

/**
 * @brief Function to transmit at n address and receive from m address
 * 
 * @param addrXmt addres to transmit
 * @param dataXmt data to transmit
 * @return uint8_t value read
 */
uint8_t xmitRcv(uint8_t addrXmt, uint8_t dataXmt);    




#endif