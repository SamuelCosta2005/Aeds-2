#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX_JOGOS 1850
#define MAX_LINHA 10000
#define TAMANHO_TABELA_HASH 21

typedef struct {
    char *dadosTemporarios;
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int appid;
    char *nome;
    char *dataLancamento;
    Data data;
    int proprietariosEstimados;
    float preco;
    char **idiomasSuportados;
    int contagemIdiomasSuportados;
    int pontuacaoMetacritic;
    float pontuacaoUsuario;
    int conquistas;
    char **editores;
    int contagemEditores;
    char **desenvolvedores;
    int contagemDesenvolvedores;
    char **categorias;
    int contagemCategorias;
    char **generos;
    int contagemGeneros;
    char **tags;
    int contagemTags;
} Jogo;

static char *duplicar_string(const char *s) {
    char *r = NULL;
    if (s) {
        int len = (int)strlen(s);
        r = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) r[i] = s[i];
        r[len] = '\0';
    } else {
        r = (char *)malloc(1);
        r[0] = '\0';
    }
    return r;
}

static void remover_fim_linha(char *s) {
    if (s) {
        int n = (int)strlen(s);
        while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
            s[n-1] = '\0';
            n--;
        }
    }
}

static char *aparar_string(char *s) {
    char *res = NULL;
    if (s) {
        int inicio = 0;
        int fim = (int)strlen(s) - 1;
        while (isspace((unsigned char)s[inicio])) inicio++;
        while (fim >= inicio && isspace((unsigned char)s[fim])) fim--;
        int len = fim - inicio + 1;
        res = (char *)malloc(len + 1);
        for (int i = 0; i < len; i++) res[i] = s[inicio + i];
        res[len] = '\0';
    } else {
        res = duplicar_string("");
    }
    return res;
}

static char *remover_aspas(char *s) {
    char *res = NULL;
    if (s) {
        int len = (int)strlen(s);
        if (len >= 2 && s[0] == '"' && s[len-1] == '"') {
            res = (char *)malloc(len - 1);
            for (int i = 1; i < len - 1; i++) res[i-1] = s[i];
            res[len-2] = '\0';
        } else {
            res = duplicar_string(s);
        }
    } else {
        res = duplicar_string("");
    }
    return res;
}

static char **dividir_linha_csv(const char *linha, int *contagem) {
    char **partes = NULL;
    int capacidade = 0, cnt = 0;
    int len = (int)strlen(linha);
    char *buf = duplicar_string(linha);
    int inicio = 0;
    int dentro_aspas = 0;
    for (int i = 0; i <= len; i++) {
        char c = (i < len) ? buf[i] : '\0';
        if (c == '"') dentro_aspas = !dentro_aspas;
        if ((c == ',' && !dentro_aspas) || c == '\0') {
            int tamanho_parte = i - inicio;
            char *parte = (char *)malloc(tamanho_parte + 1);
            for (int j = 0; j < tamanho_parte; j++) parte[j] = buf[inicio + j];
            parte[tamanho_parte] = '\0';
            if (cnt >= capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                partes = (char **)realloc(partes, sizeof(char*) * capacidade);
            }
            partes[cnt++] = parte;
            inicio = i + 1;
        }
    }
    free(buf);
    *contagem = cnt;
    return partes;
}

static char **dividir_virgula_simples(const char *s, int *contagem) {
    char **arr = NULL;
    int capacidade = 0, cnt = 0;
    int inicio = 0;
    int len = (int)strlen(s);
    for (int i = 0; i <= len; i++) {
        if (s[i] == ',' || s[i] == '\0') {
            int tamanho_parte = i - inicio;
            char *tmp = (char *)malloc(tamanho_parte + 1);
            for (int j = 0; j < tamanho_parte; j++) tmp[j] = s[inicio + j];
            tmp[tamanho_parte] = '\0';
            char *aparado = aparar_string(tmp);
            free(tmp);
            if (cnt >= capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                arr = (char **)realloc(arr, sizeof(char*) * capacidade);
            }
            arr[cnt++] = aparado;
            inicio = i + 1;
        }
    }
    *contagem = cnt;
    return arr;
}

static char **string_para_array(const char *string, int *contagemSaida) {
    char **arr = NULL;
    int L = (int)strlen(string);
    if (L < 4) {
        *contagemSaida = 0;
        return NULL;
    }
    int inicio = 2;
    int fim = L - 2;
    int sublen = fim - inicio;
    char *sub = (char *)malloc(sublen + 1);
    for (int i = 0; i < sublen; i++) sub[i] = string[inicio + i];
    sub[sublen] = '\0';
    arr = dividir_virgula_simples(sub, contagemSaida);
    free(sub);
    return arr;
}

static void liberar_array_strings(char **arr, int contagem) {
    if (arr) {
        for (int i = 0; i < contagem; i++) if (arr[i]) free(arr[i]);
        free(arr);
    }
}

static void analisar_data(Data *d) {
    d->dia = d->mes = d->ano = 0;
    if (!d->dadosTemporarios) return;
    int L = (int)strlen(d->dadosTemporarios);
    if (L == 0) return;
    char *copia = duplicar_string(d->dadosTemporarios);
    if (copia[0] == '"') {
        for (int i = 0; i < L-2; i++) copia[i] = copia[i+1];
        copia[L-2] = '\0';
    }
    char mesTok[16] = {0}, diaTok[16] = {0}, anoTok[16] = {0};
    int idx = 0, tok_idx = 0, campo = 0;
    int continuar = 1;
    for (int i = 0; continuar; i++) {
        char c = copia[i];
        if (c == ' ' || c == '\0') {
            if (campo == 0) { mesTok[tok_idx] = '\0'; campo++; }
            else if (campo == 1) { diaTok[tok_idx] = '\0'; campo++; }
            else if (campo == 2) { anoTok[tok_idx] = '\0'; campo++; }
            tok_idx = 0;
            if (c == '\0') continuar = 0;
        } else {
            if (campo == 0) mesTok[tok_idx++] = c;
            else if (campo == 1) diaTok[tok_idx++] = c;
            else if (campo == 2) anoTok[tok_idx++] = c;
        }
        if (c == '\0') continuar = 0;
    }
    char *virgula = strchr(diaTok, ',');
    if (virgula) *virgula = '\0';
    d->dia = atoi(diaTok);
    d->ano = atoi(anoTok);
    if (strcmp(mesTok, "Jan") == 0) d->mes = 1;
    else if (strcmp(mesTok, "Feb") == 0) d->mes = 2;
    else if (strcmp(mesTok, "Mar") == 0) d->mes = 3;
    else if (strcmp(mesTok, "Apr") == 0) d->mes = 4;
    else if (strcmp(mesTok, "May") == 0) d->mes = 5;
    else if (strcmp(mesTok, "Jun") == 0) d->mes = 6;
    else if (strcmp(mesTok, "Jul") == 0) d->mes = 7;
    else if (strcmp(mesTok, "Aug") == 0) d->mes = 8;
    else if (strcmp(mesTok, "Sep") == 0) d->mes = 9;
    else if (strcmp(mesTok, "Oct") == 0) d->mes = 10;
    else if (strcmp(mesTok, "Nov") == 0) d->mes = 11;
    else if (strcmp(mesTok, "Dec") == 0) d->mes = 12;
    else d->mes = 1;
    free(copia);
}

static Jogo *criar_jogo_partes(char **partes, int contagemPartes) {
    Jogo *g = (Jogo *)calloc(1, sizeof(Jogo));
    g->appid = atoi(partes[0]);
    g->nome = remover_aspas(partes[1]);
    g->dataLancamento = duplicar_string(partes[2]);
    g->data.dadosTemporarios = g->dataLancamento;
    analisar_data(&g->data);
    g->proprietariosEstimados = atoi(partes[3]);
    g->preco = (float)strtod(partes[4], NULL);
    g->idiomasSuportados = string_para_array(partes[5], &g->contagemIdiomasSuportados);
    g->pontuacaoMetacritic = atoi(partes[6]);
    g->pontuacaoUsuario = (float)strtod(partes[7], NULL);
    g->conquistas = atoi(partes[8]);
    g->editores = string_para_array(partes[9], &g->contagemEditores);
    g->desenvolvedores = string_para_array(partes[10], &g->contagemDesenvolvedores);
    g->categorias = string_para_array(partes[11], &g->contagemCategorias);
    g->generos = string_para_array(partes[12], &g->contagemGeneros);
    g->tags = string_para_array(partes[13], &g->contagemTags);
    return g;
}

static void liberar_jogo(Jogo *g) {
    if (!g) return;
    if (g->nome) free(g->nome);
    if (g->dataLancamento) free(g->dataLancamento);
    liberar_array_strings(g->idiomasSuportados, g->contagemIdiomasSuportados);
    liberar_array_strings(g->editores, g->contagemEditores);
    liberar_array_strings(g->desenvolvedores, g->contagemDesenvolvedores);
    liberar_array_strings(g->categorias, g->contagemCategorias);
    liberar_array_strings(g->generos, g->contagemGeneros);
    liberar_array_strings(g->tags, g->contagemTags);
    free(g);
}

static void imprimir_jogo(const Jogo *g) {
    printf("=> %d ", g->appid);
    printf("## %s ", g->nome);
    printf("## %02d/%02d/%d ", g->data.dia, g->data.mes, g->data.ano);
    printf("## %d ", g->proprietariosEstimados);
    printf("## %g ", g->preco);

    printf("## [");
    for (int i = 0; i < g->contagemIdiomasSuportados; i++) {
        printf("%s", g->idiomasSuportados[i]);
        if (i < g->contagemIdiomasSuportados - 1) printf(", ");
    }
    printf("] ");

    printf("## %d ", g->pontuacaoMetacritic);
    printf("## %g ", g->pontuacaoUsuario);
    printf("## %d ", g->conquistas);

    char **arrays[] = { g->editores, g->desenvolvedores, g->categorias, g->generos, g->tags };
    int contagens[] = { g->contagemEditores, g->contagemDesenvolvedores, g->contagemCategorias, g->contagemGeneros, g->contagemTags };

    for (int j = 0; j < 5; j++) {
        printf("## [");
        for (int i = 0; i < contagens[j]; i++) {
            printf("%s", arrays[j][i]);
            if (i < contagens[j]-1) printf(", ");
        }
        printf("] ");
    }

    printf("##\n");
}

static Jogo *jogos[MAX_JOGOS];

static void ler_csv(const char *caminho, int *contagemSaida) {
    FILE *f = fopen(caminho, "r");
    if (!f) { perror("Erro ao abrir CSV"); *contagemSaida = 0; return; }
    char linha[MAX_LINHA];
    if (!fgets(linha, sizeof(linha), f)) { fclose(f); *contagemSaida = 0; return; }
    int idx = 0;
    while (fgets(linha, sizeof(linha), f) && idx < MAX_JOGOS) {
        remover_fim_linha(linha);
        int contagemPartes = 0;
        char **partes = dividir_linha_csv(linha, &contagemPartes);
        if (contagemPartes < 14) {
            partes = (char **)realloc(partes, sizeof(char*) * 14);
            for (int k = contagemPartes; k < 14; k++) partes[k] = duplicar_string("");
            contagemPartes = 14;
        }
        Jogo *g = criar_jogo_partes(partes, contagemPartes);
        jogos[idx++] = g;
        for (int k = 0; k < contagemPartes; k++) free(partes[k]);
        free(partes);
    }
    fclose(f);
    *contagemSaida = idx;
}

typedef struct NoHash {
    Jogo *jogo;
    struct NoHash *proximo;
} NoHash;

typedef struct {
    NoHash **tabela;
    int tamanho;
} TabelaHash;

static int funcao_hash(const char *nome) {
    int soma = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        soma += (int)nome[i];
    }
    return soma % TAMANHO_TABELA_HASH;
}

static TabelaHash *criar_tabela_hash() {
    TabelaHash *th = (TabelaHash *)malloc(sizeof(TabelaHash));
    th->tamanho = TAMANHO_TABELA_HASH;
    th->tabela = (NoHash **)calloc(TAMANHO_TABELA_HASH, sizeof(NoHash *));
    return th;
}

static void inserir_tabela_hash(TabelaHash *th, Jogo *jogo) {
    if (!th || !jogo) return;
    
    int indice = funcao_hash(jogo->nome);
    
    NoHash *novoNo = (NoHash *)malloc(sizeof(NoHash));
    novoNo->jogo = jogo;
    novoNo->proximo = NULL;
    
    if (th->tabela[indice] == NULL) {
        th->tabela[indice] = novoNo;
    } else {
        novoNo->proximo = th->tabela[indice];
        th->tabela[indice] = novoNo;
    }
}

static void pesquisar_tabela_hash(TabelaHash *th, const char *nome) {
    if (!th || !nome) return;
    
    int indice = funcao_hash(nome);
    int encontrado = 0;
    
    char *nomeLimpo = remover_aspas((char*)nome);
    
    NoHash *atual = th->tabela[indice];
    while (atual != NULL && encontrado == 0) {
        char *nomeJogoLimpo = remover_aspas(atual->jogo->nome);
        
        if (strcmp(nomeJogoLimpo, nomeLimpo) == 0) {
            encontrado = 1;
            free(nomeJogoLimpo);
        } else {
            free(nomeJogoLimpo);
            atual = atual->proximo;
        }
    }
    
    printf("%s:  (Posicao: %d) ", nomeLimpo, indice);
    if (encontrado) {
        printf("SIM\n");
    } else {
        printf("NAO\n");
    }
    
    free(nomeLimpo);
}

static void liberar_tabela_hash(TabelaHash *th, int liberarJogos) {
    if (!th) return;
    
    for (int i = 0; i < th->tamanho; i++) {
        NoHash *atual = th->tabela[i];
        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->proximo;
            if (liberarJogos) {
                liberar_jogo(temp->jogo);
            }
            free(temp);
        }
    }
    free(th->tabela);
    free(th);
}

int main() {
    clock_t start_time, end_time;
    double total_time = 0.0;
    int total_comparisons = 0;
    start_time = clock();
    int numJogos = 0;
    ler_csv("/tmp/games.csv", &numJogos);
    
    char entrada[256];
    
    TabelaHash *tabelaHash = criar_tabela_hash();
    
    fgets(entrada, sizeof(entrada), stdin);
    remover_fim_linha(entrada);
    
    while (strcmp(entrada, "FIM") != 0) {
        int id = atoi(entrada);
        
        int i = 0;
        int encontrado = 0;
        while (i < numJogos && encontrado == 0) {
            if (jogos[i]->appid == id) {
                inserir_tabela_hash(tabelaHash, jogos[i]);
                encontrado = 1;
            }
            i++;
        }
        
        fgets(entrada, sizeof(entrada), stdin);
        remover_fim_linha(entrada);
    }
    
    fgets(entrada, sizeof(entrada), stdin);
    remover_fim_linha(entrada);
    
    while (strcmp(entrada, "FIM") != 0) {
        pesquisar_tabela_hash(tabelaHash, entrada);
        
        fgets(entrada, sizeof(entrada), stdin);
        remover_fim_linha(entrada);
    }
    
    for (int i = 0; i < tabelaHash->tamanho; i++) {
        NoHash *atual = tabelaHash->tabela[i];
        while (atual != NULL) {
            NoHash *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(tabelaHash->tabela);
    free(tabelaHash);
    
    for (int i = 0; i < numJogos; i++) {
        liberar_jogo(jogos[i]);
    }
    end_time = clock();
    total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    FILE *log_file = fopen("801757_hashIndireta.txt", "w");
    if (log_file != NULL) {
        fprintf(log_file, "801757\t%.5f\t%d\n", total_time, total_comparisons);
        fclose(log_file);
    } else {
        printf("Erro ao criar arquivo de log.\n");
    }
    return 0;
}