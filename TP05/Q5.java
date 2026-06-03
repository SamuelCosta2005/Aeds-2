import java.io.File;
import java.util.*;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.charset.Charset;

public class Q5 {
    static int comparacao = 0;
    static int movimentacoes = 0;

    static class Data {
        int dia;
        int mes;
        int ano;

        public String toString() {
            return dia + "/" + mes + "/" + ano;
        }

        void mostrar(PrintStream ps) {
            ps.printf("%02d/%02d/%02d", dia, mes, ano);
        }

        Data(String entrada) {
            dia = 1;
            mes = 1;
            ano = 1;

            String[] partes = separar(entrada);

            if (partes.length == 3) {
                mes = getMes(partes[0]);
                dia = parseDia(partes[1]);
                ano = parseAno(partes[2]);
            } else if (partes.length == 2) {
                mes = getMes(partes[0]);
                ano = parseAno(partes[1]);
            } else if (partes.length == 1) {
                ano = parseAno(partes[0]);
            }
        }

        private int getMes(String s) {
            int m = 1;
            if (s.charAt(0) == 'J' && s.charAt(1) == 'a' && s.charAt(2) == 'n')
                m = 1;
            else if (s.charAt(0) == 'F' && s.charAt(1) == 'e' && s.charAt(2) == 'b')
                m = 2;
            else if (s.charAt(0) == 'M' && s.charAt(1) == 'a' && s.charAt(2) == 'r')
                m = 3;
            else if (s.charAt(0) == 'A' && s.charAt(1) == 'p' && s.charAt(2) == 'r')
                m = 4;
            else if (s.charAt(0) == 'M' && s.charAt(1) == 'a' && s.charAt(2) == 'y')
                m = 5;
            else if (s.charAt(0) == 'J' && s.charAt(1) == 'u' && s.charAt(2) == 'n')
                m = 6;
            else if (s.charAt(0) == 'J' && s.charAt(1) == 'u' && s.charAt(2) == 'l')
                m = 7;
            else if (s.charAt(0) == 'A' && s.charAt(1) == 'u' && s.charAt(2) == 'g')
                m = 8;
            else if (s.charAt(0) == 'S' && s.charAt(1) == 'e' && s.charAt(2) == 'p')
                m = 9;
            else if (s.charAt(0) == 'O' && s.charAt(1) == 'c' && s.charAt(2) == 't')
                m = 10;
            else if (s.charAt(0) == 'N' && s.charAt(1) == 'o' && s.charAt(2) == 'v')
                m = 11;
            else if (s.charAt(0) == 'D' && s.charAt(1) == 'e' && s.charAt(2) == 'c')
                m = 12;
            return m;
        }

        private int parseDia(String s) {
            String num = "";
            for (int i = 0; i < s.length(); i++) {
                char c = s.charAt(i);
                if (c >= '0' && c <= '9')
                    num += c;
            }
            return paraInt(num);
        }

        private int parseAno(String s) {
            String num = "";
            for (int i = 0; i < s.length(); i++) {
                char c = s.charAt(i);
                if (c >= '0' && c <= '9')
                    num += c;
            }
            return paraInt(num);
        }

        private int paraInt(String s) {
            int n = 0;
            for (int i = 0; i < s.length(); i++)
                n = n * 10 + (s.charAt(i) - '0');
            return n;
        }

        private String[] separar(String s) {
            int count = 1;
            for (int i = 0; i < s.length(); i++)
                if (s.charAt(i) == ' ')
                    count++;
            String[] out = new String[count];
            String temp = "";
            int idx = 0;
            for (int i = 0; i < s.length(); i++) {
                if (s.charAt(i) == ' ') {
                    out[idx++] = temp;
                    temp = "";
                } else
                    temp += s.charAt(i);
            }
            out[idx] = temp;
            return out;
        }

        void imprimir(PrintStream ps) {
            ps.println(dia + "/" + mes + "/" + ano);
        }
    }

    static public class Jogo {
        private int id;
        private String nome;
        private Data data;
        private int proprietariosEstimados;
        private float preco;
        private String[] idiomasSuportados;
        private int pontuacaoMetacritic;
        private float pontuacaoUsuario;
        private int conquistas;
        private String editoras;
        private String desenvolvedores;
        private String[] categorias;
        private String[] generos;
        private String[] tags;

        public String getNome() {
            return nome;
        }

        public int getId() {
            return id;
        }

        public void imprimir(PrintStream ps) {
            ps.print("=> " + id + " ## " + nome + " ## ");
            data.mostrar(ps);
            ps.print(" ## " + proprietariosEstimados + " ## ");
            ps.print(String.format("%.2f", preco) + " ## [");

            for (int i = 0; i < idiomasSuportados.length; i++) {
                ps.print(idiomasSuportados[i]);
                if (i < idiomasSuportados.length - 1)
                    ps.print(", ");
            }
            ps.print("] ## " + pontuacaoMetacritic + " ## ");
            ps.printf("%.1f ## ", pontuacaoUsuario);

            ps.print(conquistas + " ## [" + editoras + "] ## [" + desenvolvedores + "] ## [");
            for (int i = 0; i < categorias.length; i++) {
                ps.print(categorias[i]);
                if (i < categorias.length - 1)
                    ps.print(", ");
            }
            ps.print("] ## [");
            for (int i = 0; i < generos.length; i++) {
                ps.print(generos[i]);
                if (i < generos.length - 1)
                    ps.print(", ");
            }
            ps.print("] ## [");
            for (int i = 0; i < tags.length; i++) {
                ps.print(tags[i]);
                if (i < tags.length - 1)
                    ps.print(", ");
            }
            ps.println("] ##");
        }

        public Jogo() {
            this.id = 0;
            this.nome = "NaN";
            this.data = new Data("NaN");
            this.proprietariosEstimados = 0;
            this.preco = 0.0f;
            this.idiomasSuportados = new String[0];
            this.pontuacaoMetacritic = 0;
            this.pontuacaoUsuario = 0;
            this.conquistas = 0;
            this.editoras = "NaN";
            this.desenvolvedores = "NaN";
            this.categorias = new String[0];
            this.generos = new String[0];
            this.tags = new String[0];
        }

        public Jogo(int id, String nome, String data, int proprietariosEstimados, float preco,
                String[] idiomasSuportados, int pontuacaoMetacritic, int pontuacaoUsuario,
                int conquistas, String editoras, String desenvolvedores,
                String[] categorias, String[] generos, String[] tags) {
            this.id = id;
            this.nome = nome;
            this.data = new Data(data);
            this.proprietariosEstimados = proprietariosEstimados;
            this.preco = preco;
            this.idiomasSuportados = idiomasSuportados;
            this.pontuacaoMetacritic = pontuacaoMetacritic;
            this.pontuacaoUsuario = pontuacaoUsuario;
            this.conquistas = conquistas;
            this.editoras = editoras;
            this.desenvolvedores = desenvolvedores;
            this.categorias = categorias;
            this.generos = generos;
            this.tags = tags;
        }
    }

    static public class OrdenacaoMerge {
        Jogo[] jogos = new Jogo[1850];
        int contadorJogos = 0;

        OrdenacaoMerge() {}

        public void inserir(Jogo elemento) {
            jogos[contadorJogos++] = elemento;
        }

        public void ordenar() {
            ordenacaoMerge(0, contadorJogos - 1);
        }

        private void ordenacaoMerge(int esquerda, int direita) {
            if (esquerda < direita) {
                int meio = (esquerda + direita) / 2;
                ordenacaoMerge(esquerda, meio);
                ordenacaoMerge(meio + 1, direita);
                merge(esquerda, meio, direita);
            }
        }

        private boolean maior(Jogo a, Jogo b) {
            comparacao++;
            if (a.preco > b.preco) return true;
            else if (a.preco < b.preco) return false;
            else return a.id > b.id;
        }

        private void merge(int esquerda, int meio, int direita) {
            int n1 = meio - esquerda + 1;
            int n2 = direita - meio;

            Jogo[] L = new Jogo[n1];
            Jogo[] R = new Jogo[n2];

            for (int i = 0; i < n1; i++)
                L[i] = jogos[esquerda + i];
            for (int j = 0; j < n2; j++)
                R[j] = jogos[meio + 1 + j];

            int i = 0, j = 0, k = esquerda;

            while (i < n1 && j < n2) {
                if (!maior(L[i], R[j])) {
                    jogos[k++] = L[i++];
                } else {
                    jogos[k++] = R[j++];
                }
                movimentacoes++;
            }

            while (i < n1) {
                jogos[k++] = L[i++];
                movimentacoes++;
            }

            while (j < n2) {
                jogos[k++] = R[j++];
                movimentacoes++;
            }
        }

        public void imprimir(PrintStream ps) {
            ps.println("| 5 preços mais caros |");
            for (int i = 1; i <= 5 && contadorJogos - i >= 0; i++) {
                jogos[contadorJogos - i].imprimir(ps);
            }

            ps.println("\n| 5 preços mais baratos |");
            for (int i = 0; i < 5 && i < contadorJogos; i++) {
                jogos[i].imprimir(ps);
            }
        }
    }

    private static String cortar(String s, int inicio, int fim) {
        String resultado = "";
        for (int i = inicio; i < fim && i < s.length(); i++) {
            resultado += s.charAt(i);
        }
        return resultado;
    }

    private static int parseIntSeguro(String s) {
        try {
            return Integer.parseInt(s);
        } catch (Exception e) {
            return 0;
        }
    }

    private static String[] parseArrayGenerico(String s) {
        s = s.trim();
        if (s.length() == 0)
            return new String[0];

        if (s.charAt(0) == '"' && s.charAt(s.length() - 1) == '"')
            s = cortar(s, 1, s.length() - 1);

        if (s.length() >= 2 && s.charAt(0) == '[' && s.charAt(s.length() - 1) == ']')
            s = cortar(s, 1, s.length() - 1);

        String[] tempArr = new String[50];
        int count = 0;
        boolean dentroAspas = false;
        String temp = "";

        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '\'')
                dentroAspas = !dentroAspas;
            else if (c == ',' && !dentroAspas) {
                tempArr[count++] = temp.trim();
                temp = "";
            } else
                temp += c;
        }

        if (!temp.isEmpty())
            tempArr[count++] = temp.trim();

        String[] resultado = new String[count];
        for (int i = 0; i < count; i++)
            resultado[i] = tempArr[i];

        return resultado;
    }

    private static boolean iguais(String a, String b) {
        boolean resp = true;
        if (a.length() != b.length()) {
            resp = false;
        } else {
            int i = 0;
            while (i < a.length() && resp == true) {
                if (a.charAt(i) != b.charAt(i)) {
                    resp = false;
                }
                i = i + 1;
            }
        }
        return resp;
    }

    private static String[] dividirCSV(String linha) {
        String[] campos = new String[14];
        int campo = 0;
        boolean dentroAspas = false;
        String temp = "";

        for (int i = 0; i < linha.length(); i++) {
            char c = linha.charAt(i);
            if (c == '\"')
                dentroAspas = !dentroAspas;
            else if (c == ',' && !dentroAspas) {
                campos[campo++] = temp.trim();
                temp = "";
            } else
                temp += c;
        }
        campos[campo] = temp.trim();
        return campos;
    }

    private static boolean quemVemPrimeiro(String frase1, String frase2) {
        return primeiroRecursivo(frase1, frase2, 0);
    }

    private static boolean primeiroRecursivo(String frase1, String frase2, int pos) {
        boolean resp = true;

        if (pos >= frase1.length() || pos >= frase2.length()) {
            if (frase1.length() < frase2.length()) {
                resp = true;
            } else {
                resp = false;
            }
        } else if (frase1.charAt(pos) > frase2.charAt(pos)) {
            resp = false;
        } else if (frase1.charAt(pos) == frase2.charAt(pos)) {
            resp = primeiroRecursivo(frase1, frase2, pos + 1);
        }

        return resp;
    }

    public static void criarLog(int comparacoes, long inicio, long fim, PrintStream ps) {
        String matricula = "801757";
        String nomeArquivo = matricula + "_mergesort.txt";
        double tempoSegundos = (fim - inicio) / 1_000_000_000.0;
        String conteudo = matricula + "\t" + comparacoes + "\t" + movimentacoes + "\t" + tempoSegundos + "\n";

        try {
            FileWriter writer = new FileWriter(nomeArquivo);
            writer.write(conteudo);
            writer.close();

        } catch (IOException e) {
            ps.println("Erro ao criar o arquivo de log!");
        }
    }

    public static void main(String[] args) {
        PrintStream ps = new PrintStream(System.out, true, Charset.forName("UTF-8"));

        long inicio = System.nanoTime();
        Jogo[] jogos = new Jogo[1850];
        int idx = 0;

        try {
            Scanner nc = new Scanner(new File("/tmp/games.csv"));
            nc.useDelimiter("\n");

            if (nc.hasNext())
                nc.next();

            while (nc.hasNext() && idx < jogos.length) {
                String linha = nc.next();
                String[] campos = dividirCSV(linha);

                String[] idiomasSuportados = parseArrayGenerico(campos[5]);
                String[] categorias = parseArrayGenerico(campos[11]);
                String[] generos = parseArrayGenerico(campos[12]);
                String[] tags = parseArrayGenerico(campos[13]);

                Jogo jogo = new Jogo(
                        Integer.parseInt(campos[0]),
                        campos[1],
                        campos[2],
                        Integer.parseInt(campos[3]),
                        Float.parseFloat(campos[4]),
                        idiomasSuportados,
                        parseIntSeguro(campos[6]),
                        parseIntSeguro(campos[7]),
                        parseIntSeguro(campos[8]),
                        campos[9],
                        campos[10],
                        categorias,
                        generos,
                        tags);
                jogos[idx++] = jogo;
            }

            nc.close();

        } catch (Exception e) {
            ps.println("Erro ao ler o arquivo: " + e.getMessage());
        }

        OrdenacaoMerge ordenador = new OrdenacaoMerge();

        Scanner sc = new Scanner(System.in);
        String temp = sc.next();
        while (temp.charAt(0) != 'F') {
            int n = Integer.parseInt(temp);
            for (int i = 0; i < idx; i++) {
                if (jogos[i].getId() == n) {
                    ordenador.inserir(jogos[i]);
                }
            }
            temp = sc.next();
        }
        ordenador.ordenar();
        ordenador.imprimir(ps);
        long fim = System.nanoTime();
        criarLog(comparacao, inicio, fim, ps);
        sc.close();
    }
}
