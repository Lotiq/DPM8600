
#include <Arduino.h>
#include <DPM_8605.h>

DPM_8605::DPM_8605()
{}

bool DPM_8605::begin(HardwareSerial& serialRef, int8_t maxRetry)
{
    _serialRef = &serialRef;
    _maxRetry = maxRetry;
}

bool DPM_8605::listen(String &response)
{
    unsigned long errorTimer = millis();
    while ((millis() - errorTimer) < 200)
    {
        if (_serialRef->available())
        {
            char inChar = (char)_serialRef->read();
            if (inChar == '\n')
            {
                // CHECK IF I \r goes there too
                return true;
            }
            else
            {
                response += inChar;
            }
        }
    }
    return false;
}

void DPM_8605::power(bool on, int8_t &statusCode)
{
    String response = "";
    int8_t retries = 0;
    bool completed = false;

    do
    {
        if (on)
        {
            _serialRef->println(F(":01w12=1,"));
        }
        else
        {
            _serialRef->println(F(":01w12=0,"));
        }

        completed = listen(response);
        
        // Add a retry
        retries += 1;

    } while ((!completed) && (retries < _maxRetry));

    if (!completed)
    {
        statusCode = 11;
        return;
    }
    else
    {
        statusCode = 0;
        return;
    }
}

void DPM_8605::write(bool vc, float value, int8_t &statusCode)
{
    int x = 0;
    String function = "";

    // x100 multiplication for voltage and x1000 for current because of the resolution
    if (vc)
    {
        x = floor(value * 100);
        function = "10";
    }
    else
    {
        x = floor(value * 1000);
        function = "11";
    }

    int8_t retries = 0;
    String response = "";
    bool completed = false;

    do
    {
        // Send command
        _serialRef->println(":01w" + function + "=" + String(x) + ",");

        // Listen for reply
        completed = listen(response);

        // Add a retry
        retries += 1;

    } while ((!completed) && (retries < _maxRetry));

    if (!completed)
    {
        statusCode = 12; // no response from setting variable
        return;
    }
    else
    {
        statusCode = 0;
        return;
    }
}

float DPM_8605::read(bool vc, int8_t &statusCode)
{

    int retries = 0;
    bool completed = false;
    String response = "";

    do
    {
        // Send command
        if (vc)
        {
            _serialRef->print(F(":01r30=0,\r\n"));
        }
        else
        {
            _serialRef->print(F(":01r31=0,\r\n"));
        }

        // Listen for a reply
        completed = listen(response);

        // Add a retry
        retries += 1;

    } while ((!completed) && (retries < _maxRetry)); // Stop running either when data received or max attempts happened.

    if (!completed)
    {
        statusCode = 13; // data not recieved
        return 0;
    }

    if (vc)
    {
        return (processString(response) / 100);
    }
    else
    {
        return (processString(response) / 1000);
    }
}

float DPM_8605::processString(String str)
{
    str.remove(0, 7);
    str.replace(".", "");
    return atof(str.c_str());
}
