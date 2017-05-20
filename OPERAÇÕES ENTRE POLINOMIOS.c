#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAM_POLINOMIO 1000

// ------------------------------ ESTRUTURAS ----------------------------------------------

typedef struct NoLista{
    int expoente;
    char variavel;
    struct NoLista *lprox;
    struct NoLista *lant;
}NoLista, *Lista;

typedef struct termo{
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
//            printf("entrei\n");
            aux->expoente = aux->expoente+aux2->expoente;
            aux->lprox=aux2->lprox;
            if (aux2->lprox!=NULL) (aux2->lprox)->lant=aux;
            free(aux2);
            aux2=aux->lprox;
//            printf("%d\n", aux->expoente);
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
//        printf("Entrei\n");
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
 //   printf("entrei1\n");
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
//        printf("entrei2\n");
    }else{
        n->lprox=ant->lprox;
        ant->lprox=n;
        n->lant=ant;
//        printf("entrei3\n");
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
            zerastring(aux->polinomio);
            //printf("%s\n", aux->polinomio);
  //          origem->indice=0;
        }else{
            aux2=(pExpressao)malloc(sizeof(expressao));
            if(aux2==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
            zerastring(aux2->polinomio);
//            printf("Entrou, %d \n", cont);
            aux->prox=aux2;
            aux2->prox=NULL;
            aux2->ant=aux;
            aux2->literais=NULL;
            aux2->sinal=0;
            aux2->indice=0;
            aux=aux2;
    //        aux2->indice=1;
        }
    }
//    printf("%d celulas criadas\n", cont);
    r=origem;
    return r;
}

void crialista(char c[1000], Lista *p, pExpressao x){ //COM BASE NA AREA "POLINOMIO" DA ESTRUTURA CRIA UMA LISTA DUPLAMENTE ENCADEADA COM UM INT PARA O EXPOENTE E UM CHAR PARA A VARIAVEL
int i=0, j=0, k, flag=1;
char *pontchar, auxiliar[100]="", auxiliar2[100]="", v[11]="0123456789";
Lista aux=*p, novoNo;
    for(i=0; c[i]!='\0'; ){
        if (c[i]!='^'){
            if (strchr(v, c[i])==NULL){
 //               printf("entrei\n");
                novoNo=(Lista)malloc(sizeof(NoLista));
                novoNo->variavel=c[i];
                //printf("%c\n", novoNo->variavel);
                i=i+2;
//                printf("%c\n", c[i]);
                pontchar=strchr(v, c[i]);
                if (pontchar){
                while((pontchar)&&(c[i]!='\0')){     //"x^32y^23c^3defblaus^51"
                    auxiliar[j]=c[i];
//                    printf("%c ", auxiliar[j]);
                    j++;
                    i++;
                    pontchar=strchr(v, c[i]);
                }
                }
//                printf("%s\n", auxiliar);
                novoNo->expoente=criaindice(auxiliar, j);
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
//        printf("%d ", v[i]);
//        printf("%d ", v[i]);
        n=n/10;
    }
}

void update(pExpressao *p){ // GERA UMA STRING A PARTIR DA LISTA DE VARIAVEIS E JOGA NA AREA POLINOMIO
int i=0, k=0, j=0, v[100], z;
char m[1000]="\0";
Lista LIST;
pExpressao EXP = *p;
for(LIST=(*p)->literais;LIST!=NULL;LIST=LIST->lprox){
    m[i]=LIST->variavel;
//    printf("%d", LIST->expoente);
//    printf("%c", m[i]);
    i++;
    m[i]='^';
//    printf("%c", m[i]);
    i++;
    numpravet(LIST->expoente, v);
    for(k=0; k<contalgarismos(LIST->expoente); k++){
        m[i]=v[k]+'0';
//        printf("%d ", v[k]);
        //m[i]=intochar(v[k]);
 //       printf("%c", m[i]);
        i++;
    }
    strcpy(EXP->polinomio, m);
}
//printf("%s", m);
}




void entrada(char c[], pExpressao *x){ // COLOCA INDICE E POLINOMIO NOS ESPAÇOS ALOCADOS
int i, j=0, k=0, l=0, flag=1;
char v[11], in[11]="\0";
strcpy(v, "0123456789");
pExpressao forfree, aux=*x;
    for (i=0; ((i<TAM_POLINOMIO)&&(c[i]!='\0')); i++){
//        zerastring(aux->polinomio);
        flag=1;
        if (!aux) break;
        if ((c[i]=='-')&&(aux->sinal==0)&&(k==0))/*caso haja "-1-1"*/{
          aux->sinal=1;
          i++;
        }
//        printf("%d\n",aux->indice);
        if ((strchr(v, c[i]))&&(flag)){
                //printf("%c\n", c[i]);
                in[k]=c[i];
                k++;
                l++;
        }else{
            //printf("%c\n", c[i]);
            if (flag){
                aux->indice = criaindice(in, k);
                if (aux->sinal==1) aux->indice= (aux->indice)*(-1);
                //printf("%d\n", aux->indice);
                flag=0;
                k=0;
//                printf("%c\n", c[i]);
            }
            //if (c[i])
            //printf("%c\n", c[i]);
            while((c[i]!='+')&&(c[i]!='*')&&(c[i]!='-')&&(c[i]!='\0')){ // COPIA TUDO ATE ACHAR UM OPERADOR
                aux->polinomio[j]=c[i];
                if ((c[i]!='^')&&(strchr(v, c[i])==NULL)&&(c[i+1]!='^')){//COLOCA ^1 NOS QUE ESTÃO SEM EXPOENTE
                    aux->polinomio[j+1]='^';
                    aux->polinomio[j+2]='1';
                    j=j+2;
               }//else{*/
                i++;
                j++;
                //}
            }
            j=0;
            aux->operador=c[i];
            l=0;
//            printf("Finalizei\n");
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
  else
    for (aux=p; aux!=NULL; aux=aux->prox){
        if(aux->indice==1){
          if (strcmp(aux->polinomio, "\0")==0){
            printf("%d %c ", aux->indice, aux->operador);
          }else{
            printf("%s %c ", aux->polinomio, aux->operador);
          }
        }else{
          if (strcmp(aux->polinomio, "\0")==0){
            printf("%d %c ", aux->indice, aux->operador);
          }else{
            printf("%d%s %c ", aux->indice, aux->polinomio, aux->operador);
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
//        aux->prox=NULL;
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
//    excluimonomio(p, r);
//    atualiza(p);
}

void multiplica2 (pExpressao *p, pExpressao *q, pExpressao r){
Lista aux=r->literais, aux2;
    (*q)->indice= (*q)->indice * r->indice;
//    (*q)->operador=r->operador;
    while (aux!=NULL){
        aux2=aux->lprox;
        aux->lprox=NULL;
        InsereLista(&((*q)->literais), aux);
        aux=aux2;
//        r->literais=aux2;
    }
    r->literais=NULL;
//    excluimonomio(p, r);
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
//        excluimonomio(p, r);
//        atualiza(p);
        return 1;
    }
}

void termosemelhante(pExpressao *p){
pExpressao aux, aux2, v;
aux=*p;
while ((aux)&&(aux->prox != NULL)){
    while (aux->operador=='*'){
        multiplica(p, &aux, aux->prox);
        excluimonomio(p, aux->prox);
//        printf("%s\n", aux->polinomio);
    }
    aux=aux->prox;
}
aux=*p;
    while (aux!=NULL){
        aux2=aux->prox;
        while (aux2!=NULL){
            v=aux2->prox;
            if(soma(p, &aux, aux2)) excluimonomio(p, aux2);
//            if (strcmp(aux->polinomio, aux2->polinomio)==0) excluimonomio(p, aux2);
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
    if (strcmp(nova, "FIM")!=0){ //SE FOR "FIM" NÃO MODIFIQUE
//        if (strchr(v, nova[j-1])==NULL){ //GARANTE QUE A FUNÇÃO ENCERRE COM UMA CONSTANTE PARA NÃO CRASHAR O WHILE DO MAIN
            nova[j]='*';
            j=j+1;
            nova[j]='1';
            j=j+1;
            nova[j]='\0';
 //       }
    }
    strcpy(c, nova);
//    printf("%s", c);
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
//termosemelhante(p);
atualiza(p);
//printaexpressao(*p);
//printf("\n");
}

void mul(pExpressao *p, pExpressao *q){ //FALTA TESTAAAAR
pExpressao aux, aux2, v, auxiliar, k, cp1, cp2, z;
int i=0, flag=1;
auxiliar=copia(*p);
  for(v=*q; v!=NULL; v=v->prox){
    for (cp1=copia(auxiliar), aux=cp1; aux!=NULL; aux=aux->prox){
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
//        termosemelhante(&cp2);
        atualiza(&cp2);
    }
    i++;
    k=fim(cp2);
  }
  excluipolinomio(q);
  excluipolinomio(p);
  *p=cp2;
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

// ---------------------- MAIN ----------------------------------------------------------

int main(){
char s[TAM_POLINOMIO]="x+1", t[TAM_POLINOMIO]="x-5";
int l;
pExpressao x, y, auxiliar;
    fflush(stdin);
    remove_espaco(s);
    fflush(stdin);
    x=criacelulas(contatermos(s));
    entrada(s, &x);
    listas(&x);
    atualiza(&x);
    termosemelhante(&x);
    removezero(&x);
    printaexpressao(x);
    printf("\n");

    fflush(stdin);
    remove_espaco(t);
    fflush(stdin);
    y=criacelulas(contatermos(t));
    entrada(t, &y);
    listas(&y);
    atualiza(&y);
    termosemelhante(&y);
    removezero(&y);
    printaexpressao(y);
    printf("\n");

    sub(&x, &y);
    auxiliar=x;
    x=copia(auxiliar);
    excluipolinomio(&auxiliar);
    termosemelhante(&x);
    removezero(&x);
    printaexpressao(x);

    return 0;
}
