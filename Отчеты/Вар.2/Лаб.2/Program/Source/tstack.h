#ifndef TSTACK_H
#define TSTACK_H

//шаблон класса Стек
template<class T>
class TStack{
public:
    T *data;
    int Top;
    int maxsize;
    explicit TStack(int size = SIZESTACK):Top(0),maxsize(size),data(new T[size]){}
    ~TStack(){delete[] data;}
    inline void push(T ch) {
        if(Top<maxsize)  {
            data[Top++]=ch;
        }
        else error("Stack full");
    }
    inline T pop() {
        if(Top!=0) {
            Top--;
            return data[Top];
        }
        else error("Stack empty");
        return 0.0;
    }
    inline int size(){return Top;}
    inline bool isempty() {return (Top==0);}
};

#endif // TSTACK_H
