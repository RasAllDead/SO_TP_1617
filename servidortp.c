

#include "estruturas.h" //SEPARAR EM ESTRUTURAS DO CLIENTE E ESTRUTURAS DO SERVIDOR?????
#include "servidor.h"
#define NANO_PER_SEC 1000000000.0
PEDIDO pedido;
RESP_SERV resp_serv;
int tempo_decorrido_de_jogo;
int tempo_de_jogo;
char str[50];
int jogadores_logados[TAM_Jog_Logados]; //a alterar  
JOGADOR equipaA[TAM_EQUIPAS];
JOGADOR equipaB[TAM_EQUIPAS];
BOLA bola;

int cliente_logado(int pid) {
    int i = 0, j = 0;

    for (j = 0; j < LENG; j++) { //verifica se algum username e pass ja foram usados para fazer login
        if (strcmp(pedido.str1, Users[j].username) == 0 && strcmp(pedido.str2, Users[j].password) == 0) {
            if (Users[j].pid != 0) {
                printf("[verifica] ja foi usada essa conta para login!\n");
                return 2; //caso ja tenham sido usadas o username e pass para entrar

            }
        }
    }

    //caso o username e pass ainda nao tenham sido usados vai verificar se o pid ja esta registado
    for (i = 0; i < TAM_Jog_Logados; i++) { //verifica se o pid é igual ao ja logado
        if (pid == jogadores_logados[i]) {
            printf("[verifica] ja se encontra alguem logado com esse pid!\n");
            return 1; // caso ja tenha alguem logad com esse pid

        }
    }

    return 0;
}

int add_pid_jogador_logado(int pid) {
    int i;
    int chegou_ao_limite = 0;
    for (i = 0; i < TAM_Jog_Logados; i++) {
        if (jogadores_logados[i] == 0) {
            jogadores_logados[i] = pid;
            chegou_ao_limite = 1;
            return chegou_ao_limite;
        }
    }
    return chegou_ao_limite;
}

void inicioDeJogo_paraClientes() {
    int i;
    union sigval val;
    val.sival_int = 1;
    for (i = 0; i < TAM_Jog_Logados; i++) {
        if (jogadores_logados[i] != 0) {
            sigqueue(jogadores_logados[i], SIGUSR1, val);
            printf("[SINAL] jogador com pid %d foi avisado do inicio do jogo!\n", jogadores_logados[i]);
        }
    }
}

void sinal_fimDeJogo_paraClientes(int s) {
    int i;
    union sigval val;
    val.sival_int = 0;
    if (s == SIGALRM) {
        for (i = 0; i < TAM_Jog_Logados; i++) {
            if (jogadores_logados[i] != 0) {
                sigqueue(jogadores_logados[i], SIGALRM, val);
                printf("[SINAL] jogador com pid %d foi avisado do fim de jogo!\n", jogadores_logados[i]);
            }
        }
    }

}

void sinal_jogadorSaiu(int s,siginfo_t *info,void *context){
    int i,j;
    int pid = info->si_int;
if (info->si_code == SI_QUEUE){
     if (s == SIGUSR2) {
        for (i = 0; i < LENG; i++) {
            if (Users[i].pid == pid) {
                Users[i].pid = 0;
                printf("[SINAL] Jogador %s com pid %d saiu do jogo!\n",Users[i].username ,pid);
            }
        }
	for(j=0;j<TAM_Jog_Logados;j++){
	    if (jogadores_logados[i] == pid) 
	    jogadores_logados[i]==0;
	}
    }
}
}

void reparte_comando(char * cmd, char * cmd1, char* cmd2, char* cmd3) {
    int contador = 0;
    char * pch;
    //  printf ("recebi a string %s \n",cmd);
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
    // printf("cmd: %s cmd1: %s cmd2: %s\n",cmd1,cmd2,cmd3);
    //  fflush(stdout);
}

bool jogador_em_jogo(int pid ){ //verifica nas equipas se algum jogador tem o mesmo pid que o remetente do pedido
    int o;
    for(o=0;o<TAM_EQUIPAS;o++){
        if(equipaA[o].pid == pid ) return true; 
    }
    
    for(o=0;o<TAM_EQUIPAS;o++){
        if(equipaB[o].pid == pid ) return true; 
    }
    return false;
}


bool escrever_no_ficheiro(char * user, char* pass){
	FILE *f;
	
        //if(strcmp(user,'\n')==0 && strcmp(pass,'\n')==0 ){
        
	f=fopen("users.txt","a");
	if (f==NULL){
		printf("Erro ao abrir o ficheiro!\n");
		exit(1);
	}
	
		REGISTADOS++; //introduz o utilizador novo ja no array
                strcpy(Users[REGISTADOS].username, user);
                strcpy(Users[REGISTADOS].password,pass);
                
                fprintf(f, "%s:%s\n", user,pass);//introduz o utilizador novo no fiheiro, assim quando o programa voltar a reiniciar ja preenche o array com ele
                
	fclose(f);
        return true;
       // }
      //  return false;
}

void partilha_pid(){
FILE *f;
	f=fopen("serv_pid.txt","w");
	if (f==NULL){
		printf("Erro ao abrir o ficheiro!\n");
		exit(1);
	}               
                fprintf(f, "%d\n", getpid());
                
	fclose(f);
}

int main() {

    fd_set fds;
    struct timeval t;
    int ret;
    char cmd[50];
    char cmd1[10], cmd2[20], cmd3[20];
    int tipo_erro_login = 0, fd, i, res, fd_resp, j, contador, tempo_de_jogo;
    //vars para verificar o tempo decorrido   
    struct timespec start, end;
    double start_sec, end_sec, elapsed_sec;   
    //dinal de alarm caso o admin nao introduza o stop o jogo acaba no tempo determinado no comando start x
    signal(SIGALRM, sinal_fimDeJogo_paraClientes);    
    //sinais que recebe    
    struct sigaction act;   
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sinal_jogadorSaiu;
    sigaction(SIGUSR2, &act, NULL);
    


    partilha_pid();

    if (access(FIFO_SERV, F_OK) == 0) {
        printf("Já existe um SERVIDOR a correr\n");
        return 5;
    }

    res = mkfifo(FIFO_SERV, 0600);
    if (res == -1) {
        printf("erro no fifo\n");
        return 6;
    }

    fd = open(FIFO_SERV, O_RDWR);
    printf("Servidor Ligado ...\n");


    //REGISTADOS = 0;
    lerficheiro();
   // reinicia_pids_de_utilizadores();

    do {//reset vars
        FD_ZERO(&fds); //limpa
        FD_SET(0, &fds); // pedido para ficar a escuta de dados vindo do teclado
        FD_SET(fd, &fds); //peido para escuta de dados vindos dos clientes
        t.tv_sec = 2; //segundos  timeout
        t.tv_usec = 0; //microsegundos

        ret = select(fd + 1, &fds, NULL, NULL, &t);
        if (ret == 0) { //timeout

        } else if (ret > 0) { //caso receba um dos pedidos introduzidos no set
            if (FD_ISSET(0, &fds)) {
                //lê comando do admin
                scanf(" %[^\n]s", cmd);

                reparte_comando(cmd, cmd1, cmd2, cmd3);
                //printf("cmd1: %s cmd2: %s cmd3: %s\n",cmd1,cmd2,cmd3);
                fflush(stdout);

                if (strcmp(cmd1, "start") == 0) {
                    printf("Entrei no start! e o duracao e %s\n", cmd2);
                    //guarda o tempo em que o jogo começou
                    clock_gettime(CLOCK_REALTIME, &start);
                    //mandar sinal a todos
                    inicioDeJogo_paraClientes();
                    tempo_de_jogo = atoi(cmd2);

                    alarm(tempo_de_jogo); //alarm de x tempo para quando chegar ao fium sem interrupçoes ele acabar com o jogo                   
                } else {

                    if (strcmp(cmd1, "stop") == 0) {

                        clock_gettime(CLOCK_REALTIME, &end); //obtem tempo em que foi mandado o comando stop
                        start_sec = start.tv_sec + start.tv_nsec / NANO_PER_SEC;
                        end_sec = end.tv_sec + end.tv_nsec / NANO_PER_SEC;
                        elapsed_sec = end_sec - start_sec; //obtem tempo entre o começo do jogo e o fim 

                        printf("Entrei no stop! e o jogo ia em  %.2f segundos\n", elapsed_sec);
                        //mandar sinal a todos                             
                        alarm(1); // ha esta maneira e tambem podemos fazer alarm(0) e depois chamar a funçao que avisa os jogadores , porque o alarm 0 apenas anula o alarm predefinido antes o que faz com que a funçao nao seja chamada)

                    } else {
                        if (strcmp(cmd1, "users") == 0) {
                            printf("Jogadores logados/emJogo : \n");
                            for (i = 0; i < LENG; i++) {
                                if (Users[i].pid != 0) {
                                    printf("--> %s: esta logado",Users[i].username);
                                    if (jogador_em_jogo(Users[i].pid)) {
                                        printf(" e esta em jogo\n");
                                    } else {
                                        printf("\n");
                                    }
                                }

                            }
                        } else {
                        if (strcmp(cmd1, "user") == 0) {
                        if(escrever_no_ficheiro(cmd2, cmd3))
                             printf("Utilizador registado com SUCESSO!\n");
                        else
                            printf("Não foi possivel registar o utilizador!\n");

                        } else {
                            if (strcmp(cmd1, "result") == 0) {



                            } else {
                                if (strcmp(cmd1, "red") == 0) {
                                    //username esta no cmd2


                                } else {
                                    if (strcmp(cmd1, "shutdownr") == 0) {



                                    } else {
                                        printf("Comando invalido\n");
                                    }
                                }
                            }
                        }
                    }
                }
                }

            } else if (FD_ISSET(fd, &fds)) {

                resp_serv.resposta = 0;
                //LER DO CLIENTE
                i = read(fd, &pedido, sizeof (pedido));
                if (i == sizeof (pedido)) {
                    printf("recebi um pedido do tipo %c... (%d bytes)\n", pedido.tipo_pedido, i);
                }
                if (pedido.tipo_pedido == 'l') {
                    printf("Tentativa de login: user: %s, pass: %s\n", pedido.str1, pedido.str2);

                    tipo_erro_login = cliente_logado(pedido.pid);
                    if (cliente_logado(pedido.pid) == 0) { //caso nao esteja logado
                        printf("[LOGIN] Não esta logado...\n");
                        //verificar se username e pass sao validos
                        for (j = 0; j < LENG; j++) {
                            if (strcmp(pedido.str1, Users[j].username) == 0 && strcmp(pedido.str2, Users[j].password) == 0) {

                                if (add_pid_jogador_logado(pedido.pid)) {
                                    Users[j].pid = pedido.pid; //atribui pid ao utilizador conhecido pois fez login
                                    resp_serv.resposta = 1;
                                    printf(" login SUCESSO!\n");
                                } else printf(" limite de jogadores logados atingido!\n");
                            }
                        }
                        //RESPONDER AO CLIENTE
                        sprintf(str, "fcliente_%d", pedido.pid);
                        fd_resp = open(str, O_WRONLY);
                        i = write(fd_resp, &resp_serv, sizeof (resp_serv)); //envia inteiro 0 ou 1 1 se login valido
                        close(fd_resp);
                        printf("Envia uma resposta - %d (%d bytes)\n", resp_serv.resposta, i);

                    } else {
                        //RESPONDER AO CLIENTE
                        printf(" login INSUCESSO!\n");
                        sprintf(str, "fcliente_%d", pedido.pid);
                        fd_resp = open(str, O_WRONLY);
                        i = write(fd_resp, &resp_serv, sizeof (resp_serv)); //envia inteiro 0 ou 1 1 se login valido
                        close(fd_resp);
                        printf("Envia uma resposta - %d (%d bytes)\n", resp_serv.resposta, i);

                    }

                } else {


                    if (pedido.tipo_pedido == 'e') {
                        //imnplementar verificaçoes
                        //  verifica_posicao_ocupada()
                        //caso nao estaje meter o pid do jogador na posiçao escolhida na equipa escolhida
                        //e dar resposta = 1 
                        //senao resposta = 0

                        //foi aprovado (APENAS PARA TESTE))
                        resp_serv.resposta = 1;


                        //RESPONDER AO CLIENTE
                        sprintf(str, "fcliente_%d", pedido.pid);
                        fd_resp = open(str, O_WRONLY);
                        i = write(fd_resp, &resp_serv, sizeof (resp_serv)); //envia inteiro 0 ou 1 1 se login valido
                        close(fd_resp);
                        printf("Envia uma resposta - %d (%d bytes)\n", resp_serv.resposta, i);


                    } else {
                        if (pedido.tipo_pedido == 'j') { //recebe as teclas que o utilizador mandar para mexert o boneco
                            printf("recebi o comando %s\n", pedido.str1);

                            //verificar que comando foi introduzido e trata conforme o comando solicitado

                        }
                    }
                }

            }
        }
    } while (true);

    close(fd);
    printf("Vou terminar...\n");
    unlink(FIFO_SERV);






    return 0;
}