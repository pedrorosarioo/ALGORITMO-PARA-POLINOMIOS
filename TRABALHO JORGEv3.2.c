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
}expressao, *pExpressao;

// ------------------------- FUNÇÕES PARA PREENCHER AS ESTRUTURAS ------------------------------------------

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
    for(i=0; i<strlen(c); i++){
        if ((c[i]=='+')||(c[i]=='*')||(c[i]=='-')){
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
    while (aux!=NULL){
        if (aux=k) break;
        aux=aux->prox;
    }
    if (aux->ant==NULL){
//        printf("Entrei\n");
        *p=aux->prox;
        (*p)->ant=NULL;
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
            strcpy(aux->polinomio, "\0");
            //printf("%s\n", aux->polinomio);
  //          origem->indice=0;
        }else{
            aux2=(pExpressao)malloc(sizeof(expressao));
            if(aux2==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
            strcpy(aux2->polinomio, "\0");
//            printf("Entrou, %d \n", cont);
            aux->prox=aux2;
            aux2->prox=NULL;
            aux2->ant=aux;
            aux2->literais=NULL;
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
    while(x%10>0){
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




void entrada(char c[], char *z, pExpressao *x){ // COLOCA INDICE E POLINOMIO NOS ESPAÇOS ALOCADOS
int i, j=0, k=0, l=0, flag=1;
char v[11], in[11]="\0";
strcpy(v, "0123456789");
pExpressao forfree, aux=*x;
    for (i=0; ((i<TAM_POLINOMIO)&&(c[i]!='\0')); i++){
        flag=1;
        if (!aux) break;
//        printf("%d\n",aux->indice);
        if ((strchr(v, c[i]))&&(flag)){
                in[k]=c[i];
                k++;
        }else{
            if (flag){
                aux->indice = criaindice(in, k);
//                printf("%c\n", aux->indice);
                flag=0;
                k=0;
//                printf("%c\n", c[i]);
            }
            if (c[i])
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
            l++;
            //flag=1;
            /*if (aux->indice == 0){
                    forfree = aux;
                    aux=aux->prox;
                    excluimonomio(x, forfree);
//                aux=aux->prox;
            }else*/
            aux=aux->prox;
        }
    }
    if(flag){
      aux->indice=criaindice(in, k);
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

// ---------------------- FUNÇÕES PARA OPERAR AS ESTRUTURAS ------------------------------
void termosemelhante(pExpressao *p){
pExpressao aux, aux2, v;
aux=*p;
if(aux){
    while (aux!=NULL){
        aux2=aux->prox;
        while (aux2!=NULL){
            v=aux2->prox;
            if (strcmp(aux2->polinomio, aux->polinomio)==0){
                switch(aux->operador){
                    case '+':
                        aux->indice = aux->indice+aux2->indice;
                        break;
                    case '-':
                        aux->indice = aux->indice-aux2->indice;
                        break;
                    }
                if (aux2->operador!='\0'){
                  aux->operador=aux2->operador;
                }else (aux2->ant)->operador=aux2->operador;
                excluimonomio(p, aux2);
                aux2=v;
            }else{
                aux2=aux2->prox;
            }
        }
        aux=aux->prox;
    }
}
}
// ---------------------- MAIN ----------------------------------------------------------

int main(){
char s[TAM_POLINOMIO]="2x^0xyx+0+1+0+1+0x^0xx";
char operadores[100];
pExpressao x;
    x=criacelulas(contatermos(s));
    entrada(s, operadores, &x);
//  printaexpressao(x);
 //   printf("%c\n", x->operador);
//    crialista(x->polinomio, &(x->literais));
//    crialista(((x->prox)->polinomio), &((x->prox)->literais));
    listas(&x);
    atualiza(&x);
    termosemelhante(&x);
    printaexpressao(x);
 //   printf("%d", (x->prox)->indice);
//printf("%d ", test->expoente);
    return 0;
}
// --------------------------------------------------------------------------------------------

/*
void inicializa(Lista *x){
    *x=NULL;
}

void InsereLista(Lista *x, int n){
Lista aux, ant=NULL, dep=*x;

aux=(Lista)malloc(sizeof(NoLista));
if (aux==NULL) return 0;
aux->chave=n;
if (dep==NULL){
    *x=aux;
    aux->prox=NULL;
    return 1;
}else{
    while (dep!=NULL){
        if (aux->chave<dep->chave)break;
        ant=dep;
        dep=dep->prox;
    }
    if (ant==NULL){
        aux->prox=*x;
        *x=aux;
    }else{
        aux->prox=ant->prox;
        ant->prox=aux;
    }
}
return 1;
}

Lista BuscaLista(Lista *x, int n){
Lista aux;
    for (aux=*x; aux!=NULL; aux=aux->prox){
        if (aux->chave==n){
            return aux;
        }
    }
return NULL;
}

int ExcluiLista(Lista *x, int n){
Lista ant=NULL, aux=*x;
if ((BuscaLista(x, n))){
    while (aux!=BuscaLista(x, n)){
        ant=aux;
        aux=aux->prox;
    }
    if (ant==NULL){
        *x=aux->prox;
        free(aux);";
    }else{
        ant->prox=aux->prox;
        free(aux);
    }
    return 1;
}
return 0;
}

int main(){
int p;
Lista x;
inicializa(&x);
p=InsereLista(&x, 1);
p=InsereLista(&x, 3);
p=InsereLista(&x, 2);
printf ("%d %d %d %d\n", BuscaLista(&x, 1),BuscaLista(&x, 2), BuscaLista(&x, 3), BuscaLista(&x, 4));
printf ("%d %d ", ExcluiLista(&x, 3), ExcluiLista(&x, 2));
return 0;
}
*/


/*int i, cont1=0, cont=0;
Lista origem, aux, aux2, r;
    for(i=0; c[i]!='\0'; i++){
        if (c[i]=='^'){
            cont1++;
        }
    }

    for(i=0; i<cont1; i++){
        if (i==0){
            origem=(Lista)malloc(sizeof(NoLista));
            if(origem==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
            aux=origem;
            aux->lprox=NULL;
            aux->lant=NULL;
            aux->expoente=0;
  //          origem->indice=0;
        }else{
            aux2=(Lista)malloc(sizeof(NoLista));
            if(aux2==NULL){
                printf("SEM MEMORIA\n");
                exit(1);
            }
            cont++;
//            printf("Entrou, %d \n", cont);
            aux->lprox=aux2;
            aux2->lprox=NULL;
            aux2->lant=aux;
            aux=aux2;
            aux->expoente=1;
    //        aux2->indice=1;
        }
    }
    printf("%d celulas criadas\n", cont);
    r=origem;
    return r;*/
