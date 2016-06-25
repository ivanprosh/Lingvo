#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <conio.h>
#include <vector>

using namespace std;

int no_of_errors;

double number;
int rows=0;
int columns=0;
bool firstrow = true;

struct Error{
    string data;
    Error(const string& s):data(s){no_of_errors++;}
};

enum Symbol
{
NUMBER,END,NEXT,SEPROW=';',FINISH=']',SEPNUM=',',START='['
};

Symbol cursymb=NEXT;

int matrixsize = 0;

void matrix(vector<int>&);
void stroki(vector<int>&);
void stroka(vector<int>&);
double chislo();
Symbol symb();

void matrix(vector<int>& result)
{
    if(cursymb!='[' && cursymb!=NEXT) { throw Error("Where is [ ?");}
    //symb();
    stroki(result);
    if(columns!=matrixsize && !firstrow) throw Error("different rows size");
    if(cursymb==FINISH) return;
    if(cursymb==NEXT) { throw Error("Where is ] ?");}
}

void stroki(vector<int>& result)
{
    rows++;
    stroka(result);
    while(1)
    {
        if(cursymb == SEPROW)
        {
            if(firstrow) {
                matrixsize = result.size();
                firstrow = false;
            }
            else if(columns!=matrixsize) throw Error("different rows size");
            columns=0;
            rows++;
            stroka(result);
        }
        else return;
    };
}

void stroka(vector<int>& result)
{
    result.push_back(chislo());
    while(1)
    {
        if(cursymb == SEPNUM)
        {
           result.push_back(chislo());
        } else return;
    };
}

double chislo()
{
    symb();
    if(cursymb == NUMBER)
    {
       columns++;
       int i = number;
       symb();
       return i;
    } else {
       throw Error("Wrong query of symbols");
    };
}

Symbol symb()
{
    char ch=0;
    do
    {
        if(!cin.get(ch))return cursymb=END;
    }
    while(ch!='\n'&&isspace(ch));
    switch(ch)
    {
        case 0      :
            return cursymb=END;
        case ',':case ']' :case '[':
            return cursymb=Symbol(ch);
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
            cin.putback(ch);
            cin>>number;
            //if(firstrow) colums++;
            return cursymb=NUMBER;
        case ';'    :
            return cursymb=SEPROW;
        case '\n'    :
            return cursymb=NEXT;
        default     :
            throw Error("Wrong symbol!");
    };
}

void print(vector<int>& result)
{
    vector<int>::iterator it = result.begin();
    cout << "matrix(<" << rows << ">,<" << columns << ">,<";
    while(it!=result.end())
        cout << *(it++) << ",";
    cout << '\b';
    cout << ">)" << endl;
}

void skip() //функция восстанавливает поток при ошибке ввода (лексической и/или синтаксической)
{
    while(cin)
    {
        char ch;
        cin.get(ch);
        if(ch==';' || ch=='\n') return;
    }
}
void init() //функция подготавливает глобальные переменные перед началом анализа
{
    no_of_errors = rows = columns = matrixsize = 0;
    firstrow =true;
}

int main()
{
    while(cin)
    {
        std::vector<int> Matrix;
        Matrix.reserve(100);
        init();
        symb();
        if(cursymb==END)break;
        if(cursymb==NEXT)continue;
        try{
        matrix(Matrix);
        }
        catch(Error err){
            cerr << "FAIL: " << err.data;
            skip();
            continue;
        }
        print(Matrix);
    };
    return 0;
}

