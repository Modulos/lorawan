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

}

String tmps;
float tmp;

#define SLEEPTIME 10000
#define LOUDNESS_AVG 3

int loudnesses[LOUDNESS_AVG];

void loop() {
    signal_loop_start();

    debugSerial.println("Deveui = " + String(LORADEV) + " => " + String(LORA_ADDR));

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

    /* Tracker code */
    /* if((tmps = gpsGetPostion(120)) != "") { */
    /*     loraSend(tmps); */
    /* } */

    sendIntAsString("battery=", getBattery());

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

        sendFloatAsString("temperature=", getTemperature(TEMP_PIN));
        sendFloatAsString("loudness=", tmp);


        /* audio representation of temperature */
        tmp = getTemperature(TEMP_PIN);

        if(tmp < 0) {
            buzz(BUZZER_PIN, 50);
        }

        if(tmp > 0 && tmp <= 20) {
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
        }

        if(tmp > 20 && tmp <= 30) {
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
        }

        if(tmp > 30 && tmp <= 40) {
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
            delay(50);
            buzz(BUZZER_PIN, 100);
        }

        if(tmp > 40) {
            buzz(BUZZER_PIN, 3000);
            delay(50);
        }



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
