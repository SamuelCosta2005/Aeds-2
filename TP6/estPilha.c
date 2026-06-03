#include <stdio.h>
#include <stdlib.h>
typedef struct No{
    int elem;
    struct No* prox;
}No;
No* novoNo(int x){
    No* a=(No*) malloc(sizeof(No));
    a->elem=x;
    a->prox=NULL;
    return a;
}
typedef struct Pilha{
    No* topo;
}Pilha;
Pilha* NovaPilha(){
    Pilha *a=(Pilha*)malloc(sizeof(Pilha));
    a->topo=novoNo(0);
    return a;
}
void Inserir(No* topo, int x){
    No* tmp=novoNo(x);
    tmp->prox=topo->prox;
    topo->prox=tmp;
}
void mostrar(No* topo){
    for(No* i=topo->prox;i!=NULL;i=i->prox){
        printf("%d ",i->elem);
    }
    printf("\n");
}
No* Remove(No* topo){
    No* tmp= topo->prox;
    if(tmp!=NULL){
        topo->prox=tmp->prox;
    }
    else{
        printf("V\n");
    }
    return tmp;
}
int main(){
    Pilha* a=NovaPilha();
    Inserir(a->topo,12);
    Inserir(a->topo,13);
    Inserir(a->topo,14);
    Inserir(a->topo,15);
    mostrar(a->topo);
    Remove(a->topo);
    mostrar(a->topo);
    return 0;
}