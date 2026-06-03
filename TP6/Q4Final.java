import java.io.*;
import java.util.*;

class Game {
    private int identificador;
    private String titulo;
    private String dataLancamento;
    private int proprietariosEstimados;
    private float preco;
    private String[] idiomasSuportados;
    private float pontuacaoMidia;
    private int conquistas;
    private float pontuacaoUsuario;
    private String[] distribuidores;
    private String[] desenvolvedores;
    private String[] categorias;
    private String[] generos;
    private String[] etiquetas;

    public int getIdentificador() { return identificador; }
    public void setIdentificador(int identificador) { this.identificador = identificador; }

    public String getTitulo() { return titulo; }
    public void setTitulo(String titulo) { this.titulo = titulo; }

    public String getDataLancamento() { return dataLancamento; }
    public void setDataLancamento(String dataLancamento) { this.dataLancamento = dataLancamento; }

    public int getProprietariosEstimados() { return proprietariosEstimados; }
    public void setProprietariosEstimados(int proprietariosEstimados) { this.proprietariosEstimados = proprietariosEstimados; }

    public float getPreco() { return preco; }
    public void setPreco(float preco) { this.preco = preco; }

    public String[] getIdiomasSuportados() { return idiomasSuportados; }
    public void setIdiomasSuportados(String[] idiomasSuportados) { this.idiomasSuportados = idiomasSuportados; }

    public float getPontuacaoMidia() { return pontuacaoMidia; }
    public void setPontuacaoMidia(float pontuacaoMidia) { this.pontuacaoMidia = pontuacaoMidia; }

    public int getConquistas() { return conquistas; }
    public void setConquistas(int conquistas) { this.conquistas = conquistas; }

    public float getPontuacaoUsuario() { return pontuacaoUsuario; }
    public void setPontuacaoUsuario(float pontuacaoUsuario) { this.pontuacaoUsuario = pontuacaoUsuario; }

    public String[] getDistribuidores() { return distribuidores; }
    public void setDistribuidores(String[] distribuidores) { this.distribuidores = distribuidores; }

    public String[] getDesenvolvedores() { return desenvolvedores; }
    public void setDesenvolvedores(String[] desenvolvedores) { this.desenvolvedores = desenvolvedores; }

    public String[] getCategorias() { return categorias; }
    public void setCategorias(String[] categorias) { this.categorias = categorias; }

    public String[] getGeneros() { return generos; }
    public void setGeneros(String[] generos) { this.generos = generos; }

    public String[] getEtiquetas() { return etiquetas; }
    public void setEtiquetas(String[] etiquetas) { this.etiquetas = etiquetas; }

    public String toString() {
        return "=> " + identificador + " ## " + titulo + " ## " + dataLancamento + " ## " + proprietariosEstimados + " ## " +
               String.format("%.2f", preco) + " ## " + Arrays.toString(idiomasSuportados).replaceAll("^\\[|\\]$", "") +
               " ## " + (int) pontuacaoMidia + " ## " + (float) conquistas + " ## " + (int) pontuacaoUsuario + " ## " +
               Arrays.toString(distribuidores) + " ## " + Arrays.toString(desenvolvedores) + " ## " +
               Arrays.toString(categorias) + " ## " + Arrays.toString(generos) + " ## " + Arrays.toString(etiquetas) + " ##";
    }
}

class No {
    Game jogo;
    No proximo;
    No(Game jogo) { this.jogo = jogo; this.proximo = null; }
}

class FilaGame {
    private No inicio;
    private No fim;
    private int contador;

    public FilaGame() {
        inicio = fim = null;
        contador = 0;
    }

    public void adicionar(Game jogo) {
        No novoNo = new No(jogo);
        if (fim == null) {
            inicio = fim = novoNo;
        } else {
            fim.proximo = novoNo;
            fim = novoNo;
        }
        contador++;
    }

    public Game remover() throws Exception {
        if (inicio == null) throw new Exception("Fila vazia");
        Game jogoRemovido = inicio.jogo;
        inicio = inicio.proximo;
        if (inicio == null) fim = null;
        contador--;
        return jogoRemovido;
    }

    public int obterTamanho() {
        return contador;
    }

    public Game obterElemento(int posicao) {
        No temporario = inicio;
        for (int k = 0; k < posicao; k++) temporario = temporario.proximo;
        return temporario.jogo;
    }
}

public class Q4Final {
    public static void main(String[] args) {
        String arquivoEntrada = "/tmp/games.csv";
        Map<Integer, Game> catalogoJogos = lerArquivoCSV(arquivoEntrada);
        FilaGame colecaoJogos = new FilaGame();

        try (BufferedReader leitor = new BufferedReader(new InputStreamReader(System.in))) {
            while (true) {
                String entrada = leitor.readLine();
                if (entrada == null) return;
                entrada = entrada.trim();
                if (entrada.equals("FIM")) break;
                int idJogo = converterParaInteiro(entrada);
                Game jogoSelecionado = catalogoJogos.get(idJogo);
                if (jogoSelecionado != null) colecaoJogos.adicionar(jogoSelecionado);
            }

            String entradaQuantidade = leitor.readLine();
            int quantidadeOperacoes = converterParaInteiro(entradaQuantidade);

            for (int i = 0; i < quantidadeOperacoes; i++) {
                String instrucao = leitor.readLine();
                if (instrucao == null) break;
                instrucao = instrucao.trim();

                if (instrucao.startsWith("I")) {
                    int idJogo = converterParaInteiro(instrucao.substring(1).trim());
                    Game novoJogo = catalogoJogos.get(idJogo);
                    if (novoJogo != null) colecaoJogos.adicionar(novoJogo);
                } else if (instrucao.equals("R")) {
                    try {
                        Game jogoRemovido = colecaoJogos.remover();
                        System.out.println("(R) " + jogoRemovido.getTitulo());
                    } catch (Exception e) {}
                }
            }

            int indice = 0;
            for (int i = 0; i < colecaoJogos.obterTamanho(); i++) {
                System.out.print("[" + indice + "] ");
                System.out.println(colecaoJogos.obterElemento(i));
                indice++;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static Map<Integer, Game> lerArquivoCSV(String caminhoArquivo) {
        Map<Integer, Game> mapaJogos = new HashMap<>();
        try (BufferedReader leitorArquivo = new BufferedReader(new FileReader(caminhoArquivo))) {
            leitorArquivo.readLine();
            String linha;
            while ((linha = leitorArquivo.readLine()) != null) {
                String[] dados = processarLinhaCSV(linha);
                if (dados.length < 14) continue;
                Game novoJogo = new Game();
                novoJogo.setIdentificador(converterParaInteiro(dados[0]));
                novoJogo.setTitulo(dados[1]);
                novoJogo.setDataLancamento(formatarData(dados[2]));
                novoJogo.setProprietariosEstimados(converterParaInteiro(dados[3]));
                novoJogo.setPreco(converterParaFloat(dados[4]));
                novoJogo.setIdiomasSuportados(separarItens(dados[5]));
                novoJogo.setPontuacaoMidia(converterParaFloat(dados[6]));
                novoJogo.setConquistas(converterParaInteiro(dados[7]));
                novoJogo.setPontuacaoUsuario(converterParaFloat(dados[8]));
                novoJogo.setDistribuidores(separarItens(dados[9]));
                novoJogo.setDesenvolvedores(separarItens(dados[10]));
                novoJogo.setCategorias(separarItens(dados[11]));
                novoJogo.setGeneros(separarItens(dados[12]));
                novoJogo.setEtiquetas(separarItens(dados[13]));
                mapaJogos.put(novoJogo.getIdentificador(), novoJogo);
            }
        } catch (Exception e) {}
        return mapaJogos;
    }

    public static String[] processarLinhaCSV(String linha) {
        List<String> campos = new ArrayList<>();
        boolean dentroAspas = false;
        StringBuilder campoAtual = new StringBuilder();
        for (int i = 0; i < linha.length(); i++) {
            char caractere = linha.charAt(i);
            if (caractere == '"') dentroAspas = !dentroAspas;
            else if (caractere == ',' && !dentroAspas) {
                campos.add(campoAtual.toString().trim());
                campoAtual.setLength(0);
            } else campoAtual.append(caractere);
        }
        campos.add(campoAtual.toString().trim());
        return campos.toArray(new String[0]);
    }

    public static String formatarData(String data) {
        if (data == null || data.isEmpty()) return "01/01/0000";
        Map<String, String> meses = new HashMap<>();
        meses.put("Jan", "01"); meses.put("Feb", "02"); meses.put("Mar", "03"); meses.put("Apr", "04");
        meses.put("May", "05"); meses.put("Jun", "06"); meses.put("Jul", "07"); meses.put("Aug", "08");
        meses.put("Sep", "09"); meses.put("Oct", "10"); meses.put("Nov", "11"); meses.put("Dec", "12");
        try {
            data = data.replace("\"", "").trim();
            String[] partes = data.split(" ");
            String dia = "01", mes = "01", ano = "0000";
            if (partes.length == 3) {
                dia = partes[1].replace(",", "");
                mes = meses.getOrDefault(partes[0], "01");
                ano = partes[2];
            } else if (partes.length == 2) {
                mes = meses.getOrDefault(partes[0], "01");
                ano = partes[1];
            } else if (partes.length == 1) {
                ano = partes[0];
            }
            return String.format("%02d/%s/%s", Integer.parseInt(dia), mes, ano);
        } catch (Exception e) { return "01/01/0000"; }
    }

    public static String[] separarItens(String texto) {
        if (texto == null || texto.isEmpty()) return new String[0];
        texto = texto.replace("[", "").replace("]", "").replace("\"", "").replace("'", "");
        String[] itens = texto.split(",");
        for (int i = 0; i < itens.length; i++) itens[i] = itens[i].trim();
        return itens;
    }

    public static int converterParaInteiro(String texto) {
        try { return Integer.parseInt(texto.trim()); } catch (Exception e) { return 0; }
    }

    public static float converterParaFloat(String texto) {
        try { return Float.parseFloat(texto.trim()); } catch (Exception e) { return 0f; }
    }
}
