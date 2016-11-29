

#include "cliente.h"
#include "estruturas.h" //SEPARAR EM ESTRUTURAS DO CLIENTE E ESTRUTURAS DO SERVIDOR?????

PEDIDO pedido;
RESP_SERV resp_serv;
bool iniciou_jogo = false;
bool jogar = false;
bool estado_espera = false;
bool entrou_em_jogo = false;



int getpid_serv(){
FILE *f;
int pid;

	f=fopen("serv_pid.txt","r");
	if (f==NULL){
		printf("Erro ao abrir o ficheiro!\n");
		exit(1);
	}               
                fscanf(f, "%d", &pid);
                
	fclose(f);
return pid;
}

void recebe_sinal(int s, siginfo_t *info, void *context) { //avisos a aparecer mas tem de ter sitio...
   // printf("Recebi o sinal %d do processo %d...\n", s, info->si_pid);
    if (info->si_code == SI_QUEUE) {

        if (info->si_signo == SIGUSR1) { //recebe este sinal quando o admin inicia um jogo
           // printf("o jogo iniciou %d!", info->si_int);
            entrou_em_jogo = false;
            iniciou_jogo = true;
        } else {
            if (info->si_signo == SIGALRM) { //recebe este sinal quando o jogo que esta a decorrer acaba
              //  printf("o jogo terminou %d!", info->si_int);
                entrou_em_jogo = false;
                iniciou_jogo = false;
                jogar = false;
            }
        }
    }
    fflush(stdout);
}

void reparte_comando(char * cmd, char * cmd1, char* cmd2, char* cmd3) {
    int contador = 0;
    char * pch;
  //  printf("recebi a string %s \n", cmd);
    pch = strtok(cmd, " ");
    strcpy(cmd1, " ");
    strcpy(cmd2, " ");
    strcpy(cmd3, " ");
    while (pch != NULL) {
        if (contador == 0)
            strcpy(cmd1, pch);
        if (contador == 1)
            strcpy(cmd2, pch);
        if (contador == 2)
            strcpy(cmd3, pch);
        // printf ("%s\n",pch);
        pch = strtok(NULL, "[^ \n]");

        contador++;
    }
   // printf("cmd1: %s cmd2: %s cmd3: %s\n", cmd1, cmd2, cmd3);
    //  fflush(stdout);
}

void Avisa_servidor_VouSair() {
    int i;
    union sigval val;
    val.sival_int = getpid();
 
    sigqueue(getpid_serv(), SIGUSR2, val);   
    
}


int main(void) {

    char nome[TAM] = "";
    bool sairlogin = false;

    //----------------------------------------------------------------------
    int fd, i, res = 0, fd_resp;
    char str[50];
    struct sigaction act;

    fd_set fds;
    struct timeval t;
    int ret;

    char cmd[50];
    char cmd1[10], cmd2[20], cmd3[20];

    //sinais que recebe
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = recebe_sinal;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGALRM, &act, NULL);

    if (access(FIFO_SERV, F_OK) != 0) {
        printf("O servidor não está a correr...\n");
        return 1;
    }

    pedido.pid = getpid(); //vai buscar o pid do cliente
    sprintf(str, "fcliente_%d", pedido.pid);
    mkfifo(str, 0600); // cria named pipe

    fd = open(FIFO_SERV, O_WRONLY);

    initscr(); //construtor da ncurses
    raw(); /* Line buffering disabled	*/
    

    do {
        if (jogar) {
            if (!entrou_em_jogo) { //caso tenha entrado em jogo vai jogar
               
		limpa_linha_comandos();
                limpa_janela_lvl1();
                limpa_janela_lvl2();
                limpa_campo();
                limpa_resultado();

                //do{
                //strcpy(cmd,"");
                imprime_linha_comandos(cmd);

                reparte_comando(cmd, cmd1, cmd2, cmd3);
                //&& (cmd2[0] <='9' && cmd2[0] >= '0') nao funciona ...
                if (strcmp(cmd1, "A") == 0 || strcmp(cmd1, "B") == 0) {//equipas disponiveis

                    //pode ter uma verificaçao para ver se esta a mandar o pedido numa forma correta tipo : A 3 sendo referente a equipa A , posiçao 3
                    strcpy(pedido.str1, cmd1);
                    strcpy(pedido.str2, cmd2);

                    pedido.tipo_pedido = 'e'; //escolher equipa e jogador
                    //manda o server o nome e pass
                    i = write(fd, &pedido, sizeof (pedido));
                    //printf("Enviei um pedido...(%d bytes)\n",i);

                    //recebe se o login foi valido ou nao
                    fd_resp = open(str, O_RDONLY);
                    i = read(fd_resp, &resp_serv, sizeof (resp_serv));
                    close(fd_resp);
                    //printf("Recebi uma resposta do escolhe equipas... RESULTADO = %d (%d bytes)\n",res,i);

                    if (resp_serv.resposta == 1) {//  se entrar que dizer que o jogador nao esta a ser usado 
                        entrou_em_jogo = true;
                    }
                } else {
                    //mensagem a dizer que o comando esta errado 

                }

                if (strcmp(cmd1, "sair") == 0)
                    jogar = false;
                //}while((iniciou_jogo == true && entrou_em_jogo == false) || strcmp(cmd1,"sair")!=0);
            } else {

                limpa_linha_comandos();
                limpa_janela_lvl1();
                limpa_janela_lvl2();
                limpa_campo();

                imprime_moldura();
                imprime_campo();
                imprime_resultado(5, 4);
                imprime_tempo(120);
                imprime_precione_tecla_para_sair();
                sleep(3);
            }
        } else {

            if (sairlogin == false) {
                if (imprime_janela_1(pedido.str1)) { // caso pretende fazer login |1|
                    do {
                        imprime_login_janela1(pedido.str1, pedido.str2);
                        pedido.tipo_pedido = 'l';
                        //manda o server o nome e pass
                        i = write(fd, &pedido, sizeof (pedido));
                        //printf("Enviei um pedido...(%d bytes)\n",i);

                        //recebe se o login foi valido ou nao
                        fd_resp = open(str, O_RDONLY);
                        i = read(fd_resp, &resp_serv, sizeof (resp_serv));
                        close(fd_resp);
                        //printf("Recebi uma resposta... RESULTADO = %d (%d bytes)\n",res,i);

                        if (resp_serv.resposta == 0) {//  se entrar que dizer que o login nao foi valido
                            if (!imprime_login_incorreto(pedido.str1)) {
                                sairlogin = true;
                            }
                        } else { // se foi valido sai da fase de login
                            sairlogin = true;
                            estado_espera = true;
                        }
                    } while (sairlogin == false);
                }
            }
            if (estado_espera) {
                pedido.pid = getpid();

                do {
                    FD_ZERO(&fds); //limpa
                    FD_SET(0, &fds); // pedido para ficar a escuta de dados vindo do teclado
                    t.tv_sec = 0; //segundos  timeout
                    t.tv_usec = 50000; //microsegundos
                    //  printf("passei aqui.\n");
                    ret = select(fd + 1, &fds, NULL, NULL, &t);
                    if (ret == 0) { //timeout

                        if (iniciou_jogo) {
                            imprime_janela_tem_jogo_disponivel();
                            imprime_linha_comandos(pedido.str1);


                        } else {
                            imprime_janela_espera_de_jogo();
                            imprime_linha_comandos(pedido.str1);

                        }
                    } else if (ret > 0) { //caso prima uma tecla
                        if (iniciou_jogo) {
                            imprime_janela_tem_jogo_disponivel();
                            imprime_linha_comandos(pedido.str1);


                        } else {
                            imprime_janela_espera_de_jogo();
                            imprime_linha_comandos(pedido.str1);

                        }
                    }
                    if (iniciou_jogo)
                        if (strcmp(pedido.str1, "jogar") == 0)
                            jogar = true;

                } while ((strcmp(pedido.str1, "sair") != 0) && (strcmp(pedido.str1, "jogar") != 0));

            }

        }// fim do else de iniciou_jogo

    } while (strcmp(pedido.str1, "sair") != 0);
    //caso pretenda sair |2|
    imprime_janela_realizado_por();



    //imprime_moldura(); 
    //imprime_campo();
    //imprime_resultado(5,4);
    //imprime_tempo(120);
    //imprime_precione_tecla_para_sair();

    //limpa_linha_comandos();
    //limpa_janela_lvl1();
    //limpa_janela_lvl2();
    //limpa_campo();
    //limpa_resultado();
    //imprime_janela_realizado_por();
    //imprime_janela_espera_de_jogo();
    //imprime_janela_tem_jogo_disponivel();
    //admin_imprime_janela_sem_jogos_a_decorrer();
    //admin_imprime_janela_com_jogos_a_decorrer();
    //imprime_cartao_vermelho();
    //imprime_servidor_encerrado_em_jogo();
    //admin_user_adicionado(false,NULL);
    //admin_cartao_vermelho(false,nome);

    //getch();// fica a espera de uma tecla para continuar

  Avisa_servidor_VouSair();

    close(fd);
    unlink(str);
    getch();
    system("clear");
    endwin(); // destrutor da ncurses
    return 0;
}
