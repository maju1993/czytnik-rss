//#include <iostream>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>


int skocz_do(FILE * wp, char *tab)
{
	char *t = (char *)malloc(sizeof(char) * strlen(tab) + 1);
	char c;
	int i;
	int k = 1;
	while((c = getc(wp)) != EOF)
	{
		k = 1;
		if(c == '<')
		{
			for(i = 0; i < strlen(tab); ++i)
				if(c != tab[i])
				{
					//printf("c: %c rozne od tab[i]: %c\n", c, tab[i]);
					k = 0;
					break;
				}
				else
					if(((c = getc(wp)) != EOF) && (c != '>'))
						continue;
					else
						break;
			if(k)
			{
				//printf("konczymy--------------------------------------------------\n");
				break;
			}
			else
			{
				//printf("lecimy do >\n");
				//while(c != '>')
					//c = getc(wp);
			}
		}		
	}
	if (c==EOF)
		return -1;
}

void tekst_pomiedzy(FILE *wp, FILE *zap, char *t)
{
	char c;
	int i;
	while((c = getc(wp)) != EOF)
		if(c == '<')
		{
			c = getc(wp);
			if(c == '/')
			{
				c = getc(wp);
				if(c == t[1])
					break;
				else
				{
					putc('<',zap);
					putc('/',zap);
					putc(c, zap);
				}
			}
			else
			{
				putc(c, zap);
			}
		}
		else
		{
			putc(c, zap);
			
		}
}

int nast_new(FILE * wp, FILE * zap)
{
	if (skocz_do(wp, "<item>")==-1)
		return -1;
	fputs("<item>",zap);
	skocz_do(wp, "<title>");
	fputs("<title>",zap);
	tekst_pomiedzy(wp, zap, "<title>");
	fputs("</title>",zap);
	skocz_do(wp, "<link>");
	fputs("<link>",zap);
	tekst_pomiedzy(wp, zap, "<link>");
	fputs("</link>",zap);
	skocz_do(wp, "<description>");
	fputs("<description>",zap);
	tekst_pomiedzy(wp, zap, "<description>");
	fputs("</description>",zap);
	skocz_do(wp, "<pubDate>");
	fputs("<pubDate>",zap);
	tekst_pomiedzy(wp, zap, "<pubDate>");
	fputs("</pubDate>",zap);
}

int parse(char* zapis)
{
	FILE *wp;
	char nazwa[100] = { "./data/data.rss" }; // nazwa pliku rss, ktory bedzie obrabiany parserem
	char* sciezka=malloc(255);
	strcpy(sciezka,"./data/");
	strcat(sciezka,zapis);
	FILE *zap = fopen(sciezka, "w");
	if((wp = fopen(nazwa, "r")) == NULL)
		printf("ERROR:  Nie można odczytac pliku o nazwie: %s\n", nazwa);
	else
	{
		
		char c;
		char t[] = { "<item>" };
		
		while((nast_new(wp, zap))!=-1);
		fclose(zap);
		fclose(wp);
	}
	wp=fopen(sciezka,"r");
	char c;
	if (c=getc(wp)==EOF)
		return -1;
}




