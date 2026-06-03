import java.util.Scanner;
class Metodo{
    public static String TransformaMinusculo(String W1){
        char primeira = W1.charAt(0);
        if (primeira >= 'A' && primeira <= 'Z') {
            primeira = (char)(primeira + 32); // transforma em minúscula
        }

        String resultado = "" + primeira;
        for (int i = 1; i < W1.length(); i++) {
            resultado += W1.charAt(i);
        }

        return resultado;
    }
    public static boolean Anagrama(String W1, String W3){
        boolean a=false;
        for(int i=0;i<W1.length();i++){
            a=false;
            for(int j=0;j<W3.length();j++){
                if(W1.charAt(i)==W3.charAt(j)){
                    a=true;
                    j=W3.length();
                }
            }
            if(a==false){
                i=W1.length();
            }
        }
        return a;
    }
}
public class Anagrama {
    public static void main(String[] args) {
        Scanner sc= new Scanner(System.in);
        String W1;
        String Hifen;
        String W3;
        W1=sc.next();
        while(W1.charAt(0)!='F'|| W1.charAt(1)!='I'|| W1.charAt(2)!='M'){
            Hifen=sc.next();
            W3=sc.next();
            W1=Metodo.TransformaMinusculo(W1);
            W3=Metodo.TransformaMinusculo(W3);
            if(Metodo.Anagrama(W1, W3)){
                System.out.println("SIM");
            }
            else{
                System.out.println("N\u00C3O");
            }
            W1=sc.next();
        }

        sc.close();
    }
}
