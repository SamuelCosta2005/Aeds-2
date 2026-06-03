#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINHA_TAMANHO 4096
#define MAX_CAMPO_TAMANHO 512
#define MAX_ELEMENTOS_ARRAY 50
#define MAX_IDS 100

typedef struct
{
    int id;
    char *nome;
    char *dataLancamento;
    int proprietariosEstimados;
    float preco;
    char **idiomasSuportados;
    int contadorIdiomasSuportados;
    int pontuacaoMetacritic;
    float pontuacaoUsuario;
    int conquistas;
    char **editores;
    int contadorEditores;
    char **desenvolvedores;
    int contadorDesenvolvedores;
    char **categorias;
    int contadorCategorias;
    char **generos;
    int contadorGeneros;
    char **tags;
    int contadorTags;
} Jogo;

typedef struct NoAVL
{
    Jogo *jogo;
    int altura;
    struct NoAVL *esquerda, *direita;
} NoAVL;

typedef struct
{
    NoAVL *raiz;
} ArvoreAVL;

void analisarECarregarJogo(Jogo *jogo, char *linha);
void imprimirJogo(Jogo *jogo);
void liberarJogo(Jogo *jogo);
char *obterProximoCampo(char *linha, int *posicao);
char **dividirString(const char *str, char delimitador, int *contador);
char *trim(char *str);
char *formatarData(char *dataStr);
void imprimirArrayString(char **arr, int contador);
void copiaProfundaJogo(Jogo *destino, const Jogo *origem);

ArvoreAVL *criarArvore();
int alturaNo(NoAVL *no);
int obterFatorBalanceamento(NoAVL *no);
int maximo(int a, int b);
NoAVL *rotacaoDireita(NoAVL *no);
NoAVL *rotacaoEsquerda(NoAVL *no);
NoAVL *inserirRecursivo(NoAVL *no, Jogo *jogo);
void inserirNaArvore(ArvoreAVL *arvore, Jogo *jogo);

bool pesquisarNaArvore(ArvoreAVL *arvore, const char *nome, FILE *arquivoLog);
bool pesquisarRecursivo(NoAVL *no, const char *nome, FILE *arquivoLog);

void liberarArvoreRecursivo(NoAVL *no);
void liberarArvoreCompleta(ArvoreAVL *arvore);

char **ids;
int tamanhoIds = 0;
int contadorComparacoes = 0;

int main()
{
    char bufferLinha[MAX_LINHA_TAMANHO];
    const char *caminhoArquivo = "C:\\Users\\samue\\OneDrive\\Área de Trabalho\\TP7\\games.csv";
    
    FILE *arquivoLog = fopen("801757_AVL.txt", "w");
    if (arquivoLog == NULL) {
        perror("Erro ao criar arquivo de log");
        return 1;
    }

    ids = (char **)malloc(sizeof(char *) * MAX_IDS);
    for (int i = 0; i < MAX_IDS; i++)
    {
        ids[i] = (char *)malloc(sizeof(char) * MAX_CAMPO_TAMANHO);
    }

    char entrada[MAX_CAMPO_TAMANHO];
    bool fimEncontrado = false;
    
    while (!fimEncontrado && fgets(entrada, MAX_CAMPO_TAMANHO, stdin) != NULL)
    {
        entrada[strcspn(entrada, "\n")] = 0;
        if (strcmp(entrada, "FIM") == 0) {
            fimEncontrado = true;
        } else {
            strcpy(ids[tamanhoIds], entrada);
            tamanhoIds++;
        }
    }

    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int contadorJogos = 0;
    fgets(bufferLinha, MAX_LINHA_TAMANHO, arquivo);
    while (fgets(bufferLinha, MAX_LINHA_TAMANHO, arquivo) != NULL)
    {
        contadorJogos++;
    }
    fclose(arquivo);

    Jogo *todosJogos = (Jogo *)malloc(sizeof(Jogo) * contadorJogos);
    if (todosJogos == NULL)
    {
        printf("Erro de alocação de memória\n");
        return 1;
    }

    arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao reabrir o arquivo");
        free(todosJogos);
        return 1;
    }

    fgets(bufferLinha, MAX_LINHA_TAMANHO, arquivo);
    int i = 0;
    while (fgets(bufferLinha, MAX_LINHA_TAMANHO, arquivo) != NULL)
    {
        analisarECarregarJogo(&todosJogos[i], bufferLinha);
        i++;
    }
    fclose(arquivo);

    ArvoreAVL *arvore = criarArvore();

    for (i = 0; i < tamanhoIds; i++)
    {
        int idAlvo = atoi(ids[i]);
        bool jogoEncontrado = false;
        
        for (int j = 0; !jogoEncontrado && j < contadorJogos; j++)
        {
            if (todosJogos[j].id == idAlvo)
            {
                Jogo *novoJogo = (Jogo *)malloc(sizeof(Jogo));
                copiaProfundaJogo(novoJogo, &todosJogos[j]);
                inserirNaArvore(arvore, novoJogo);
                jogoEncontrado = true;
            }
        }
    }

    fimEncontrado = false;
    while (!fimEncontrado && fgets(entrada, MAX_CAMPO_TAMANHO, stdin) != NULL)
    {
        entrada[strcspn(entrada, "\n")] = 0;

        if (strcmp(entrada, "FIM") == 0) {
            fimEncontrado = true;
            continue;
        }

        printf("%s: ", entrada);
        fprintf(arquivoLog, "%s: ", entrada);

        if (pesquisarNaArvore(arvore, entrada, arquivoLog))
        {
            printf("SIM\n");
            fprintf(arquivoLog, "SIM\n");
        }
        else
        {
            printf("NAO\n");
            fprintf(arquivoLog, "NAO\n");
        }
    }
    
    liberarArvoreCompleta(arvore);

    for (i = 0; i < contadorJogos; i++)
    {
        liberarJogo(&todosJogos[i]);
    }
    free(todosJogos);

    for (i = 0; i < MAX_IDS; i++)
    {
        free(ids[i]);
    }
    free(ids);
    
    fclose(arquivoLog);

    return 0;
}

bool pesquisarRecursivo(NoAVL *no, const char *nome, FILE *arquivoLog)
{
    bool resultado = false;
    
    if (no != NULL)
    {
        int comparacao = strcmp(nome, no->jogo->nome);

        if (comparacao == 0)
        {
            resultado = true;
        }
        else if (comparacao < 0)
        {
            printf("esq ");
            fprintf(arquivoLog, "esq ");
            resultado = pesquisarRecursivo(no->esquerda, nome, arquivoLog);
        }
        else
        {
            printf("dir ");
            fprintf(arquivoLog, "dir ");
            resultado = pesquisarRecursivo(no->direita, nome, arquivoLog);
        }
    }
    
    return resultado;
}

bool pesquisarNaArvore(ArvoreAVL *arvore, const char *nome, FILE *arquivoLog)
{
    bool resultado = false;
    
    if (arvore->raiz != NULL)
    {
        printf("raiz ");
        fprintf(arquivoLog, "raiz ");
        resultado = pesquisarRecursivo(arvore->raiz, nome, arquivoLog);
    }
    
    return resultado;
}

ArvoreAVL *criarArvore()
{
    ArvoreAVL *arvore = (ArvoreAVL *)malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL;
    return arvore;
}

int maximo(int a, int b)
{
    int resultado;
    
    if (a > b) {
        resultado = a;
    } else {
        resultado = b;
    }
    
    return resultado;
}

int alturaNo(NoAVL *no)
{
    int resultado = 0;
    
    if (no != NULL) {
        resultado = no->altura;
    }
    
    return resultado;
}

int obterFatorBalanceamento(NoAVL *no)
{
    int resultado = 0;
    
    if (no != NULL) {
        resultado = alturaNo(no->direita) - alturaNo(no->esquerda);
    }
    
    return resultado;
}

NoAVL *criarNovoNo(Jogo *jogo)
{
    NoAVL *novo = (NoAVL *)malloc(sizeof(NoAVL));
    novo->jogo = jogo;
    novo->esquerda = novo->direita = NULL;
    novo->altura = 1;
    return novo;
}

NoAVL *rotacaoDireita(NoAVL *y)
{
    NoAVL *x = y->esquerda;
    NoAVL *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = maximo(alturaNo(y->esquerda), alturaNo(y->direita)) + 1;
    x->altura = maximo(alturaNo(x->esquerda), alturaNo(x->direita)) + 1;

    return x;
}

NoAVL *rotacaoEsquerda(NoAVL *x)
{
    NoAVL *y = x->direita;
    NoAVL *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = maximo(alturaNo(x->esquerda), alturaNo(x->direita)) + 1;
    y->altura = maximo(alturaNo(y->esquerda), alturaNo(y->direita)) + 1;

    return y;
}

NoAVL *inserirRecursivo(NoAVL *no, Jogo *jogo)
{
    NoAVL *resultado = no;
    
    if (no == NULL) {
        resultado = criarNovoNo(jogo);
    } else {
        int comparacao = strcmp(jogo->nome, no->jogo->nome);

        if (comparacao < 0) {
            no->esquerda = inserirRecursivo(no->esquerda, jogo);
        } else if (comparacao > 0) {
            no->direita = inserirRecursivo(no->direita, jogo);
        } else {
            liberarJogo(jogo);
            free(jogo);
            resultado = no;
        }

        if (comparacao != 0) {
            no->altura = 1 + maximo(alturaNo(no->esquerda), alturaNo(no->direita));

            int balanceamento = obterFatorBalanceamento(no);

            if (balanceamento < -1 && strcmp(jogo->nome, no->esquerda->jogo->nome) < 0) {
                resultado = rotacaoDireita(no);
            } else if (balanceamento > 1 && strcmp(jogo->nome, no->direita->jogo->nome) > 0) {
                resultado = rotacaoEsquerda(no);
            } else if (balanceamento < -1 && strcmp(jogo->nome, no->esquerda->jogo->nome) > 0) {
                no->esquerda = rotacaoEsquerda(no->esquerda);
                resultado = rotacaoDireita(no);
            } else if (balanceamento > 1 && strcmp(jogo->nome, no->direita->jogo->nome) < 0) {
                no->direita = rotacaoDireita(no->direita);
                resultado = rotacaoEsquerda(no);
            } else {
                resultado = no;
            }
        }
    }
    
    return resultado;
}

void inserirNaArvore(ArvoreAVL *arvore, Jogo *jogo)
{
    arvore->raiz = inserirRecursivo(arvore->raiz, jogo);
}

void liberarArvoreRecursivo(NoAVL *no)
{
    if (no != NULL)
    {
        liberarArvoreRecursivo(no->esquerda);
        liberarArvoreRecursivo(no->direita);
        liberarJogo(no->jogo);
        free(no->jogo);
        free(no);
    }
}

void liberarArvoreCompleta(ArvoreAVL *arvore)
{
    liberarArvoreRecursivo(arvore->raiz);
    free(arvore);
}

char **copiarArrayString(char **arr, int contador) {
    char **novoArr = (char **)malloc(sizeof(char *) * contador);
    for (int i = 0; i < contador; i++) {
        novoArr[i] = strdup(arr[i]);
    }
    return novoArr;
}

void copiaProfundaJogo(Jogo *destino, const Jogo *origem)
{
    *destino = *origem;

    destino->nome = strdup(origem->nome);
    destino->dataLancamento = strdup(origem->dataLancamento);

    destino->idiomasSuportados = copiarArrayString(origem->idiomasSuportados, origem->contadorIdiomasSuportados);
    destino->editores = copiarArrayString(origem->editores, origem->contadorEditores);
    destino->desenvolvedores = copiarArrayString(origem->desenvolvedores, origem->contadorDesenvolvedores);
    destino->categorias = copiarArrayString(origem->categorias, origem->contadorCategorias);
    destino->generos = copiarArrayString(origem->generos, origem->contadorGeneros);
    destino->tags = copiarArrayString(origem->tags, origem->contadorTags);
}

void analisarECarregarJogo(Jogo *jogo, char *linha)
{
    int posicao = 0;

    jogo->id = atoi(obterProximoCampo(linha, &posicao));
    jogo->nome = obterProximoCampo(linha, &posicao);
    jogo->dataLancamento = formatarData(obterProximoCampo(linha, &posicao));
    jogo->proprietariosEstimados = atoi(obterProximoCampo(linha, &posicao));

    char *precoStr = obterProximoCampo(linha, &posicao);
    if (strcmp(precoStr, "Free to Play") == 0 || strlen(precoStr) == 0) {
        jogo->preco = 0.0f;
    } else {
        jogo->preco = atof(precoStr);
    }
    free(precoStr);

    char *idiomaStr = obterProximoCampo(linha, &posicao);
    idiomaStr[strcspn(idiomaStr, "]")] = 0;
    memmove(idiomaStr, idiomaStr + 1, strlen(idiomaStr));
    for (int i = 0; idiomaStr[i]; i++) {
        if (idiomaStr[i] == '\'') {
            idiomaStr[i] = ' ';
        }
    }
    jogo->idiomasSuportados = dividirString(idiomaStr, ',', &jogo->contadorIdiomasSuportados);
    free(idiomaStr);

    jogo->pontuacaoMetacritic = atoi(obterProximoCampo(linha, &posicao));
    jogo->pontuacaoUsuario = atof(obterProximoCampo(linha, &posicao));
    jogo->conquistas = atoi(obterProximoCampo(linha, &posicao));

    jogo->editores = dividirString(obterProximoCampo(linha, &posicao), ',', &jogo->contadorEditores);
    jogo->desenvolvedores = dividirString(obterProximoCampo(linha, &posicao), ',', &jogo->contadorDesenvolvedores);
    jogo->categorias = dividirString(obterProximoCampo(linha, &posicao), ',', &jogo->contadorCategorias);
    jogo->generos = dividirString(obterProximoCampo(linha, &posicao), ',', &jogo->contadorGeneros);
    jogo->tags = dividirString(obterProximoCampo(linha, &posicao), ',', &jogo->contadorTags);
}

void imprimirJogo(Jogo *jogo)
{
    char dataFormatada[12];
    strcpy(dataFormatada, jogo->dataLancamento);
    if (dataFormatada[1] == '/')
    {
        memmove(dataFormatada + 1, dataFormatada, strlen(dataFormatada) + 1);
        dataFormatada[0] = '0';
    }

    printf("=> %d ## %s ## %s ## %d ## %.2f ## ",
           jogo->id, jogo->nome, dataFormatada, jogo->proprietariosEstimados, jogo->preco);
    imprimirArrayString(jogo->idiomasSuportados, jogo->contadorIdiomasSuportados);
    printf(" ## %d ## %.1f ## %d ## ",
           jogo->pontuacaoMetacritic,
           jogo->pontuacaoUsuario,
           jogo->conquistas);
    imprimirArrayString(jogo->editores, jogo->contadorEditores);
    printf(" ## ");
    imprimirArrayString(jogo->desenvolvedores, jogo->contadorDesenvolvedores);
    printf(" ## ");
    imprimirArrayString(jogo->categorias, jogo->contadorCategorias);
    printf(" ## ");
    imprimirArrayString(jogo->generos, jogo->contadorGeneros);
    printf(" ## ");
    imprimirArrayString(jogo->tags, jogo->contadorTags);
    printf(" ##\n");
}

void liberarJogo(Jogo *jogo)
{
    free(jogo->nome);
    free(jogo->dataLancamento);
    
    for (int i = 0; i < jogo->contadorIdiomasSuportados; i++)
        free(jogo->idiomasSuportados[i]);
    free(jogo->idiomasSuportados);
    
    for (int i = 0; i < jogo->contadorEditores; i++)
        free(jogo->editores[i]);
    free(jogo->editores);
    
    for (int i = 0; i < jogo->contadorDesenvolvedores; i++)
        free(jogo->desenvolvedores[i]);
    free(jogo->desenvolvedores);
    
    for (int i = 0; i < jogo->contadorCategorias; i++)
        free(jogo->categorias[i]);
    free(jogo->categorias);
    
    for (int i = 0; i < jogo->contadorGeneros; i++)
        free(jogo->generos[i]);
    free(jogo->generos);
    
    for (int i = 0; i < jogo->contadorTags; i++)
        free(jogo->tags[i]);
    free(jogo->tags);
}

char *obterProximoCampo(char *linha, int *posicao)
{
    char *campo = (char *)malloc(sizeof(char) * MAX_CAMPO_TAMANHO);
    int i = 0;
    bool entreAspas = false;

    if (linha[*posicao] == '"')
    {
        entreAspas = true;
        (*posicao)++;
    }

    bool campoCompleto = false;
    
    while (!campoCompleto && linha[*posicao] != '\0')
    {
        if (entreAspas)
        {
            if (linha[*posicao] == '"')
            {
                (*posicao)++;
                campoCompleto = true;
            } else {
                campo[i++] = linha[(*posicao)++];
            }
        }
        else
        {
            if (linha[*posicao] == ',')
            {
                campoCompleto = true;
            } else {
                campo[i++] = linha[(*posicao)++];
            }
        }
    }

    if (linha[*posicao] == ',')
    {
        (*posicao)++;
    }

    campo[i] = '\0';
    return campo;
}

char **dividirString(const char *str, char delimitador, int *contador)
{
    int contadorInicial = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == delimitador) {
            contadorInicial++;
        }
    }
    *contador = contadorInicial + 1;

    char **resultado = (char **)malloc(sizeof(char *) * (*contador));
    char buffer[MAX_CAMPO_TAMANHO];
    int indice_str = 0;
    int indice_resultado = 0;

    for (int i = 0; i <= strlen(str); i++)
    {
        if (str[i] == delimitador || str[i] == '\0')
        {
            buffer[indice_str] = '\0';
            resultado[indice_resultado] = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
            strcpy(resultado[indice_resultado], trim(buffer));
            indice_resultado++;
            indice_str = 0;
        }
        else
        {
            buffer[indice_str++] = str[i];
        }
    }
    return resultado;
}

char *trim(char *str)
{
    char *inicio = str;
    char *fim;
    
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    
    if (*inicio == 0) {
        return inicio;
    }
    
    fim = inicio + strlen(inicio) - 1;
    while (fim > inicio && isspace((unsigned char)*fim)) {
        fim--;
    }
    
    fim[1] = '\0';
    
    return inicio;
}

char *formatarData(char *dataStr)
{
    char *dataFormatada = (char *)malloc(sizeof(char) * 12);
    char mesStr[4] = {0};
    char dia[3] = "01";
    char ano[5] = "0000";

    sscanf(dataStr, "%s", mesStr);

    char *numeroMes = "01";
    if (strcmp(mesStr, "Jan") == 0) {
        numeroMes = "01";
    } else if (strcmp(mesStr, "Feb") == 0) {
        numeroMes = "02";
    } else if (strcmp(mesStr, "Mar") == 0) {
        numeroMes = "03";
    } else if (strcmp(mesStr, "Apr") == 0) {
        numeroMes = "04";
    } else if (strcmp(mesStr, "May") == 0) {
        numeroMes = "05";
    } else if (strcmp(mesStr, "Jun") == 0) {
        numeroMes = "06";
    } else if (strcmp(mesStr, "Jul") == 0) {
        numeroMes = "07";
    } else if (strcmp(mesStr, "Aug") == 0) {
        numeroMes = "08";
    } else if (strcmp(mesStr, "Sep") == 0) {
        numeroMes = "09";
    } else if (strcmp(mesStr, "Oct") == 0) {
        numeroMes = "10";
    } else if (strcmp(mesStr, "Nov") == 0) {
        numeroMes = "11";
    } else if (strcmp(mesStr, "Dec") == 0) {
        numeroMes = "12";
    }

    char *ptr = dataStr;
    while (*ptr && !isdigit(*ptr)) {
        ptr++;
    }
    
    if (isdigit(*ptr)) {
        sscanf(ptr, "%[^,], %s", dia, ano);
    }

    sprintf(dataFormatada, "%s/%s/%s", dia, numeroMes, ano);
    free(dataStr);
    return dataFormatada;
}

void imprimirArrayString(char **arr, int contador)
{
    printf("[");
    for (int i = 0; i < contador; i++)
    {
        printf("%s", arr[i]);
        if (i < contador - 1)
        {
            printf(", ");
        }
    }
    printf("]");
}