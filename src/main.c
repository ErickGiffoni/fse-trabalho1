/**
 * @file main.c
 
 * @author Erick Giffoni (github.com/ErickGiffoni)
 * @brief C code for communication in a Raspberry 4 using UART
 * @version 0.1
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "uart/uart.h"

int main(){

   openUart("/dev/serial0");
   setCommunicationOptions();

   sendString("alo sou eu bola de fogo", 23);
   getStringResponse();

   sendInt(49);
   getIntResponse();

   sendFloat(33.33);
   getFloatResponse();

   closeUart();

   return 0;
} // end of main
