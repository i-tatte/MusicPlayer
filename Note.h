#ifndef NOTE_INCLUDED
#define NOTE_INCLUDED

class Note {
public:
    Note();
    Note(float t, bool info, int freq);
    float getTime();
    bool getInfo();
    int getFreq();

private:
    float _time = 0;
    bool _info = true; //true:note_ON, false:note_OFF
    int _freq = 1;
};

#endif
