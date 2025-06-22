#include <stdio.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define NAVIO 3
#define AGUA 0

// Funcao para inicializar o tabuleiro com 0 (agua)
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Funcao para verificar se o navio cabe e nao sobrepoe outros
int podeColocarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                     int linha, int coluna, char direcao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int l = linha + (direcao == 'V' ? i : 0);
        int c = coluna + (direcao == 'H' ? i : 0);

        // Verifica limites do tabuleiro
        if (l >= TAMANHO_TABULEIRO || c >= TAMANHO_TABULEIRO) {
            return 0; // invalido
        }

        // Verifica sobreposicao
        if (tabuleiro[l][c] != AGUA) {
            return 0;
        }
    }
    return 1;
}

// Funcao para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                     int linha, int coluna, char direcao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int l = linha + (direcao == 'V' ? i : 0);
        int c = coluna + (direcao == 'H' ? i : 0);
        tabuleiro[l][c] = NAVIO;
    }
}

// Funcao para exibir o tabuleiro no console
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("   ");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d ", i); // cabecalho da coluna
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i); // numero da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    // Inicializa o tabuleiro com agua (0)
    inicializarTabuleiro(tabuleiro);

    // Coordenadas e direcoes dos navios 
    int linha1 = 2, coluna1 = 3;
    char direcao1 = 'H'; // Horizontal

    int linha2 = 5, coluna2 = 7;
    char direcao2 = 'V'; // Vertical

    // Validacao e posicionamento do primeiro navio
    if (podeColocarNavio(tabuleiro, linha1, coluna1, direcao1)) {
        posicionarNavio(tabuleiro, linha1, coluna1, direcao1);
    } else {
        printf("Erro ao posicionar o navio 1!\n");
        return 1;
    }

    // Validacao e posicionamento do segundo navio
    if (podeColocarNavio(tabuleiro, linha2, coluna2, direcao2)) {
        posicionarNavio(tabuleiro, linha2, coluna2, direcao2);
    } else {
        printf("Erro ao posicionar o navio 2!\n");
        return 1;
    }

    // Exibe o tabuleiro final com os navios
    printf("\nTabuleiro de Batalha Naval:\n");
    exibirTabuleiro(tabuleiro);

    return 0;
}
