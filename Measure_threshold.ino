#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
    #else
    #include "WProgram.h"
    #endif

    #include "EMGFilters.h"

    #define TIMING_DEBUG 1

    #define SensorInputPin A0 // input pin number

    EMGFilters myFilter;

    SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_1000HZ;

    NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

    static int Threshold = 0;

    void setup() {
        myFilter.init(sampleRate, humFreq, true, true, true);
        Serial.begin(115200);
    }

    void loop() {
        int Value = analogRead(SensorInputPin);

        int DataAfterFilter = myFilter.update(Value);

        int envlope = sq(DataAfterFilter);

        envlope = (envlope > Threshold) ? envlope : 0;

            Serial.print("Squared Data: ");
            Serial.println(envlope);
    }
