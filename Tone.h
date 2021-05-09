#ifndef TONE_INCLUDED
#define TONE_INCLUDED

class Tone {
public:
    Tone();
    bool addNote(int freq);
    bool delNote(int freq);
    bool contains(int freq);
    void clear();
    int calcWave();

private:
    static const int maxNum = 10;
    int _notes[maxNum][2]; //[t/2][count(us)]*10
    long _prevTime;
    int _count;
};

#endif
