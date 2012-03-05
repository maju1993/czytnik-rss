#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu();
int isAddCommand( char command[]);
int isListCommand( char command[]);
int isUpCommand( char command[]);
int isDelCommand( char command[]);

int main (int argc, char** argv)
{
	menu();
	return 0;
}

int isAddCommand( char command[])
{
	char* temp;
	temp=strndup(command, 11);
	return strcmp(temp,"channel add");
}

int isListCommand( char command[])
{
	char* temp;
	temp=strndup(command, 12);
	return strcmp(temp,"channel list");
}

int isUpCommand( char command[])
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel update");
}


int isDelCommand( char command[])
{
	char* temp;
	temp=strndup(command, 14);
	return strcmp(temp,"channel delete");
}


void menu()
{
  char command[255];
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
			printf("dodano kanał %s\n",link);
			URLOpenBlockinStream(0,link,&pStream,0,0);
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
