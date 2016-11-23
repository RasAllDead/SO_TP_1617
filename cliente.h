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


int TAMX =71; // tamanho X da janela (sem a borda) (Tamanho da moldura))
int TAMY = 31; // tamanho Y da janela (sem a borda)) (Tamanho da moldura))

void imprime_moldura () {
    int i=0, X=2, Y=1; // (Y,X) iniciais 
    char chmolduta = '*'; // char que é usado para desenhar moldura
    
    move(Y,X);
    for(i=0;i<X+TAMX;i++)
        addch(chmolduta);
    
    move(Y+TAMY,X);
    for(i=0;i<X+TAMX;i++)
        addch(chmolduta);
    
    
    for(i=1;i<Y+TAMY-1;i++){
        move(Y+i,X);
        addch(chmolduta);
    }
    
     for(i=1;i<Y+TAMY-1;i++){
        move(Y+i,X+TAMX+1);
        addch(chmolduta);
    }
    
}

bool imprime_janela_1(){ // Janela inicial Login? ou Sair?
    int i=0,Y=7,X=24;
    char opcao = 'x';
    bool logar_ou_sair;
    
    imprime_moldura();
    
    move(Y-1,X-6);
    for(i=0;i<40;i++)
        addch('-');
    move(Y,X-4);
    for(i=0;i<36;i++)
        addch('-');
    
    move(Y+2,X);
    printw("JOGO DE FUTEBOL MULTIJOGADOR");
    move(Y+4,X+6);
    printw("Trabalho Pratico");
    move(Y+5,X+8);
    printw("SO 2016/2017");
    
    move(Y+7,X-4);
    for(i=0;i<36;i++)
        addch('-');
    move(Y+8,X-6);
    for(i=0;i<40;i++)
        addch('-');
    
    move(Y+14,X+11);
    printw("|1| Login");
    
    move(Y+16,X+11);
    printw("|2| Sair");
    
    do{
        move(Y+12,X+4);
        printw("Escolha uma opcao: | |");
        move(Y+12,X+24);
        opcao=(char)getch();
    }while(opcao != '1' && opcao !='2');
        
        addch(opcao);
        sleep(0.5);
    
    if(opcao == '1'){
        logar_ou_sair = true;
    }
    else{
        logar_ou_sair = false;
    }
    
    return logar_ou_sair;
    
}

void imprime_campo(){
    int i = 0, Y = 5 ,X = 13;
    int largura_campo = 51, altura_campo = 21, meio_campo = 25, tamanho_balizas = 9;
    
    move(Y,X); //linha superior
    for(i=0; i<largura_campo ;i++)
        addch('-');
    
    move(Y+altura_campo-1,X); // linha inferior
    for(i=0; i<largura_campo ;i++)
        addch('-');
    
    for(i=1; i<altura_campo-1 ;i++){ // linha esquerda
        move(Y+i,X);
        addch('|');
    }
    
      for(i=1; i<altura_campo-1 ;i++){ // linha direita
        move(Y+i,X+largura_campo-1);
        addch('|');
    }
    
    move(Y+8,X+meio_campo-2); //meio campo
    for(i= 0; i< 5 ; i++){ 
        addch('-');      
    }
    
    move(Y+12,X+meio_campo-2); //meio campo
    for(i= 0; i< 5 ; i++){ 
        addch('-');      
    }
    
    for(i=9;i<12;i++){
        move(Y+i,X+meio_campo-4); //meio campo
        addch('-');
    }
    
      for(i=9;i<12;i++){
        move(Y+i,X+meio_campo-4); // meio campo
        addch('-');
    }
    
      for(i=9;i<12;i++){
        move(Y+i,X+meio_campo+4); // meio campo
        addch('-');
    }
    
    
      for(i=1; i<altura_campo-1 ;i++){ // linha meio campo
        move(Y+i,X+meio_campo);
        addch('|');
    }
     
    for(i=6; i<6+tamanho_balizas ;i++){ // baliza direita
        move(Y+i,X+largura_campo-1);
        addch('H');
    }
    for(i=6; i<6+tamanho_balizas ;i++){ // baliza direita
        move(Y+i,X+largura_campo);
        addch('H');
    }
       for(i=6; i<6+tamanho_balizas ;i++){ // baliza direita
        move(Y+i,X+largura_campo+1);
        addch('H');
    }
    
    
    
    for(i=6; i<6+tamanho_balizas ;i++){ // baliza esquerda
        move(Y+i,X);
        addch('H');
    }
    for(i=6; i<6+tamanho_balizas ;i++){ // baliza esquerda
        move(Y+i,X-1);
        addch('H');
    }
     for(i=6; i<6+tamanho_balizas ;i++){ // baliza esquerda
        move(Y+i,X-2);
        addch('H');
    }
    
    
    
    for (i = 6; i < 15;i++){ //grande area esquerda
        move(Y+i,X+8);
        addch('-');
    }
    
    move(Y+4,X+1);
    for(i=0;i < 5;i++)
        addch('-');
    
    move(Y+5,X+7);
    addch('-');
    
    move(Y+16,X+1);
    for(i=0;i < 5;i++)
        addch('-');
    
    move(Y+15,X+7);
    addch('-');
    
    
    for (i = 6; i < 15;i++){ //grande area direita
        move(Y+i,X+largura_campo-8);
        addch('-');
    }
    
    move(Y+4,X+largura_campo-6);
    for(i=0;i < 5;i++)
        addch('-');
    
    move(Y+5,X+largura_campo-7);
    addch('-');
    
    move(Y+16,X+largura_campo-6);
    for(i=0;i < 5;i++)
        addch('-');
    
    move(Y+15,X+largura_campo-7);
    addch('-');
    
    
    
}

void imprime_resultado(int equipa_vermelha, int equipa_azul){

    int i=0,meio_campo = 25, Y = 2, X=13;

    move(Y,X+meio_campo-4);
    for(i=0;i<9;i++)
        addch('-');
    
    move(Y+2,X+meio_campo-4);
    for(i=0;i<9;i++)
     addch('-');
    
    move(Y+1,X+meio_campo-4);
    addch('|');
    
    move(Y+1,X+meio_campo);
    addch('|');
    
    move(Y+1,X+meio_campo+4);
    addch('|');
    
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    init_pair(2,COLOR_BLACK,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    move(Y+1,X+meio_campo-3);
    printw(" %d ",equipa_vermelha);
    refresh();
    attroff(COLOR_PAIR(1));
    
    attron(COLOR_PAIR(2));
    move(Y+1,X+meio_campo+1);
    printw(" %d ",equipa_azul);
    attroff(COLOR_PAIR(2));
    
    
}

void imprime_linha_comandos (char *comando) {
    int i=0, X=4, Y=28; // (Y,X) iniciais 
    int largura = TAMX-6,altura = 4;
    char str[50];
    char chmoldura = '*';
    
    move(Y,X);
    for(i=0;i<X+largura;i++)
        addch(chmoldura);
    
    move(Y+2,X+4);
    printw(">> ");
    scanw("%[^\n]s%",str);
    strcpy(comando,str);
    
    move(Y+2,X+7);
    printw("                                                               ");
    move(Y+2,X+7);

    
}

bool imprime_login_incorreto(){
  int i=0,j, X=4, Y=17,deslocamentox=25; // (Y,X) iniciais 
    char chmoldura = ' ';
    bool tentar_ou_sair;
    char opcao = 'x';
    
    for(j=1; j<8 ;j++){
    move(Y+j,X-1);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    move(Y+3,X+deslocamentox);
    printw("|1| Tentar Novamente");
    
    move(Y+5,X+deslocamentox);
    printw("|2| Sair");
    
    do{
        move(Y+1,X+17);
        printw("Username ou Password invalidos: | |");
        move(Y+1,X+50);
        opcao=(char)getch(); // --------------------------ERRO NAO LE O CARACTER BEM
    }while(opcao != '1' && opcao !='2');
    
     if(opcao == '1'){
        tentar_ou_sair = true;
    }
    else{
        tentar_ou_sair = false;
    }
    
    return tentar_ou_sair;
}

void imprime_login_janela1(char *username,char *password){
    int i=0,j, X=4, Y=17,deslocamentox=25; // (Y,X) iniciais 
    char chmoldura = ' ';
    char nome[10],pass[10];
    
     for(j=1; j<8 ;j++){ // limpa ecra
        move(Y+j,X-1);
        for(i=0;i<TAMX;i++)
            addch(chmoldura);
    }
    
    move(Y+1,X+17);
        printw("INTRODUZA O SEU USERNAME E PASSWORD:");
        move(Y+1,X+50);

    move(Y+3,X+deslocamentox);
    printw("USERNAME: ");
    scanw("%[^\n]s%",username);
   
    
    move(Y+5,X+deslocamentox);
    printw("PASSWORD: ");
    scanw("%[^\n]s%",password);
    sleep(0.5);
    

}

void limpa_linha_comandos(){
    int i=0,j, X=4, Y=28; // (Y,X) iniciais 
    char chmoldura = ' ';
    
    for(j=1; j<4 ;j++){
    move(Y+j,X-1);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    move(Y+2,X+4);
    printw(">> ");
    
}

void limpa_janela_lvl1(){ // LIMPA janela excepto moldura e linha de comandos
    int i=0,j, X=3, Y=1; // (Y,X) iniciais 
    char chmoldura = ' ';
    
    for(j=1; j<TAMY-4 ;j++){
    move(Y+j,X);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    
}

void limpa_janela_lvl2(){ // limpa janela excepto moldura
    int i=0,j, X=3, Y=1; // (Y,X) iniciais 
    char chmoldura = ' ';
    
    for(j=1; j<TAMY;j++){
    move(Y+j,X);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    
}

void limpa_campo(){ // limpa janela excepto moldura
    int i=0,j, X=3, Y=4; // (Y,X) iniciais 
    char chmoldura = ' ';
    
    for(j=1; j<Y+18;j++){
    move(Y+j,X);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    
}

void limpa_resultado(){ // limpa janela excepto moldura
    int i=0,j, X=3, Y=1; // (Y,X) iniciais 
    char chmoldura = ' ';
    
    for(j=1; j<Y+3;j++){
    move(Y+j,X);
    for(i=0;i<TAMX;i++)
        addch(chmoldura);
    }
    
    
}

void imprime_janela_realizado_por(){
   
    int i=0,Y=7,X=24;
    char opcao;
    
    limpa_janela_lvl2();
    move(Y-1,X-6);
    for(i=0;i<40;i++)
        addch('-');
    move(Y,X-4);
    for(i=0;i<36;i++)
        addch('-');
    
    move(Y+2,X);
    printw("JOGO DE FUTEBOL MULTIJOGADOR");
    move(Y+4,X+6);
    printw("Trabalho Pratico");
    move(Y+5,X+8);
    printw("SO 2016/2017");
    
    move(Y+7,X-4);
    for(i=0;i<36;i++)
        addch('-');
    move(Y+8,X-6);
    for(i=0;i<40;i++)
        addch('-');
    
    move(Y+12,X+3);
    printw("Trabalho Realizador Por:");
    
    move(Y+14,X+2);
    printw("Kristhian Simoes nº21230192");
    
    move(Y+16,X+4);
    printw("Sergio Cruz nº21230463");
    
    move(Y+18,X+14);
    for(i=0;i < 3;i++){
        addch('.');
        //sleep(1); // ERRO NAO ESTA A FAZER O SLEEP COMO SE PRETENDE
    }
    
    move(Y+19,X+15);  
     
  
}

void imprime_janela_espera_de_jogo(){
    
    int Y=9,X=5;

    limpa_janela_lvl2();
    
    move(Y,X+27);
    printw("DE MOMENTO");
    move(Y+2,X+19);
    printw("NAO EXISTEM JOGOS A DECORRER");
    move(Y+4,X+24);
    printw("POR FAVOR AGUARDE");
    move(Y+6,X+31);
    printw("...");
    
    move(Y+17,X+14);
    printw("[DIGITAR 'SAIR' PARA ABANDONAR O JOGO]");
    
    //imprime_linha_comandos();
     
}

void imprime_janela_tem_jogo_disponivel(){
    
    int Y=9,X=5, i ,YY=7,XX=24; // XX YY é da moldura extra

    limpa_janela_lvl2();
    
    move(YY-1,XX-6);
    for(i=0;i<40;i++)
        addch('-');
    move(YY,XX-4);
    for(i=0;i<36;i++)
        addch('-');
    
    move(Y,X+24);
    printw("JOGO A DECORRER!");
    move(Y+2,X+23);
    printw("DIGITE 'JOGAR' PARA");
    move(Y+4,X+25);
    printw("ENTRAR NO JOGO!");
   // move(Y+6,X+31);
   // printw("...");
    
      
    move(YY+8,XX-4);
    for(i=0;i<36;i++)
        addch('-');
    move(YY+9,XX-6);
    for(i=0;i<40;i++)
        addch('-');
    
    move(Y+17,X+14);
    printw("[DIGITAR 'SAIR' PARA ABANDONAR O JOGO]");
    
    //imprime_linha_comandos();
     
}

void admin_imprime_janela_sem_jogos_a_decorrer(){
    
    int Y=9,X=5;

    limpa_janela_lvl2();
    
    move(Y,X+19);
    printw("NAO EXISTEM JOGOS A DECORRER");
    move(Y+2,X+19);
    printw("PARA INICIAR UM DIGITE 'START'");
    move(Y+4,X+16);
    printw("SEGUIDO DO TEMPO DE DURACAO DO JOGO");
    //move(Y+6,X+31);
    //printw("...");
    
    move(Y+17,X+12);
    printw("[DIGITAR 'AJUDA' PARA VER LISTA DE COMANDOS]");
    
   // imprime_linha_comandos();
     
}

void admin_imprime_janela_com_jogos_a_decorrer(){
    
    int Y=9,X=5, i ,YY=7,XX=24; // XX YY é da moldura extra
    limpa_janela_lvl2();
    
    move(YY-1,XX-6);
    for(i=0;i<40;i++)
        addch('-');
    move(YY,XX-4);
    for(i=0;i<36;i++)
        addch('-');
    
    move(Y,X+24);
    printw("JOGO A DECORRER!");
    move(Y+2,X+23);
    printw("DIGITE 'ENTRAR' PARA");
    move(Y+4,X+27);
    printw("VER O JOGO!");
   // move(Y+6,X+31);
   // printw("...");
    
      
    move(YY+8,XX-4);
    for(i=0;i<36;i++)
        addch('-');
    move(YY+9,XX-6);
    for(i=0;i<40;i++)
        addch('-');
    
    move(Y+17,X+12);
    printw("[DIGITAR 'AJUDA' PARA VER LISTA DE COMANDOS]");
    
    //imprime_linha_comandos();
     
}

void imprime_precione_tecla_para_sair(){
    
    int Y=9,X=5;
    move(Y+17,X+12);
    printw("[PRECIONE A TECLA 'Q' PARA SAIR DA PARTIDA]");
}

void imprime_cartao_vermelho(){
    int Y=9,X=5;
    
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    attron(COLOR_PAIR(1));
    move(Y+19,X+16);
    printw("ACABOU DE RECEBER UM CARTAO VERMELHO!"); 
    move(Y+20,X+22);
    printw("VAI SER EXPULSO DA PARTIDA!"); 
    move(Y+21,X+32);
    attroff(COLOR_PAIR(1));
    printw("...");  
}

void imprime_servidor_encerrado_em_jogo(){
    int Y=9,X=5;
    
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    attron(COLOR_PAIR(1));
    move(Y+19,X+22);
    printw("O SERVIDOR FOI DESLIGADO!"); 
    move(Y+20,X+24);
    printw("O JOGO SERA ENCERRADO"); 
    move(Y+21,X+32);
    attroff(COLOR_PAIR(1));
    printw("...");  
}

void admin_user_adicionado(bool adicionado, char * username  ){
    
    int Y=9,X=5;
    
    
    
    if(adicionado == true){
        move(Y+20,X+10);
        printw("UTILIZADOR [%s] FOI ADICIONADO COM SUCESSO",username);
    }
    else{
        move(Y+20,X+10);
        printw("[ERRO] UTILIZADOR NAO FOI ADICIONADO COM SUCESSO");
    }
}

void admin_cartao_vermelho(bool sucesso, char * username){
    
    int Y=9,X=5;
    
    
    
    if(sucesso == true){
        move(Y+20,X+11);
        printw("CARTAO VERMELHO ATRIBUIDO AO JOGADOR [%s]",username);
    }
    else{
        move(Y+20,X+16);
        printw("[ERRO] AO ATRIBUIR UM CARTAO VERMELHO");
    }
}

void imprime_tempo(int tempo_atual){

    int i=0,meio_campo = 25, Y = 2, X=45;

    move(Y,X+meio_campo-4);
    for(i=0;i<7;i++)
        addch('-');
    
    move(Y+2,X+meio_campo-4);
    for(i=0;i<7;i++)
     addch('-');
    
    move(Y+1,X+meio_campo-4);
    addch('|');
    
    move(Y+1,X+meio_campo+2);
    addch('|');
   
    move(Y+1,X+meio_campo-3);
    printw(" %d ",tempo_atual);
    
    
}