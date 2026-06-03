#include <stdio.h>
#include <string.h>

int VerificaTrue(char Tung[], int tam, int i){
    int resp=1;
    if(i<=tam/2){
        if(Tung[i]!=Tung[tam-1-i]){
                resp=0;
        }
        else{
            i++;
            resp=VerificaTrue(Tung, tam, i);
        }
    }
    return resp;
}

int TamanhoChar(char Sahur[]){
    int tam=0;
    for(int i=0;Sahur[i]!='\0';i++){
        tam++;
    }
    return tam;
}

int main()
{
    char aaa[500];
    int Tam;

    scanf("%499[^\n]", aaa);
    getchar();

    while(aaa[0]!='F' || aaa[1]!='I' || aaa[2]!='M'){
        Tam=TamanhoChar(aaa);

        if(VerificaTrue(aaa,Tam,0)==1){
            printf("SIM\n");
        }
        else{
            printf("NAO\n");
        }

        scanf("%499[^\n]", aaa);
        getchar();
    }

    return 0;
}
