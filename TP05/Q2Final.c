#include <stdio.h>
#include <string.h>
#include <time.h>

int comparacoes = 0;
int movimentacoes = 0;
#define MAX_JOGOS 1850  
#define MAX_LINHA 10000    

typedef struct {
    int id;
    char nome[200];
    char dataLancamento[20];
    int proprietariosEstimados;
    float preco;
    char idiomasSuportados[500];
    int pontuacaoMetacritic;
    float pontuacaoUsuario;
    int conquistas;
    char editoras[500];
    char desenvolvedores[500];
    char categorias[500];
    char generos[500];
    char tags[1000];
} Jogo;

int ehDigito(char c) {
    return (c >= '0' && c <= '9');
}

int stringParaInt(char *s) {
    int resultado = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (ehDigito(s[i])) resultado = resultado * 10 + (s[i] - '0');
        i++;
    }
    return resultado;
}

float stringParaFloat(char *s) {
    float resultado = 0.0f;
    int i = 0;
    int parteDecimal = 0;
    float divisor = 10.0f;
    if (s == NULL || strcmp(s, "") == 0 || strcmp(s, "Free to Play") == 0){ 
        return 0.0f;
    }
    while (s[i] != '\0') {
        char c = s[i];
        if (ehDigito(c)) {
            if (!parteDecimal) resultado = resultado * 10 + (c - '0');
            else { resultado += (c - '0') / divisor; divisor *= 10; }
        } else if (c == ',' || c == '.') parteDecimal = 1;
        i++;
    }
    return resultado;
}

void removerAspas(char *dest, char *orig) {
    int i = 0, j = 0;
    while (orig[i] != '\0') {
        if (orig[i] != '"') {
            dest[j++] = orig[i];
        }
        i++;
    }
    dest[j] = '\0';
}

void limparLista(char *dest, char *orig) {
    int i = 0, j = 0;
    while (orig[i] != '\0') {
        char c = orig[i];
        if (c != '[' && c != ']' && c != '"' && c != '\'') {
            dest[j++] = c;
            if (c == ',') dest[j++] = ' ';
        }
        i++;
    }
    dest[j] = '\0';
}

void converterData(char *dest, char *orig) {
    char mesStr[4];
    int dia = 0, ano = 0;
    int i = 0, j = 0;
    for (i = 0; i < 3 && orig[i] != '\0'; i++)
        mesStr[i] = orig[i];
    mesStr[i] = '\0';
    while (orig[i] == ' ') i++;
    dia = 0;
    while (orig[i] >= '0' && orig[i] <= '9') {
        dia = dia * 10 + (orig[i] - '0');
        i++;
    }
    while (orig[i] == ',' || orig[i] == ' ') i++;
    ano = 0;
    while (orig[i] >= '0' && orig[i] <= '9') {
        ano = ano * 10 + (orig[i] - '0');
        i++;
    }
    char mesNum[3] = "00";
    if (mesStr[0]=='J' && mesStr[1]=='a' && mesStr[2]=='n'){ 
        mesNum[0]='0'; mesNum[1]='1'; 
    }
    else if (mesStr[0]=='F'){ 
        mesNum[0]='0'; mesNum[1]='2'; 
    }
    else if (mesStr[0]=='M' && mesStr[2]=='r'){ 
        mesNum[0]='0'; mesNum[1]='3'; 
    }
    else if (mesStr[0]=='A' && mesStr[1]=='p'){ 
        mesNum[0]='0'; mesNum[1]='4'; 
    }
    else if (mesStr[0]=='M' && mesStr[2]=='y'){ 
        mesNum[0]='0'; mesNum[1]='5'; 
    }
    else if (mesStr[0]=='J' && mesStr[1]=='u' && mesStr[2]=='n'){ 
        mesNum[0]='0'; mesNum[1]='6'; 
    }
    else if (mesStr[0]=='J' && mesStr[1]=='u' && mesStr[2]=='l'){ 
        mesNum[0]='0'; mesNum[1]='7'; 
    }
    else if (mesStr[0]=='A' && mesStr[1]=='u'){ 
        mesNum[0]='0'; mesNum[1]='8'; 
    }
    else if (mesStr[0]=='S'){
        mesNum[0]='0'; mesNum[1]='9'; 
        }
    else if (mesStr[0]=='O'){ 
        mesNum[0]='1'; mesNum[1]='0'; 
    }
    else if (mesStr[0]=='N'){ 
        mesNum[0]='1'; mesNum[1]='1'; 
    }
    else if (mesStr[0]=='D'){ 
        mesNum[0]='1'; mesNum[1]='2'; 
    }
    if (dia < 10)
        sprintf(dest, "0%d/%c%c/%d", dia, mesNum[0], mesNum[1], ano);
    else
        sprintf(dest, "%d/%c%c/%d", dia, mesNum[0], mesNum[1], ano);
}

int dividirCSV(char linha[], char campos[][1000]) {
    int campoAtual = 0, j = 0, dentroAspas = 0, i = 0;
    while (linha[i] != '\0') {
        char c = linha[i];
        if (c == '"') dentroAspas = !dentroAspas;
        else if (c == ',' && !dentroAspas) {
            campos[campoAtual][j] = '\0';
            campoAtual++;
            j = 0;
        } else {
            campos[campoAtual][j++] = c;
        }
        i++;
    }
    campos[campoAtual][j] = '\0';
    return campoAtual + 1;
}

Jogo criarJogo(char campos[][1000]) {
    Jogo jogo;
    jogo.id = stringParaInt(campos[0]);
    removerAspas(jogo.nome, campos[1]);
    char dataLimpa[50];
    removerAspas(dataLimpa, campos[2]);
    converterData(jogo.dataLancamento, dataLimpa);
    jogo.proprietariosEstimados = stringParaInt(campos[3]);
    jogo.preco = stringParaFloat(campos[4]);
    limparLista(jogo.idiomasSuportados, campos[5]);
    jogo.pontuacaoMetacritic = (strcmp(campos[6], "")==0 || strcmp(campos[6],"null")==0) ? -1 : stringParaInt(campos[6]);
    jogo.pontuacaoUsuario = (strcmp(campos[7], "")==0 || strcmp(campos[7],"tbd")==0) ? -1.0f : stringParaFloat(campos[7]);
    jogo.conquistas = (strcmp(campos[8], "")==0 || strcmp(campos[8],"null")==0) ? 0 : stringParaInt(campos[8]);
    limparLista(jogo.editoras, campos[9]);
    limparLista(jogo.desenvolvedores, campos[10]);
    limparLista(jogo.categorias, campos[11]);
    limparLista(jogo.generos, campos[12]);
    limparLista(jogo.tags, campos[13]);
    return jogo;
}

Jogo* buscarJogoPorId(Jogo jogos[], int total, int idBusca) {
    Jogo* resultado = NULL; 

    for (int i = 0; i < total; i++) {
        if (jogos[i].id == idBusca) {
            resultado = &jogos[i]; 
        }
    }
    return resultado; 
}

void imprimirJogo(Jogo *j) {
    printf("=>%d ## %s ## %s ## %d ## %.2f ## [%s] ## %d ## %.1f ## %d ## [%s] ## [%s] ## [%s] ## [%s] ## [%s] ##\n",
        j->id, j->nome, j->dataLancamento, j->proprietariosEstimados, j->preco,
        j->idiomasSuportados, j->pontuacaoMetacritic, j->pontuacaoUsuario, j->conquistas,
        j->editoras, j->desenvolvedores, j->categorias, j->generos, j->tags);
}

void ordenacaoSelecao(Jogo jogos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            int p = 0;
            while (jogos[j].nome[p] == jogos[menor].nome[p] && jogos[j].nome[p] != '\0' && jogos[menor].nome[p] != '\0') {
                p++;
                comparacoes+=2;
            }
            char c1 = jogos[j].nome[p];
            char c2 = jogos[menor].nome[p];
            if (c1 == '\0' || c1 < c2) menor = j;
        }
        if (menor != i) {
            Jogo temp = jogos[i];
            jogos[i] = jogos[menor];
            jogos[menor] = temp;
            movimentacoes+=3;
        }
    }
}

void gravarLogSelecao(const char* matricula, int comparacoes, int movimentacoes, double tempoExecucao) {
    char nomeArquivo[100];
    sprintf(nomeArquivo, "%s_selecao.txt", matricula);
    FILE *f = fopen(nomeArquivo, "w");
    if (f) {
        fprintf(f, "%s\t%d\t%d\t%.6f\n", matricula, comparacoes, movimentacoes, tempoExecucao);
        fclose(f);
    }
}

int main() {
    clock_t inicio = clock();
    Jogo jogos[MAX_JOGOS];
    int totalJogos = 0;
    char linha[MAX_LINHA];
    FILE *arquivo = fopen("/tmp/games.csv", "r");
    if (!arquivo) {
        printf("Arquivo nao encontrado\n");
        return 1;
    }
    fgets(linha, MAX_LINHA, arquivo);
    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        int len = strlen(linha);
        while (len > 0 && (linha[len-1]=='\n' || linha[len-1]=='\r')) { 
            linha[len-1]='\0'; len--; 
        }
        if (strcmp(linha,"") != 0) {
            char campos[20][1000];
            int numCampos = dividirCSV(linha, campos);
            if (numCampos >= 14) {
                jogos[totalJogos] = criarJogo(campos);
                totalJogos++;
            }
        }
    }
    fclose(arquivo);
    ordenacaoSelecao(jogos, totalJogos);
    char entrada[50];
    int idsBuscados[1000];
    int totalBuscados = 0;
    while (scanf("%s", entrada) == 1 && strcmp(entrada, "FIM") != 0) {
        idsBuscados[totalBuscados++] = stringParaInt(entrada);
    }
    for (int i = 0; i < totalJogos; i++) {
        int deveImprimir = 0;
        for (int j = 0; j < totalBuscados; j++) {
            if (jogos[i].id == idsBuscados[j]) deveImprimir = 1;
        }
        if (deveImprimir) imprimirJogo(&jogos[i]);
    }
    clock_t fim = clock();
    double tempoExecucao= fim-inicio;
    char* matricula= "801757";
    gravarLogSelecao(matricula, comparacoes, movimentacoes, tempoExecucao);
}