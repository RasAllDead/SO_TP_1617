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

#define FIFO_SERV "fservidor"

#define LENG 5
#define TAM 20
#define TAM_Jog_Logados 5
#define TAM_EQUIPAS 9
#define TAM_MAX_MENSAGEM 500

int REGISTADOS=0;

/* ficheiro correspondente ao FIFO do cliente n (n -> "%d") */
//#define CLIENT_FIFO "/tmp/resp_%dfifo"
//#define SERV_FIFO "/tmp/fifo_serv"

typedef struct coordenadas{
int x;
int y;
}POSICAO;

typedef struct jogador_em_jogo{
    int pid;                            //pid do utilizador que controla o jogador em causa
    POSICAO pos;                         //coordenada X eY da posiçao do jogado
}JOGADOR;



struct utilizador{
    char nome[TAM];        //Username do utilizador
    int pid;                            //pid do utilizador
};

typedef struct bola {
    POSICAO pos;                              //cordenada x e y da posicao da bola
    int pid_jogador_com_bola;           //pid do jogador que tem a posse da bola
}BOLA;

struct golos{
    int tempo;                         //registo do tempo em que foi parcado o golo
    char nome[TAM];        //Username do utilizador
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

typedef struct pedido{
    int pid;                            //pid do utilizador
    char tipo_pedido;   
    char str1[TAM];        //Ler o username ou um comando
    char str2[TAM];        //Ler a pass ou uma tecla
    int estado_jogo;       //saber se ja ha um jogo a decorrer ou nao (nos casos em que o cliente entra depois de ja ter começado um jogo) 
}PEDIDO;

typedef struct resposta_servidor{
int resposta;
}RESP_SERV;

//typedef struct {
    //int pid;                            //pid do utilizador
   // char tecla;                         //tecla clicada
   // char comando[TAM_MAX_COMANDO];      //comando introduzido
//}mensagem;

struct resposta{
    //array de atualizadados
    int tempo_atual;                    // tempo atual do jogo
    int resultado_atual[2];             // resultado atual
    //ponteiro para struct bola talvez para saber dados da bola?
    char mensagem[TAM_MAX_MENSAGEM];    //mensagem
};

typedef struct 
{
	char username[TAM];
	char password[TAM];
        int pid;
}Ler_do_Fich;


Ler_do_Fich Users[LENG];