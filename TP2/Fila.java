import java.util.*;

class Celula {
    int valor;
    Celula prox;

    public Celula(int valor) {
        this.valor = valor;
        this.prox = null;
    }
}

class Fila {
    Celula primeiro = null;
    Celula ultimo = null;
    Random rd = new Random();

    public void enfileira(int valor) {
        Celula nova = new Celula(valor);
        if (primeiro == null) {
            primeiro = nova;
            ultimo = nova;
        } else {
            ultimo.prox = nova;
            ultimo = nova;
        }
    }

    public void mostraFila() {
        for(Celula aux=primeiro; aux!=null;aux=aux.prox){
            System.out.print(aux.valor+" -> ");
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Digite o tamanho da fila: ");
        int tam = sc.nextInt();
        sc.close();

        Fila f = new Fila();
        for (int i = 0; i < tam; i++) {
            f.enfileira(new Random().nextInt(100) + 1);
        }

        f.mostraFila();
    }
}
