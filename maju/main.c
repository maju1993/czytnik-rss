#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


typedef struct e {
char* title;
char* link;
char* description;
char* pubdate;
char* guid;
struct e* next;
struct e* prev;
}*item;


void parse( item m);
void menu();
void downloadfile(char* link);
int isAddCommand( char* command);
int isListCommand( char* command);
int isUpCommand( char* command);
int isDelCommand( char* command);

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int main (int argc, char** argv)
{
	menu();
	return 0;
}

int isAddCommand( char* command)
{
	char* temp;
	temp=strndup(command, 11);
	return strcmp(temp,"channel add");
}

int isListCommand( char* command)
{
	char* temp;
	temp=strndup(command, 12);
	return strcmp(temp,"channel list");
}

int isUpCommand( char* command)
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel update");
}


int isDelCommand( char* command)
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel delete");
}

void downloadfile(char* link)
{
	CURL *curl_handle;
	link[strlen(link+1)]='\0';
	link[strlen(link)]='/';
		
  static const char *pagefilename = "data.rss";
  FILE *pagefile;
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl_handle = curl_easy_init();
 
  curl_easy_setopt(curl_handle, CURLOPT_URL, link);
 
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
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


void parse( item m)
{
	
}

void menu()
{
  char* command=malloc(255);
  char* link;
	FILE* pStream;
	do
	{
		fgets(command,255,stdin);
		system("clear");
		command[(int)strlen(command)-1]='\0';	
		printf("%s \n",command);
		if (isAddCommand(command)==0)
		{
			link=strndup(command+12, strlen(command));
			downloadfile(link);
			printf("dodano kanał %s\n",link);
			item m=malloc(sizeof*m);
			parse(m);
			
		
			//czydobrylink(), sciagnij(), dodajdobazy();
		}
		if (isListCommand(command)==0)
		{
			link=strndup(command+13, strlen(command));
			printf("listuj kanał %s\n",link);
			//czydobrylink(), sciagnij(), dodajdobazy();
		}
		if (isUpCommand(command)==0)
		{
			link=strndup(command+15, strlen(command));
			printf("updatuj kanał %s\n",link);
			//czydobrylink(), sciagnij(), dodajdobazy();
		}
		if (isDelCommand(command)==0)
		{
			link=strndup(command+15, strlen(command));
			printf("usunieto kanał %s\n",link);
			//czydobrylink(), sciagnij(), dodajdobazy();
		}

	}
	while( strcmp(command,"exit"));
}
