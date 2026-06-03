import java.util.Scanner;
import java.util.Random;
class Metodo{
    public static String altera(String newResp, char a, char b){
        char[] alterado=newResp.toCharArray();
        for(int i=0;i<newResp.length();i++){
            if(newResp.charAt(i)==a){
                alterado[i]=b;
            }
        }
        newResp=new String(alterado);
        return newResp;
    }
}
public class AlteracaoRd {
    public static void main(String[] args){
        Scanner sc=new Scanner(System.in);
        Random rd=new Random();

        rd.setSeed(4);
        String palavra=sc.nextLine();

        while(palavra.charAt(0)!='F' || palavra.charAt(1)!='I' || palavra.charAt(2)!='M'){
            char Rd=(char)('a' + Math.abs(rd.nextInt()) % 26);
            char Rd2=(char)('a' + Math.abs(rd.nextInt()) % 26);
            palavra=Metodo.altera(palavra, Rd, Rd2);
            System.out.println(palavra);
            palavra=sc.nextLine();
        }
        sc.close();
    }
}
