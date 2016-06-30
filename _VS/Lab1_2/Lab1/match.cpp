#include "match.h"
#include <stdlib.h>

#define tabch '\t'

char *ch="";
char *nexch="";

int streq(char *str1,char *str2);
int astreq(char *str1,char *str2,int len);
int alpha(char c);
int numeric(char c);
int an(char c);
int inchar(void);
int gch(void);



/************************************/
/*  Внешние процедуры				*/
/************************************/

/****************************************/
/*  процедура инициализации             */
/*  присваивает переменным ch, nexch    */
/****************************************/
void match_init(char* line)
{
	ch = line;
	nexch = ch+1;
}

/****************************************/
/*  процедура деинициализации           */
/****************************************/
void match_done()
{
	ch = nexch = "";
}

/****************************************/
/*  Сравнение константы с содержимым    */
/*  буфера строк. Продвижение указателя */
/*  буфера, если было совпадение        */
/****************************************/
int match(char *lit)
{
	int k;

	skipblanks();
	if (k=streq(ch,lit))
	     {  ch=ch+k; nexch=ch+1; return 1; }
	return 0;
}
//вернуть символ в поток
void rematch()
{
	nexch--; ch--;
}
/******************************/
/*      'match'  , но         */
/*    по 'len'  символам      */
/******************************/
int amatch(char *lit,int len)
{
	int k;

	skipblanks();
	if (k=astreq(ch,lit,len))
	{
		ch=ch+k;    
		nexch=ch+1;
		while(an(*ch)) inchar();
		return 1;
	}
	return 0;
}

/****************/
/* чтение метки */
/****************/
int symname(char *sname)
{
	int k;

	skipblanks();
	if (alpha(*ch)==0) 
		return 0;
        k=1;
	while (an(*ch)) 
		sname[k++]=gch();
	sname[k]=0;
	sname[0]=k-1;
	return 1;
}

/**********************/
/* проверка на число  */
/**********************/
int number(int val[])
{
	int k,minus;
	char c;

	k=minus=1;
	while (k)
	{
		k=0;
		if (match("+")) k=1;
		if (match("-")) { minus=(-minus); k=1;}
	}
	if (numeric(*ch)==0) return 0;
	while (numeric(*ch))
	{
		c=inchar();
		k=k*10+(c-'0');
	}
	if (minus<0) k=(-k);
        val[0]=k;
	return 1;
}

/***********************************/
/*  пропуск пустых символов        */
/***********************************/
void skipblanks()
{
	while(1)
	{
		if (*ch==0) break;

		if (*ch==' ') gch();
		else if (*ch==tabch) gch(); 
		else return;
	}
}

/***********************************/
/*  пропуск лексемы и следующ      */
/*       пустых символов           */
/***********************************/
void skipchars()
{
	if (an(inchar())) 
		while(an(*ch))
			gch();
	else 
		while(an(*ch)==0)
		{  
			if (*ch==0)  break; 
			gch();
		}
	skipblanks();
}


/************************************************/
/*  Внутренние процедуры                        */
/************************************************/

/*********************/
/*  сравнение строк  */
/*********************/

int streq(char *str1,char *str2)
{
	int k;

	k=0;
	while (*str2)
	    {
	    if ((*str1)!=(*str2)) return 0;
	    k++; str1++; str2++;
	    }
	return k;
}

/************************/
/*    сравнение строк   */
/*  по 'len'  символам  */
/************************/

int astreq(char *str1,char *str2,int len)
{
	int k;

	k=0;
	while (k<len)
	{
		if ((*str1)!=(*str2)) break;
		if (*str1==0) break;
		if (*str2==0) break;
		k++; 
		str1++; 
		str2++;
	}
        if (an(*str1)) return 0;
	if (an(*str2)) return 0;
	return k;
}

/*******************************/
/*  проверка вводимого символа */
/*******************************/
int alpha(char c)
{
	if ((c>='a') & (c<='z')) return 1;
	if ((c>='A') & (c<='Z')) return 1;
	return(c=='_');
}

int numeric(char c)
{
	return((c>='0')&(c<='9'));
}

int an(char c)
{
	if ((c>='0') & (c<='9')) return 1;
	if ((c>='a') & (c<='z')) return 1;
	if ((c>='A') & (c<='Z')) return 1;
	return (c=='_');
}

/********************************/
/*  Получение одонго символа	*/
/********************************/

int inchar()
{
	if (*ch==0) return 0;
	return(gch());
}

/****************************************/
/*  получение очередн. вх. символа	    */
/****************************************/

int gch()		
{
	if(*ch==0)
		return 0;
	else {  
		nexch++; 
		return (*ch++);  
	}
}


