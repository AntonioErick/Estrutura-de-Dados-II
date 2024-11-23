#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para calcular o vetor de prefixo (LPS - Maior Prefixo Sufixo)
void calcularLPSArray(const char *padrao, int *lps, int comprimento_padro) {
    int comprimento = 0; // Comprimento do prefixo anterior
    lps[0] = 0; // LPS[0] é sempre 0
    int i = 1;

    while (i < comprimento_padro) {
        if (padrao[i] == padrao[comprimento]) {
            comprimento++;
            lps[i] = comprimento;
            i++;
        } else {
            if (comprimento != 0) {
                comprimento = lps[comprimento - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Função para encontrar a substring usando o algoritmo KMP
int buscarKMP(const char *texto, const char *padrao, int *contagem_loop) {
    int comprimento_texto = strlen(texto);
    int comprimento_padro = strlen(padrao);
    int *lps = (int *)malloc(comprimento_padro * sizeof(int));
    int i = 0; // Índice para texto
    int j = 0; // Índice para padrão
    *contagem_loop = 0; // Inicializa a contagem de loops

    calcularLPSArray(padrao, lps, comprimento_padro);

    while (i < comprimento_texto) {
        (*contagem_loop)++; // Incrementa a contagem de loops

        if (padrao[j] == texto[i]) {
            i++;
            j++;
        }

        if (j == comprimento_padro) {
            free(lps);
            return i - j; // Retorna a posição inicial da substring
        } else if (i < comprimento_texto && padrao[j] != texto[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return -1; // Não encontrado
}

int main() {
    const char *nome_arquivo = "Pi.txt";
    char *digitos_pi;
    char numero_busca[100];
    FILE *arquivo;
    long tamanho_arquivo;
    size_t tamanho_lido;
    int contagem_loop;

    // Solicita ao usuário o número a ser pesquisado
    printf("Digite o numero que deseja encontrar na sequencia de pi: ");
    if (scanf("%99s", numero_busca) != 1) {
        fprintf(stderr, "Erro ao ler a entrada.\n");
        return 1;
    }

    // Abre o arquivo
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo");
        return 1;
    }

    // Obtém o tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    // Aloca memória para a sequência de dígitos de pi
    digitos_pi = (char *)malloc(tamanho_arquivo + 1);
    if (digitos_pi == NULL) {
        perror("Nao foi possivel alocar memoria");
        fclose(arquivo);
        return 1;
    }

    // Lê o conteúdo do arquivo
    tamanho_lido = fread(digitos_pi, 1, tamanho_arquivo, arquivo);
    if (tamanho_lido != tamanho_arquivo) {
        perror("Erro ao ler o arquivo");
        free(digitos_pi);
        fclose(arquivo);
        return 1;
    }
    digitos_pi[tamanho_arquivo] = '\0'; // Termina a string

    // Fecha o arquivo
    fclose(arquivo);

    // Procura o número na sequência de pi
    int posicao = buscarKMP(digitos_pi, numero_busca, &contagem_loop);
    if (posicao != -1) {
        printf("Numero encontrado na posicao %d\n", posicao);
    } else {
        printf("Numero nao encontrado na sequencia.\n");
    }

    // Exibe a contagem de loops
    printf("Numero de iteracoes no loop: %d\n", contagem_loop);

    // Libera a memória alocada
    free(digitos_pi);

    return 0;
}