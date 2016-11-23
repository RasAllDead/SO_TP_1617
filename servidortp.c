

#include "estruturas.h" //SEPARAR EM ESTRUTURAS DO CLIENTE E ESTRUTURAS DO SERVIDOR?????
#include "servidor.h"
int fd,i,res,fd_resp,j;
msglog msgl;
mensagem msg;
char str[50];
    
void *fase1_login () {
     do{
            //LER DO CLIENTES
            i = read(fd,&msgl,sizeof(msgl));
            if (i==sizeof(msgl)){
                printf("recebi um pedido... (%d butes)\n",i);
                printf("login para %s com userneme %s\n",msgl.nome,msgl.pass);
            }

            //verificar se username e pass sao vlidos
            for(j=0;j<LENG;j++){
                if(strcmp(msgl.nome,Users[j].username)==0 && strcmp(msgl.pass,Users[j].password)==0){
                    Users[j].pid=msgl.pid; //atribui pid ao utilizador conhecido pois fez login
                    msgl.login = 1;
                }
            }

            //RESPONDER AO CLIENTE
            sprintf(str,"ccc%d",msgl.pid);
            fd_resp = open(str,O_WRONLY);
            i = write(fd_resp,&msgl.login,sizeof(msgl.login));//envia inteiro 0 ou 1 1 se login valido
            close(fd_resp);
            printf("Envia uma resposta - %d (%d bytes)\n",msgl.login,i);
        }while(msgl.login!=1);
        
        return NULL;
}

void *fase2_depois_de_logado(){
    
        do{
            //LER DO CLIENTES
            i = read(fd,&msg,sizeof(msg));
            if (i==sizeof(msgl)){
                printf("recebi um pedido... (%d butes)\n",i);
                printf("comando %s tecla %c\n",msg.comando,msg.tecla);
            }

            //verificar que comando foi introduzido e trata conforme o comando solicitado
    

            //RESPONDER AO CLIENTE
            sprintf(str,"ccc%d",msg.pid);
            fd_resp = open(str,O_WRONLY);
            i = write(fd_resp,&msgl.login,sizeof(msgl.login));//envia inteiro 0 ou 1 1 se login valido
            close(fd_resp);
            printf("Envia uma resposta - %d (%d bytes)\n",msgl.login,i);
        }while(msg.comando!="shutdown");
        return NULL;
}

int main(){
    int ret1,ret2;
    pthread_t thread1,thread2;
   
    mkfifo(FIFO_SERV,0600);
    fd = open(FIFO_SERV,O_RDWR);
    printf("Servidor Ligado ...\n");
    
    ret1=pthread_create(&thread1,NULL,fase1_login,NULL);
    ret2=pthread_create(&thread1,NULL,fase2_depois_de_logado,NULL);
    
    REGISTADOS = 0;
    lerficheiro();
    
    if(msgl.login != 1){
        pthread_join(thread1,NULL);
    }
    else{
        pthread_join(thread2,NULL);
    }


    
    
    
    return 0;
}