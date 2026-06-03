#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_MAXIMO_LINHA 10000

typedef struct {
    char *dadosTemporarios;
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int idAplicativo;
    char *nome;
    char *dataLancamento;
    Data data;
    int proprietariosEstimados;
    float preco;
    char **idiomasSuportados;
    int quantidadeIdiomas;
    int pontuacaoMetacritic;
    float pontuacaoUsuario;
    int conquistas;
    char **editoras;
    int quantidadeEditoras;
    char **desenvolvedores;
    int quantidadeDesenvolvedores;
    char **categorias;
    int quantidadeCategorias;
    char **generos;
    int quantidadeGeneros;
    char **tags;
    int quantidadeTags;
} Jogo;

typedef struct No {
    Jogo *jogo;
    struct No *proximo;
} No;

typedef struct {
    No *primeiro;
    No *ultimo;
    int tamanho;
} Lista;

char *duplicarString(const char *stringOriginal) {
    if (!stringOriginal) return NULL;
    int comprimento = strlen(stringOriginal);
    char *copia = (char *)malloc(comprimento + 1);
    strcpy(copia, stringOriginal);
    return copia;
}

static void removerCaracteresFinais(char *string) {
    if (!string) return;
    int comprimento = strlen(string);
    while (comprimento > 0 && (string[comprimento-1] == '\n' || string[comprimento-1] == '\r')) {
        string[comprimento-1] = '\0';
        comprimento--;
    }
}

static char *limparEspacos(char *string) {
    if (!string) return duplicarString("");
    int inicio = 0;
    int fim = strlen(string) - 1;
    while (isspace((unsigned char)string[inicio])) inicio++;
    while (fim >= inicio && isspace((unsigned char)string[fim])) fim--;
    int comprimento = fim - inicio + 1;
    char *resultado = (char *)malloc(comprimento + 1);
    strncpy(resultado, string + inicio, comprimento);
    resultado[comprimento] = '\0';
    return resultado;
}

static char *removerAspas(char *string) {
    if (!string) return duplicarString("");
    int comprimento = strlen(string);
    if (comprimento >= 2 && string[0] == '"' && string[comprimento-1] == '"') {
        char *resultado = (char *)malloc(comprimento - 1);
        strncpy(resultado, string + 1, comprimento - 2);
        resultado[comprimento-2] = '\0';
        return resultado;
    }
    return duplicarString(string);
}

static char **dividirLinhaCSV(const char *linha, int *contador) {
    char **partes = NULL;
    int capacidade = 0, quantidade = 0;
    int comprimento = strlen(linha);
    char *buffer = duplicarString(linha);
    int inicio = 0;
    int dentroDeAspas = 0;
    
    for (int i = 0; i <= comprimento; i++) {
        char caractere = (i < comprimento) ? buffer[i] : '\0';
        if (caractere == '"') dentroDeAspas = !dentroDeAspas;
        if ((caractere == ',' && !dentroDeAspas) || caractere == '\0') {
            int tamanhoParte = i - inicio;
            char *parte = (char *)malloc(tamanhoParte + 1);
            strncpy(parte, buffer + inicio, tamanhoParte);
            parte[tamanhoParte] = '\0';
            
            if (quantidade >= capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                partes = (char **)realloc(partes, sizeof(char*) * capacidade);
            }
            partes[quantidade++] = parte;
            inicio = i + 1;
        }
    }
    free(buffer);
    *contador = quantidade;
    return partes;
}

static char **dividirPorVirgula(const char *string, int *contador) {
    char **array = NULL;
    int capacidade = 0, quantidade = 0;
    int inicio = 0;
    int comprimento = strlen(string);
    
    for (int i = 0; i <= comprimento; i++) {
        if (string[i] == ',' || string[i] == '\0') {
            int tamanhoParte = i - inicio;
            char *temporario = (char *)malloc(tamanhoParte + 1);
            strncpy(temporario, string + inicio, tamanhoParte);
            temporario[tamanhoParte] = '\0';
            
            char *limpo = limparEspacos(temporario);
            free(temporario);
            
            if (quantidade >= capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                array = (char **)realloc(array, sizeof(char*) * capacidade);
            }
            array[quantidade++] = limpo;
            inicio = i + 1;
        }
    }
    *contador = quantidade;
    return array;
}

static char **converterStringParaArray(const char *string, int *quantidade) {
    int comprimento = strlen(string);
    if (comprimento < 4) {
        *quantidade = 0;
        return NULL;
    }
    
    int inicio = 2;
    int fim = comprimento - 2;
    int tamanhoSubstring = fim - inicio;
    char *substring = (char *)malloc(tamanhoSubstring + 1);
    strncpy(substring, string + inicio, tamanhoSubstring);
    substring[tamanhoSubstring] = '\0';
    
    char **array = dividirPorVirgula(substring, quantidade);
    free(substring);
    return array;
}

static void liberarArrayStrings(char **array, int quantidade) {
    if (!array) return;
    for (int i = 0; i < quantidade; i++) free(array[i]);
    free(array);
}

static void analisarData(Data *data) {
    data->dia = data->mes = data->ano = 0;
    if (!data->dadosTemporarios) return;
    
    char *copia = duplicarString(data->dadosTemporarios);
    if (copia[0] == '"') {
        int comprimento = strlen(copia);
        for (int i = 0; i < comprimento-2; i++) copia[i] = copia[i+1];
        copia[comprimento-2] = '\0';
    }
    
    char tokenMes[16] = {0}, tokenDia[16] = {0}, tokenAno[16] = {0};
    int indice = 0, indiceToken = 0, campo = 0;
    
    for (int i = 0; ; i++) {
        char caractere = copia[i];
        if (caractere == ' ' || caractere == '\0') {
            if (campo == 0) { tokenMes[indiceToken] = '\0'; campo++; }
            else if (campo == 1) { tokenDia[indiceToken] = '\0'; campo++; }
            else if (campo == 2) { tokenAno[indiceToken] = '\0'; break; }
            indiceToken = 0;
        } else {
            if (campo == 0) tokenMes[indiceToken++] = caractere;
            else if (campo == 1) tokenDia[indiceToken++] = caractere;
            else if (campo == 2) tokenAno[indiceToken++] = caractere;
        }
        if (caractere == '\0') break;
    }
    
    char *virgula = strchr(tokenDia, ',');
    if (virgula) *virgula = '\0';
    
    data->dia = atoi(tokenDia);
    data->ano = atoi(tokenAno);
    
    if (strcmp(tokenMes, "Jan") == 0) data->mes = 1;
    else if (strcmp(tokenMes, "Feb") == 0) data->mes = 2;
    else if (strcmp(tokenMes, "Mar") == 0) data->mes = 3;
    else if (strcmp(tokenMes, "Apr") == 0) data->mes = 4;
    else if (strcmp(tokenMes, "May") == 0) data->mes = 5;
    else if (strcmp(tokenMes, "Jun") == 0) data->mes = 6;
    else if (strcmp(tokenMes, "Jul") == 0) data->mes = 7;
    else if (strcmp(tokenMes, "Aug") == 0) data->mes = 8;
    else if (strcmp(tokenMes, "Sep") == 0) data->mes = 9;
    else if (strcmp(tokenMes, "Oct") == 0) data->mes = 10;
    else if (strcmp(tokenMes, "Nov") == 0) data->mes = 11;
    else if (strcmp(tokenMes, "Dec") == 0) data->mes = 12;
    else data->mes = 1;
    
    free(copia);
}

static Jogo *criarJogoAPartirDePartes(char **partes, int quantidadePartes) {
    Jogo *novoJogo = (Jogo *)calloc(1, sizeof(Jogo));
    novoJogo->idAplicativo = atoi(partes[0]);
    novoJogo->nome = removerAspas(partes[1]);
    novoJogo->dataLancamento = duplicarString(partes[2]);
    novoJogo->data.dadosTemporarios = novoJogo->dataLancamento;
    analisarData(&novoJogo->data);
    novoJogo->proprietariosEstimados = atoi(partes[3]);
    novoJogo->preco = (float)atof(partes[4]);
    novoJogo->idiomasSuportados = converterStringParaArray(partes[5], &novoJogo->quantidadeIdiomas);
    novoJogo->pontuacaoMetacritic = atoi(partes[6]);
    novoJogo->pontuacaoUsuario = (float)atof(partes[7]);
    novoJogo->conquistas = atoi(partes[8]);
    novoJogo->editoras = converterStringParaArray(partes[9], &novoJogo->quantidadeEditoras);
    novoJogo->desenvolvedores = converterStringParaArray(partes[10], &novoJogo->quantidadeDesenvolvedores);
    novoJogo->categorias = converterStringParaArray(partes[11], &novoJogo->quantidadeCategorias);
    novoJogo->generos = converterStringParaArray(partes[12], &novoJogo->quantidadeGeneros);
    novoJogo->tags = converterStringParaArray(partes[13], &novoJogo->quantidadeTags);
    return novoJogo;
}

static void liberarJogo(Jogo *jogo) {
    if (!jogo) return;
    if (jogo->nome) free(jogo->nome);
    if (jogo->dataLancamento) free(jogo->dataLancamento);
    liberarArrayStrings(jogo->idiomasSuportados, jogo->quantidadeIdiomas);
    liberarArrayStrings(jogo->editoras, jogo->quantidadeEditoras);
    liberarArrayStrings(jogo->desenvolvedores, jogo->quantidadeDesenvolvedores);
    liberarArrayStrings(jogo->categorias, jogo->quantidadeCategorias);
    liberarArrayStrings(jogo->generos, jogo->quantidadeGeneros);
    liberarArrayStrings(jogo->tags, jogo->quantidadeTags);
    free(jogo);
}

static void imprimirJogo(const Jogo *jogo) {
    printf("=> %d ", jogo->idAplicativo);
    printf("## %s ", jogo->nome);
    printf("## %02d/%02d/%d ", jogo->data.dia, jogo->data.mes, jogo->data.ano);
    printf("## %d ", jogo->proprietariosEstimados);
    printf("## %.2f ", jogo->preco);

    printf("## [");
    for (int i = 0; i < jogo->quantidadeIdiomas; i++) {
        printf("%s", jogo->idiomasSuportados[i]);
        if (i < jogo->quantidadeIdiomas - 1) printf(", ");
    }
    printf("] ");

    printf("## %d ", jogo->pontuacaoMetacritic);
    printf("## %.1f ", jogo->pontuacaoUsuario);
    printf("## %d ", jogo->conquistas);

    printf("## [");
    for (int i = 0; i < jogo->quantidadeEditoras; i++) {
        printf("%s", jogo->editoras[i]);
        if (i < jogo->quantidadeEditoras - 1) printf(", ");
    }
    printf("] ");

    printf("## [");
    for (int i = 0; i < jogo->quantidadeDesenvolvedores; i++) {
        printf("%s", jogo->desenvolvedores[i]);
        if (i < jogo->quantidadeDesenvolvedores - 1) printf(", ");
    }
    printf("] ");

    printf("## [");
    for (int i = 0; i < jogo->quantidadeCategorias; i++) {
        printf("%s", jogo->categorias[i]);
        if (i < jogo->quantidadeCategorias - 1) printf(", ");
    }
    printf("] ");

    printf("## [");
    for (int i = 0; i < jogo->quantidadeGeneros; i++) {
        printf("%s", jogo->generos[i]);
        if (i < jogo->quantidadeGeneros - 1) printf(", ");
    }
    printf("] ");

    printf("## [");
    for (int i = 0; i < jogo->quantidadeTags; i++) {
        printf("%s", jogo->tags[i]);
        if (i < jogo->quantidadeTags - 1) printf(", ");
    }
    printf("] ##\n");
}

void inicializarLista(Lista *lista) {
    lista->primeiro = (No *)malloc(sizeof(No));
    lista->ultimo = lista->primeiro;
    lista->primeiro->proximo = NULL;
    lista->tamanho = 0;
}

void inserirNoInicio(Lista *lista, Jogo *jogo) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->jogo = jogo;
    novoNo->proximo = lista->primeiro->proximo;
    lista->primeiro->proximo = novoNo;
    if (lista->primeiro == lista->ultimo) {
        lista->ultimo = novoNo;
    }
    lista->tamanho++;
}

void inserirNaPosicao(Lista *lista, Jogo *jogo, int posicao) {
    if (posicao < 0 || posicao > lista->tamanho) return;
    
    if (posicao == 0) {
        inserirNoInicio(lista, jogo);
        return;
    }
    
    No *noAtual = lista->primeiro;
    for (int j = 0; j < posicao; j++) {
        noAtual = noAtual->proximo;
    }
    
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->jogo = jogo;
    novoNo->proximo = noAtual->proximo;
    noAtual->proximo = novoNo;
    
    if (novoNo->proximo == NULL) {
        lista->ultimo = novoNo;
    }
    lista->tamanho++;
}

void inserirNoFim(Lista *lista, Jogo *jogo) {
    lista->ultimo->proximo = (No *)malloc(sizeof(No));
    lista->ultimo = lista->ultimo->proximo;
    lista->ultimo->jogo = jogo;
    lista->ultimo->proximo = NULL;
    lista->tamanho++;
}

Jogo *removerDoInicio(Lista *lista) {
    if (lista->primeiro == lista->ultimo) return NULL;
    
    No *temporario = lista->primeiro->proximo;
    Jogo *jogoRemovido = temporario->jogo;
    lista->primeiro->proximo = temporario->proximo;
    
    if (temporario == lista->ultimo) {
        lista->ultimo = lista->primeiro;
    }
    
    free(temporario);
    lista->tamanho--;
    return jogoRemovido;
}

Jogo *removerDaPosicao(Lista *lista, int posicao) {
    if (posicao < 0 || posicao >= lista->tamanho) return NULL;
    
    if (posicao == 0) {
        return removerDoInicio(lista);
    }
    
    No *noAtual = lista->primeiro;
    for (int j = 0; j < posicao; j++) {
        noAtual = noAtual->proximo;
    }
    
    No *temporario = noAtual->proximo;
    Jogo *jogoRemovido = temporario->jogo;
    noAtual->proximo = temporario->proximo;
    
    if (temporario == lista->ultimo) {
        lista->ultimo = noAtual;
    }
    
    free(temporario);
    lista->tamanho--;
    return jogoRemovido;
}

Jogo *removerDoFim(Lista *lista) {
    if (lista->primeiro == lista->ultimo) return NULL;
    
    No *noAtual = lista->primeiro;
    while (noAtual->proximo != lista->ultimo) {
        noAtual = noAtual->proximo;
    }
    
    No *temporario = lista->ultimo;
    Jogo *jogoRemovido = temporario->jogo;
    lista->ultimo = noAtual;
    lista->ultimo->proximo = NULL;
    
    free(temporario);
    lista->tamanho--;
    return jogoRemovido;
}

void imprimirLista(Lista *lista) {
    for (No *noAtual = lista->primeiro->proximo; noAtual != NULL; noAtual = noAtual->proximo) {
        imprimirJogo(noAtual->jogo);
    }
}

void liberarLista(Lista *lista) {
    No *noAtual = lista->primeiro;
    while (noAtual != NULL) {
        No *temporario = noAtual;
        noAtual = noAtual->proximo;
        if (temporario->jogo) liberarJogo(temporario->jogo);
        free(temporario);
    }
}

static Jogo **lerArquivoCSV(const char *caminho, int *quantidadeJogos) {
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) { 
        printf("Erro ao abrir CSV: %s\n", caminho); 
        *quantidadeJogos = 0; 
        return NULL; 
    }
    
    char linha[TAMANHO_MAXIMO_LINHA];
    if (!fgets(linha, sizeof(linha), arquivo)) { 
        fclose(arquivo); 
        *quantidadeJogos = 0; 
        return NULL; 
    }
    
    Jogo **jogos = NULL;
    int capacidade = 0, indice = 0;
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        removerCaracteresFinais(linha);
        int quantidadePartes = 0;
        char **partes = dividirLinhaCSV(linha, &quantidadePartes);
        
        if (quantidadePartes < 14) {
            partes = (char **)realloc(partes, sizeof(char*) * 14);
            for (int k = quantidadePartes; k < 14; k++) partes[k] = duplicarString("");
            quantidadePartes = 14;
        }
        
        Jogo *novoJogo = criarJogoAPartirDePartes(partes, quantidadePartes);
        
        if (indice >= capacidade) {
            capacidade = (capacidade == 0) ? 100 : capacidade * 2;
            jogos = (Jogo **)realloc(jogos, sizeof(Jogo*) * capacidade);
        }
        jogos[indice++] = novoJogo;
        
        for (int k = 0; k < quantidadePartes; k++) free(partes[k]);
        free(partes);
    }
    
    fclose(arquivo);
    *quantidadeJogos = indice;
    return jogos;
}

static Jogo *encontrarJogoPorId(Jogo **jogos, int quantidade, int id) {
    for (int i = 0; i < quantidade; i++) {
        if (jogos[i]->idAplicativo == id) {
            return jogos[i];
        }
    }
    return NULL;
}

int main() {
    int quantidadeTotalJogos = 0;
    Jogo **todosJogos = lerArquivoCSV("/tmp/games.csv", &quantidadeTotalJogos);
    
    Lista lista;
    inicializarLista(&lista);
    
    char entrada[256];
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        removerCaracteresFinais(entrada);
        if (strcmp(entrada, "FIM") == 0) break;
        
        int id = atoi(entrada);
        Jogo *jogo = encontrarJogoPorId(todosJogos, quantidadeTotalJogos, id);
        if (jogo) {
            Jogo *copia = (Jogo *)malloc(sizeof(Jogo));
            memcpy(copia, jogo, sizeof(Jogo));
            copia->nome = duplicarString(jogo->nome);
            copia->dataLancamento = duplicarString(jogo->dataLancamento);
            inserirNoFim(&lista, copia);
        }
    }
    
    int numeroOperacoes;
    scanf("%d", &numeroOperacoes);
    getchar();
    
    for (int i = 0; i < numeroOperacoes; i++) {
        fgets(entrada, sizeof(entrada), stdin);
        removerCaracteresFinais(entrada);
        
        char comando[3];
        strncpy(comando, entrada, 2);
        comando[2] = '\0';
        
        if (strcmp(comando, "II") == 0) {
            // Inserir no início
            int id = atoi(entrada + 3);
            Jogo *jogo = encontrarJogoPorId(todosJogos, quantidadeTotalJogos, id);
            if (jogo) {
                Jogo *copia = (Jogo *)malloc(sizeof(Jogo));
                memcpy(copia, jogo, sizeof(Jogo));
                copia->nome = duplicarString(jogo->nome);
                copia->dataLancamento = duplicarString(jogo->dataLancamento);
                inserirNoInicio(&lista, copia);
            }
        }
        else if (strcmp(comando, "I*") == 0) {
            // Inserir em posição específica
            char *token = strtok(entrada + 3, " ");
            int posicao = atoi(token);
            token = strtok(NULL, " ");
            int id = atoi(token);
            
            Jogo *jogo = encontrarJogoPorId(todosJogos, quantidadeTotalJogos, id);
            if (jogo) {
                Jogo *copia = (Jogo *)malloc(sizeof(Jogo));
                memcpy(copia, jogo, sizeof(Jogo));
                copia->nome = duplicarString(jogo->nome);
                copia->dataLancamento = duplicarString(jogo->dataLancamento);
                inserirNaPosicao(&lista, copia, posicao);
            }
        }
        else if (strcmp(comando, "IF") == 0) {
            // Inserir no fim
            int id = atoi(entrada + 3);
            Jogo *jogo = encontrarJogoPorId(todosJogos, quantidadeTotalJogos, id);
            if (jogo) {
                Jogo *copia = (Jogo *)malloc(sizeof(Jogo));
                memcpy(copia, jogo, sizeof(Jogo));
                copia->nome = duplicarString(jogo->nome);
                copia->dataLancamento = duplicarString(jogo->dataLancamento);
                inserirNoFim(&lista, copia);
            }
        }
        else if (strcmp(comando, "RI") == 0) {
            // Remover do início
            Jogo *jogoRemovido = removerDoInicio(&lista);
            if (jogoRemovido) {
                printf("(R) %s\n", jogoRemovido->nome);
                liberarJogo(jogoRemovido);
            }
        }
        else if (strcmp(comando, "R*") == 0) {
            // Remover de posição específica
            int posicao = atoi(entrada + 3);
            Jogo *jogoRemovido = removerDaPosicao(&lista, posicao);
            if (jogoRemovido) {
                printf("(R) %s\n", jogoRemovido->nome);
                liberarJogo(jogoRemovido);
            }
        }
        else if (strcmp(comando, "RF") == 0) {
            // Remover do fim
            Jogo *jogoRemovido = removerDoFim(&lista);
            if (jogoRemovido) {
                printf("(R) %s\n", jogoRemovido->nome);
                liberarJogo(jogoRemovido);
            }
        }
    }
    
    imprimirLista(&lista);
    
    liberarLista(&lista);
    for (int i = 0; i < quantidadeTotalJogos; i++) {
        liberarJogo(todosJogos[i]);
    }
    free(todosJogos);
    
    return 0;
}