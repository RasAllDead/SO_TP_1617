

#include "estruturas.h" //SEPARAR EM ESTRUTURAS DO CLIENTE E ESTRUTURAS DO SERVIDOR?????
#include "servidor.h"

PEDIDO pedido;
RESP_SERV resp_serv;
char str[50];
int jogadores_logados[TAM_Jog_Logados] = {-1}; //a alterar    

int cliente_logado(int pid) {
    int i = 0, j = 0;

    for (j = 0; j < LENG; j++) { //verifica se algum username e pass ja foram usados para fazer login
        if (strcmp(pedido.str1, Users[j].username) == 0 && strcmp(pedido.str2, Users[j].password) == 0) {
            if (Users[j].pid != -1) {
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

int main() {

    fd_set fds;
    struct timeval t;
    int ret;
    char cmd[20];

    int tipo_erro_login = 0, fd, i, res, fd_resp, j;

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
    reinicia_pids_de_utilizadores();

    do {//reset vars
        FD_ZERO(&fds); //limpa
        FD_SET(0, &fds); // pedido para ficar a escuta de dados vindo do teclado
        FD_SET(fd, &fds); //peido para escuta de dados vindos dos clientes
        t.tv_sec = 2; //segundos  timeout
        t.tv_usec = 0; //microsegundos

        ret = select(fd + 1, &fds, NULL, NULL, &t);
        if (ret == 0) { //timeout
            //printf(".");
           // fflush(stdout);
            
        } else if (ret > 0) { //caso receba um dos pedidos introduzidos no set
            if (FD_ISSET(0, &fds)) {
                scanf("%s", cmd);
                printf("COMMAND: %s", cmd);
                fflush(stdout);
            } else if (FD_ISSET(fd, &fds)) {

                resp_serv.resposta = 0;
                //LER DO CLIENTE
                i = read(fd, &pedido, sizeof (pedido));
                if (i == sizeof (pedido)) {
                    printf("recebi um pedido do tipo %c... (%d bytes)\n", pedido.tipo_pedido, i);

                    // printf("login para %s com userneme %s\n",pedido.str1,pedido.str2);
                }


                if (pedido.tipo_pedido == 'l') {
                    printf("Tentativa de login: user: %s, pass: %s\n", pedido.str1, pedido.str2);


                    tipo_erro_login = cliente_logado(pedido.pid);
                    if (cliente_logado(pedido.pid) == 0) { //caso nao esteja logado
                        printf("[LOGIN] Não esta logado...\n");
                        //verificar se username e pass sao validos
                        for (j = 0; j < LENG; j++) {
                            if (strcmp(pedido.str1, Users[j].username) == 0 && strcmp(pedido.str2, Users[j].password) == 0) {
                                Users[j].pid = pedido.pid; //atribui pid ao utilizador conhecido pois fez login
                                resp_serv.resposta = 1;
                                printf(" login SUCESSO!\n");
                            }
                        }
                        //RESPONDER AO CLIENTE
                        sprintf(str, "ccc%d", pedido.pid);
                        fd_resp = open(str, O_WRONLY);
                        i = write(fd_resp, &resp_serv, sizeof (resp_serv)); //envia inteiro 0 ou 1 1 se login valido
                        close(fd_resp);
                        printf("Envia uma resposta - %d (%d bytes)\n", resp_serv.resposta, i);

                    } else {
                        //RESPONDER AO CLIENTE
                        printf(" login INSUCESSO!\n");
                        sprintf(str, "ccc%d", pedido.pid);
                        fd_resp = open(str, O_WRONLY);
                        i = write(fd_resp, &resp_serv, sizeof (resp_serv)); //envia inteiro 0 ou 1 1 se login valido
                        close(fd_resp);
                        printf("Envia uma resposta - %d (%d bytes)\n", resp_serv.resposta, i);

                    }

                } else {

                    if (pedido.tipo_pedido == 'j') {
                        printf("recebi o comando %s\n", pedido.str1);

                        //verificar que comando foi introduzido e trata conforme o comando solicitado

                        //RESPONDER AO CLIENTE
                        // sprintf(str,"ccc%d",msg.pid);
                        // sprintf(str,"ccc%d",pedido.pid);
                        // fd_resp = open(str,O_WRONLY);
                        // i = write(fd_resp,&resp_serv,sizeof(resp_serv));//envia inteiro 0 ou 1 1 se login valido
                        // close(fd_resp);
                        // printf("Envia uma resposta - %d (%d bytes)\n",resp_serv.resposta,i);
                    }

                }

                }
            }
        }
        while (true);

        close(fd);
        printf("Vou terminar...\n");
        unlink(FIFO_SERV);






    return 0;
}