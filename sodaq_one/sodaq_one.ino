#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <Sodaq_UBlox_GPS.h>
#include "nsarduino.h"
#include "internal.h"

#define debugSerial SerialUSB

void signal_loop_start()
{
    blink(30); delay(50);
    blink(30); delay(50);
    blink(30); delay(50);
}

#define TEMP_PIN 2
#define LOUDNESS_PIN 0
#define BUZZER_PIN 6
#define WATER_SENSOR_PIN 6

int cnt;

void setup() {
    while ((!SerialUSB) && (millis() < 10000)){
        // Wait 10 seconds for the Serial Monitor
    }

    loraSetup();

    /* Enable the pins 2/3, 6/7 and 8/9 */
    pinMode(11, OUTPUT);
    digitalWrite(11, HIGH);

    setupLED();

    // setupWater(WATER_SENSOR_PIN);
    setupBuzzer(BUZZER_PIN);

    // gpsSetup();

    // setupBuzzer();

    // setupCompass();
//    setupSunLight();

    buzz(BUZZER_PIN, 100);
    cnt = 0;
}

String tmps;
float tmp;

#define SLEEPTIME 10000

#define LOUDNESS_AVG 60
int loudnesses[LOUDNESS_AVG];

void loop() {
    signal_loop_start();

    debugSerial.println("Deveui=" + String(LORADEV) + " => " + String(LORA_ADDR));

    /* if(hasWater(WATER_SENSOR_PIN)) { */
    /*     debugSerial.println("Having water"); */
    /*     loraSend(getWater(WATER_SENSOR_PIN)); */
    /*     buzz(BUZZER_PIN, 5000); */
    /* } else { */
    /*     loraSend(getWater(WATER_SENSOR_PIN)); */
    /*     debugSerial.println("it's dry"); */
    /* } */

//    loraSend(getSunLight());
    // loraSend(getTempHumidHDC1000());
    // loraSend(getCompass());

    sendIntAsString("battery=", getBattery());

    /* Tracker code */
    /* if((tmps = gpsGetPostion(120)) != "") { */
    /*     loraSend(tmps); */
    /* } */

    /* Temp & loudness code */
    if(cnt < LOUDNESS_AVG) {
        loudnesses[cnt] = readLoudness(LOUDNESS_PIN);
        debugSerial.println("temploudness=" + String(loudnesses[cnt]));
        cnt++;
    } else {
        tmp = 0;
        for(cnt = 0; cnt < LOUDNESS_AVG; cnt++) {
            tmp += loudnesses[cnt];
        }
        tmp = tmp / (float) (cnt+1);

        sendFloatAsString("loudness=", tmp);
        sendFloatAsString("temperature=", getTemperature(TEMP_PIN));
        cnt = 0;
    }

    delay(SLEEPTIME);
}


/* Node = 2
48
48
48
52
65
51
48
66

hex: 3030303441333042
*/

/* temp / humidity code (not functional)
