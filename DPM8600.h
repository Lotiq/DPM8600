/*
  DPM8600.h - Library for communicating with DPM_8600 series power converters. Works for TTL DPM8600s only. 
  Created by Timothy Lobiak, February 15, 2020.
  Released into the public domain.

  -------------------- METHODS --------------------
  DPM8600(address) - Constructor with a string address. Regular address is "01".
    * address - [int8_t] - address of the power converter. DEFAULT = 1

  begin(serial, maxRetry) - Initialization. Returns [Int] 1  on success, or negative error number on error.
    * serial - [Hardware Serial] - a serial object passed as reference for communication.
    * maxRetry - [int8_t] - maximum number of retries upon failing to communicate with the power converter before generating an error. DEFAULT = 3

  power(on) - Turning the power on and off. Returns [Int] 1 on success, or negative error number on error.
    * on - [Bool] - True for power on, false for power off.

  write(cmd, value) - writing command. CMD is a character, upper or lower case that designates the command to be used. Value is the value that is being set. 
    * 'v' or 'V' - [Float] - max voltage in volts
    * 'c' or 'C' - [Float] - max current in amps
    * 'p' or 'P' - Same as method 'power(on)'. 1 for on, 0 for off, anything else is an error

  writeVC(v,c) - Writing command for both current and voltage together. Both are float values, voltage goes first.
  Returns 1 on success, or negative error number on error.

  read(cmd) - Read function. Returns [Float] values of the specified command. Available commands are:
   - 'v' or 'V' - voltage.
   - 'c' or 'C' - current.
   - 'p' or 'P' - power. 0 - power off, 1 - power on.
   - 's' or 'S' -  CC / CV status. 0 - CV, 1 - CC.
   - 'm' or 'M' - max current. Outputs the max current of the converter (e.g. 5, 8, 16 or 24).
   - 't' or 'T' (or any other character) - internal temperature. Outputs internal temperature of the machine in CELCIUS.
   Returns variable on success, or negative error number on error.

  -------------------- ERRORS --------------------

  Errors are returned as NEGATIVE numbers for ALL functions. E.g. read('v') can return -10, indicating an error. This way you can check if the function succeeds for any of them.
  -1 - error on begining DPM8600.
  -10 - error on voltage reading
  -11 - error on current reading.
  -12 - error on power reading.
  -13 - error on CC/CV reading.
  -14 - error on max current reading.
  -15 - error on temperature reading.
  -16 - value outside boundaries
  -20 - wrong value sent to power writing function
  -21 - error setting current
  -22 - error setting power on/off.
  -23 - error setting voltage
  -24 - error setting current and voltage
  -25 - wrong writing command
*/

#ifndef DPM8600_h
#define DPM8600_h

#include "Arduino.h"

class DPM8600
{
  public:
    DPM8600(int8_t address = 1);
    int begin(HardwareSerial &serial, int8_t maxRetry = 3);
    int power(bool on);
    int write(char cmd, float value);
    int writeVC(float v, float c); 
    float read(char cmd);

  private:
    bool listen(String &response);
    float processString(String str);
    
    HardwareSerial *_serial;
    int8_t _maxRetry;
    String _address;
};

#endif
