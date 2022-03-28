
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "EMGFilters.h"
#define WINDOWS 1
#include "Mouse.h"

#include "Keyboard.h"
int platform = WINDOWS;
#define SensorInputPin A0   //sensor input pin number

unsigned long debug = 1;
unsigned long rightclick = 300;
unsigned long leftclick = 550;
unsigned long copy = 3000;
unsigned long paste = 5300;

EMGFilters myFilter;

SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

void setup()
{
  myFilter.init(sampleRate, humFreq, true, true, true);
  Serial.begin(115200);
  Keyboard.begin();
  Mouse.begin();
}

void loop()
{
  int data = analogRead(SensorInputPin);
  int dataAfterFilter = myFilter.update(data);  
  int envelope = sq(dataAfterFilter);   

  int      right1 = (envelope > rightclick) ? envelope : 0;
  int      left1 = (envelope > leftclick ) ? envelope : 0;
  int      copy1 = (envelope > copy) ? envelope : 0;
  int      paste1 = (envelope > paste) ? envelope : 0;

  if (debug > 0 )
  {
    if (getEMGCount1(paste1))
    {
      Serial.println("paste activated ");
      getEMGCount2(-1);
      getEMGCount3(-1);
      getEMGCount4(-1);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(86);
      delayMicroseconds(100);
      Keyboard.releaseAll();
      delay(1000);
    }
    else if (getEMGCount2(copy1))
    {
      Serial.println("copy activated");
      getEMGCount3(-1);
      getEMGCount4(-1);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(67);
      delayMicroseconds(100);
      Keyboard.releaseAll();
      delay(1000);
    }
    else if (getEMGCount3(left1))
    {
            if (!Mouse.isPressed(MOUSE_LEFT))
            {
              Mouse.press(MOUSE_LEFT);
              Serial.println("Press down Left click");
            }
            else if (Mouse.isPressed(MOUSE_LEFT))
            {
              Mouse.release(MOUSE_LEFT);
             Serial.println(" Release Left click");
            }
            getEMGCount4(-1);
            delay(1000);
    }
    else if (getEMGCount4(right1))
    {
      Serial.println("Press right click ");
            Mouse.press(MOUSE_RIGHT);
            Mouse.release(MOUSE_RIGHT);
    }
  }
  else {
    Serial.println(envelope);
  }
  delayMicroseconds(1000);
}

/*
   if get EMG signal,return 1;
   Following Function getEMGCount1 are all same just for different gesture
*/
int getEMGCount1(int gforce_envelope)
{
  static long integralData = 0;
  static long integralDataEve = 0;
  static bool remainFlag = false;
  static unsigned long timeMillis = 0;
  static unsigned long timeBeginzero = 0;
  static long fistNum = 0;
  static int  TimeStandard = 200;
  /*
    The integral is processed to continuously add the signal value
    and compare the integral value of the previous sampling to determine whether the signal is continuous
  */
  integralDataEve = integralData;
  integralData += gforce_envelope;
  /*
    If the integral is constant, and it doesn't equal 0, then the time is recorded;
    If the value of the integral starts to change again, the remainflag is true, and the time record will be re-entered next time
  */
  if (gforce_envelope ==  -1)     // once an action has been detected, all readings set back to 0
  {
    integralDataEve = integralData = 0;
    remainFlag = false;
    timeMillis = 0;
    timeBeginzero = 0;
    return 0 ;
  }
  if ((integralDataEve == integralData) && (integralDataEve != 0))
  {
    timeMillis = millis();
    if (remainFlag)
    {
      timeBeginzero = timeMillis;
      remainFlag = false;
      return 0;
    }
    /* If the integral value exceeds 200 ms, the integral value is clear 0,return that get EMG signal */
    if ((timeMillis - timeBeginzero) > TimeStandard)
    {
      integralDataEve = integralData = 0;
      return 1;
    }
    return 0;
  }
  else {
    remainFlag = true;
    return 0;
  }
}

int getEMGCount2(int gforce_envelope)
{
  static long integralData = 0;
  static long integralDataEve = 0;
  static bool remainFlag = false;
  static unsigned long timeMillis = 0;
  static unsigned long timeBeginzero = 0;
  static long fistNum = 0;
  static int  TimeStandard = 200;

  integralDataEve = integralData;
  integralData += gforce_envelope;
  if (gforce_envelope ==  -1)
  {
    integralDataEve = integralData = 0;
    remainFlag = false;
    timeMillis = 0;
    timeBeginzero = 0;
    return 0 ;
  }
  if ((integralDataEve == integralData) && (integralDataEve != 0))
  {
    timeMillis = millis();
    if (remainFlag)
    {
      timeBeginzero = timeMillis;
      remainFlag = false;
      return 0;
    }
    if ((timeMillis - timeBeginzero) > TimeStandard)
    {
      integralDataEve = integralData = 0;
      return 1;
    }
    return 0;
  }
  else {
    remainFlag = true;
    return 0;
  }
}

int getEMGCount3(int gforce_envelope)
{
  static long integralData = 0;
  static long integralDataEve = 0;
  static bool remainFlag = false;
  static unsigned long timeMillis = 0;
  static unsigned long timeBeginzero = 0;
  static long fistNum = 0;
  static int  TimeStandard = 200;

  integralDataEve = integralData;
  integralData += gforce_envelope;

  if (gforce_envelope ==  -1)     // once an action has been detected, all readings set back to 0
  {
    integralDataEve = integralData = 0;
    remainFlag = false;
    timeMillis = 0;
    timeBeginzero = 0;
    return 0 ;
  }
  if ((integralDataEve == integralData) && (integralDataEve != 0))
  {
    timeMillis = millis();
    if (remainFlag)
    {
      timeBeginzero = timeMillis;
      remainFlag = false;
      return 0;
    }
    if ((timeMillis - timeBeginzero) > TimeStandard)
    {
      integralDataEve = integralData = 0;
      return 1;
    }
    return 0;
  }
  else {
    remainFlag = true;
    return 0;
  }
}

int getEMGCount4(int gforce_envelope)
{
  static long integralData = 0;
  static long integralDataEve = 0;
  static bool remainFlag = false;
  static unsigned long timeMillis = 0;
  static unsigned long timeBeginzero = 0;
  static long fistNum = 0;
  static int  TimeStandard = 200;

  integralDataEve = integralData;
  integralData += gforce_envelope;

  if (gforce_envelope ==  -1)     // once an action has been detected, all readings set back to 0
  {
    integralDataEve = integralData = 0;
    remainFlag = false;
    timeMillis = 0;
    timeBeginzero = 0;
    return 0 ;
  }
  if ((integralDataEve == integralData) && (integralDataEve != 0))
  {
    timeMillis = millis();
    if (remainFlag)
    {
      timeBeginzero = timeMillis;
      remainFlag = false;
      return 0;
    }

    if ((timeMillis - timeBeginzero) > TimeStandard)
    {
      integralDataEve = integralData = 0;
      return 1;
    }
    return 0;
  }
  else {
    remainFlag = true;
    return 0;
  }
}
