import java.util.Scanner;
public class Palindromo {
    public static void main(String[] args){
        Scanner sc= new Scanner(System.in);
        String palavra=sc.nextLine();
        while(palavra.charAt(0)!='F' || palavra.charAt(1)!='I' || palavra.charAt(2)!='M'){
            if(VerificaTrue(palavra)){
                System.out.println("SIM");
            }
            else{
                System.out.println("NAO");
            }
        palavra=sc.nextLine();
        }
        sc.close();
    }
    public static boolean VerificaTrue(String Tung){
        boolean a=true;
        for(int i=0;i<Tung.length()/2;i++){
            if(Tung.charAt(i)!=Tung.charAt(Tung.length()-i-1)){
                a=false;
                i=Tung.length()/2;
            }
        }
        return a;
    }
}
//~((A+B)+(~B)+(~(BCD)))
//((~(A o B).~(CD)).~(EF))