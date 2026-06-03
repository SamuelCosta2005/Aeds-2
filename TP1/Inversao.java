import java.util.Scanner;
class SoMetodo{
    public static String Invertida(String ababa){
        char[] a=ababa.toCharArray();
        for(int i=0;i<ababa.length()/2;i++){
            char tmp=a[i];
            a[i]=a[ababa.length()-i-1];
            a[ababa.length()-i-1]=tmp;
        }
        ababa=new String(a);
        return ababa;
    }
}
class Inversao{
    public static void main(String[] args){
        Scanner sc=new Scanner(System.in);
        String balerina=sc.nextLine();
        while(!balerina.equals("FIM")){
            balerina=SoMetodo.Invertida(balerina);
            System.out.println(balerina);
            balerina=sc.nextLine();
        }
        sc.close();
    }
}