#include <DPM8600.h>

DPM8600 converter;
float v;
void setup() {
  Serial.begin(9600);

  // Initating power converter
  converter.begin(Serial);

  // Setting both Voltage and Current to 2V and 0.1A respectively
  converter.writeVC(2, 0.1);
}

void loop() {

  // Turn on the power
  converter.power(true);
  delay(2000);

  // Read voltage if we want to. 
  // Measurements should be taken at least 0.5s after power on, as converter needs time to adjust
  v = converter.read('v');

  // Turn off the power
  converter.power(false);
  delay(2000);
}
