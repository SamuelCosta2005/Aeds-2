#include <stdio.h>
int SomaRec(int num, int soma){
    if(num>0){
        soma+=num%10;
        SomaRec(num/10,soma);
    }
    else{
        return soma;
    }
}
int main(){
    int a;
    while(scanf("%d",&a)==1){
        a=SomaRec(a,0);
        printf("%d\n", a);
    }
    return 0;
}