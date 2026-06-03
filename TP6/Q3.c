#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_JOGOS 1850
#define MAX_LINHA 10000

typedef struct {
    char *dadosTemp;
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Jogo {
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
    char **editoras;
    int contagemEditoras;
    char **desenvolvedores;
    int contagemDesenvolvedores;
    char **categorias;
    int contagemCategorias;
    char **generos;
    int contagemGeneros;
    char **tags;
    int contagemTags;
    struct Jogo *prox;  //ponteiro para o próximo Jogo na lista/estrutura
} Jogo;
//Inicio Fila

typedef struct Pilha {
    Jogo* topo;
} Pilha;

Pilha* NovaPilha() {
    Pilha* a = (Pilha*) malloc(sizeof(Pilha));
    a->topo = (Jogo*) malloc(sizeof(Jogo));  // Nó cabeça
    a->topo->prox = NULL;
    return a;
}

void Inserir(Jogo* topo, Jogo* jogo) {
    jogo->prox = topo->prox;
    topo->prox = jogo;
}
static void imprimirJogo(const Jogo *j) {
    printf("=> %d ", j->appid);
    printf("## %s ", j->nome);
    printf("## %02d/%02d/%d ", j->data.dia, j->data.mes, j->data.ano);
    printf("## %d ", j->proprietariosEstimados);
    printf("## %g ", j->preco);
    printf("## [");
    if (j->contagemIdiomasSuportados > 1) {
        for (int i = 0; i < j->contagemIdiomasSuportados - 1; ++i)
            printf("%s, ", j->idiomasSuportados[i]);
        printf("%s", j->idiomasSuportados[j->contagemIdiomasSuportados - 1]);
    } else if (j->contagemIdiomasSuportados == 1) {
        printf("%s", j->idiomasSuportados[0]);
    }
    printf("] ");
    printf("## %d ", j->pontuacaoMetacritic);
    printf("## %g ", j->pontuacaoUsuario);
    printf("## %d ", j->conquistas);
    printf("## [");
    if (j->contagemEditoras > 1) {
        for (int i = 0; i < j->contagemEditoras - 1; ++i)
            printf("%s, ", j->editoras[i]);
        printf("%s", j->editoras[j->contagemEditoras - 1]);
    } else if (j->contagemEditoras == 1) {
        printf("%s", j->editoras[0]);
    }
    printf("] ");
    printf("## [");
    if (j->contagemDesenvolvedores > 1) {
        for (int i = 0; i < j->contagemDesenvolvedores - 1; ++i)
            printf("%s, ", j->desenvolvedores[i]);
        printf("%s", j->desenvolvedores[j->contagemDesenvolvedores - 1]);
    } else if (j->contagemDesenvolvedores == 1) {
        printf("%s", j->desenvolvedores[0]);
    }
    printf("] ");
    printf("## [");
    if (j->contagemCategorias > 1) {
        for (int i = 0; i < j->contagemCategorias - 1; ++i)
            printf("%s, ", j->categorias[i]);
        printf("%s", j->categorias[j->contagemCategorias - 1]);
    } else if (j->contagemCategorias == 1) {
        printf("%s", j->categorias[0]);
    }
    printf("] ");
    printf("## [");
    if (j->contagemGeneros > 1) {
        for (int i = 0; i < j->contagemGeneros - 1; ++i)
            printf("%s, ", j->generos[i]);
        printf("%s", j->generos[j->contagemGeneros - 1]);
    } else if (j->contagemGeneros == 1) {
        printf("%s", j->generos[0]);
    }
    printf("] ");
    printf("## [");
    if (j->contagemTags > 1) {
        for (int i = 0; i < j->contagemTags - 1; ++i)
            printf("%s, ", j->tags[i]);
        printf("%s", j->tags[j->contagemTags - 1]);
    } else if (j->contagemTags == 1) {
        printf("%s", j->tags[0]);
    }
    printf("] ##\n");
}
void mostrar(Jogo* topo) {
    int n=0;
    for (Jogo* i = topo->prox; i != NULL; i = i->prox) {
        printf("[%d] ",n);
        imprimirJogo(i);
    }
    printf("\n");
}

Jogo* Remove(Jogo* topo) {
    Jogo* tmp = topo->prox;
    if (tmp != NULL) {
        topo->prox = tmp->prox;
    } else {
        printf("V\n");
    }
    return tmp;
}
static char *duplicar_string_seguro(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    if (r) strcpy(r, s);
    return r;
}
static void remover_fim_linha(char *s) {
    int n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) { s[n-1] = '\0'; n--; }
}

static char *trim(char *s) {
    if (!s) return NULL;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return duplicar_string_seguro("");
    char *fim = s + strlen(s) - 1;
    while (fim > s && isspace((unsigned char)*fim)) fim--;
    int comprimento = (int)(fim - s) + 1;
    char *res = malloc(comprimento + 1);
    strncpy(res, s, comprimento);
    res[comprimento] = '\0';
    return res;
}
static char *remover_aspas(char *s) {
    int comprimento = strlen(s);
    if (comprimento >= 2 && s[0] == '"' && s[comprimento-1] == '"') {
        char *r = malloc(comprimento - 1);
        strncpy(r, s+1, comprimento-2);
        r[comprimento-2] = '\0';
        return r;
    }
    return duplicar_string_seguro(s);
}

static char **dividir_linha_csv(const char *linha, int *contagem) {
    char *buf = duplicar_string_seguro(linha);
    int comprimento = strlen(buf);
    char **partes = NULL;
    int capacidade_partes = 0;
    int contagem_partes = 0;
    int em_aspas = 0;
    int inicio = 0;
    for (int i = 0; i <= comprimento; ++i) {
        char c = (i < comprimento) ? buf[i] : '\0';
        if (c == '"') em_aspas = !em_aspas;
        if ((c == ',' && !em_aspas) || c == '\0') {
            int tamanho_parte = i - inicio;
            char *parte = malloc(tamanho_parte + 1);
            strncpy(parte, buf + inicio, tamanho_parte);
            parte[tamanho_parte] = '\0';
            if (contagem_partes + 1 > capacidade_partes) {
                capacidade_partes = capacidade_partes == 0 ? 8 : capacidade_partes * 2;
                partes = realloc(partes, sizeof(char*) * capacidade_partes);
            }
            partes[contagem_partes++] = parte;
            inicio = i + 1;
        }
    }
    free(buf);
    *contagem = contagem_partes;
    return partes;
}

static char **dividir_virgula_simples(const char *s, int *contagem) {
    char *buf = duplicar_string_seguro(s);
    int capacidade = 0, cnt = 0;
    char **arr = NULL;
    char *p = buf;
    char *tok;
    while ((tok = strsep(&p, ",")) != NULL) {
        char *ttrim = trim(tok);
        if (cnt + 1 > capacidade) {
            capacidade = capacidade == 0 ? 8 : capacidade * 2;
            arr = realloc(arr, sizeof(char*) * capacidade);
        }
        arr[cnt++] = ttrim;
    }
    free(buf);
    *contagem = cnt;
    return arr;
}

static char **stringParaArray(const char *string, int *contagemSaida) {
    int L = strlen(string);
    if (L < 4) { *contagemSaida = 0; return NULL; }
    int inicio = 2;
    int fim = L - 2;
    int subcomprimento = fim - inicio;
    char *sub = malloc(subcomprimento + 1);
    strncpy(sub, string + inicio, subcomprimento);
    sub[subcomprimento] = '\0';
    int cnt = 0;
    char **partes = dividir_virgula_simples(sub, &cnt);
    free(sub);
    if (cnt > 0) {
        char *primeiro = partes[0];
        int k = strlen(primeiro);
        if (k >= 2) {
            char *nf = malloc(k - 1);
            int idx = 0;
            for (int i = 1; i < k - 1; ++i) nf[idx++] = primeiro[i];
            nf[idx] = '\0';
            free(primeiro);
            partes[0] = nf;
        }
    }
    for (int i = 1; i < cnt; ++i) {
        char *t = partes[i];
        int k = strlen(t);
        if (k >= 4) {
            char *nf = malloc(k - 3);
            int idx = 0;
            for (int j = 2; j < k - 1; ++j) nf[idx++] = t[j];
            nf[idx] = '\0';
            free(t);
            partes[i] = nf;
        } else {
            char *tmp = trim(t);
            free(t);
            partes[i] = tmp;
        }
    }
    *contagemSaida = cnt;
    return partes;
}

static char **stringParaArray2(const char *string, int *contagemSaida) {
    char *temp;
    int L = strlen(string);
    if (L >= 1 && string[0] == '"') {
        temp = malloc(L - 1);
        strncpy(temp, string + 1, L - 2);
        temp[L - 2] = '\0';
    } else {
        temp = duplicar_string_seguro(string);
    }
    int cnt = 0;
    char **partes = dividir_virgula_simples(temp, &cnt);
    free(temp);
    *contagemSaida = cnt;
    return partes;
}

static char *trimIngles(const char *string) {
    int L = strlen(string);
    if (L <= 4) return duplicar_string_seguro("");
    int inicio = 2;
    int fim = L - 2;
    int comprimento = fim - inicio;
    char *r = malloc(comprimento + 1);
    strncpy(r, string + inicio, comprimento);
    r[comprimento] = '\0';
    return r;
}

static void liberar_array_strings(char **arr, int contagem) {
    if (!arr) return;
    for (int i = 0; i < contagem; ++i) if (arr[i]) free(arr[i]);
    free(arr);
}

static void Data_analisar(Data *d) {
    d->dia = 0; d->mes = 0; d->ano = 0;
    if (!d->dadosTemp) return;
    int L = strlen(d->dadosTemp);
    if (L == 0) return;
    if (d->dadosTemp[0] == '"') {
        char *novosDados = malloc(L - 1);
        int pos = 0;
        for (int i = 1; i < L - 1; ++i) novosDados[pos++] = d->dadosTemp[i];
        novosDados[pos] = '\0';
        char *copia = duplicar_string_seguro(novosDados);
        char *tok = NULL;
        char *saveptr = NULL;
        tok = strtok_r(copia, " ", &saveptr);
        char mesTok[16] = {0};
        char diaTok[16] = {0};
        char anoTok[16] = {0};
        if (tok) strncpy(mesTok, tok, sizeof(mesTok)-1);
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok) strncpy(diaTok, tok, sizeof(diaTok)-1);
        tok = strtok_r(NULL, " ", &saveptr);
        if (tok) strncpy(anoTok, tok, sizeof(anoTok)-1);
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
        free(novosDados);
    }
}

static Jogo *novoJogoDePartes(char **partes, int contagemPartes) {
    Jogo *j = calloc(1, sizeof(Jogo));
    j->appid = atoi(partes[0]);
    char *nomeTemp = partes[1];
    if (nomeTemp[0] == '"' && nomeTemp[strlen(nomeTemp)-1] == '"') {
        j->nome = remover_aspas(nomeTemp);
    } else {
        j->nome = duplicar_string_seguro(nomeTemp);
    }
    j->dataLancamento = duplicar_string_seguro(partes[2]);
    j->data.dadosTemp = j->dataLancamento;
    Data_analisar(&j->data);
    j->proprietariosEstimados = atoi(partes[3]);
    j->preco = (float)strtod(partes[4], NULL);
    if (strchr(partes[5], ',') != NULL) {
        j->idiomasSuportados = stringParaArray(partes[5], &j->contagemIdiomasSuportados);
    } else {
        j->contagemIdiomasSuportados = 1;
        j->idiomasSuportados = malloc(sizeof(char*));
        char *t = trimIngles(partes[5]);
        j->idiomasSuportados[0] = t;
    }
    j->pontuacaoMetacritic = atoi(partes[6]);
    j->pontuacaoUsuario = (float)strtod(partes[7], NULL);
    j->conquistas = atoi(partes[8]);
    if (strlen(partes[9]) > 0 && strchr(partes[9], ',') != NULL) {
        j->editoras = stringParaArray2(partes[9], &j->contagemEditoras);
    } else {
        j->editoras = dividir_virgula_simples(partes[9], &j->contagemEditoras);
    }
    if (strchr(partes[10], ',') != NULL) {
        j->desenvolvedores = stringParaArray2(partes[10], &j->contagemDesenvolvedores);
    } else {
        j->desenvolvedores = dividir_virgula_simples(partes[10], &j->contagemDesenvolvedores);
    }
    if (strchr(partes[11], ',') != NULL) {
        j->categorias = stringParaArray2(partes[11], &j->contagemCategorias);
    } else {
        j->categorias = dividir_virgula_simples(partes[11], &j->contagemCategorias);
    }
    if (strchr(partes[12], ',') != NULL) {
        j->generos = stringParaArray2(partes[12], &j->contagemGeneros);
    } else {
        j->generos = dividir_virgula_simples(partes[12], &j->contagemGeneros);
    }
    if (strchr(partes[13], ',') != NULL) {
        j->tags = stringParaArray2(partes[13], &j->contagemTags);
    } else {
        j->tags = dividir_virgula_simples(partes[13], &j->contagemTags);
    }
    return j;
}

static void liberarJogo(Jogo *j) {
    if (!j) return;
    if (j->nome) free(j->nome);
    if (j->dataLancamento) free(j->dataLancamento);
    liberar_array_strings(j->idiomasSuportados, j->contagemIdiomasSuportados);
    liberar_array_strings(j->editoras, j->contagemEditoras);
    liberar_array_strings(j->desenvolvedores, j->contagemDesenvolvedores);
    liberar_array_strings(j->categorias, j->contagemCategorias);
    liberar_array_strings(j->generos, j->contagemGeneros);
    liberar_array_strings(j->tags, j->contagemTags);
    free(j);
}

static Jogo *jogos[MAX_JOGOS];

static void lerCSV(const char *caminho, int *contagemSaida) {
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
            partes = realloc(partes, sizeof(char*) * 14);
            for (int k = contagemPartes; k < 14; ++k) partes[k] = duplicar_string_seguro("");
            contagemPartes = 14;
        }
        Jogo *j = novoJogoDePartes(partes, contagemPartes);
        jogos[idx++] = j;
        for (int k = 0; k < contagemPartes; ++k) free(partes[k]);
        free(partes);
    }
    fclose(f);
    *contagemSaida = idx;
}

int main(void) {
    int numJogos = 0;
    lerCSV("/tmp/games.csv", &numJogos);
    
    // Criar pilha vazia
    Pilha* pilha = NovaPilha();
    
    char entrada[256];
    
    // Fase 1: Ler IDs até "FIM" e inserir na pilha
    while (fgets(entrada, sizeof(entrada), stdin)) {
        remover_fim_linha(entrada);
        if (strcmp(entrada, "FIM") == 0) {
            break;
        }
        int id = atoi(entrada);
        // Buscar o jogo no array principal
        Jogo* jogoEncontrado = NULL;
        for (int i = 0; i < numJogos; ++i) {
            if (jogos[i]->appid == id) {
                jogoEncontrado = jogos[i];
                break;
            }
        }
        // Se encontrou, inserir na pilha
        if (jogoEncontrado != NULL) {
            Inserir(pilha->topo, jogoEncontrado);
        }
    }
    //quantidade de comandos
    int numComandos;
    if (fgets(entrada, sizeof(entrada), stdin)) {
        remover_fim_linha(entrada);
        numComandos = atoi(entrada);
    }
    //inserção e remoção
    for (int i = 0; i < numComandos; i++) {
        if (!fgets(entrada, sizeof(entrada), stdin)) break;
        remover_fim_linha(entrada);
        
        if (entrada[0] == 'I') {
            int id = atoi(entrada + 2);
            Jogo* jogoEncontrado = NULL;
            for (int j = 0; j < numJogos; ++j) {
                if (jogos[j]->appid == id) {
                    jogoEncontrado = jogos[j];
                    break;
                }
            }
            if (jogoEncontrado != NULL) {
                Inserir(pilha->topo, jogoEncontrado);
            }
        } else if (entrada[0] == 'R') {
            // Comando de remoção: "R"
            Jogo* removido = Remove(pilha->topo);
            if (removido != NULL) {
                printf("(R) %s\n", removido->nome);
                liberarJogo(removido);
            }
        }
    }
    
    // Mostrar pilha resultante
    printf("Pilha resultante:\n");
    mostrar(pilha->topo);
    
    // Liberar memória
    for (int i = 0; i < numJogos; ++i) {
        liberarJogo(jogos[i]);
    }
    // TODO: Implementar liberarPilha(pilha);
    
    return 0;
}