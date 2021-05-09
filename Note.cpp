#include "Note.h"

Note::Note() {
    _time = 0;
    _info = false;
    _freq = 0;
}

Note::Note(float t, bool info, int freq) {
    _time = t;
    _info = info;
    _freq = freq;
}

float Note::getTime() {
    return _time;
}

bool Note::getInfo() {
    return _info;
}

int Note::getFreq() {
    return _freq;
}
