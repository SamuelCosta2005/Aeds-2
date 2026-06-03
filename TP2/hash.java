import java.util.Scanner;
public class hash {
    public static int PesquisaHash(int num, int tam){
        return num%tam;
    }
    public static void main(String[] args) {
        Scanner sc=new Scanner(System.in);
        int tam=sc.nextInt();
        int[] vetor=new int[tam];
        for(int i=0;i<tam;i++){
            vetor[i]=i;
        }
        System.out.println("Digite um numero a ser pesquisado");
        int num=sc.nextInt();
        System.out.println(vetor[PesquisaHash(num, tam)]);
        sc.close();
        
    }
}
