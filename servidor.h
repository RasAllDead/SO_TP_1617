#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>


void lerficheiro(){
	FILE *f;
	
	f=fopen("users.txt","r");
	if (f==NULL){
		printf("Erro ao abrir o ficheiro!\n");
		exit(1);
	}
	while(fscanf(f,"%[^:]:%[^\n]\n",Users[REGISTADOS].username,Users[REGISTADOS].password)==2)
		REGISTADOS++;
	fclose(f);
}

void reinicia_pids_de_utilizadores(){
int i;
for(i=0;i<REGISTADOS;i++){
Users[i].pid = -1;
}
}