#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define TAM_POLINOMIO 1000

// ------------------------------ ESTRUTURAS ----------------------------------------------

typedef struct NoLista{
    int expoente;
    char variavel;
    struct NoLista *lprox;
    struct NoLista *lant;
}NoLista, *Lista;

typedef struct termo{
int invertido;
char indicec[100];
int denominador;
int indice; // Guarda o indice, exemplo: 2x, ele guarda o 2
char polinomio[TAM_POLINOMIO]; // Guarda uma string contendo os termos do monomio, exemplo 2x^2y^2, ele guarda a string "x^2y^2"
char operador;
struct termo *prox;
struct termo *ant;// Aponta para a proxima estrutura e a anterior da fila
NoLista *literais; // Aponta para uma fila com os literais
int sinal;
}expressao, *pExpressao;

// ------------------------- FUNÇÕES PARA PREENCHER AS ESTRUTURAS ------------------------------------------

void zerastring(char c[]){ //PREENCHE A STRING COM \0 PARA EVITAR LIXO DE MEMORIA
int i;
    for(i=0; i<=TAM_POLINOMIO; i++){
        c[i]='\0';
    }
}

int criaindice(char a[], int n){ // TRANSFORMA UM NUMERO REPRESENTADO EM CHAR PARA INT
int i, j=0, r=0;
    for(i=n-1; i>=0; i--){
        r=r+(a[i]-48)*(pow(10,j)+0.1);
        j++;
    }
    if (n==0) return 1;
    return r;
}

void consertaexpoente(Lista *l){ // x^nx^n VIRA x^n+1
Lista aux2, aux=*l;
int i;
    while(aux!=NULL){
    aux2=aux->lprox;
        while ((aux2)&&(aux->variavel==aux2->variavel)){
            aux->expoente = aux->expoente+aux2->expoente;
            aux->lprox=aux2->lprox;
            if (aux2->lprox!=NULL) (aux2->lprox)->lant=aux;
            free(aux2);
            aux2=aux->lprox;
        }
        aux=aux->lprox;
    }
}

int contatermos(char c[]){ // CONTA QUANTOS OPERANDOS A STRING TEM
int cont=0, i;
char v[]="+-*";
    for(i=0; i<strlen(c); i++){
        if ((c[i]=='+')||(c[i]=='*')||(c[i]=='-')){
          if((i!=0)&&(strchr(v, c[i-1])==NULL))
            cont++;
        }
    }
return cont+1;
}

void limpavariaveis(Lista *l){ // LIMPA A LISTA DE VARIAVEIS (PARA EXCLUIR O MONOMIO)
Lista aux;
aux=*l;
if(aux){
    if (aux->lprox!=NULL){
        limpavariaveis(&(aux->lprox));
    }
    free(aux);
    }
}

void excluimonomio(pExpressao *p, pExpressao k){ //EXCLUI UM MONOMIO DE UM POLINOMIO
pExpressao aux= *p;
if (aux){
    while (aux->prox!=NULL){
        if (aux==k) break;
        aux=aux->prox;
    }
    if (aux->ant==NULL){
        *p=aux->prox;
        if(aux->prox) (*p)->ant=NULL;
    }else if (aux->prox==NULL){
        (aux->ant)->prox=aux->prox;
    }else{
        (aux->ant)->prox=aux->prox;
        (aux->prox)->ant=aux->ant;
    }
    aux->prox=NULL;
    aux->ant=NULL;
    limpavariaveis(&(aux->literais));
    free(aux);
}
}

void InsereLista(Lista *x, Lista n){ //INSERE DE FORMA ORDENADA UMA VARIAVEL NA LISTA
Lista ant=NULL, dep=*x;

if (dep==NULL){
    *x=n;
    n->lprox=NULL;
    n->lant=NULL;
}else{
    while (dep!=NULL){
        if (n->variavel<dep->variavel)break;
        ant=dep;
        dep=dep->lprox;
    }
    if (ant==NULL){
        n->lprox=*x;
        n->lant=NULL;
        *x=n;
    }else{
        n->lprox=ant->lprox;
        ant->lprox=n;
        n->lant=ant;
    }
}
}

pExpressao criacelulas(int n){//cria o espaço para comportar a expressão
int i, cont=0;
pExpressao r, origem, aux, aux2;
    for(i=0; i<n; i++){
        if (i==0){
            origem=(pExpressao)malloc(sizeof(expressao));
            if(origem==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
            aux=origem;
            aux->prox=NULL;
            aux->ant=NULL;
            aux->literais=NULL;
            aux->sinal=0;
            aux->indice=0;
            aux->denominador=1;
            zerastring(aux->polinomio);
        }else{
            aux2=(pExpressao)malloc(sizeof(expressao));
            if(aux2==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
            zerastring(aux2->polinomio);
            aux->prox=aux2;
            aux2->prox=NULL;
            aux2->ant=aux;
            aux2->literais=NULL;
            aux2->sinal=0;
            aux2->denominador=1;
            aux=aux2;
        }
    }
    r=origem;
    return r;
}

pExpressao criadenominador(){
pExpressao aux;
    aux=(pExpressao)malloc(sizeof(expressao));
    if(!aux) exit(1);
    aux->prox=NULL;
    aux->ant=NULL;
    aux->literais=NULL;
    aux->sinal=0;
    aux->indice=1;
    aux->denominador=1;
    zerastring(aux->polinomio);
    aux->operador='\0';
    return aux;
}

void crialista(char c[1000], Lista *p, pExpressao x){ //COM BASE NA AREA "POLINOMIO" DA ESTRUTURA CRIA UMA LISTA DUPLAMENTE ENCADEADA COM UM INT PARA O EXPOENTE E UM CHAR PARA A VARIAVEL
int i=0, j=0, k, flag=1, flag2=0;
char *pontchar, auxiliar[100]="", auxiliar2[100]="", v[11]="0123456789";
Lista aux=*p, novoNo;
    for(i=0; c[i]!='\0'; ){
        if (c[i]!='^'){
            if (strchr(v, c[i])==NULL){
                novoNo=(Lista)malloc(sizeof(NoLista));
                novoNo->variavel=c[i];
                i=i+2;
                if(c[i]=='-'){
                  flag2=1;
                  i++;
                }
                pontchar=strchr(v, c[i]);
                if (pontchar){
                    while((pontchar)&&(c[i]!='\0')){     //"x^32y^23c^3defblaus^51"
                        auxiliar[j]=c[i];
                        j++;
                        i++;
                        pontchar=strchr(v, c[i]);
                    }
                }
                if (flag2) novoNo->expoente=criaindice(auxiliar, j) * -1;
                else novoNo->expoente=criaindice(auxiliar, j);
                if (novoNo->expoente!=0) InsereLista(p, novoNo);
                else strcpy(x->polinomio, "\0");
                for(k=0; k<=j; k++){
                    auxiliar[k]='\0';
                }
                j=0;
            }

        } else i++;
    }
}



int contalgarismos(int x){ // CONTA QUANTOS ALGARISMOS UM NUMERO TEM
int cont=0;
if(x<0) x=x*-1;
    while((x/10>0)||(x%10>0)){
        x=x/10;
        cont++;
    }
return cont;
}

void numpravet(int n, int *v){ // PASSA CADA ALGARISMO DE UM NUMERO PARA UM ESPAÇO DO VETOR
int i, k, cont=0;
    for(i=contalgarismos(n)-1; i>=0; i--){
        v[i]=n%10;
        n=n/10;
    }
}

void numprachar(int n, char c[]){
int v[100], k, i=0;
numpravet(n, v);
  for(k=0; k<contalgarismos(n); k++){
        c[i]=v[k]+'0';
        i++;
  }
  c[i]='\0';
}

void update(pExpressao *p){ // GERA UMA STRING A PARTIR DA LISTA DE VARIAVEIS E JOGA NA AREA POLINOMIO
int i=0, k=0, j=0, v[100], z, flag;
char m[1000]="\0";
Lista LIST;
pExpressao EXP = *p;
    for(LIST=(*p)->literais;LIST!=NULL;LIST=LIST->lprox){
        flag=1;
        m[i]=LIST->variavel;
        i++;
        m[i]='^';
        i++;
        if(LIST->expoente<0){
            flag=0;
            z=LIST->expoente * -1;
            m[i]='-';
            i++;
        }
        if (flag) numpravet(LIST->expoente, v);
        else numpravet(z, v);
        for(k=0; k<contalgarismos(LIST->expoente); k++){
            m[i]=v[k]+'0';
            i++;
        }
    strcpy(EXP->polinomio, m);
    }
}




void entrada(char c[], pExpressao *x){ // COLOCA INDICE E POLINOMIO NOS ESPAÇOS ALOCADOS
int i, j=0, k=0, l=0, flag=1;
char v[11], in[11]="\0";
strcpy(v, "0123456789");
pExpressao forfree, aux=*x;
    for (i=0; ((i<TAM_POLINOMIO)&&(c[i]!='\0')); i++){
        flag=1;
        if (!aux) break;
        if ((c[i]=='-')&&(aux->sinal==0)&&(k==0))/*caso haja "-1-1"*/{
          aux->sinal=1;
          i++;
        }
        if ((strchr(v, c[i]))&&(flag)){
                in[k]=c[i];
                k++;
                l++;
        }else{
            if (flag){
                aux->indice = criaindice(in, k);
                if (aux->sinal==1) aux->indice= (aux->indice)*(-1);
                flag=0;
                k=0;
            }
            while((c[i]!='+')&&(c[i]!='*')&&(c[i]!='-')&&(c[i]!='\0')){ // COPIA TUDO ATE ACHAR UM OPERADOR
                aux->polinomio[j]=c[i];
                if ((c[i]!='^')&&(strchr(v, c[i])==NULL)&&(c[i+1]!='^')){//COLOCA ^1 NOS QUE ESTÃO SEM EXPOENTE
                    aux->polinomio[j+1]='^';
                    aux->polinomio[j+2]='1';
                    j=j+2;
               }
                i++;
                j++;
            }
            j=0;
            aux->operador=c[i];
            l=0;
            if ((aux->ant)&&((aux->ant)->operador=='-')){
                    (aux->ant)->operador='+';
                    aux->indice=(aux->indice)*(-1);
                    aux=aux->prox;
            }else
            aux=aux->prox;
        }
    }
    if((flag)){
      aux->indice=criaindice(in, k);
      if (aux->sinal==1) aux->indice= (aux->indice)*(-1);
    }if ((aux)&&(aux->ant)&&((aux->ant)->operador=='-')){
      (aux->ant)->operador='+';
      aux->indice=(aux->indice)*(-1);
      aux=aux->prox;
    }
}

void listas(pExpressao *p){
pExpressao aux=*p;
    while(aux!=NULL){
        crialista(aux->polinomio, &(aux->literais), aux);
        consertaexpoente(&(aux->literais));
        aux=aux->prox;
    }
}

void atualiza(pExpressao *p){
pExpressao aux=*p;
    while(aux!=NULL){
        consertaexpoente(&(aux->literais));
        update(&aux);
        aux=aux->prox;
    }
}

void printaexpressao(pExpressao p){
pExpressao aux;
  if (p==NULL) printf("0 ");
  else{
    for (aux=p; aux->prox!=NULL; aux=aux->prox){
        if(aux->indice==1){
          if (strcmp(aux->polinomio, "\0")==0){
            printf("%d %c ", aux->indice, aux->operador);
          }else{
            printf("%s %c ", aux->polinomio, aux->operador);
          }
        }else if(aux->indice==-1){
          if (strcmp(aux->polinomio, "\0")==0){
            printf("-%d %c ", aux->indice, aux->operador);
          }else{
            printf("-%s %c ", aux->polinomio, aux->operador);
          }
        }else{
            if (strcmp(aux->polinomio, "\0")==0){
                printf("%d %c ", aux->indice, aux->operador);
            }else{
                printf("%d%s %c ", aux->indice, aux->polinomio, aux->operador);
            }
        }
    }
    if(aux->indice==1){
          if (strcmp(aux->polinomio, "\0")==0){
            printf("%d", aux->indice);
          }else{
            printf("%s", aux->polinomio);
          }
    }else if(aux->indice==-1){
          if (strcmp(aux->polinomio, "\0")==0){
            printf("-%d", aux->indice);
          }else{
            printf("-%s", aux->polinomio);
          }
    }else{
          if (strcmp(aux->polinomio, "\0")==0){
            printf("%d", aux->indice);
          }else{
            printf("%d%s", aux->indice, aux->polinomio);
          }
    }
  }
}

void excluipolinomio (pExpressao *p){
pExpressao aux=*p, aux2;
    while (*p){
    aux2 = (*p)->prox;
    aux->prox=NULL;
    excluimonomio(p, aux);
    *p=aux2;
    aux=*p;
    }
}

void removezero (pExpressao *p){
pExpressao aux=*p, aux2;
int flag=0;
    while (aux){
      aux2 = aux->prox;
      if (aux->indice==0){
        if (aux==*p) flag=1;
        if (aux->ant) (aux->ant)->operador = aux->operador;
        excluimonomio(p, aux);
      }
      if (flag){
        *p=aux2;
        flag=0;
      }
      aux=aux2;
  }
  atualiza(p);
}


// ---------------------- FUNÇÕES PARA OPERAR AS ESTRUTURAS ------------------------------
void multiplica (pExpressao *p, pExpressao *q, pExpressao r){
Lista aux=r->literais, aux2;
    (*q)->indice= (*q)->indice * r->indice;
    (*q)->operador=r->operador;
    while (aux!=NULL){
        aux2=aux->lprox;
        aux->lprox=NULL;
        InsereLista(&((*q)->literais), aux);
        aux=aux2;
    }
    r->literais=NULL;
}

void multiplica2 (pExpressao *p, pExpressao *q, pExpressao r){//
Lista aux=r->literais, aux2;
    (*q)->indice= (*q)->indice * r->indice;
    while (aux!=NULL){
        aux2=aux->lprox;
        aux->lprox=NULL;
        InsereLista(&((*q)->literais), aux);
        aux=aux2;
    }
    r->literais=NULL;
    atualiza(p);
}

int soma (pExpressao *p, pExpressao *q, pExpressao r){
Lista aux=r->literais;
    if (strcmp((*q)->polinomio, r->polinomio)!=0) return 0;
    else{
        if ((*q)->operador == '+') (*q)->indice = (*q)->indice + r->indice;
        else if ((*q)->operador == '-') (*q)->indice = (*q)->indice + r->indice;
        if(r->operador!='\0') (*q)->operador=r->operador;
        else (r->ant)->operador=r->operador;
        return 1;
    }
}

void termosemelhante(pExpressao *p){ //(2x+3y+x*x+4x = x^2+6x+3y)
pExpressao aux, aux2, v;
aux=*p;
while ((aux)&&(aux->prox != NULL)){
    while (aux->operador=='*'){
        multiplica(p, &aux, aux->prox);
        excluimonomio(p, aux->prox);
    }
    aux=aux->prox;
}
aux=*p;
    while (aux!=NULL){
        aux2=aux->prox;
        while (aux2!=NULL){
            v=aux2->prox;
            if(soma(p, &aux, aux2)) excluimonomio(p, aux2);
            aux2=v;
        }
        aux=aux->prox;
    }
    atualiza(p);
}

void remove_espaco(char c[]){
char v[]="0123456789";
int i, j=0;
char nova[100], *r;
    for (i=0; c[i]!='\0'; i++){
        if (c[i]!=' '){
            nova[j]=c[i];
            j++;
        }
    }
    nova[j]='\0';
    if (strcmp(nova, "FIM")!=0){
            nova[j]='*';
            j=j+1;
            nova[j]='1';
            j=j+1;
            nova[j]='\0';
    }
    strcpy(c, nova);
}

int verificad(pExpressao p){ //VERIFICA SE O POLINOMIO É COMPOSTO APENAS PELA CONSTANTE 1
  if(p){
    if ((p->indice==1)&&(p->prox==NULL)&&(p->ant==NULL)&&(strcmp(p->polinomio, "\0")==0)) return 0;
    return 1;
  }
}

void inverte(pExpressao p[2]){
pExpressao aux;
    aux=p[0];
    if (aux->indice!=0){
      p[0]=p[1];
      p[1]=aux;
    }
}

// --------------------------------------------------------------------------------------
// ----------------------------------------------- OPERAÇÕES ENTRE POLINOMIOS -----------

pExpressao copia(pExpressao p){
int cont=0;
pExpressao aux, aux2, aux3;
    for(aux=p; aux!=NULL; aux=aux->prox){
        cont++;
    }
    aux=p;
    aux2=criacelulas(cont);
    for(aux3=aux2; aux3!=NULL; aux3=aux3->prox, aux=aux->prox){
        aux3->indice=aux->indice;
        strcpy(aux3->polinomio, aux->polinomio);
        aux3->sinal=aux->sinal;
        aux3->operador=aux->operador;
    }
    listas(&aux2);
    atualiza(&aux2);
    return aux2;
}

pExpressao fim(pExpressao p){
pExpressao aux=p;
    while(aux->prox!=NULL) aux=aux->prox;
    return aux;
}

pExpressao salto(int x, pExpressao p){
int i;
    for(i=0; i<x; i++){
        if(p) p=p->prox;
    }
    return p;
}


void som(pExpressao *p, pExpressao *q){
  pExpressao aux;
  aux=fim(*p);
  aux->operador='+';
  aux->prox=*q;
  *q=NULL;
  atualiza(p);
}

void mul(pExpressao *p, pExpressao *q){ //multiplica 2 polinomios
pExpressao aux, aux2, v, auxiliar, k, cp1, cp2, z;
int i=0, flag=1;
  for(v=*q; v!=NULL; v=v->prox){
    for (cp1=copia(*p), aux=cp1; aux!=NULL; aux=aux->prox){
        z=copia(*q);
        aux2=salto(i, z);
        multiplica2(&cp1, &aux, aux2);
        excluipolinomio(&z);
    }
    if (flag){
        cp2=cp1;
        flag=0;
    }else{
        k->prox=cp1;
        k->operador='+';
        atualiza(&cp2);
    }
    i++;
    k=fim(cp2);
  }
  excluipolinomio(q);
  excluipolinomio(p);
  *p=cp2;
}

void multp(pExpressao p[2], pExpressao q[2]){
    mul(&p[0], &q[0]);
    mul(&p[1], &q[1]);
}

void divp(pExpressao p[2], pExpressao q[2]){
  inverte(q);
  multp(p, q);
}

void sub(pExpressao *p, pExpressao *q){
pExpressao d;
  d=criacelulas(1);
  d->indice=-1;
  d->sinal=1;
  strcpy(d->polinomio, "\0");
  mul(q, &d);
  som(p, q);
}

void subp(pExpressao p[2], pExpressao q[2]){
pExpressao aux, aux2, expr_1[2], expr_2[2];

  aux=copia(p[1]);
  aux2=copia(q[1]);

  mul(&p[0], &aux2);
  mul(&q[0], &aux);

  sub(&p[0], &q[0]);
  mul(&p[1], &q[1]);

  atualiza(&p[0]);
  atualiza(&p[1]);

}

void somp(pExpressao p[2], pExpressao q[2]){
pExpressao aux, aux2, expr_1[2], expr_2[2];
if (q[0]->indice!=0){
  aux=copia(p[1]);
  aux2=copia(q[1]);

  mul(&p[0], &aux2);
  mul(&q[0], &aux);

  som(&p[0], &q[0]);
  mul(&p[1], &q[1]);
}
}

//void div(pExpressao *p, pExpressao *q);
//----------------------- GENESIS -------------------------------------------------------
void cria(pExpressao p[2], char s[]){
    fflush(stdin);
    remove_espaco(s);
    fflush(stdin);
    p[0]=criacelulas(contatermos(s));
    p[1]=criadenominador();
    entrada(s, &p[0]);
    listas(&p[0]);
    atualiza(&p[0]);
    termosemelhante(&p[0]);
    removezero(&p[0]);
}

void clean(pExpressao *p){
pExpressao aux=copia(*p);
free(*p);
*p=aux;
aux=NULL;
}

void refresh(pExpressao p[2]){
  clean(&p[0]);
  termosemelhante(&p[0]);
  removezero(&p[0]);
  clean(&p[1]);
  termosemelhante(&p[1]);
  removezero(&p[1]);
}

void printapolinomio(pExpressao p[2]){
    if ((verificad(p[1]))){
        printf("(");
        printaexpressao(p[0]);
        printf(")/(");
        printaexpressao(p[1]);
        printf(")");
    }
    else{
        printaexpressao(p[0]);
    }
}

// ----------------------------- MENU ---------------------------------------------------------

void menu_altera_Acumulador(pExpressao p[2], int i){
  system("cls");
  printf("A%d: ", i);
  printapolinomio(p);
  printf("\n");
  printf("\n");
  printf("                            SELECIONE A OPCAO:\n");
  printf("\n");
  printf("                          [1] Sobrescrever                       \n");
  printf("                          [2] Somar\n");
  printf("                          [3] Subtrair\n");
  printf("                          [4] Multiplicar                       \n");
  printf("                          [5] Dividir\n");
  printf("                          [6] Inverter\n");
  printf("                          [ ] Retornar\n");
  printf("\n");
}

void OP_altera_Acumulador(pExpressao p[2], int i){
char OP;
pExpressao aux[2];
char s[TAM_POLINOMIO];

  OP=getch();
  while (OP!=' '){

    if (OP=='1'){
      excluipolinomio(&p[0]);
      excluipolinomio(&p[1]);
      printf("Digite a expressao: ");
      scanf(" %[^\n]s", s);
      fflush(stdin);
      cria(p, s);
      menu_altera_Acumulador(p, i);

    }else if(OP=='2'){
      printf("Digite a expressao que você deseja somar: ");
      scanf(" %[^\n]s", s);
      fflush(stdin);
      cria(aux, s);
      somp(p, aux);
      refresh(p);
      menu_altera_Acumulador(p, i);
    }else if(OP=='3'){
      printf("Digite a expressao que você deseja subtrair: ");
      scanf(" %[^\n]s", s);
      fflush(stdin);
      cria(aux, s);
      subp(p, aux);
      refresh(p);
      menu_altera_Acumulador(p, i);

    }else if(OP=='4'){
      printf("Digite a expressao que você deseja multiplicar: ");
      scanf(" %[^\n]s", s);
      fflush(stdin);
      cria(aux, s);
      multp(p, aux);
      refresh(p);
      menu_altera_Acumulador(p, i);

    }else if(OP=='5'){
      printf("Digite a expressao que você deseja dividir: ");
      scanf(" %[^\n]s", s);
      fflush(stdin);
      cria(aux, s);
      divp(p, aux);
      refresh(p);
      menu_altera_Acumulador(p, i);

    }else if(OP=='6'){
      inverte(p);
      menu_altera_Acumulador(p, i);
    }
  OP=getch();
  }
}

void menu_opera_Acumuladores(pExpressao p[2], pExpressao q[2]){
  system("cls");
  printf("A1: ");
  printapolinomio(p);
  printf("\n");
  printf("\n");
  printf("A2: ");
  printapolinomio(q);
  printf("\n");
  printf("\n");
  printf("                            SELECIONE A OPCAO:\n");
  printf("\n");
  printf("                          [1] Somar\n");
  printf("                          [2] Subtrair\n");
  printf("                          [3] Multiplicar                       \n");
  printf("                          [4] Dividir\n");
  printf("                          [5] Inverter acumuladores                            \n");
  printf("                          [6] A1=A2                            \n");
  printf("                          [7] A2=A1                            \n");
  printf("                          [ ] Retornar\n");
  printf("\n");
}

void OP_opera_Acumuladores(pExpressao p[2], pExpressao q[2]){
char OP;
pExpressao aux[2];
char s[TAM_POLINOMIO];

  OP=getch();
  while (OP!=' '){

    if (OP=='1'){
        aux[0]=copia(q[0]);
        aux[1]=copia(q[1]);
        somp(p,aux);
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }else if(OP=='2'){
        aux[0]=copia(q[0]);
        aux[1]=copia(q[1]);
        subp(p,aux);
        refresh(p);
        menu_opera_Acumuladores(p, q);
    }else if(OP=='3'){
        aux[0]=copia(q[0]);
        aux[1]=copia(q[1]);
        multp(p,aux);
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }else if(OP=='4'){
        aux[0]=copia(q[0]);
        aux[1]=copia(q[1]);
        divp(p,aux);
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }else if(OP=='5'){
        aux[0]=q[0];
        aux[1]=q[1];
        q[0]=p[0];
        q[1]=p[1];
        p[0]=aux[0];
        p[1]=aux[1];
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }else if(OP=='6'){
        p[0]=copia(q[0]);
        p[1]=copia(q[1]);
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }else if(OP=='7'){
        q[0]=copia(p[0]);
        q[1]=copia(p[1]);
        refresh(p);
        menu_opera_Acumuladores(p, q);

    }
  OP=getch();
  }
}

void menu_inicial(pExpressao p[2], pExpressao q[2]){
  system("cls");
  //textcolor(3);
  printf("==============================================================================\n");
  printf("                            CALCULADORA DE POLINOMIOS                        \n");
  printf("==============================================================================\n");
  printf("\n");
  printf("A1: ");
  printapolinomio(p);
  printf("\n");
  printf("\n");
  printf("A2: ");
  printapolinomio(q);
  printf("\n");
  printf("\n");
  printf("                            DIGITE A OPCAO:\n");
  printf("\n");
  printf("                          [1] Alterar acumulador 1                       \n");
  printf("                          [2] Alterar acumulador 2                       \n");
  printf("                          [3] Operacao entre acumuladores                            \n");
  printf("                          [ ] Sair do sistema                            \n");
  printf("\n");
}

void OP_inicial(pExpressao p[2], pExpressao q[2]){
char OP;
int i;
pExpressao aux[2];
char s[TAM_POLINOMIO];

  menu_inicial(p,q);
  OP=getch();

  while (OP!=' '){
    if (OP=='1'){
        menu_altera_Acumulador(p, 1);
        OP_altera_Acumulador(p, 1);
    }else if (OP=='2'){
        menu_altera_Acumulador(q, 2);
        OP_altera_Acumulador(q, 2);
    }else if (OP=='3'){
        menu_opera_Acumuladores(p,q);
        OP_opera_Acumuladores(p,q);
    }
    menu_inicial(p,q);
    OP=getch();
  }
}

// ---------------------- MAIN ----------------------------------------------------------

int main(){
char s[TAM_POLINOMIO]="1", t[TAM_POLINOMIO]="1";
pExpressao x[2], y[2];
    cria(x, s);
    cria(y, t);
    OP_inicial(x,y);
return 0;
}
