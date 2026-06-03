import java.util.Scanner;
class Pessoa{
    public static double IMC(int peso, double altura){
        return peso/(altura*altura);
    }
}
class Teste{
    public static void main(String[] args) {//psvm
        Scanner Isabela = new Scanner(System.in);// inicialização do scanner sc
        //int peso=Isabela.nextInt();//Você cria a variável já igualando num scanner
        //double altura=Isabela.nextDouble();
        //System.out.println(Pessoa.IMC(peso, altura));//sout
        //System.out.print(Pessoa.IMC(88, 1.75));
        //char a=(char)('a'+2);// Pra fazer o cast que é basicamente  transformar para o dado desejado, é a estrutura aqui
        String N5=Isabela.nextLine();
        String N1=Isabela.next();
        String N2=Isabela.next();
        String N3=Isabela.next();
        String N4=Isabela.next();
        System.out.println(N1+N2+N3+N4);//Concatenação
        System.out.println(N5);
        //System.out.println(a);
        Isabela.close();
    }
}
