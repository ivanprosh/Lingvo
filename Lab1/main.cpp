#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <conio.h>

using namespace std;

istream* input;
int no_of_errors;

double value;
int curcolumn=0;
int rows=0;
int colums=0;
bool firstrow = true;

enum Token_value
{
NUMBER,END,PRINT,SEPARATE_ROWS=';',CLOSE=']',SEPARATE_TOKENS=',',OPEN='['
};

Token_value curr_tok=PRINT;

int currindex = 0;
int arr[100];
void row();
double prim();
void matrix(bool get);
int error(const string &s);
Token_value get_token();

void matrix()
{
    row();
    for(;;)
    {
        switch(curr_tok)
        {
            case SEPARATE_ROWS  :
                if(!firstrow && curcolumn!=colums)
                    {error("ROWS SIZE ARE DIFFERENT"); return;}
                row();
                break;
            case CLOSE  :   if(!firstrow && curcolumn!=colums) error("ROWS SIZE ARE DIFFERENT");
                            return;
            default     :   error("] OR ; EXPECTED");return;
        };
    };
};

void row()
{
    curcolumn = 0;
    arr[currindex++]=prim(); curcolumn++;
    for(;;)
    {
        switch(curr_tok)
        {
            case SEPARATE_TOKENS    :   arr[currindex++]=prim(); curcolumn++;break;

            default     :   return;
        };
    };
};

double prim()
{
    get_token();
    switch(curr_tok)
    {
        case NUMBER :
        {
            double v = value;
            get_token();
            return v;
        };        

        default     :   return error("FAIL EXPRESSION EXPECTED");
    };
};

Token_value get_token()
{
    char ch=0;
    do{if(!input->get(ch))return curr_tok=END;}
    while(ch!='\n'&&isspace(ch));
    switch(ch)
    {
        case 0      :   return curr_tok=END;
        case ',':case ']' :case '[':
            return curr_tok=Token_value(ch);
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
        case '.'    :
            input->putback(ch);
            *input>>value;
            if(firstrow) colums++;
            return curr_tok=NUMBER;
        case ';'    :
            rows++;
            firstrow = false;
            return curr_tok=SEPARATE_ROWS;
        case '\n'    :
            return curr_tok=PRINT;
        default     :
            error("INCORECT WORD");
            return curr_tok=PRINT;
    };
};

int error(const string &s)
{
    no_of_errors++;
    cerr<<rows<<" :-: FAIL:"<<s<<'\n';
    (*input).ignore(cin.rdbuf()->in_avail());
    return 1;
};

void print(int* arr)
{
    cout << "matrix(<" << rows+1 << ">,<" << colums << ">,<";
    while(*arr!=0)
        cout << *(arr++) << ",";
    cout << '\b';
    cout << ">)" << endl;
}

int main(int argc, char* argv[])
{
    switch(argc)
    {
        case 1:
            input=&cin;
            break;
        case 2:
            input=new istringstream(argv[1]);
            break;
        default:
            error("To much parameters!!!");
            return 1;
    };

    while(input)
    {
        currindex = 0;
        memset(arr,0,100);
        get_token();        
        if(curr_tok==END)break;
        if(curr_tok==PRINT)continue;
        if(curr_tok!='[' && curr_tok!='\n') { error("[ EXPECTED"); continue;}
        matrix();
        if(!no_of_errors) {
            print(arr);
        }
        no_of_errors = rows = colums = curcolumn = 0;
        firstrow =true;
    };
    if(input!=&cin)delete input;

    return no_of_errors;
};

