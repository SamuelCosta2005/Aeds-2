import java.util.Scanner;
public class Ciframento {
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        String palavra = sc.nextLine();

        while (palavra.charAt(0)!='F' || palavra.charAt(1)!='I' || palavra.charAt(2)!='M') {            
            System.out.println(Transformado(palavra));
            palavra = sc.nextLine();
        }
        sc.close();
    }
    public static String Transformado(String transformada){
        String Nova="";
        char[] ababa=transformada.toCharArray();
        for(int i = 0; i < transformada.length(); i++){
            if(ababa[i]>=32 && ababa[i]<=127){
                ababa[i]=(char)(ababa[i]+3);
            }
            Nova+=""+ababa[i];
        }
        return Nova;
    }
}
