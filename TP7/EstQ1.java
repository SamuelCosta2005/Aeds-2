class No{
    int elem;
    No esq;
    No dir;
    public No(int x){
        this.elem=x;
        this.esq=null;
        this.dir=null;
    }
}
class Arvore{
    No raiz;
    public Arvore(){
        raiz=null;
    }
    public void inserir(int x) throws Exception{
        raiz=inserir(raiz,x);
    }
    public No inserir(No i, int x) throws Exception{
        if(i==null){
            i=new No(x);
        }
        else if(x<i.elem){
            i.esq=inserir(i.esq,x);
        }
        else if(x>i.elem){
            i.dir=inserir(i.dir, x);
        }
        else{
            throw new Exception("Tem");
        }
        return i;
    }
    public boolean pesquisar(int x){
        return pesquisar(raiz,x);
    }
    public boolean pesquisar(No i, int x) {
        boolean resp=false;
        if(i!=null){
            if(i.elem==x){
                resp=true;
            }
            else if(x<i.elem){
                resp=pesquisar(i.esq, x);
            }
            else if(x>i.elem){
                resp=pesquisar(i.dir, x);
            }
        }
        return resp;
    }
    public void show(No i){
        if(i!=null){
            show(i.esq);
            System.out.print(i.elem+" ");
            show(i.dir);
        }
    }
    
}
class EstQ1 {
    public static void main(String[] args) throws Exception {
        Arvore a=new Arvore();
        a.inserir(8);
        a.inserir(4);
        a.inserir(12);
        a.inserir(2);
        a.inserir(6);
        a.inserir(10);
        a.inserir(14);
        a.inserir(1);
        a.inserir(3);
        a.inserir(5);
        a.inserir(7);
        a.inserir(9);
        a.inserir(11);
        a.inserir(13);
        a.inserir(15);
        System.out.println(a.pesquisar(9));
        a.show(a.raiz);
        System.out.println(a.pesquisar(3774));
    }
}
