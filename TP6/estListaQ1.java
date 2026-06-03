import java.util.Scanner;
class Lista{
    int[] array;
    int n;
    public Lista(){
        this.array=new int[200];
        this.n=0;
    }
    public void inserirFIm(int x){//IF
        array[n]=x;
        n++;
    }
    public void InserirInicio(int x){ //II
        for(int i=n-1;i>=0;i--){
            array[i+1]=array[i];
        }
        array[0]=x;
        n++;
    }
    public void inserirPos(int x, int num){//I*
        if(x>0 && x<=n+1){
            for(int i=n-1;i>=x-1;i--){
                array[i+1]=array[i]; // 1 2  num 3
            }
            array[x-1]=num;
            n++;
        }
    }
    public int RemoveFim(){//RF
        return array[--n];
    }
    public int RemoveInicio() throws Exception{//RI
        int tmp=array[0];
        if(n>0){
            for(int i=0;i<n-1;i++){
                array[i]=array[i+1];
            }
            n--;
        }
        else{
            throw new Exception("V");
        }
        return tmp;
    }
    public int removePos(int x) throws Exception{//R*
        int tmp=array[x-1];
        if(n>0){
            for(int i=x-1;i<n;i++){// 1 2 3
                array[i]=array[i+1];
            }
            n--;
        }
        else{
            throw new Exception("V");
        }
        return tmp;
    }
}
class estListaQ1{
    public static void main(String[] args) throws Exception {
        Lista l=new Lista();
        Scanner sc=new Scanner(System.in);
        while(sc.hasNext()){
            String c=sc.next();
            if(c.equals("II")){
                l.InserirInicio(sc.nextInt());
            }
            else if(c.equals("IF")){
                l.inserirFIm(sc.nextInt());
            }
            else if(c.equals("I*")){
                l.inserirPos(sc.nextInt(), sc.nextInt());
            }
            else if(c.equals("RI")){
                System.out.println(l.RemoveInicio());
            }
            else if(c.equals("RF")){
                System.out.println(l.RemoveFim());
            }
            else if(c.equals("R*")){
                System.out.println(l.removePos(sc.nextInt()));
            }
            else{
                for(int i=0;i<l.n;i++){
                    System.out.print(l.array[i]+" ");
                }
                System.out.println();
            }
        }
        sc.close();
    }
}