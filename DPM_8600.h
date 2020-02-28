/*
  DPM_8600.h - Library for communicating with DPM_8600 series power converters. Works for TTL DPMs only. 
  Created by Timothy Lobiak, February 15, 2020.
  Released into the public domain.

  Methods:
  DPM_8600(address) - Constructor with a string address. Regular address is "01".

  begin(serial, maxRetry) - Initialization. Needs to be passed reference to a !HARDWARE! serial and maximum number of retries for communication.
  Returns 1 on success, or negative error number on error.

  power(on) - Turning the power on and off. True switches the power on, while false switches it off. 
  Returns 1 on success, or negative error number on error.

  write(cmd, value) - writing command. CMD is a character, upper or lower case that designates the command to be used. Value is the value that is being set. 
    - 'v' or 'V' - max voltage
    - 'c' or 'C' - max current
    - 'p' or 'P' (or any other character) - Same as method 'power(on)'. 1 for on, 0 for off.

  writeVC(v,c) - Writing command for both current and voltage together. Both are float values, voltage goes first.
  Returns 1 on success, or negative error number on error.

  read(cmd) - Read function that returns float values of the specified command. Available commands are:
   - 'v' or 'V' - voltage.
   - 'c' or 'C' - current.
   - 'p' or 'P' - power. 0 - power off, 1 - power on.
   - 's' or 'S' -  CC / CV status. 0 - CV, 1 - CC.
   - 'm' or 'M' - max current. Outputs the max current of the converter (e.g. 5, 8, 16 or 24).
   - 't' or 'T' (or any other character) - internal temperature. Outputs internal temperature of the machine in !CELCIUS!.
   Returns variable on success, or negative error number on error.

  Errors are returned as !NEGATIVE! numbers for !ALL! functions. E.g. read('v') can return -10, indicating an error. This way you can check if the function passes for any of them.
  -1 - error on begining DPM8600.
  -10 - error on voltage reading
  -11 - error on current reading.
  -12 - error on power reading.
  -13 - error on CC/CV reading.
  -14 - error on max current reading.
  -15 - error on temperature reading.
  -20 - wrong value sent to power writing function
  -21 - error setting current
  -22 - error setting power on/off.
  -23 - error setting voltage
  -24 - error setting current and voltage
*/

#ifndef DPM_8600_h
#define DPM_8600_h

#include "Arduino.h"

class DPM_8600
{
  public:
    DPM_8600(int8_t address);
    int begin(HardwareSerial &serial, int8_t maxRetry);
    int power(bool on);
    int write(char cmd, float value);
    int writeVC(float v, float c); 
    float read(char cmd);

  private:
    bool listen(String &response);
    float processString(String str);
    
    HardwareSerial *_serial;
    int8_t _maxRetry;
    int _maxCurrent;
    String _address;
};

#endif
