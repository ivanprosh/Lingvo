#include "match.h"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <conio.h>
#include <vector>

extern void match_init(char* line);

using namespace std;

int no_of_errors;

int Number[10];
int curnumber=0;
int rows=0;
int columns=0;
bool firstrow = true;
char indata[200];

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
int chislo();
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

int chislo()
{
    symb();
    if(cursymb == NUMBER)
    {
       columns++;
       int i = curnumber;
	   curnumber = 0;
       symb();
       return i;
    } else {
       throw Error("Wrong query of symbols");
    };
}

Symbol symb()
{
    char ch=0;
	int i = -1;

	if (match(",")) return cursymb = SEPNUM;
	if (match("]")) return cursymb = FINISH;
	if (match("[")) return cursymb = START;
	if (number(Number)) {
		while (Number[++i])
		{
			curnumber = curnumber * 10 + Number[i];
		}
		return cursymb = NUMBER;
	}
	if (match(";")) return cursymb=SEPROW;
	if (match("\n")) return cursymb=NEXT;
    throw Error("Wrong symbol!");
    
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
	match_done();
    no_of_errors = rows = columns = matrixsize = 0;
    firstrow =true;
	cin >> indata;
	match_init(indata);
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

