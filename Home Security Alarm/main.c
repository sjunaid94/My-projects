/*
 * Syed Junaid Khalid
 *
 * Final Project
 *
 * main.c
 *
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include "launchpad.h"
#include "motion.h"
#include "mic.h"
#include "buzzer.h"


typedef enum {Off, On, SwitchOff, SwitchOn} switch_t ;

static switch_t motionDetection = Off; //initially motion will be off and Mic will be on.
static switch_t micInput = On;

// Buzzer-related constants
#define BUZZER_CHECK_INTERVAL 10
#define BUZZER_ON_TIME  30000
#define BUZZER_OFF_TIME (3000 - BUZZER_ON_TIME)

typedef struct
{
    switch_t state;            // the running state of the buzzer system
    bool buzzing;       // if the buzzer is buzzing
    int32_t timeLeft;   // the time left for the buzzer to buzz or not buzz
} buzzer_t;
static buzzer_t buzzer = { Off, false, 0 };

// The buzzer play callback function
void buzzerPlay(uint32_t time)
{
    uint32_t delay = BUZZER_CHECK_INTERVAL;     // the delay for next callback

    switch (buzzer.state)
    {
    case Off:           // the buzzer system is turned off, do nothing
        break;

    case On:            // the buzzer system is active, turn buzzer on and off

        if (buzzer.buzzing)

            {
                //uprintf("%s\n\r", "Motion detecting!");
            // If the buzzer has been buzzing for enough time, turn it off
                if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
                {
                    buzzerOff();
                    buzzer.buzzing = false;
                    buzzer.timeLeft = BUZZER_OFF_TIME;
            }
        }
            else
            {
            // If the buzzer has been silent for enough time, turn it on
                if ((buzzer.timeLeft -= BUZZER_CHECK_INTERVAL) <= 0)
                {
                    buzzerOn();
                    buzzer.buzzing = true;
                    buzzer.timeLeft = BUZZER_ON_TIME;
            }
        }
        break;

    case SwitchOff:             // De-activate the buzzer system
        if (buzzer.buzzing)
            buzzerOff();
        buzzer.state = Off;
        buzzer.buzzing = Off;
        break;

    case SwitchOn:              // Activate the buzzer system
        buzzerOn();
        buzzer.state = On;
        buzzer.buzzing = true;
        buzzer.timeLeft = BUZZER_ON_TIME;
        break;
    }

    // schedule the next callback
    schdCallback(buzzerPlay, time + delay);
}


uint32_t delay; // to delay the led

void
InputVal(uint32_t time)                         //Sound Sensor function to activate when push button SW2 is pressed
{

   uint32_t delay = 1000;
    int sense = SoundVal();                        //SoundVal comes from assembly
    printf("%u\n",sense);
    if (micInput == On){                       //If sound sensor detects sound it will turn on
    if (sense>= 750){                         //Only detects sound frequency over 750
     ledTurnOnOff(true, false, false);           //LED color red
               delay = 3000;
               //Delay of light for 3 seconds.
               buzzer.state = SwitchOn;
           }
           else {
               ledTurnOnOff(false, false, false);
               //if no sound LED is off
               buzzer.state = SwitchOff;
                          delay = 1000;
           }
    }
    schdCallback(InputVal, time + delay);
}


void
MotionCheck(uint32_t time)                          //For Motion Activation
{

   uint32_t delay = 100;

        if(motionDetection == On){                      //Checks for Motion Detection
         if(motionDetect()){                        //If motion is detected, LED TURNS ON
                ledTurnOnOff(true, false, false);     //LED color red
                delay = 1000;
                //Current led is delayed for 3 seconds
                buzzer.state = SwitchOn;

        }
        else {
            ledTurnOnOff(false, false, false);
            //If nothing is detected, LED will stay off
            buzzer.state = SwitchOff;
            delay = 1000;
        }

        }
     schdCallback(MotionCheck, time + delay);
}


void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay = 250;

    switch (code) {
    case 1:                         // SW1 is the case that it's day time and is only using the motion sensor

        uprintf("%s\n\r" ,"Motion Activation");
            micInput = Off;      //Sound State is off since it's day time
        if(motionDetection == On){
            motionDetection = Off;
        }
        else{
            motionDetection = On;
        }
        delay = 250;
        break;

    case 2:                         // SW2 is the case where it's night time and is only using the sound sensor

        uprintf("%s\n\r", "Sound Activation");
        motionDetection = Off;     // Motion State is off since it's night time
        if(micInput == On){
           micInput = Off;
        }
        else{
            micInput = On;
        }
        delay = 250;
        break;

    }
     schdCallback(checkPushButton, time + delay);
    }


int main(void){
    lpInit();
    motionInit();
    soundInitializer();
    buzzerInit();

    uprintf("%s\n\r", "Final Project");

    schdCallback(buzzerPlay, 1000);
    schdCallback(checkPushButton, 1010);
    schdCallback(MotionCheck, 1015);
    schdCallback(InputVal, 1030);

    while (true) {
        schdExecute();
    }
}
