#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <conio.h>
#include <complex>

using namespace std;

istream* input;
int no_of_errors;

double      value;

int no_of_strings=0;

enum Token_value
{
NUMBER,END,IM,PLUS='+',MINUS='-',MUL='*',PRINT=';'
};

Token_value curr_tok=PRINT;

complex<double> term(bool get);
complex<double> prim(bool get);
complex<double> expr(bool get);
int error(const string &s);
Token_value get_token();

complex<double> expr(bool get)
{
    complex<double> left=term(get);
    for(;;)
    {
        switch(curr_tok)
        {
            case PLUS   :   left+=term(true);break;
            case MINUS  :   left-=term(true);break;
            default     :   return left;
        };
    };
};

complex<double> term(bool get)
{
    complex<double> left=prim(get);
    for(;;)
    {
        switch(curr_tok)
        {
            case MUL    :   left*=prim(true);break;

            default     :   return left;
        };
    };
};

complex<double> prim(bool get)
{
    if(get)get_token();
    switch(curr_tok)
    {
        case IM :
        {
            complex<double> v(0.0,value);
            get_token();
            return v;
        };
        case NUMBER :
        {
            complex<double> v(value,0.0);
            get_token();
            return v;
        };        
        case MINUS  :   return -prim(true);

        default     :   return error("PRIMARY EXPRESSION EXPECTED");
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

        case '*': case '+':
        case '-':
            return curr_tok=Token_value(ch);
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
        case '.'    :
            input->putback(ch);
            *input>>value;
            input->get(ch);
            if(ch=='i') return curr_tok=IM;
            input->putback(ch);
            return curr_tok=NUMBER;
        case ';'    :
        case '\n'   :
            no_of_strings++;
            return curr_tok=PRINT;

        default     :
            error("INCORECT WORD");
            return curr_tok=PRINT;
    };
};

int error(const string &s)
{
    no_of_errors++;
    cerr<<no_of_strings<<" :-: FAIL:"<<s<<'\n';
    return 1;
};

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
    complex<double> answer(0.0,0.0);
    while(input)
    {
        get_token();
        if(curr_tok==END)break;
        if(curr_tok==PRINT)continue;
        answer = expr(false);
        if(!no_of_errors)
            if(answer.imag()) cout<<"OK complex(" << answer.imag() <<", " <<answer.real()<<")" << endl;
            else cout<<"OK, "<< answer.real() << endl;
        no_of_errors = 0;
    };
    if(input!=&cin)delete input;

    return no_of_errors;
};

