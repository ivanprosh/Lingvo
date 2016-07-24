#include <iostream>
#include <cctype>
#include <conio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include "cop.h"
#include "tstack.h"
#include "str_switch.h"

using namespace std;

std::filebuf FILE_input,FILE_output;
const string output_name = "output.txt";
const string input_name = "input.txt";

int no_of_errors;
int no_of_strings=0;
int error(const string &s);

const int SIZEPROG = 64000;
const short SIZEVART = 20;
const short SIZESTACK = 256;
const short SIZENAME = 10;

//����� ����������� ������
class VM
{
    pair<bool,double> variables[SIZEVART];
    ostream* output;
public:
    VM(ostream* out = &std::cout,int sizeStack=SIZESTACK):Code(nullptr),Data(sizeStack),it(0),output(out) {
        for(int i=0;i<SIZEVART;i++)
            //������������� ������� �������
            variables[i].first = false;
    }
    ~VM(){if(Code) delete[] Code;}
    TStack<double> Data;
    char *Code;
    int it; //������� ������� � ����

    //����������� ������  ����
    void setCode(char *ptr,int size = SIZEPROG){
        if(Code) delete[] Code;
        Code = new char[size];
        memset(Code,0,size);
        for(int i=0;i<size;i++)
        {
            Code[i] = *(ptr+i);
        }
    }
    //�������� ����������
    void load(const int& addr)
    {
        if(addr>=SIZEVART) {error("addr out of range"); return;}
        //������ ����� ������
        variables[addr].first = true;
        variables[addr].second = 0.0;
        it+=sizeof(int);
    }
    //���������� �������� ���������� �� ������� ��������
    void push(const int& addr)
    {
        if(addr>=SIZEVART) {error("addr out of range"); return;}
        if(variables[addr].first)
            variables[addr].second = Data.pop();
        else
            error("cann't find variable in table");
        it+=sizeof(int);
    }
    //�������� � ���� �������� �������� �� �������
    void pull(const int& addr)
    {
        if(addr>=SIZEVART) {error("addr out of range"); return;}
        if(variables[addr].first)
            Data.push(variables[addr].second);
        else
            error("cann't find variable in table");
        it+=sizeof(int);
    }

    //���������� �����
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
    //�����
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
    //��������
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
    //�������
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
    //���������
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

    //����� �� �����
    void print()
    {
        if(!Data.isempty()) *output << "Result is: " << Data.pop() << std::endl;
        else error("cann't res, stack is empty");
        it++;
    }

    //������ ����������� ������
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
            //������ ��� ������ �� ������ �������
            case CNVAR:
                load(Code[++it]);
                break;
            case CSVAR:
                push(Code[++it]);
                break;
            case CPULL:
                pull(Code[++it]);
                break;
            //������ ������ � �������
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

void put_commd( char** prg, char value)
{
    **prg = value;
    (*prg)++;
}

template<class Type>
void put_value(char** prg, Type value)
{
    char* ptr = (char*)&value;
    for (int i=0; i<sizeof(Type); i++)
    {
        char cur = ptr[i];
        put_commd(prg, ptr[i]);
    }
}

int error(const string &s)
{
    no_of_errors++;
    cerr<<no_of_strings<<" :-: FAIL:"<<s<<'\n';
    return 1;
}

int main(int argc, char* argv[])
{
    // ��� ���������
    char Source[SIZEPROG];
    char *pointer = Source;

    string word;
    int addr;
    double value;
    FILE_input.open("input.txt",std::ios::in);
    std::istream is(&FILE_input);

    //�������������� ��������, (x1=x1+3*5;)

    while(is)
    {
        is >> word;
        SWITCH (word) {
        CASE("CSVAR"):
            put_commd(&pointer, CSVAR);
            is>>addr;
            put_value(&pointer, addr);
            break;
        CASE("CNVAR"):
            put_commd(&pointer, CNVAR);
            is>>addr;
            put_value(&pointer, addr);
            break;
        CASE("CPUSH"):
            put_commd(&pointer, CPUSH);
            is>>value;
            put_value(&pointer, value);
            break;
        CASE("CPULL"):
            put_commd(&pointer, CPULL);
            is>>addr;
            put_value(&pointer, addr);
            break;
        CASE("CMULT"):
            put_commd(&pointer, CMULT);
            break;
        CASE("CADD"):
            put_commd(&pointer, CADD);
            break;
        CASE("CSUB"):
            put_commd(&pointer, CSUB);
            break;
        CASE("CDIV"):
            put_commd(&pointer, CDIV);
            break;
        CASE("COUT"):
            put_commd(&pointer, COUT);
            break;
        CASE("CHALT"):
            put_commd(&pointer, CHALT);
            break;
        default:
            break;
        }

    }
    FILE_input.close();

    //����� ������ � ���� output.txt
    FILE_output.open ("output.txt",std::ios::out);
    std::ostream os(&FILE_output);

    //� ������������� �������� ������������ ������������ ����� cout
    VM MyVm(&os);

    MyVm.setCode(Source);
    MyVm.start();

    FILE_output.close();
    return no_of_errors;
}
