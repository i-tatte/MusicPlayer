#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

class Queue {
public:
    Queue();
    int get(int index);
    void push(int num);
    void clear();
    int getAverage();

private:
    static const int _size = 8;
    int _data[_size];
    int _pointer;
    int _dataNum;
};

#endif
