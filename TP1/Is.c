#include <stdio.h>
#include <string.h>

char* VerificaVogal(char palavra[]){
    static char resp[4] = "SIM";
    int i;
    for(i=0; i<strlen(palavra); i++){
        if((palavra[i]>=65 && palavra[i]<=90) || (palavra[i]>=97 && palavra[i]<=122)){
            if(palavra[i]!='A' && palavra[i]!='a' && 
               palavra[i]!='E' && palavra[i]!='e' &&
               palavra[i]!='I' && palavra[i]!='i' &&
               palavra[i]!='O' && palavra[i]!='o' &&
               palavra[i]!='U' && palavra[i]!='u'){
                strcpy(resp,"NAO");
                i = strlen(palavra);
            }
        }
        else{
            strcpy(resp,"NAO");
            i = strlen(palavra);
        }
    }
    return resp;
}


char* VerificaConsoante(char palavra[]){
    static char resp[4] = "SIM";
    int i;
    for(i=0; i<strlen(palavra); i++){
        if((palavra[i]>65 && palavra[i]<=90) || (palavra[i]>97 && palavra[i]<=122)){
            if(palavra[i]=='A' && palavra[i]=='a' && palavra[i]=='E' && palavra[i]=='e' &&
               palavra[i]=='I' && palavra[i]=='i' && palavra[i]=='O' && palavra[i]=='o' &&
               palavra[i]=='U' && palavra[i]=='u'){
                strcpy(resp,"NAO");
                i=strlen(palavra);
            }
        }else{
            strcpy(resp,"NAO");
            i=strlen(palavra);
        }
    }
    return resp;
}

char* VerificaInt(char palavra[]){
    static char resp[4] = "SIM";
    int i;
    for(i=0; i<strlen(palavra); i++){
        if(palavra[i]<'0' || palavra[i]>'9'){
            strcpy(resp,"NAO");
            i=strlen(palavra);
        }
    }
    return resp;
}

char* VerificaReal(char palavra[]){
    static char resp[4] = "NAO";
    int Ind=0;
    int i;
    for(i=0; i<strlen(palavra); i++){
        if(palavra[i]>='0'&& palavra[i]<='9'||palavra[i]=='.' || palavra[i]==','){
            if(palavra[i]=='.' || palavra[i]==','){
                Ind++;
            }
        }
        else{
            strcpy(resp,"NAO");
            Ind=0;
        }
    }
    if(Ind==1){
        strcpy(resp,"SIM");
    }else{
        strcpy(resp,"NAO");
    }
    return resp;
}

int main(){
    char word[1000];
    fgets(word, 1000, stdin);
    word[strcspn(word, "\n")] = '\0';
    while(strcmp(word,"FIM")!=0){
        printf("%s %s %s %s\n", VerificaVogal(word), VerificaConsoante(word), VerificaInt(word), VerificaReal(word));
        fgets(word, 1000, stdin);
        word[strcspn(word, "\n")] = '\0'; 
    }
    return 0;
}
