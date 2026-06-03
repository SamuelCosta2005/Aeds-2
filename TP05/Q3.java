import java.io.*;
import java.util.*;
import java.nio.charset.StandardCharsets;

class Jogo {

    private int id;
    private String nome;
    private String dataLancamento;
    private int proprietariosEstimados;
    private float preco;
    private String[] idiomasSuportados;
    private float pontuacaoMedia; 
    private int conquistas;
    private float pontuacaoUsuario;
    private String[] editoras;
    private String[] desenvolvedores;
    private String[] categorias;
    private String[] generos;
    private String[] tags;

    public int getId() { return id; }
    public void setId(int id) { this.id = id; }
    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }
    public String getDataLancamento() { return dataLancamento; }
    public void setDataLancamento(String dataLancamento) { this.dataLancamento = dataLancamento; }
    public int getProprietariosEstimados() { return proprietariosEstimados; }
    public void setProprietariosEstimados(int proprietariosEstimados) { this.proprietariosEstimados = proprietariosEstimados; }
    public float getPreco() { return preco; }
    public void setPreco(float preco) { this.preco = preco; }
    public String[] getIdiomasSuportados() { return idiomasSuportados; }
    public void setIdiomasSuportados(String[] idiomasSuportados) { this.idiomasSuportados = idiomasSuportados; }
    public float getPontuacaoMedia() { return pontuacaoMedia; }
    public void setPontuacaoMedia(float pontuacaoMedia) { this.pontuacaoMedia = pontuacaoMedia; }
    public int getConquistas() { return conquistas; }
    public void setConquistas(int conquistas) { this.conquistas = conquistas; }
    public float getPontuacaoUsuario() { return pontuacaoUsuario; }
    public void setPontuacaoUsuario(float pontuacaoUsuario) { this.pontuacaoUsuario = pontuacaoUsuario; }
    public String[] getEditoras() { return editoras; }
    public void setEditoras(String[] editoras) { this.editoras = editoras; }
    public String[] getDesenvolvedores() { return desenvolvedores; }
    public void setDesenvolvedores(String[] desenvolvedores) { this.desenvolvedores = desenvolvedores; }
    public String[] getCategorias() { return categorias; }
    public void setCategorias(String[] categorias) { this.categorias = categorias; }
    public String[] getGeneros() { return generos; }
    public void setGeneros(String[] generos) { this.generos = generos; }
    public String[] getTags() { return tags; }
    public void setTags(String[] tags) { this.tags = tags; }

    public String toString() {
        return "=> " + id + " ## " + nome + " ## " + dataLancamento + " ## " + proprietariosEstimados + " ## " +
               String.format(java.util.Locale.US, "%.2f", preco) + " ## " +
               Arrays.toString(idiomasSuportados) + " ## " +
               ((int)pontuacaoMedia) + " ## " +
               ((float)conquistas) + " ## " + ((int)pontuacaoUsuario) + " ## " +
               Arrays.toString(editoras) + " ## " +
               Arrays.toString(desenvolvedores) + " ## " +
               Arrays.toString(categorias) + " ## " +
               Arrays.toString(generos) + " ## " +
               Arrays.toString(tags) + " ##";
    }
}

public class Q3 {
    static final String CAMINHO_CSV = "/tmp/games.csv";
    static final String MATRICULA = "801757";

    static long comparacoes = 0;
    static long movimentacoes = 0;

    public static void main(String[] args) {
        Map<Integer, Jogo> porId = carregarCSV(CAMINHO_CSV);

        List<Jogo> selecionados = new ArrayList<>();
        try (BufferedReader in = new BufferedReader(new InputStreamReader(System.in, StandardCharsets.UTF_8))) {
            String s;
            while ((s = in.readLine()) != null) {
                s = s.trim();
                if (s.equals("FIM")) break;
                if (s.isEmpty()) continue;
                Integer id = tentarParseInt(s);
                if (id != null) {
                    Jogo j = porId.get(id);
                    if (j != null) selecionados.add(j);
                }
            }
        } catch (IOException e) {
        }

        long inicio = System.nanoTime();
        ordenacaoHeap(selecionados);
        long fim = System.nanoTime();

        for (Jogo j : selecionados) System.out.println(j.toString());

        double ms = (fim - inicio) / 1_000_000.0;
        escreverLog(MATRICULA + "_heapsort.txt",
                MATRICULA + "\t" + comparacoes + "\t" + movimentacoes + "\t" + String.format(Locale.US, "%.3f", ms));
    }

    static Map<Integer, Jogo> carregarCSV(String caminho) {
        Map<Integer, Jogo> mapa = new HashMap<>();
        try (BufferedReader br = new BufferedReader(new FileReader(caminho, StandardCharsets.UTF_8))) {
            br.readLine();
            String linha;
            while ((linha = br.readLine()) != null) {
                String[] t = quebrarLinhaCSV(linha);
                if (t.length < 14) continue; 

                Jogo j = new Jogo();

                j.setId(converterInt(t[0]));
                j.setNome(t[1]);
                j.setDataLancamento(formatarData(t[2]));
                j.setProprietariosEstimados(converterInt(t[3]));
                j.setPreco(converterFloat(t[4]));
                j.setIdiomasSuportados(separarLista(t[5]));
                j.setPontuacaoMedia(converterFloat(t[6]));
                j.setConquistas(converterInt(t[7]));
                j.setPontuacaoUsuario(converterFloat(t[8]));
                j.setEditoras(separarLista(t[9]));
                j.setDesenvolvedores(separarLista(t[10]));
                j.setCategorias(separarLista(t[11]));
                j.setGeneros(separarLista(t[12]));
                j.setTags(separarLista(t[13]));

                mapa.put(j.getId(), j);
            }
        } catch (IOException e) {
        }
        return mapa;
    }

    static String[] quebrarLinhaCSV(String linha) {
        List<String> partes = new ArrayList<>();
        boolean aspas = false;
        StringBuilder atual = new StringBuilder();
        for (int i = 0; i < linha.length(); i++) {
            char c = linha.charAt(i);
            if (c == '"') aspas = !aspas;          
            else if (c == ',' && !aspas) {    
                partes.add(atual.toString());
                atual.setLength(0);
            } else atual.append(c);
        }
        partes.add(atual.toString());
        for (int i = 0; i < partes.size(); i++) partes.set(i, partes.get(i).trim());
        return partes.toArray(new String[0]);
    }

    static String formatarData(String raw) {
        if (raw == null || raw.isEmpty()) return "01/01/0000";
        raw = raw.replace("\"", "").trim();
        Map<String,String> meses = new HashMap<>();
        meses.put("Jan","01"); meses.put("Feb","02"); meses.put("Mar","03"); meses.put("Apr","04");
        meses.put("May","05"); meses.put("Jun","06"); meses.put("Jul","07"); meses.put("Aug","08");
        meses.put("Sep","09"); meses.put("Oct","10"); meses.put("Nov","11"); meses.put("Dec","12");
        try {
            String[] partes = raw.split(" ");
            String dia = "01", mes = "01", ano = "0000";
            if (partes.length == 3) {
                dia = partes[1].replace(",", "");
                mes = meses.getOrDefault(partes[0], "01");
                ano = partes[2];
            } else if (partes.length == 2) {
                mes = meses.getOrDefault(partes[0], "01");
                ano = partes[1];
            } else if (partes.length == 1) ano = partes[0];
            return String.format("%02d/%s/%s", Integer.parseInt(dia), mes, ano);
        } catch (Exception e) {
            return "01/01/0000"; 
        }
    }

    static String[] separarLista(String s) {
        if (s == null) return new String[0];
        s = s.replace("[", "").replace("]", "").replace("'", "").replace("\"", "").trim();
        if (s.isEmpty()) return new String[0];
        String[] arr = s.split(",");
        for (int i = 0; i < arr.length; i++) arr[i] = arr[i].trim();
        return arr;
    }

    static Integer tentarParseInt(String s) {
        try { return Integer.parseInt(s.trim()); } catch (Exception e) { return null; }
    }
    static int converterInt(String s) {
        try { return Integer.parseInt(s.trim()); } catch (Exception e) { return 0; }
    }
    static float converterFloat(String s) {
        try { return Float.parseFloat(s.trim()); } catch (Exception e) { return 0.0f; }
    }

    static void ordenacaoHeap(List<Jogo> a) {
        int n = a.size();
        for (int i = n/2 - 1; i >= 0; i--) construirHeap(a, n, i);
        for (int fim = n - 1; fim > 0; fim--) {
            trocar(a, 0, fim);
            construirHeap(a, fim, 0);
        }
    }

    static void construirHeap(List<Jogo> a, int tamanhoHeap, int i) {
        while (true) {
            int maior = i;
            int esq = 2*i + 1;
            int dir = 2*i + 2;

            if (esq < tamanhoHeap && maiorQue(a.get(esq), a.get(maior))) maior = esq;
            if (dir < tamanhoHeap && maiorQue(a.get(dir), a.get(maior))) maior = dir;

            if (maior != i) {
                trocar(a, i, maior);
                i = maior;
            } else break;
        }
    }

    static boolean maiorQue(Jogo a, Jogo b) {
        comparacoes++;
        if (a.getProprietariosEstimados() != b.getProprietariosEstimados())
            return a.getProprietariosEstimados() > b.getProprietariosEstimados();
        comparacoes++;
        return a.getId() > b.getId();
    }

    static void trocar(List<Jogo> a, int i, int j) {
        if (i == j) return;
        Jogo tmp = a.get(i);
        a.set(i, a.get(j));
        a.set(j, tmp);
        movimentacoes += 3;
    }

    static void escreverLog(String nomeArquivo, String linha) {
        try (PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream(nomeArquivo), StandardCharsets.UTF_8))) {
            pw.println(linha);
        } catch (IOException e) {
        }
    }
}