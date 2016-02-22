/*
 SAA1064 Library - Hex Counter

 This library works with the SAA1064 7 segement LED controller.
 It should also work with any compatable controller, but I haven't
 tested it.
 The display used is a standard two digit - 7 segment display 
 (kingbright DA56-11EWA or compatable)

 Hex example 

 This example displays the numbers 1 through FFFF on the display

    The Circuit:
    
    ----SAA1064----
    PIN24 to Arduino Analog pin 5
    PIN25 to Arduino Analog pin 4
    4.7k Pull up resitors between live and pins 24 and 25
    2nf Ceramic capacitor between PIN2 and ground
    All other connections as per datasheet
    2N2222A transistors connected to PIN11 and PIN14 as per datasheet   
   
    ----LED DISPLAY---
    As per datasheet.
    
    SAA1064 datasheet available from: 
    http://www.datasheetcatalog.org/datasheet/philips/SAA1064.pdf
    
    DA56-11EWA datasheet available from: 
    http://www.datasheetsite.com/datasheet/DA56-11EWA
*/
 #include <Wire.h>
 #include <SAA1064.h>

 SAA1064 led(0x70); //create new LED device at address 0x70. The library automatically
                    //compensates for the 7 digit address system used by the arduino
 
 void setup()
 {
   led.multiplexOn(); //turn on multiplexing so that we can use more than 2 digits
 }
 
 void loop()
 {
   for (long i = 0; i < 0xFFFF; i++)
   {
     led.sendHex(i); //send the number to the display
     delay(20);
   }
 }
