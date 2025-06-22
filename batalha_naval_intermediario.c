#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 10
#define TAM_NAVIO 3
#define NAVIO 3
#define AGUA 0
#define ACERTO 1
#define ERRO -1

// Inicializa o tabuleiro com agua (0)
void inicializarTabuleiro(int tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tabuleiro[i][j] = AGUA;
}

// Verifica se e possivel posicionar navio H ou V
int podeColocarHV(int tabuleiro[TAM][TAM], int l, int c, char d) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        int li = l + (d == 'V' ? i : 0);
        int ci = c + (d == 'H' ? i : 0);
        if (li >= TAM || ci >= TAM || tabuleiro[li][ci] != AGUA)
            return 0;
    }
    return 1;
}

// Verifica se pode colocar diagonal
int podeColocarDiagonal(int tabuleiro[TAM][TAM], int l, int c, char tipo) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        int li = l + i;
        int ci = tipo == '\\' ? c + i : c - i;
        if (li >= TAM || ci < 0 || ci >= TAM || tabuleiro[li][ci] != AGUA)
            return 0;
    }
    return 1;
}

// Coloca navio H ou V com valor 3
void colocarHV(int tabuleiro[TAM][TAM], int l, int c, char d) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        int li = l + (d == 'V' ? i : 0);
        int ci = c + (d == 'H' ? i : 0);
        tabuleiro[li][ci] = NAVIO;
    }
}

// Coloca navio diagonal com valor 3
void colocarDiagonal(int tabuleiro[TAM][TAM], int l, int c, char tipo) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        int li = l + i;
        int ci = tipo == '\\' ? c + i : c - i;
        tabuleiro[li][ci] = NAVIO;
    }
}

// Exibe o tabuleiro do jogador (com navios visiveis)
void exibirTabuleiroJogador(int tabuleiro[TAM][TAM]) {
    printf("   ");
    for (int i = 0; i < TAM; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == NAVIO) printf("3 "); // Navio
            else if (tabuleiro[i][j] == ACERTO) printf("X ");
            else if (tabuleiro[i][j] == ERRO) printf("O ");
            else printf("0 "); // Agua
        }
        printf("\n");
    }
}

// Exibe o tabuleiro da maquina (sem mostrar navios)
void exibirTabuleiroInimigo(int tabuleiro[TAM][TAM]) {
    printf("   ");
    for (int i = 0; i < TAM; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == ACERTO) printf("X ");
            else if (tabuleiro[i][j] == ERRO) printf("O ");
            else printf("0 ");
        }
        printf("\n");
    }
}

// Executa ataque e retorna resultado
int atacar(int tabuleiro[TAM][TAM], int l, int c) {
    if (tabuleiro[l][c] == NAVIO) {
        tabuleiro[l][c] = ACERTO;
        return 1;
    } else if (tabuleiro[l][c] == AGUA) {
        tabuleiro[l][c] = ERRO;
        return 0;
    }
    return -1; // ja atacado
}

// Conta acertos
int contarAcertos(int tabuleiro[TAM][TAM]) {
    int cont = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (tabuleiro[i][j] == ACERTO) cont++;
    return cont;
}

int main() {
    int jogador[TAM][TAM];
    int maquina[TAM][TAM];
    int totalNavios = 4 * TAM_NAVIO;

    srand(time(NULL));
    inicializarTabuleiro(jogador);
    inicializarTabuleiro(maquina);

    // Navios do jogador
    colocarHV(jogador, 1, 2, 'H');
    colocarHV(jogador, 4, 5, 'V');
    colocarDiagonal(jogador, 6, 1, '\\');
    colocarDiagonal(jogador, 0, 8, '/');

    // Navios da maquina (aleatorios)
    int colocados = 0;
    while (colocados < 2) {
        int l = rand() % 8;
        int c = rand() % 8;
        char d = (rand() % 2 == 0) ? 'H' : 'V';
        if (podeColocarHV(maquina, l, c, d)) {
            colocarHV(maquina, l, c, d);
            colocados++;
        }
    }
    while (colocados < 4) {
        int l = rand() % 8;
        int c = rand() % 8;
        char d = (rand() % 2 == 0) ? '\\' : '/';
        if (podeColocarDiagonal(maquina, l, c, d)) {
            colocarDiagonal(maquina, l, c, d);
            colocados++;
        }
    }

    // Loop do jogo
    while (1) {
        int lin, col;
        printf("\nSeu tabuleiro:\n");
        exibirTabuleiroJogador(jogador);
        printf("\nTabuleiro da maquina:\n");
        exibirTabuleiroInimigo(maquina);

        printf("\nDigite linha e coluna para atacar (0-9): ");
        scanf("%d %d", &lin, &col);
        if (lin < 0 || lin >= TAM || col < 0 || col >= TAM) {
            printf("Coordenadas invalidas!\n");
            continue;
        }

        int resultado = atacar(maquina, lin, col);
        if (resultado == 1) printf("Voce ACERTOU!\n");
        else if (resultado == 0) printf("Voce errou...\n");
        else printf("Voce ja atacou essa posicao.\n");

        // Maquina ataca
        int lin_bot, col_bot;
        do {
            lin_bot = rand() % TAM;
            col_bot = rand() % TAM;
        } while (jogador[lin_bot][col_bot] == ACERTO || jogador[lin_bot][col_bot] == ERRO);

        int bot_resultado = atacar(jogador, lin_bot, col_bot);
        printf("A maquina atacou (%d, %d): %s\n", lin_bot, col_bot, bot_resultado == 1 ? "ACERTOU!" : "Errou.");

        // Fim de jogo
        if (contarAcertos(maquina) >= totalNavios) {
            printf("\nParabens! Voce venceu!\n");
            break;
        }
        if (contarAcertos(jogador) >= totalNavios) {
            printf("\nA maquina venceu! Fim de jogo.\n");
            break;
        }
    }

    return 0;
}
