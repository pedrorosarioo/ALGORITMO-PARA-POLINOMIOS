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
void zerastring(char c[]){
int i;
    for(i=0; i<=TAM_POLINOMIO; i++){
        c[i]='\0';
    }
}

int criaindice(char a[], int n){
int i, j=0, r=0;
    for(i=n-1; i>=0; i--){
        r=r+(a[i]-48)*(pow(10,j)+0.1);
        j++;
    }
    if (n==0) return 1;
    return r;
}

void consertaexpoente(Lista *l){
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

int contatermos(char c[]){
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

void limpavariaveis(Lista *l){
Lista aux;
aux=*l;
if(aux){
    if (aux->lprox!=NULL){
        limpavariaveis(&(aux->lprox));
    }
    free(aux);
    }
}

void excluimonomio(pExpressao *p, pExpressao k){
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

void InsereLista(Lista *x, Lista n){
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

void crialista(char c[1000], Lista *p, pExpressao x){
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



int contalgarismos(int x){
int cont=0;
    while((x/10>0)||(x%10>0)){
        x=x/10;
        cont++;
    }
return cont;
}

void numpravet(int n, int *v){
int i, k, cont=0;
    for(i=contalgarismos(n)-1; i>=0; i--){
        v[i]=n%10;
//        printf("%d ", v[i]);
//        printf("%d ", v[i]);
        n=n/10;
    }
}

void update(pExpressao *p){
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
    excluimonomio(p, r);
    atualiza(p);
}

void soma (pExpressao *p, pExpressao *q, pExpressao r){
Lista aux=r->literais;
    if (strcmp((*q)->polinomio, r->polinomio)==0){
        if ((*q)->operador == '+') (*q)->indice = (*q)->indice + r->indice;
        else if ((*q)->operador == '-') (*q)->indice = (*q)->indice + r->indice;
        if(r->operador!='\0') (*q)->operador=r->operador;
        else (r->ant)->operador=r->operador;
        excluimonomio(p, r);
        atualiza(p);
    }
}

void termosemelhante(pExpressao *p){
pExpressao aux, aux2, v;
aux=*p;
while ((aux)&&(aux->prox != NULL)){
    while (aux->operador=='*'){
        multiplica(p, &aux, aux->prox);
//        printf("%s\n", aux->polinomio);
    }
    aux=aux->prox;
}
aux=*p;
    while (aux!=NULL){
        aux2=aux->prox;
        while (aux2!=NULL){
            v=aux2->prox;
            soma(p, &aux, aux2);
            aux2=v;
        }
        aux=aux->prox;
    }
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

// ---------------------- MAIN ----------------------------------------------------------

int main(){
char s[TAM_POLINOMIO]="a";
int l;
pExpressao x;
    fflush(stdin);
    scanf(" %[^\n]s", s);
//    printf("%s\n", s);
    remove_espaco(s);
//    fflush(stdin);
    while(strcmp(s, "FIM")!=0){
        fflush(stdin);
        x=criacelulas(contatermos(s));
//        printf("%s\n", s);
        entrada(s, &x);
//    printf("%c", x->operador);
//    printf("%d", ((x->prox)->prox)->indice);
//    printf("%d\n", contalgarismos(10));
//    printaexpressao(x);
//    printf("%c\n", x->operador);
//    crialista(x->polinomio, &(x->literais));
//    crialista(((x->prox)->polinomio), &((x->prox)->literais));
        listas(&x);
//    multiplica(&x, &x, x->prox);
//    printaexpressao(x);
        atualiza(&x);
//    printaexpressao(x);
//    arrumanegativos(&x);
        termosemelhante(&x);
        printaexpressao(x);
        printf("\n");
        zerastring(s);
//        printf("%s\n", s);
        excluipolinomio(&x);
//        if(x==NULL) printf("deubom\n");
        scanf(" %[^\n]s", s);
//        printf("%s\n", s);
        remove_espaco(s);
    }
 //   printf("%d", (x->prox)->indice);
//printf("%d ", test->expoente);*/
    return 0;
}
