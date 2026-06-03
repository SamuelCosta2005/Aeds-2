#include <stdio.h>
#include <stdlib.h>
typedef struct Celula{
    int elem;
    struct Celula* prox;
}Celula;
Celula* Nova(int x){
    Celula* a=(Celula*) malloc(sizeof(Celula));
    a->elem=x;
    a->prox=NULL;
    return a;
}
typedef struct{
    Celula* topo;
}Pilha;
Pilha nova(){
    Pilha a;
    a.topo=Nova(0);
    return a;
}
void Inserir(int x, Pilha* p){
    Celula* tmp=Nova(x);
    tmp->prox=p->topo->prox;
    p->topo->prox=tmp;
}
void Mostrar(Pilha* p){
    for(Celula* i=p->topo->prox;i!=NULL;i=i->prox){
        printf("%d ", i->elem);
    }
}
char Pesquisa(Pilha* p, int x){
    char resp='N';
    for(Celula* i=p->topo->prox;i!=NULL;i=i->prox){
        if(i->elem==x){
            resp='S';
        }
    }
    return resp;
}
int Remove(Pilha* a){
    int resp=-1;
    if(a->topo->prox!=NULL){
        resp=a->topo->prox->elem;
        a->topo=a->topo->prox;
    }
    return resp;
}
int main(){
    Pilha st=nova();
    char comando;
    while(scanf(" %c",&comando)==1){
        if(comando=='E'){
            int num;
            scanf("%d",&num);
            Inserir(num,&st);
        }
        else if(comando=='M'){
            if(st.topo->prox!=NULL){
                Mostrar(&st);
            }
            else{
                printf("V");
            }
            printf("\n");
        }
        else if(comando=='P'){
            int num;
            scanf("%d",&num);
            printf("%c\n",Pesquisa(&st,num));
        }
        else if(comando=='D'){
            printf("%d\n",Remove(&st));
        }
    }
    return 0;
}