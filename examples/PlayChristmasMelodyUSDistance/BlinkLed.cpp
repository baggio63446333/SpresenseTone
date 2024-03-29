/*
 * This library let multiple LEDs, connected to any Arduino output, blink non blocking and independently.
 * therefore you must call update continuously in loop()
 */
#include <Arduino.h>

#include "BlinkLed.h"

BlinkLed::BlinkLed(uint8_t aLedPin) {
    init(aLedPin, false);
    setOnOffTime(1000, 1000);
}

BlinkLed::BlinkLed(uint8_t aLedPin, bool aInitState, unsigned int aOnTime, unsigned int aOffTime) {
    init(aLedPin, aInitState);
    setOnOffTime(aOnTime, aOffTime);
}

void BlinkLed::init(uint8_t aLedPin, bool aInitState) {
    pin = aLedPin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
    state = aInitState;
}

/*
 * No count specified here, so set to BLINK_LED_FOREVER
 */
void BlinkLed::setOnOffTime(unsigned int aOnTime, unsigned int aOffTime) {
    onDelay = aOnTime;
    offDelay = aOffTime;
}

// must be called continuously in loop()
void BlinkLed::update() {

    if (!enabled) {
        return;
    }

    if (state) {
        // check for on time gone to change state
        if ((millis() - lastUpdate >= onDelay)) {
            toggle();
            // count blinks
            if (numberOfBlinks > 0) {
                numberOfBlinks--;
                if (numberOfBlinks == 0) {
                    // stop blinking
                    enabled = false;
                }
            }
        }
    } else if ((millis() - lastUpdate >= offDelay)) {
        toggle();
    }
}

void BlinkLed::start(signed int aBlinkCount, unsigned int aOnTime, unsigned int aOffTime) {
    onDelay = aOnTime;
    offDelay = aOffTime;
    start(aBlinkCount);
}

/*
 * set to 50% duty cycle
 */
void BlinkLed::start(signed int aBlinkCount, unsigned int aPeriod) {
    onDelay = offDelay = aPeriod / 2;
    start(aBlinkCount);
}

/*
 * set to 50% duty cycle
 */
void BlinkLed::start(signed int aBlinkCount) {
    numberOfBlinks = aBlinkCount;
    start();
}

// Force ON and enable blink
void BlinkLed::start() {
    digitalWrite(pin, HIGH);
    state = true;
    enabled = true;
    lastUpdate = millis();
}

/*
 * set to 50% duty cycle
 */
void BlinkLed::blink(signed int aBlinkCount, unsigned int aPeriod) {
    start(aBlinkCount, aPeriod);
    while (numberOfBlinks > 0) {
        delay(1);
        update();
    }
}

/*
 * No count specified here, so set to BLINK_LED_FOREVER
 */
void BlinkLed::startWithOnOffTime(unsigned int aOnTime, unsigned int aOffTime) {
    start(BLINK_LED_FOREVER, aOnTime, aOffTime);
}

/*
 * set to 50% duty cycle
 */
void BlinkLed::startWithPeriod(unsigned int aPeriod) {
    start(BLINK_LED_FOREVER, aPeriod);
}

/*
 * set to 50% duty cycle
 */
void BlinkLed::startWithFrequency(float aFrequency) {
    offDelay = onDelay = 500.0 / aFrequency;
    start(BLINK_LED_FOREVER);
}

void BlinkLed::startWithOnTime(unsigned int aOnTime) {
    onDelay = aOnTime;
    start();
}

void BlinkLed::startWithOffTime(unsigned int aOffTime) {
    offDelay = aOffTime;
    start();
}

// Toggle state and set new timestamp
void BlinkLed::toggle() {
    state = !state;
    digitalWrite(pin, state);
    lastUpdate = millis();
}

// Force ON and disable blink
void BlinkLed::on() {
    digitalWrite(pin, HIGH);
    state = true;
    enabled = false;
}

// Force off and disable blink
void BlinkLed::stop() {
    digitalWrite(pin, LOW);
    state = false;
    enabled = false;
}

// Force off and disable blink - the same as stop
void BlinkLed::off() {
    digitalWrite(pin, LOW);
    state = false;
    enabled = false;
}

/*
 * Might be useful if using negative logic i.e. LED is connected to VCC.
 * You can also access "enabled" directly, but this is not the recommended way to use a class ;-)
 */
void BlinkLed::setEnabled(bool aIsEnabled) {
    enabled = aIsEnabled;
}
