#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STR 64
#define MAX_LINE 8192
#define MAX_ARRAY_ITEM 200
#define CSV_PATH "/tmp/games.csv"

typedef struct
{
    int id;
    char nome[MAX_STR];
    char dataLancamento[MAX_STR];
    int proprietariosEstimados;
    float preco;
    char idiomasSuportados[MAX_ARRAY_ITEM][MAX_STR];
    int contadorIdiomasSuportados;
    int pontuacaoMetacritic;
    float pontuacaoUsuario;
    int conquistas;
    char editoras[MAX_ARRAY_ITEM][MAX_STR];
    int contadorEditoras;
    char desenvolvedoras[MAX_ARRAY_ITEM][MAX_STR];
    int contadorDesenvolvedoras;
    char categorias[MAX_ARRAY_ITEM][MAX_STR];
    int contadorCategorias;
    char generos[MAX_ARRAY_ITEM][MAX_STR];
    int contadorGeneros;
    char etiquetas[MAX_ARRAY_ITEM][MAX_STR];
    int contadorEtiquetas;
} Jogo;

// função para remover espaços em branco do início e do fim
void removerEspacos(char *texto)
{
    if (!texto || *texto == '\0')
        return;
    char *inicio = texto;
    while (*inicio && isspace((unsigned char)*inicio))
        inicio++;
    if (inicio != texto)
        memmove(texto, inicio, strlen(inicio) + 1);
    int comprimento = strlen(texto);
    while (comprimento > 0 && isspace((unsigned char)texto[comprimento - 1]))
        texto[--comprimento] = '\0';
}

//função para remover caracteres específicos de uma string [], '
void substituirCaracteresString(char *texto)
{
    if (!texto || *texto == '\0')
        return;
    char *ponteiroLeitura = texto;
    char *ponteiroEscrita = texto;
    while (*ponteiroLeitura)
    {
        if (*ponteiroLeitura != '[' && *ponteiroLeitura != ']' && *ponteiroLeitura != '\'')
        {
            *ponteiroEscrita = *ponteiroLeitura;
            ponteiroEscrita++;
        }
        ponteiroLeitura++;
    }
    *ponteiroEscrita = '\0';
}

void analisarData(const char *dataArquivo, char *dataLancamento)
{
    if (dataArquivo == NULL || strlen(dataArquivo) == 0)
    {
        strcpy(dataLancamento, " ");
        return;
    }
    char copiaData[256];
    strncpy(copiaData, dataArquivo, sizeof(copiaData) - 1);
    copiaData[sizeof(copiaData) - 1] = '\0';

    char dataLimpa[256];
    int j = 0;
    for (int i = 0; copiaData[i] != '\0' && j < (int)sizeof(dataLimpa) - 1; i++)
    {
        if (copiaData[i] != '"')
            dataLimpa[j++] = copiaData[i];
    }
    dataLimpa[j] = '\0';
    removerEspacos(dataLimpa);

    const char *meses[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char textoMes[16];
    int dia = 0, mes = 0, ano = 0;

    if (sscanf(dataLimpa, "%s %d, %d", textoMes, &dia, &ano) == 3)
    {
        for (int i = 0; i < 12; i++)
            if (strncmp(textoMes, meses[i], 3) == 0)
                mes = i + 1;
        if (mes > 0 && ano > 0)
        {
            sprintf(dataLancamento, "%02d/%02d/%04d", dia, mes, ano);
            return;
        }
    }
    if (sscanf(dataLimpa, "%s %d", textoMes, &ano) == 2)
    {
        for (int i = 0; i < 12; i++)
            if (strncmp(textoMes, meses[i], 3) == 0)
                mes = i + 1;
        if (mes > 0 && ano > 0)
        {
            sprintf(dataLancamento, "01/%02d/%04d", mes, ano);
            return;
        }
    }
    if (sscanf(dataLimpa, "%d", &ano) == 1 && ano >= 1900)
    {
        sprintf(dataLancamento, "01/01/%04d", ano);
        return;
    }
    strcpy(dataLancamento, " ");
}

int analisarCampoArray(const char *entrada, char array[][MAX_STR], int maximo)
{
    char temporario[MAX_LINE];
    int contador = 0;
    if (!entrada || strlen(entrada) == 0)
        return 0;
    strncpy(temporario, entrada, sizeof(temporario) - 1);
    temporario[sizeof(temporario) - 1] = '\0';
    removerEspacos(temporario);
    substituirCaracteresString(temporario);
    removerEspacos(temporario);

    char *token = strtok(temporario, ",");
    while (token != NULL && contador < maximo)
    {
        removerEspacos(token);
        size_t comprimento = strlen(token);
        if (comprimento > 0 && token[0] == '"')
            memmove(token, token + 1, comprimento);
        comprimento = strlen(token);
        if (comprimento > 0 && token[comprimento - 1] == '"')
            token[comprimento - 1] = '\0';
        removerEspacos(token);
        if (strlen(token) > 0)
        {
            strncpy(array[contador], token, MAX_STR - 1);
            array[contador][MAX_STR - 1] = '\0';
            contador++;
        }
        token = strtok(NULL, ",");
    }
    return contador;
}

// função para ler um registro CSV
int lerRegistroCSV(FILE *arquivo, char *buffer, size_t tamanhoBuffer)
{
    buffer[0] = '\0';
    char temporario[MAX_LINE];
    if (fgets(temporario, sizeof(temporario), arquivo) == NULL)
        return 0;
    strncpy(buffer, temporario, tamanhoBuffer - 1);
    buffer[tamanhoBuffer - 1] = '\0';
    int contadorAspas = 0;
    for (char *p = buffer; *p; p++)
        if (*p == '"')
            contadorAspas++;
    bool fim = (contadorAspas % 2 == 0);
    while (!fim && fgets(temporario, sizeof(temporario), arquivo) != NULL)
    {
        size_t restante = tamanhoBuffer - strlen(buffer) - 1;
        if (restante > 0)
            strncat(buffer, temporario, restante);
        for (char *p = temporario; *p; p++)
            if (*p == '"')
                contadorAspas++;
        fim = (contadorAspas % 2 == 0);
    }
    size_t comprimento = strlen(buffer);
    while (comprimento > 0 && (buffer[comprimento - 1] == '\n' || buffer[comprimento - 1] == '\r'))
        buffer[--comprimento] = '\0';
    return (buffer[0] != '\0');
}

// função para ler uma linha CSV em campos
int lerLinhaCSV(char *linha, char campos[][MAX_LINE], int maximoCampos)
{
    int indiceCampo = 0;
    bool entreAspas = false;
    char *inicio = linha;
    char *p = linha;
    size_t comprimento = strlen(linha);
    if (comprimento > 0 && linha[comprimento - 1] == '\n')
        linha[--comprimento] = '\0';
    if (comprimento > 0 && linha[comprimento - 1] == '\r')
        linha[--comprimento] = '\0';
    while (*p && indiceCampo < maximoCampos)
    {
        if (*p == '"')
            entreAspas = !entreAspas;
        else if (*p == ',' && !entreAspas)
        {
            int tamanhoCampo = p - inicio;
            if (tamanhoCampo > MAX_LINE - 1)
                tamanhoCampo = MAX_LINE - 1;
            strncpy(campos[indiceCampo], inicio, tamanhoCampo);
            campos[indiceCampo][tamanhoCampo] = '\0';
            removerEspacos(campos[indiceCampo]);
            indiceCampo++;
            inicio = p + 1;
        }
        p++;
    }
    if (indiceCampo < maximoCampos)
    {
        int tamanhoCampo = strlen(inicio);
        if (tamanhoCampo > MAX_LINE - 1)
            tamanhoCampo = MAX_LINE - 1;
        strncpy(campos[indiceCampo], inicio, tamanhoCampo);
        campos[indiceCampo][tamanhoCampo] = '\0';
        removerEspacos(campos[indiceCampo]);
        indiceCampo++;
    }
    return indiceCampo;
}

// função para criar um objeto Jogo a partir dos campos lidos
Jogo Jogo_criar(char campos[][MAX_LINE], int numeroCampos)
{
    Jogo j;
    memset(&j, 0, sizeof(Jogo));
    j.pontuacaoMetacritic = -1;
    j.pontuacaoUsuario = -1.0f;
    strcpy(j.dataLancamento, " ");
    if (numeroCampos < 14)
        return j;
    char idLimpo[MAX_STR];
    int k = 0;
    for (int i = 0; campos[0][i]; i++)
        if (isdigit(campos[0][i]))
            idLimpo[k++] = campos[0][i];
    idLimpo[k] = '\0';
    j.id = k > 0 ? atoi(idLimpo) : 0;
    strncpy(j.nome, campos[1], MAX_STR - 1);
    j.nome[MAX_STR - 1] = '\0';
    analisarData(campos[2], j.dataLancamento);
    char proprietariosLimpos[MAX_STR];
    int j_index = 0;
    for (int i = 0; campos[3][i]; i++)
        if (isdigit(campos[3][i]))
            proprietariosLimpos[j_index++] = campos[3][i];
    proprietariosLimpos[j_index] = '\0';
    j.proprietariosEstimados = j_index > 0 ? atoi(proprietariosLimpos) : 0;
    if (strcasecmp(campos[4], "Free to Play") == 0)
        j.preco = 0.0f;
    else
    {
        char precoTemporario[MAX_STR];
        strncpy(precoTemporario, campos[4], MAX_STR - 1);
        for (char *c = precoTemporario; *c; c++)
            if (*c == ',')
                *c = '.';
        j.preco = strtof(precoTemporario, NULL);
    }
    j.contadorIdiomasSuportados = analisarCampoArray(campos[5], j.idiomasSuportados, MAX_ARRAY_ITEM);
    j.pontuacaoMetacritic = strlen(campos[6]) == 0 ? -1 : atoi(campos[6]);
    char pontuacaoTemporaria[MAX_STR];
    strncpy(pontuacaoTemporaria, campos[7], MAX_STR - 1);
    removerEspacos(pontuacaoTemporaria);
    if (strlen(pontuacaoTemporaria) == 0 || strcmp(pontuacaoTemporaria, "tbd") == 0)
        j.pontuacaoUsuario = -1.0f;
    else
    {
        for (char *c = pontuacaoTemporaria; *c; c++)
            if (*c == ',')
                *c = '.';
        j.pontuacaoUsuario = strtof(pontuacaoTemporaria, NULL);
    }
    j.conquistas = strlen(campos[8]) == 0 ? 0 : atoi(campos[8]);
    j.contadorEditoras = analisarCampoArray(campos[9], j.editoras, MAX_ARRAY_ITEM);
    j.contadorDesenvolvedoras = analisarCampoArray(campos[10], j.desenvolvedoras, MAX_ARRAY_ITEM);
    j.contadorCategorias = analisarCampoArray(campos[11], j.categorias, MAX_ARRAY_ITEM);
    j.contadorGeneros = analisarCampoArray(campos[12], j.generos, MAX_ARRAY_ITEM);
    j.contadorEtiquetas = analisarCampoArray(campos[13], j.etiquetas, MAX_ARRAY_ITEM);
    return j;
}

void Jogo_imprimir(const Jogo *j)
{
    printf("=> %d ## %s ## %s ## %d ## %.2f ## [", j->id, j->nome, j->dataLancamento, j->proprietariosEstimados, j->preco);
    for (int i = 0; i < j->contadorIdiomasSuportados; i++)
        printf("%s%s", j->idiomasSuportados[i], (i < j->contadorIdiomasSuportados - 1) ? ", " : "");
    printf("] ## %d ## %.1f ## %d ## [", j->pontuacaoMetacritic, j->pontuacaoUsuario, j->conquistas);
    for (int i = 0; i < j->contadorEditoras; i++)
        printf("%s%s", j->editoras[i], (i < j->contadorEditoras - 1) ? ", " : "");
    printf("] ## [");
    for (int i = 0; i < j->contadorDesenvolvedoras; i++)
        printf("%s%s", j->desenvolvedoras[i], (i < j->contadorDesenvolvedoras - 1) ? ", " : "");
    printf("] ## [");
    for (int i = 0; i < j->contadorCategorias; i++)
        printf("%s%s", j->categorias[i], (i < j->contadorCategorias - 1) ? ", " : "");
    printf("] ## [");
    for (int i = 0; i < j->contadorGeneros; i++)
        printf("%s%s", j->generos[i], (i < j->contadorGeneros - 1) ? ", " : "");
    printf("] ## [");
    for (int i = 0; i < j->contadorEtiquetas; i++)
        printf("%s%s", j->etiquetas[i], (i < j->contadorEtiquetas - 1) ? ", " : "");
    printf("] ## \n");
}

// função para buscar um jogo pelo ID
Jogo *buscarJogoPorId(Jogo *todosJogos, int totalJogos, int id)
{
    for (int i = 0; i < totalJogos; i++)
        if (todosJogos[i].id == id)
            return &todosJogos[i];
    return NULL;
}

//Pilha
typedef struct CelulaPilha
{
    Jogo *jogo;
    struct CelulaPilha *proximo;
} CelulaPilha;

typedef struct
{
    CelulaPilha *topo;
    int tamanho;
} PilhaJogos;

void inicializarPilha(PilhaJogos *p)
{
    p->topo = NULL;
    p->tamanho = 0;
}

void empilhar(PilhaJogos *p, Jogo *j)
{
    CelulaPilha *nova = (CelulaPilha *)malloc(sizeof(CelulaPilha));
    nova->jogo = j;
    nova->proximo = p->topo;
    p->topo = nova;
    p->tamanho++;
}

Jogo *desempilhar(PilhaJogos *p)
{
    if (!p->topo)
        return NULL;
    CelulaPilha *remover = p->topo;
    Jogo *j = remover->jogo;
    p->topo = remover->proximo;
    free(remover);
    p->tamanho--;
    return j;
}

void mostrarPilha(PilhaJogos *p)
{
    if (p->tamanho == 0)
        return;
    Jogo **auxiliar = (Jogo **)malloc(p->tamanho * sizeof(Jogo *));
    int indice = 0;
    for (CelulaPilha *c = p->topo; c; c = c->proximo)
        auxiliar[indice++] = c->jogo;
    for (int i = p->tamanho - 1; i >= 0; i--)
    {
        printf("[%d] ", p->tamanho - 1 - i);
        Jogo_imprimir(auxiliar[i]);
    }
    free(auxiliar);
}

int main()
{
    FILE *arquivo = fopen(CSV_PATH, "r");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", CSV_PATH);
        return 1;
    }

    // Alocação inicial para todos os jogos
    int capacidadeTotal = 1024;
    Jogo *todosJogos = (Jogo *)malloc(capacidadeTotal * sizeof(Jogo));
    if (!todosJogos)
    {
        fprintf(stderr, "Erro alocar jogos\n");
        fclose(arquivo);
        return 1;
    }

    int totalJogos = 0;
    char linha[MAX_LINE];
    char campos[14][MAX_LINE];

    bool primeiraLinha = lerRegistroCSV(arquivo, linha, sizeof(linha));
    if (!primeiraLinha)
    {
        fclose(arquivo);
        free(todosJogos);
        return 1;
    }

    bool leituraCSV = true;
    while (leituraCSV)
    {
        bool registroValido = lerRegistroCSV(arquivo, linha, sizeof(linha));
        if (registroValido)
        {
            int numeroCampos = lerLinhaCSV(linha, campos, 14);
            if (numeroCampos == 14)
            {
                if (totalJogos >= capacidadeTotal)
                {
                    int novaCapacidade = capacidadeTotal * 2;
                    Jogo *temporario = (Jogo *)realloc(todosJogos, novaCapacidade * sizeof(Jogo));
                    if (temporario)
                    {
                        todosJogos = temporario;
                        capacidadeTotal = novaCapacidade;
                    }
                    else
                    {
                        leituraCSV = false;
                    }
                }
                if (leituraCSV)
                    todosJogos[totalJogos++] = Jogo_criar(campos, numeroCampos);
            }
        }
        else
            leituraCSV = false;
    }
    fclose(arquivo);

    PilhaJogos pilha;
    inicializarPilha(&pilha);

    // Leitura dos IDs para empilhar inicialmente
    bool fimEntrada = false;
    while (!fimEntrada)
    {
        bool leu = fgets(linha, sizeof(linha), stdin);
        if (leu)
        {
            linha[strcspn(linha, "\n")] = 0;
            removerEspacos(linha);
            if (strcmp(linha, "FIM") == 0)
                fimEntrada = true;
            else if (strlen(linha) > 0)
            {
                int id;
                if (sscanf(linha, "%d", &id) == 1)
                {
                    Jogo *j = buscarJogoPorId(todosJogos, totalJogos, id);
                    if (j)
                        empilhar(&pilha, j);
                }
            }
        }
        else
            fimEntrada = true;
    }

    // Lê o número de comandos a serem executados
    bool comandosExistem = fgets(linha, sizeof(linha), stdin) != NULL;
    int numeroComandos = 0;
    if (comandosExistem)
    {
        removerEspacos(linha);
        if (sscanf(linha, "%d", &numeroComandos) != 1)
            numeroComandos = 0;
    }

    int i = 0;
    while (i < numeroComandos)
    {
        bool leu = fgets(linha, sizeof(linha), stdin) != NULL;
        if (leu)
        {
            linha[strcspn(linha, "\n")] = 0;
            removerEspacos(linha);
            if (strlen(linha) > 0)
            {
                if (linha[0] == 'I')
                {
                    int id;
                    sscanf(linha + 2, "%d", &id);
                    Jogo *j = buscarJogoPorId(todosJogos, totalJogos, id);
                    if (j)
                        empilhar(&pilha, j);
                }
                else if (linha[0] == 'R')
                {
                    Jogo *j = desempilhar(&pilha);
                    if (j)
                        printf("(R) %s\n", j->nome);
                }
                i++;
            }
        }
        else
            i++;
    }

    mostrarPilha(&pilha);
    free(todosJogos);
    return 0;
}