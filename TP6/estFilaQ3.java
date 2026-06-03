class No{
    int elem;
    No prox;
    public No(int x){
        this.elem=x;
        this.prox=null;
    }
}
class Fila{
    No inicio;
    public Fila(){
        this.inicio=null;
    }
    public void Inserir(int x){
        No tmp=new No(x);
        No i=inicio;
        if(i!=null){
            while(i.prox!=null){
                i=i.prox;
            }
            i.prox=tmp;
        }
        else{
            inicio=tmp;
        }
    }
    public No Remove() throws Exception{
        No tmp=null;
        if(inicio!=null){
            tmp=inicio;
            inicio=inicio.prox;
        }
        else{
            throw new Exception();
        }
        return tmp;
    }
    public void Mostrar(){
        for(No i=inicio;i!=null;i=i.prox){
            System.out.print(i.elem+" ");
        }
        System.out.println();
    }
}
class estFilaQ3 {
    public static void main(String[] args) throws Exception {
        Fila a=new Fila();
        a.Inserir(3);
        a.Inserir(43);
        a.Inserir(1);
        a.Mostrar();
        a.Remove();
        a.Mostrar();
    }
}
