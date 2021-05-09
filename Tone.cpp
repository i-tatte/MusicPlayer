#include "Tone.h"
#include <Arduino.h>

Tone::Tone() {
    _prevTime = micros();
    _count = 0;
    for(int i = 0; i < maxNum; i++) {
        _notes[i][0] = 0;
        _notes[i][1] = 0;
    }
}

bool Tone::addNote(int freq) {
    if(contains(freq)) return false;
    if(_count < 10) {
        _notes[_count][0] = 1000000 / freq / 2;
        _count++;
        return true;
    }
    return false;
}

bool Tone::delNote(int freq) {
    bool del = false;
    for(int i = 0; i < maxNum; i++) {
        if(_notes[i][0] == 1000000 / freq / 2) {
            del = true;
            _notes[i][0] = 0;
            _notes[i][1] = 0;
            _count--;
        }
        if(del && i < maxNum - 1) {
            _notes[i][0] = _notes[i + 1][0];
            _notes[i][1] = _notes[i + 1][1];
        }
    }
    return del;
}

bool Tone::contains(int freq) {
    for(int i = 0; i < maxNum; i++) {
        if(_notes[i][0] == 1000000 / freq / 2) {
            return true;
        }
    }
    return false;
}

void Tone::clear() {
    _prevTime = micros();
    _count = 0;
    for(int i = 0; i < maxNum; i++) {
        _notes[i][0] = 0;
        _notes[i][1] = 0;
    }
}

int Tone::calcWave() {
    if(_notes[0][0] == 0) return 0;
    long now = micros();
    int elapse = now - _prevTime;
    _prevTime = now;

    int ret = 128; //真ん中128 いっこ12
    int on = 0;
    for(int i = 0; i < maxNum; i++) {
        if(_notes[i][0] == 0) {
            break;
        }
        _notes[i][1] += elapse;
        on += (_notes[i][1] / _notes[i][0] % 2 == 0) ? 1 : -1;
    }
    return ret + on;
}
