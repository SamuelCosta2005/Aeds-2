import java.util.Scanner;
class Hash{
    No[] array;
    int tab;
    int nr;
    int r;
    public Hash(){
        this.tab=21;
        this.r=9;
        this.array=new No[tab+r];
        this.nr=0;
    }
    public int hash(String a){
        int x=0;
        for(int i=0;i<a.length();i++){
            x+=a.charAt(i);
        }
        return x%tab;
    }
    public void Inserir(No x){
        int pos=hash(x.elem);
        if(array[pos]==null){
            array[pos]=x;
        }
        else{
            if(nr<r){
                array[tab+nr]=x;
                nr++;
            }
        }
    }
    public boolean pesquisa(String x){
        boolean resp=false;
        int pos=hash(x);
        if(array[pos]!=null){
            if(array[pos].elem.equals(x)){
                resp=true;
            }
            else{
                for(int i=tab;i<tab+nr;i++){
                    if(array[i].elem.equals(x)){
                        resp=true;
                        i=tab+nr;
                    }
                }
            }
        }
        return resp;
    }
    public void show(){
        for(int i=0;i<array.length;i++){
            if(i==21){
                System.out.println();
                System.out.println("Área de reserva");
            }
            if(array[i]==null){
                System.out.print("null ");
            }
            else{
                System.out.print(array[i].elem+" ");
            }
        }
    }
}
class No{
    String elem;
    public No(String x){
        this.elem=x;
    }
}
class EstQ1{
    public static void main(String[] args) {
        Scanner sc=new Scanner(System.in);
        Hash tabela=new Hash();
        tabela.Inserir(new No("ana"));
        tabela.Inserir(new No("bia"));
        tabela.Inserir(new No("carlos"));
        tabela.Inserir(new No("dudu"));
        tabela.Inserir(new No("eric"));
        tabela.Inserir(new No("fabi"));
        tabela.Inserir(new No("gabi"));
        tabela.Inserir(new No("heitor"));
        tabela.Inserir(new No("igor"));
        tabela.Inserir(new No("joao"));
        tabela.Inserir(new No("karen"));
        tabela.Inserir(new No("luis"));
        tabela.Inserir(new No("maria"));
        tabela.Inserir(new No("nina"));
        tabela.Inserir(new No("olga"));
        tabela.Inserir(new No("paula"));
        tabela.Inserir(new No("quim"));
        tabela.Inserir(new No("rita"));
        tabela.Inserir(new No("sara"));
        tabela.Inserir(new No("tati"));
        tabela.Inserir(new No("udo"));
        tabela.Inserir(new No("vini"));
        tabela.Inserir(new No("will"));
        tabela.Inserir(new No("xuxa"));
        tabela.Inserir(new No("yuri"));
        tabela.Inserir(new No("zara"));
        
        tabela.show();
        sc.close();
    }
}