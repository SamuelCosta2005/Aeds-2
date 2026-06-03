import java.util.Scanner;
class Metodos{
    public static String VerificaVogal(String palavra){
        String resp="SIM";
        for(int i=0;i<palavra.length();i++){
            if(palavra.charAt(i)>=65 && palavra.charAt(i)<=90 || palavra.charAt(i)>=97 && palavra.charAt(i)<=122){
                if(palavra.charAt(i)!='A'&&palavra.charAt(i)!='a'&&palavra.charAt(i)!='E'&&palavra.charAt(i)!='e'&&palavra.charAt(i)!='I'
                &&palavra.charAt(i)!='i'&&palavra.charAt(i)!='O'&&palavra.charAt(i)!='o'&&palavra.charAt(i)!='U'&&palavra.charAt(i)!='u'){
                    resp="NAO";
                    i=palavra.length();
                }
            }
            else{
                resp="NAO";
                i=palavra.length();
            }
        }
        return resp;
    }
    public static String VerificaConsoante(String palavra){
        String resp="SIM";
        for(int i=0;i<palavra.length();i++){
            if(palavra.charAt(i)>65 && palavra.charAt(i)<=90 || palavra.charAt(i)>97 && palavra.charAt(i)<=122){
                if(palavra.charAt(i)=='A'&&palavra.charAt(i)=='a'&&palavra.charAt(i)=='E'&&palavra.charAt(i)=='e'&&palavra.charAt(i)=='I'
                &&palavra.charAt(i)=='i'&&palavra.charAt(i)=='O'&&palavra.charAt(i)=='o'&&palavra.charAt(i)=='U'&&palavra.charAt(i)=='u'){
                    resp="NAO";
                    i=palavra.length();
                }
            }
            else{
                resp="NAO";
                i=palavra.length();
            }
        }
        return resp;
    }
    public static String VerificaInt(String palavra){
        String resp="SIM";
        for(int i=0;i<palavra.length();i++){
            if(palavra.charAt(i)<'0'||palavra.charAt(i)>'9'){
                resp="NAO";
                i=palavra.length();
            }
        }
        return resp;
    }
    public static String VerificaReal(String palavra){
        String resp="NAO";
        int Ind=0;
        for(int i=0;i<palavra.length();i++){
            if(palavra.charAt(i)=='.'||palavra.charAt(i)==','){
                Ind++;
            }
        }
        if(Ind==1){
            resp="SIM";
        }
        else{
            resp="NAO";
        }
        return resp;
    }
}
class Is{
    public static void main(String[] args) {
        Scanner sc= new Scanner(System.in);
        String word=sc.nextLine();
        while(!word.equals("FIM")){
            System.out.println(Metodos.VerificaVogal(word)+" "+Metodos.VerificaConsoante(word)+" "+Metodos.VerificaInt(word)+" " + Metodos.VerificaReal(word));
            word=sc.nextLine();
        }
        sc.close();
    }
}