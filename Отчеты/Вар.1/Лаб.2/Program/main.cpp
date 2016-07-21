#include <iostream>
#include <string>
#include <hash_map>
#include <cctype>
//#include <sstream>
#include <conio.h>
#include <math.h>
#include "cop.h"

using namespace std;

int no_of_errors;
int no_of_strings=0;
int error(const string &s);

const int SIZEPROG = 64000;
const short SIZEVART = 20;
const short SIZESTACK = 256;
const short SIZENAME = 10;

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

//класс виртуальной машины
class VM
{
    //хэш таблица символов(переменных)
    static hash_map<string,double> table;
public:
    VM(int sizeStack=SIZESTACK):Code(nullptr),Data(sizeStack),NameVar(new char[SIZENAME]),it(0) {}
    ~VM(){if(Code) delete[] Code; delete[] NameVar;}
    TStack<double> Data;
    char *Code;
    char* NameVar; //временная переменная для хранения текущего имени
    int it; //текущая позиция в коде

    //получить из потока имя и скопировать его во внутр. переменную
    void getname()
    {
        int len;
        strncpy(NameVar, &Code[it], SIZENAME);
        len = strlen(NameVar);
        NameVar[len] = 0;
        char nlptr =  NameVar[0];
        it += len+1;
    }
    //скопировать массив исходного кода
    void setCode(char *ptr,int size = SIZEPROG){
        if(Code) delete[] Code;
        Code = new char[size];
        memset(Code,0,size);
        for(int i=0;i<size;i++)
        {
            Code[i] = *(ptr+i);
        }
    }
    //добавить переменную в таблицу символов
    static void add_var(const string& name)
    {
        if (table.find(name)!= table.end()) error("duplicate variable");
        else{
            if (table.size() > SIZEVART) error("too many variables");
            else table[name] = 0.0;
        }

    }
    //установить значение переменной из таблицы символов
    static void set_var(const string& name,double value)
    {
        try{
            table.at(name) = value;
        }
        catch (out_of_range) {
            error("Cann't add variable");
        }
    }
    /***************математические методы******************/
    //вычисление корня
    void sqrt()
    {
        if(!Data.isempty())
        {
            double x = Data.pop();
            Data.push(std::sqrt(x));
            it++;
        }
        else error("cann't execute sqrt, stack is empty");
    }
    //сумма
    void add()
    {
        if(Data.size()>1)
        {
            double x = Data.pop();
            double y = Data.pop();
            Data.push(x+y);
            it++;
        }
        else error("cann't execute add, stack is empty");
    }
    //разность
    void sub()
    {
        if(Data.size()>1)
        {
            double y = Data.pop();
            double x = Data.pop();
            Data.push(x-y);
            it++;
        }
        else error("cann't execute add, stack is empty");
    }
    //деление
    void div()
    {
        if(Data.size()>1)
        {
            double y = Data.pop();
            double x = Data.pop();
            if(y) Data.push(x/y);
            else error("devide by zero");
            it++;
        }
        else error("cann't execute add, stack is empty");
    }
    //умножение
    void mul()
    {
        if(Data.size()>1)
        {
            double x = Data.pop();
            double y = Data.pop();
            Data.push(x*y);
            it++;
        }
        else error("cann't execute mul, stack is empty");
    }
    /***************конец математических методов******************/
    //вывод на экран
    void print()
    {
        if(!Data.isempty()) std::cout << "Result is: " << Data.pop() << std::endl;
        else error("cann't res, stack is empty");
        it++;
        //так можно вывести значение переменной x1
        //cout << VM::table.at("x1") << endl;
    }

    //запуск виртуальной машины
    void start()
    {
        if(Code == nullptr) {
            error("Not found code");
            return;
        }
        while(1)
        {
            char cur = Code[it];
            int curit = it;
            switch (Code[it]) {
            //в зависимости от кода функции выбираем действие
            case CNVAR:
                it++;
                getname();
                add_var(string(NameVar));
                break;
            case CSVAR:
                it++;
                getname();
                set_var(NameVar, Data.pop());
                break;
            case CPUSH:
                it++;
                Data.push(*((double*)(Code+it)));
                it+=sizeof(double);
                break;
            case CMULT:
                mul();
                break;
            case CADD:
                add();
                break;
            case CSUB:
                sub();
                break;
            case CDIV:
                div();
                break;
            case CSQRT:
                sqrt();
                break;
            case COUT:
                print();
                break;
            case CHALT:
                return;
            default:
                error("Cann't understand command.");
                break;
            }
        }
    }
};

hash_map<string,double> VM::table;

/***********функции взяты из учебного примера vm.c ************/

void put_commd( char** prg, char value)
{
    **prg = value;
    (*prg)++;
}

void put_value(char** prg, double value)
{
    char* ptr = (char*)&value;
    for (int i=0; i<sizeof(double); i++)
    {
        char cur = ptr[i];
        put_commd(prg, ptr[i]);
    }
}
void put_strng(char** prg,const char* string)
{
    int len = strlen((string));
    if (len >= SIZENAME-1) len = SIZENAME;
    strncpy((*prg),(char*)(string), SIZENAME);
    *prg += len+1;
}
/***********конец функций, взятых из учебного примера vm.c ************/

int error(const string &s)
{
    no_of_errors++;
    cerr<<no_of_strings<<" :-: FAIL:"<<s<<'\n';
    return 1;
}

int main(int argc, char* argv[])
{
    char Program[SIZEPROG]; /* код программы*/
    char *ptr = Program;

    //добавить переменную
    VM::add_var("x");
    VM::add_var("y");
    //выдаст ошибку, так как уже есть такая переменная
    VM::add_var("x");
    //установить значение
    VM::set_var("pi",3.1415); //выдаст исключение, так как pi не определена
    VM::add_var("pi");
    VM::set_var("pi",3.1415); //теперь ok

    // так создаются переменные в программе ВМ
    put_commd(&ptr, CNVAR); put_strng(&ptr, "x1");
    put_commd(&ptr, CPUSH); put_value(&ptr, 2.0);
    put_commd(&ptr, CSVAR); put_strng(&ptr, "x1");

    //арифметические операции, подсчет корня квадратного из (3*3 + 4*4)
    put_commd(&ptr, CPUSH);	put_value(&ptr, 3);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 3);
    put_commd(&ptr, CMULT);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 4);
    put_commd(&ptr, CPUSH);	put_value(&ptr, 4);
    put_commd(&ptr, CMULT);
    put_commd(&ptr, CADD);
    put_commd(&ptr, CSQRT);

    //выводим результат, останавливаем машину
    put_commd(&ptr, COUT);
    put_commd(&ptr, CHALT);
    VM MyVm;

    MyVm.setCode(Program);
    MyVm.start();

    _getch();
    return no_of_errors;
}
