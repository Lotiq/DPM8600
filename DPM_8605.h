/*
  DPM_8605.h - Library for communicating with DPM_8605 power converters.
  Created by Timothy Lobiak, February 15, 2020.
  Released into the public domain.

  Methods:
  DPM_8605() - Constructor
  begin(SerialRef) - Initialization. Needs to be passed reference to serial
  power(on, statusCode) - Turning the power on and off. True switches the power on, while false switches it off. statusCode is necessary for giving out status code
  write(vc, value, statusCode) - vc (true for voltage, false for current)

  Errors:
  0 - NO ERROR
  11 - fail on switching power.
  12 - fail on write.
  13 - fail during read.
  
*/

#ifndef DPM_8605_h
#define DPM_8605_h

#include "Arduino.h"

class DPM_8605
{
public:
  DPM_8605(void);

  bool begin(HardwareSerial &serialRef, int8_t maxRetry);
  void power(bool on, int8_t &statusCode);
  void write(bool vc, float value, int8_t &statusCode);
  float read(bool vc, int8_t &statusCode);

private:
  bool listen(String &response);
  float processString(String str);
  HardwareSerial *_serialRef;
  int8_t _maxRetry;
};

#endif
