#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "parser.h"


void menu();
void downloadfile(char link[]);
int isAddCommand( char command[]);
int isListCommand( char command[]);
int isUpCommand( char command[]);
int isShowCommand( char command[]);
int isDelCommand( char command[]);
int isUpAllCommand( char command[]);
int znajdz_kanal(char *title);
void listchannels(int );
int deletechannel(char *);
void update_channel(char *title);
void update_all();

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int isHelpCommand( char command[])
{
	char* temp;
	temp=strndup(command, 4);
	return strcmp(temp,"help");
}

int isAddCommand( char command[])
{
	char* temp;
	temp=strndup(command, 11);
	return strcmp(temp,"channel add");
}

int isShowCommand( char command[])
{
	char* temp;
	temp=strndup(command, 4);
	return strcmp(temp,"show");
}

int isListCommand( char command[])
{
	char* temp;
	temp=strndup(command, 8);
	return strcmp(temp,"list all");
}

int isUpCommand( char command[])
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel update");
}

int isUpAllCommand( char command[])
{
	char* temp;
	temp=strndup(command, 10);
	return strcmp(temp,"update all");
}

int isRmCommand( char command[])
{
	char* temp;
	temp=strndup(command, 10);
	return strcmp(temp,"remove all");
}


int isDelCommand( char command[])
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel delete");
}

void downloadfile(char link[])
{
	CURL *curl_handle;
  static const char *pagefilename = "./data/data.rss";
  FILE *pagefile;
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl_handle = curl_easy_init();
 
  curl_easy_setopt(curl_handle, CURLOPT_URL, link);
 
  //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L); //DEBUGOWANIE CURL
 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  pagefile = fopen(pagefilename, "wb");
  if (pagefile) {
 
    curl_easy_setopt(curl_handle, CURLOPT_FILE, pagefile);
 
    curl_easy_perform(curl_handle);
 
    fclose(pagefile);
  }
 
  curl_easy_cleanup(curl_handle);
	
  curl_global_cleanup();
}

void save_channel(FILE *zap, char * title, char * link)
{
	int i = 0;
	while(title[i] != '\0')
	{
		putc(title[i], zap);
		++i;
	}
	putc('\n', zap);
	i = 0;
	while(link[i] != '\0')
	{
		putc(link[i], zap);
		++i;
	}
	putc('\n', zap);
}

int RemoveAll()
{
	FILE *wp = fopen("./data/list.data", "r");
	if(wp!=NULL)
	{
		char* buf=malloc(255);
		char* sciezka=malloc(255);
		int i=0;
		int found=0;
		while ( fgets (buf , 255 , wp) != NULL )
		{
			buf[strlen(buf)-1]='\0';
			strcpy(sciezka,"./data/");
			strcat(sciezka,buf);
			remove(sciezka);
			fgets (buf , 255 , wp);
		}
		fclose(wp);
	}
	wp=fopen("./data/list.data", "r");
	if(wp!=NULL)
		remove("./data/list.data");
	wp=fopen("./data/temp.data", "r");
	if(wp!=NULL)
		remove("./data/temp.data");
	wp=fopen("./data/data.rss", "r");
	if(wp!=NULL)
		remove("./data/data.rss");
	printf("Baza danych została usunięta.\n");
}

int addchannel(char*title, char* link)
{
	downloadfile(link);	
	if (parse(title)==-1)
	{
		printf("Błąd: Podany link do kanłu rss jest nieprawidłowy. Wpisz help aby uzyskać pomoc!\n");
		return -2;		
	}
	FILE* wp = fopen("./data/list.data", "r");
	if(wp == NULL)
		wp = fopen("./data/list.data", "w");
	FILE* zap;
	zap=fopen("./data/temp.data","a+");
	if(!wyszukaj(wp, zap, title))
	{
		FILE* zap2;
		zap2=fopen("./data/list.data","a+");
		fseek ( zap2 , 0 , SEEK_END );
		save_channel(zap2, title, link);
		printf("Dodano kanał %s\n",title);
		fclose(zap2);	
	}
	else
	{
	  printf("Podany kanal: >%s< juz istnieje, nie można dodać. Wpisz help aby uzyskać pomoc!\n", title);
	}	
	fclose(wp);
	fclose(zap);	
}


// czy_aktualizowac - 1 jesli ma aktualizowac wszystkie kanaly, 0 jesli nie. Dla 0 bedzie wyswietlac na standardowym wyjsciu, dla 1 nie
void listchannels(int czy_aktualizowac)
{
	FILE* wczt;
	wczt=fopen("./data/list.data","r+");
	char buf[100];
	char *zawa;
	int i = 0;
	if(wczt != NULL)
	{
		char c;
		if (c=getc(wczt)==EOF)
		{
			printf("Błąd: Brak dodanych kanałów. Wpisz help aby uzyskać pomoc!\n");
			return;
		}
		fseek ( wczt , 0 , SEEK_SET );
		while((c = getc(wczt)) != EOF)
		{
			if(!czy_aktualizowac)
				putc(c, stdout);
			
			if(c == '\n')
			{
				zawa = (char*)malloc(i * sizeof(char));
				int k = 0;
				for( ; k < i; ++k)
					zawa[k] = buf[k];
				zawa[k]='\0';
				if(czy_aktualizowac)
					update_channel(zawa);
				while((c = getc(wczt)) != EOF)
					if(c == '\n')
					{
						i = 0;
						break;
					}
			}
			else
				buf[i++] = c;
		}
		fclose(wczt);
	}
	else
		printf("Błąd: Brak kanałów do wyświetlenia, żaden nie został jeszcze dodany. Wpisz help aby uzyskać pomoc!\n");
}

void writeHelp()
{
	FILE* wp=fopen("help","r");
	char c;	
	while((c=getc(wp))!=EOF)
		putc(c,stdout);
}

void menu() 
{
  char command[255];
	do
	{
		fgets(command,255,stdin);
		command[(int)strlen(command)-1]='\0';
		if (isAddCommand(command)==0)
		{
			char* link;
			char* title;
			int i=0;
			int k=0;
			int b=0;
			int a=0;
			for (i=0;i<strlen(command);i++)
			{
				if (b==1 && a==1 && command[i]==32 )
				{
					k=i;
					break;
				}
				if (b==1 && command[i]==32 )
					a=1;
				if (command[i]==32 )
					b=1;
			}
			if (k!=0)
			{
				link=strndup(command+12, k-12);
				title=strndup(command+k+1,i-k-1);
				addchannel(title,link);
			}
			else
			{
				printf("Błąd: Nie podano nazwy kanału. Wpisz help aby uzyskać pomoc!\n");
			}
		}
		if (isListCommand(command)==0)
		{
			listchannels(0);			
		}
		if (isHelpCommand(command)==0)
		{
			writeHelp();			
		}
		if (isUpAllCommand(command)==0)
		{
			update_all();			
		}
		if (isRmCommand(command)==0)
		{
			RemoveAll();			
		}
		if (isUpCommand(command)==0)
		{
			char* title=malloc(255);
			title=strndup(command+15, strlen(command)-15);
			update_channel(title);
		}
		if (isDelCommand(command)==0)
		{
			char* title=malloc(255);
			title=strndup(command+15, strlen(command)-15);
			deletechannel(title);
		}
		if (isShowCommand(command)==0)
		{		
			int count=0;
			int i=0;
			while(command[strlen(command)-1-i]!=32)
			{
				i++;
			}
			char* s;
			s=strndup(command+strlen(command)-i,i);
			count=atoi(s);			
			char *name=strndup(command+5, strlen(command)-6-i);
			int a=Show(name,count);

			if (a==-1)
				printf("Błąd:Podany kanał rss nie istnieje w bazie. Wpisz help aby uzyskać pomoc!\n");
			if (a!=-2 && a!=-1 && a!=-5)
				printf("Błąd:Podany kanał posiada tylko %i wiadomości. Wpisz help aby uzyskać pomoc!\n",a); 
		}
		if(isAddCommand(command)!=0 && isShowCommand(command)!=0 && isDelCommand(command)!=0 && isUpCommand(command)!=0 && isListCommand(command)!=0 && isUpAllCommand(command)!=0 && isAddCommand(command)!=0 && isRmCommand(command)!=0 && isHelpCommand(command)!=0 && (strcmp(command,"exit"))!=0)
			printf("Błąd: Nieznane polecenie. Wpisz help aby uzyskać pomoc!\n");

	}
	while( strcmp(command,"exit"));
}

// jeśli znajdzie tekst w pliku, to zwraca 1, jeśli nie to 0

int wyszukaj(FILE *wp, FILE *zap, char *szuk)
{
	char c;
	int i = 0;
	char buf[10];
	int zwrot = 0;
	while((c = getc(wp)) != EOF)
	{
		i = 0;
		if(c == szuk[0])
		{
			buf[0] = c;
			while((c = getc(wp)) != EOF)
			{
				buf[i] = c;
				++i;
				if(i == strlen(szuk))
					break;
			}		
			if(c == '\n') // jeśli znajdzie tekst w pliku, to zwraca 1, jeśli nie to 0
			{
				zwrot = 1;
				return 1;
			}
			else
			{
				int z = 0;
				for( ; z < strlen(szuk); ++z)
					putc(szuk[z], zap);
				putc(c, zap);
				while((c = getc(wp)) != EOF)
				{
					putc(c, zap);
					if(c == '\n')
						break;
				}	
				zwrot = 0;	
			}
		}
		else
		{
			putc(c, zap);
			while((c = getc(wp)) != EOF)
			{
				putc(c, zap);
				if(c == '\n')
					break;
			}
			zwrot = 0;
		}

	}
	return 0;
}

int isadded(char* title)
{
	FILE *wp = fopen("./data/list.data", "r");
	if(wp==NULL)
		return -3;
	char* buf=malloc(255);
	int i=0;
	int found=1;
	while ( fgets (buf , 255 , wp) != NULL )
	{
		buf[strlen(buf)-1]='\0';
		if (strcmp(title,buf)!=0)
		{
			fgets (buf , 255 , wp);
		}
		else
		{
			fgets (buf , 255 , wp);
			found=0;
		}
	}
	fclose(wp);
	return found;
}

char* getlink(char* title)
{
	FILE *wp = fopen("./data/list.data", "r");
	char* buf=malloc(255);
	int i=0;
	int found=1;
	while ( fgets (buf , 255 , wp) != NULL )
	{
		buf[strlen(buf)-1]='\0';
		if (strcmp(title,buf)==0)
		{
			fgets (buf , 255 , wp);
			buf[strlen(buf)-1]='\0';
			fclose(wp);
			return buf;
		}
	}
	fclose(wp);
	return NULL;
}


int deletechannel( char* title)
{
	FILE *wp = fopen("./data/list.data", "r");
	//remove("./data/temp.data");
	FILE *zap = fopen("./data/temp.data", "w");
	if(zap==NULL || wp==NULL)
	{
		printf("Błąd: Podana nazwa kanału nie istnieje. Wpisz help aby uzyskać pomoc!\n");
		return -1;
	}
	char* buf=malloc(255);
	int i=0;
	int found=0;
	while ( fgets (buf , 255 , wp) != NULL )
	{
		buf[strlen(buf)-1]='\0';
		if (strcmp(title,buf)!=0)
		{
			fputs(buf,zap);
			fputs("\n",zap);
			fgets (buf , 255 , wp);
			fputs(buf,zap);
		}
		else
		{
			fgets (buf , 255 , wp);
			found=1;
		}
	}
	fclose(wp);
	fclose(zap);
	if (found==0)
	{
		printf("Błąd: Podana nazwa kanału nie istnieje. Wpisz help aby uzyskać pomoc!\n");
		return -1;
	}
	if (found !=0)
	{
		char* sciezka=malloc(255);
		strcpy(sciezka,"./data/");
		strcat(sciezka,title);
		remove(sciezka);
		remove("./data/list.data");
		rename("./data/temp.data","./data/list.data");
		printf("Kanał został unięty.\n");
		return 0;
	}
}




void update_channel(char *title)
{
	if(isadded(title)!=0) // kanalu nie znaleziono
	{
		printf("Kanal o nazwie \"%s\" nie istnieje. Wpisz help aby uzyskać pomoc!\n", title);
		return;
	}
	char* link=malloc(255);
	strcpy(link,getlink(title));
	downloadfile(link);	
	parse(title);
	printf("Kanał \"%s\" został pomyślnie zaktualizowany.\n",title);
}


// aktualizuje wszystkie kanaly. trzeba tylko wywolac update channel dla kazdego kanalu.
void update_all()
{
	listchannels(1);
}

int Show(char* title, int count)
{
	if(isadded(title)!=0) // kanalu nie znaleziono
	{
		printf("Błąd: Kanal o nazwie \"%s\" nie istnieje. Wpisz help aby uzyskać pomoc!\n", title);
		return -5;
	}
	char* sciezka=malloc(255);
	strcpy(sciezka,"./data/");
	strcat(sciezka,title);
	FILE *wp = fopen(sciezka, "r");
	if (wp==NULL)
	{
		printf("Błąd: Brak kanałów do wyświetlenia, żaden nie został jeszcze dodany. Wpisz help aby uzyskać pomoc!\n");
		return -4;
	}

	int i=0;
	for (i=0 ;i<count;i++)
	{
		if (skocz_do(wp, "<item>")==-1)
		{
			//*count=i+1;
			return i;
		}
		skocz_do(wp, "<title>");
		fputs("\t",stdout);
		tekst_pomiedzy(wp, stdout, "<title>");
		fputs("\n\n",stdout);
		skocz_do(wp, "<description>");
		tekst_pomiedzy(wp, stdout, "<description>");
		fputs("\n",stdout);
		fputs("\t\t\t\t\t",stdout);
		skocz_do(wp, "<pubDate>");
		tekst_pomiedzy(wp, stdout, "<pubDate>");
		fputs("\n\n\n",stdout);
	}
	return -2;
}


int main (int argc, char** argv)
{

	menu();
	
	return 0;
}


