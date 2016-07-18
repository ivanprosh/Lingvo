#include <iostream>
#include <string>
#include <map>
#include <cctype>
//#include <sstream>
#include <conio.h>

using namespace std;

int no_of_errors;
int no_of_strings=0;
int error(const string &s);

const short SIZEPROG = 64000;
const short SIZEVART = 20;
const short SIZESTACK = 256;
const short SIZENAME = 10;

unsigned char Program[SIZEPROG]; /* код программы*/
hash_map<string,int> table(int(),10);

//шаблон класса Стек
template<class T>
class TStack{
    T *data;
    int Top;
    int maxsize;
    explicit TStack(int size = SIZESTACK):Top(0),maxsize(size),data(new T[size]){}
    ~TStack(){delete[] data;}
    inline push(T ch) {
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
    inline bool isempty() {return (Top==0);}
};

TStack<double> Stack;

double mul()
{
    if(Dstack.size()>1)
    {
        double x = Dstack.top();
        Dstack.pop();
        double y = Dstack.top();
        Dstack.pop();
        return(x*y);
    }
    else return error("cann't execute mul, stack is empty");
}
double div()
{
    if(Dstack.size()>1)
    {
        double y = Dstack.top();
        Dstack.pop();
        double x = Dstack.top();
        Dstack.pop();
        if(y) return(x/y);
        else return error("cann't execute div, devide by zero");
    }
    else return error("cann't execute div, stack is empty");
}
void add()
{
    if(Dstack.size()>1)
    {
        double x = Dstack.top();
        Dstack.pop();
        double y = Dstack.top();
        Dstack.pop();
        Dstack.push(x+y);
    }
    else error("cann't execute add, stack is empty");
}
void diff()
{
    if(Dstack.size()>1)
    {
        double y = Dstack.top();
        Dstack.pop();
        double x = Dstack.top();
        Dstack.pop();
        Dstack.push(x-y);
    }
    else error("cann't execute diff, stack is empty");
}

void add_var(const string& name)
{
    if (table.find(name)!= table.end()) error("duplicate variable");
    else{
        if (table.size() > SIZEVART) error("too many variables");
        else table[name] = 0.0;
    }
}
void set_var(const string& name,double value)
{
    try{
        table.at(name) = value;
    }
    catch (out_of_range(const string msg)) {
        error("Cann't set variable.");
    }
}

void put_commd(unsigned char** prg, unsigned char value)
{
    **prg = value;
    (*prg)++;
}

void put_value(unsigned char** prg, double value)
{
    int i;
    unsigned char* ptr = (unsigned char*)&value;
    for (i=0; i<sizeof(double); i++)
        put_commd(prg, ptr[i]);
}
void put_strng(unsigned char** prg, unsigned char* string)
{
    int len = strlen(string);
    if (len >= SIZENAME-1) len = SIZENAME;
    strncpy(*prg, string, SIZENAME);
    *prg += len+1;
}

int error(const string &s)
{
    no_of_errors++;
    cerr<<no_of_strings<<" :-: ERROR:"<<s<<'\n';
    return 1;
}

void start()
{
;
}

int main(int argc, char* argv[])
{
    unsigned char *ptr = Program;

    //добавить переменную
    add_var("x");
    add_var("y");
    add_var("x");
    //установить значение
    set_var("pi",3.1415); //выпадет исключение
    add_var("pi");
    set_var("pi",3.1415); //теперь ok

    // так создаются переменные в программе ВМ
    put_commd(&ptr, CNVAR); put_strng(&ptr, "x1");
    put_commd(&ptr, CPUSH); put_value(&ptr, 2.0);
    put_commd(&ptr, CSVAR); put_strng(&ptr, "x1");

    // арифметические операции корень из (3*3 + 4*4)
    put_commd(&ptr, CPUSH);	put_value(&ptr, 3);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 3);
    put_commd(&ptr, CMULT);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 4);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 4);
    put_commd(&ptr, CMULT);
    put_commd(&ptr, CADD);
    put_commd(&ptr, CSQRT);

    /* выводим результат, останавливаем машину */
    put_commd(&ptr, COUT);
    put_commd(&ptr, CHALT);

    start();

    return no_of_errors;
}
