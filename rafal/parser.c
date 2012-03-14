
#include <stdio.h>

/*
caly kod byl testowany na tym szablonie:
--------------------------------------------------------------------------------------
<?xml version="1.0" encoding="UTF-8" ?>
<rss version="2.0">
 <channel>
  <title>Nazwa kanału</title>
  <link>http://www.4programmers.net/</link>
  <description><![CDATA[Informacje o kanale]]></description>
 
<item>
     <title>Pierwszy news</title>
     <link>http://www.4programmers.net/news.php?id=1</link>
     <description><![CDATA[Treść newsa]]></description>
</item>
 
<item>
     <title>Drugi news</title>
     <link>http://www.4programmers.net/news.php?id=2</link>
     <description><![CDATA[Treść newsa]]></description>
</item>
  
<item>
     <title>Drugi3333 news</title>
     <link>http://www.4programmers.net/news.php?id=2</link>
     <description><![CDATA[Treść newsa]]></description>
</item>
  
<item>
     <title>Drugi4444 news</title>
     <link>http://www.4programmers.net/news.php?id=2</link>
     <description><![CDATA[Treść newsa]]></description>
</item>
  
<item>
     <title>Drugi55555 news</title>
     <link>http://www.4programmers.net/news.php?id=2</link>
     <description><![CDATA[Treść newsa]]></description>
</item>
 
 </channel>
</rss>
--------------------------------------------------------------------------------------
*/

void pomin(FILE *wp) // pomija < znacznik >
{
	char c;
	while((c = getc(wp)) != EOF)
		if(c == '>')
			break;	
}

void tekst(FILE *wp, FILE *wz) // wyswietla tekst pomiedzy < znacznik > jakis tekst < znacznik >
{
	char c;
	while((c = getc(wp)) != EOF)
		if(c != '<' && c != '>')
		{
			putc(c, stdout);
			putc(c, wz);
		}
		else
			break;
}

void linia(FILE *wp, FILE *wz) //wyswietla cala linie pomijajac znaczniki
{
	pomin(wp);
	tekst(wp, wz);
	pomin(wp);
	printf("\n");
	putc('\n', wz);
}

void tresc(FILE *wp, FILE *wz) // wyswietla tresc pomiedzy < description > tresc < description >
{
	tekst(wp, wz);
	pomin(wp);
	printf("\n");
	putc('\n', wz);
}

void nazwa_kanalu(FILE *wp, FILE *wz) // wyswietla nazwe kanalu i informacje o kanale
{
	pomin(wp);
	pomin(wp);
	pomin(wp);
	linia(wp, wz);
	linia(wp, wz);
	pomin(wp);
	tekst(wp, wz);
	tresc(wp, wz);
}

void news(FILE *wp, FILE *wz) // wyswietla informacje o pojedynczym newsie
{
	pomin(wp);
	linia(wp, wz);
	linia(wp, wz);
	pomin(wp);
	tekst(wp, wz);
	tresc(wp, wz);
	pomin(wp);
	printf("++");
}


int main(int argc, char **argv)
{
	FILE *wp;

	char nazwa[100] = { "a.txt" }; // nazwa pliku rss, ktory bedzie obrabiany parserem
	char zapis[100] = { "wynik.txt" }; // nazwa pliku z tekstem obrobionym parserem
	
	FILE *zap = fopen(zapis, "w");
	if((wp = fopen(nazwa, "r")) == NULL)
		printf("Nie można odczytac pliku o nazwie: %s\n", nazwa);
	else
	{
		printf("Czytam z pliku o nazwie: %s\n", nazwa);
		printf("----------------------------------------\n");
		char c;
		nazwa_kanalu(wp, zap);
		int i = 0;
		for( ; i < 5; ++i)
			news(wp, zap);

		printf("-------------------------------\n");
		while((c = getc(wp)) != EOF)
		{
			putc(c, stdout);

		}

	}
	


	return 0;
}
