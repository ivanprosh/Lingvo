#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cop.h"

/*
	������樨 �㭪権
*/
void init();
void cerror(char* mess);
int is_empty(void);
void push(double num);
double pull(void);
void run(void);
double get_value(void);
void get_name(char* name);
double get_var(char* name);
void set_var(char* name, double value);
void add_var(char* name);
int is_var(char* name);
int do_command(unsigned char cop);


/*
	��।������
*/
#define SIZESTK		20 
#define SIZEPROG	1000
#define SIZENAME	10
typedef struct {
	char name[SIZENAME];
	double value;
} Variable;
#define SIZEVART	10


/*
	�������� �����
*/
double Stack[SIZESTK];	/* �⥪			*/
int Top; 		/* ���設� �⥪�	*/
int Counter;		/* ���稪 ������	*/
FILE* vmin;		/* ��⮪ ����� ������	*/
FILE* vmout;		/* ��⮪ �뢮�� ������	*/
unsigned char Program[SIZEPROG]; /* ��� �ணࠬ��*/
Variable Vars[SIZEVART];/* ��६����		*/
int nVars;		/* ������⢮ �ᯮ��㥬�� ��६����� */

/*
	�ᯮ����⥫�� �㭪樨
*/
void cerror(char* mess)
{
	fprintf(stderr, "\nVM error: %s\nCounter=%d\nstack list:\n", mess, Counter);
	while(!is_empty()) 
		fprintf(stderr, "%g\n", pull());
	fprintf(stderr, "VM stoped.\n", mess);
	exit(-1);
}
double get_value()
{
	double* value;
	value = (double*)&Program[Counter];
	Counter += sizeof(double);
	return *value;
}
void get_name(char* name)
{
	int len;

	strncpy(name, &Program[Counter], SIZENAME);
	len = strlen(name);
	name[len] = 0;
	Counter += len;
}

void init()
{
	Counter = 0;
	nVars = 0;
}

/*
	����� � �⥪��
*/
int is_empty()
{
	return 	(Top==0);
}
void push(double num) 
{  
	if(Top<SIZESTK)  {
		Stack[Top++]=num;
	}
	else cerror("Stack full");
}
double pull(void)
{  
	if(Top!=0) { 
		Top--;  
		return Stack[Top]; 
	} 
	else cerror("Stack empty");
	return 0.0;
}


/*
	ࠡ�� � ��६���묨
*/
double get_var(char* name)
{
	int i;
	for (i=0; i<nVars; i++)
		if (strcmp(name, Vars[i].name)==0)
			return Vars[i].value;
	cerror("variable not found to get");
	return 0.0;
}
void set_var(char* name, double value)
{
	int i;
	for (i=0; i<nVars; i++)
		if (strcmp(name, Vars[i].name)==0) {
			Vars[i].value = value;
			return;
		}
	cerror("variable not found to set");
}
void add_var(char* name)
{
	if (is_var(name)) cerror("duplicate variable");
	if (nVars == SIZEVART-1) cerror("too many variables");
	strcpy(Vars[nVars].name, name);
	Vars[nVars].value = 0.0;
	nVars++;
}
int is_var(char* name)
{
	int i;
	for (i=0; i<nVars; i++)
		if (strcmp(name, Vars[i].name)==0)
			return 1;
	return 0;
}


/*
	����㠫쭠� ��設�
*/
int do_command(unsigned char cop)
{
	double op1, op2;	//���࠭��
	char name[SIZENAME];

	switch(cop) {
	case CNOP:	
			break;
	case CPUSH:
			push(get_value());				
			break;
	case CIN:
			fscanf(vmin, "%lf", &op1);
			push(op1);
			break;
	case COUT:	
			fprintf(vmout, "%g", pull());
			break;
	case CNVAR:
			get_name(name);
			add_var(name);
			break;
	case CGVAR:
			get_name(name);
			push(get_var(name));
			break;
	case CSVAR:
			get_name(name);
			set_var(name, pull());
			break;
	case CADD:
			push(pull()+pull());
			break;
	case CSUB: 
			op1 = pull();
			op2 = pull();
			push(op2-op1);
			break;
	case CMULT:
			push(pull()*pull());
			break;
	case CDIV: 
			op1 = pull();
			op2 = pull();
			if (op1==0.0) cerror("divide by zero");
			push(op2/op1);
			break;
	case CHALT:
			return 0;
	default:
			cerror("undefined command");
	}
	return 1;
}

void run()
{ 
	int CodeOperation;	//��� ����樨

	while(1) {
		CodeOperation=Program[Counter++];
		if (!do_command(CodeOperation)) break;
	}
	if (!is_empty()) cerror("program stoped but stack not empty");
}

/*
	������� �㭪�� ��� �⫠��� ����㠫쭮� ��設�
*/
#ifdef DEBUG_VM
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

int main() 
{
	unsigned char* ptr = Program;

	/* ⠪ ����� ࠧ����� ��६��� �� ����᪠ �� */
	add_var("pi");
	set_var("pi", 3.14159265);

	/* ⠪ ᮧ������ ��ଥ����� � �ணࠬ�� �� */
	put_commd(&ptr, CNVAR); put_strng(&ptr, "x1");
	put_commd(&ptr, CPUSH); put_value(&ptr, 2.0);
	put_commd(&ptr, CSVAR); put_strng(&ptr, "x1");

	/* ���᫥��� ��ࠦ���� (2.3+4.7)*2.1 */
	put_commd(&ptr, CPUSH);	put_value(&ptr, 2.3);
	put_commd(&ptr, CPUSH);	put_value(&ptr, 4.7);
	put_commd(&ptr, CADD);
	put_commd(&ptr, CPUSH);	put_value(&ptr, 2.1);
	put_commd(&ptr, CMULT);

	/* ����� १���� �� Pi */
	put_commd(&ptr, CGVAR); put_strng(&ptr, "pi");
	put_commd(&ptr, CDIV);

	/* 㬭����� �� x1 */
	put_commd(&ptr, CGVAR); put_strng(&ptr, "x1");
	put_commd(&ptr, CMULT);

	/* �뢮��� १����, ��⠭�������� ��設� */
 	put_commd(&ptr, COUT);
	put_commd(&ptr, CHALT);

	vmin = stdin;
	vmout = stdout;

	run();

	return 1;
}
#endif
