

#include "cliente.h"
#include "estruturas.h" //SEPARAR EM ESTRUTURAS DO CLIENTE E ESTRUTURAS DO SERVIDOR?????

msglog msgl;
mensagem msg;

int main(void){
    //VARIAVEIS TESTE -> ALGUMAS A MUDAR PARA AS VARIAVEIS DAS ESTRUTURAS ---
    char nome[10] = "AAAAAAAAAA";
    bool sairlogin=false;
    //----------------------------------------------------------------------
    int fd,i,res=0,fd_resp;
    char str[50];
    
    if(access(FIFO_SERV,F_OK) != 0) {
		printf("O servidor não está a correr...\n");
		return 1;
    }
    msgl.pid = getpid(); //vai buscar o pid do cliente
    sprintf(str,"ccc%d",msgl.pid);
    mkfifo(str,0600); // cria named pipe
    
    fd = open(FIFO_SERV,O_WRONLY);
    
    initscr(); //construtor da ncurses
    raw();/* Line buffering disabled	*/
    
   
   
    if(imprime_janela_1()){ // caso pretende fazer login |1|
        do{
            imprime_login_janela1(msgl.nome,msgl.pass);
            //manda o server o nome e pass
            i=write(fd,&msgl,sizeof(msgl));
            //printf("Enviei um pedido...(%d bytes)\n",i);
            
            //recebe se o login foi valido ou nao
            fd_resp = open(str,O_RDONLY);
            i = read(fd_resp, &res, sizeof(res));
            close(fd_resp);
            //printf("Recebi uma resposta... RESULTADO = %d (%d bytes)\n",res,i);
            
            if (res == 0){//  se entrar que dizer que o login nao foi valido
               if(!imprime_login_incorreto()){
                   sairlogin = true;
               }
            }
            else{ // se foi valido sai da fase de login
                sairlogin = true;
            }
        }while(sairlogin==false);
       
        if(res==1){
            msg.pid = getpid();
            do{
                imprime_janela_espera_de_jogo();
                imprime_linha_comandos(msg.comando);
                //manda o comando introduzido
                i=write(fd,&msg,sizeof(msg));
                //printf("Enviei um pedido...(%d bytes)\n",i);

                //recebe se o login foi valido ou nao
                fd_resp = open(str,O_RDONLY);
                i = read(fd_resp, &res, sizeof(res));
                close(fd_resp);
                //printf("Recebi uma resposta... RESULTADO = %d (%d bytes)\n",res,i);

            }while(strcmp(msg.comando,"sair"));
        }
    }
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
    close(fd);
    unlink(str);
    getch();
    system("clear");
    endwin(); // destrutor da ncurses
    return 0;
}
