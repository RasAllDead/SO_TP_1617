#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>

#define FIFO_SERV "sss"

#define LENG 5
#define TAM_MAX_USERNAME 10
#define TAM_MAX_PASSWORD 10
#define TAM_MAX_MENSAGEM 500
#define TAM_MAX_COMANDO 20

int REGISTADOS=0;

/* ficheiro correspondente ao FIFO do cliente n (n -> "%d") */
//#define CLIENT_FIFO "/tmp/resp_%dfifo"
//#define SERV_FIFO "/tmp/fifo_serv"


typedef struct jogador_em_jogo jogador;
struct jogador_em_jogo{
    char nome[TAM_MAX_USERNAME];        //Username do utilizador
    int pid;                            //pid do utilizador que controla o jogador em causa
    int posicao;                        //posicao do jogador em campo
    char equipa;                        //'V' para equipa vermelha 'A' para equipa azul
    int X;                              //coordenada X da posiçao do jogador
    int Y;                              //coordenada Y da posiçao do jogador
    bool tem_bola;                      //var para saber se o jugador esta com a posse da bola

};

struct utilizador{
    char nome[TAM_MAX_USERNAME];        //Username do utilizador
    int pid;                            //pid do utilizador
};

struct bola {
    int x;                              //cordenada x da posicao da bola
    int y;                              //coordenada y da posicao da bola
    int pid_jogador_com_bola;           //pid do jogador que tem a posse da bola
};

struct golos{
    int tempo;                         //registo do tempo em que foi parcado o golo
    char nome[TAM_MAX_USERNAME];        //Username do utilizador
    char equipa;                        //'V' para equipa vermelha 'A' para equipa azul
};

typedef struct info_do_jogo jogo;
struct info_do_jogo {
    int resultado[2];                   //resultado atual do jogo 0 - 0 ('V','A')
    int n_golos_marcados;               // numero de golos marcados ate ao momento
    //array de golos                    // array de estruturas de golos
    int tempo_atual;                    // tempo atual do jogo
    int tempo_max_jogo;                 //tempo maximo estipulado pelo admin para terminar o jogo
    char mensagem[TAM_MAX_MENSAGEM];    //mensagem
    
};

typedef struct atualiza_dados_jogadores atualiza;
struct atualiza_dados_jogadores{
    int posicao;                        //posicao no jogo do jogador
    char equipa;                        //equipa do jogador
    int x;                              //coordenada x da sua posicao
    int y;                              //coordenada y da sua posicao
};

typedef struct mensagem_login msglog;
struct mensagem_login{
    char nome[TAM_MAX_USERNAME];        //Username do utilizador
    char pass[TAM_MAX_PASSWORD];        // palavra pass do utilizador
    int login;                      //se for 0 ainda nao fez login se for 1 login aceite
    int pid;                            //pid do utilizador
};

typedef struct {
    int pid;                            //pid do utilizador
    char tecla;                         //tecla clicada
    char comando[TAM_MAX_COMANDO];      //comando introduzido
}mensagem;

struct resposta{
    //array de atualizadados
    int tempo_atual;                    // tempo atual do jogo
    int resultado_atual[2];             // resultado atual
    //ponteiro para struct bola talvez para saber dados da bola?
    char mensagem[TAM_MAX_MENSAGEM];    //mensagem
};

typedef struct 
{
	char username[30];
	char password[15];
        int pid;
}Ler_do_Fich;



Ler_do_Fich Users[LENG];