#include "base-output.h"

DigitalOut::DigitalOut() {
        state = false;
}

DigitalOut::DigitalOut(int pin) {
        _pin = pin;
        pinMode(_pin, OUTPUT);
        off();
}

void DigitalOut::init() {
        pinMode(_pin, OUTPUT);
        off();
}

void DigitalOut::on() {
        digitalWrite(_pin, LOW);
        state = true;
}

void DigitalOut::off() {
        digitalWrite(_pin, HIGH);
        state = false;
}

void DigitalOut::set(bool state) {
        digitalWrite(_pin, state);
}

void DigitalOut::toggle() {
        digitalWrite(_pin, !digitalRead(_pin));
}

void DigitalOut::onDelay(unsigned long delay_time) {
        on();
        _delayTime = delay_time;
        _delayStart = millis();
        _delayState = true;
}

void DigitalOut::offDelay(unsigned long delay_time) {
        off();
        _delayTime = delay_time;
        _delayStart = millis();
        _delayState = true;
}

void DigitalOut::update() {
        if (_delayState) {
                unsigned long currentMillis = millis();
                if (currentMillis - _delayStart >= _delayTime) {
                        off();
                        _delayState = false;
                }
        }
}

void DigitalOut::setPins(uint8_t __pin) {
        _pin = __pin;
}

bool DigitalOut::getState() {
        return state;
}