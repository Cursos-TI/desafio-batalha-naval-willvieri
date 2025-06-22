#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 10

#define VAZIO '0'
#define NAVIO '3'
#define ACERTO 'O'
#define ERRO 'X'
#define AREA_ATAQUE '5'

char tab_jogador[TAM][TAM];
char tab_maquina[TAM][TAM];

void inicializarTabuleiro(char tab[TAM][TAM]) {
    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++)
            tab[i][j] = VAZIO;
}

void exibirTabuleiro(char tab[TAM][TAM], int mostrarNavios) {
    printf("   ");
    for(int c = 0; c < TAM; c++) printf("%2d ", c);
    printf("\n");
    for(int l = 0; l < TAM; l++) {
        printf("%2d ", l);
        for(int c = 0; c < TAM; c++) {
            char simbolo = tab[l][c];
            if (!mostrarNavios && simbolo == NAVIO)
                printf(" 0 ");
            else if (simbolo == AREA_ATAQUE)
                printf(" %c ", AREA_ATAQUE);  // Mostra '5'
            else
                printf(" %c ", simbolo);
        }
        printf("\n");
    }
}

void posicionarNaviosMaquina() {
    int navios = 0;
    while(navios < 15) {
        int l = rand() % TAM;
        int c = rand() % TAM;
        if(tab_maquina[l][c] == VAZIO) {
            tab_maquina[l][c] = NAVIO;
            navios++;
        }
    }
}

void posicionarNaviosJogador() {
    tab_jogador[0][8] = NAVIO;
    tab_jogador[1][2] = NAVIO; tab_jogador[1][3] = NAVIO; tab_jogador[1][4] = NAVIO; tab_jogador[1][7] = NAVIO;
    tab_jogador[2][6] = NAVIO;
    tab_jogador[4][5] = NAVIO;
    tab_jogador[5][5] = NAVIO;
    tab_jogador[6][1] = NAVIO; tab_jogador[6][5] = NAVIO;
    tab_jogador[7][2] = NAVIO;
    tab_jogador[8][3] = NAVIO;
}

int ataqueSimples(int l, int c) {
    if(l<0 || l>=TAM || c<0 || c>=TAM) return 0;
    if(tab_maquina[l][c] == NAVIO) {
        tab_maquina[l][c] = ACERTO;
        return 1;
    } else if(tab_maquina[l][c] == VAZIO) {
        tab_maquina[l][c] = ERRO;
        return 0;
    }
    return 0;
}

void limparAreaAtaque() {
    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++)
            if(tab_maquina[i][j] == AREA_ATAQUE)
                tab_maquina[i][j] = VAZIO;
}

void marcarEAtacar(int l, int c, int *acertos) {
    if(l>=0 && l<TAM && c>=0 && c<TAM) {
        if(tab_maquina[l][c] == NAVIO) {
            tab_maquina[l][c] = ACERTO;
            (*acertos)++;
        } else if(tab_maquina[l][c] == VAZIO || tab_maquina[l][c] == AREA_ATAQUE) {
            tab_maquina[l][c] = ERRO;
        }
    }
}

void marcarAreaHabilidade(int tipo, int l, int c) {
    limparAreaAtaque();
    if (tipo == 1) { // Cone
        for (int i = 0; i < 3; i++)
            for (int j = -i; j <= i; j++) {
                int x = l + i, y = c + j;
                if (x >= 0 && x < TAM && y >= 0 && y < TAM)
                    tab_maquina[x][y] = AREA_ATAQUE;
            }
    } else if (tipo == 2) { // Cruz
        for (int i = -2; i <= 2; i++) {
            if (l + i >= 0 && l + i < TAM)
                tab_maquina[l + i][c] = AREA_ATAQUE;
            if (c + i >= 0 && c + i < TAM)
                tab_maquina[l][c + i] = AREA_ATAQUE;
        }
    } else if (tipo == 3) { // Octaedro
        for (int dx = -2; dx <= 2; dx++)
            for (int dy = -2; dy <= 2; dy++)
                if (abs(dx) + abs(dy) <= 2) {
                    int x = l + dx, y = c + dy;
                    if (x >= 0 && x < TAM && y >= 0 && y < TAM)
                        tab_maquina[x][y] = AREA_ATAQUE;
                }
    }
}

int aplicarAtaqueEspecial(int l, int c, int tipo) {
    int acertos = 0;
    if (tipo == 1) {
        for (int i = 0; i < 3; i++)
            for (int j = -i; j <= i; j++)
                marcarEAtacar(l + i, c + j, &acertos);
    } else if (tipo == 2) {
        for (int i = -2; i <= 2; i++) {
            marcarEAtacar(l + i, c, &acertos);
            marcarEAtacar(l, c + i, &acertos);
        }
    } else if (tipo == 3) {
        for (int dx = -2; dx <= 2; dx++)
            for (int dy = -2; dy <= 2; dy++)
                if (abs(dx) + abs(dy) <= 2)
                    marcarEAtacar(l + dx, c + dy, &acertos);
    }
    limparAreaAtaque(); // limpa os * depois de aplicar
    return acertos;
}

void ataqueMaquina() {
    int l, c;
    do {
        l = rand() % TAM;
        c = rand() % TAM;
    } while(tab_jogador[l][c] == ACERTO || tab_jogador[l][c] == ERRO);

    printf("Maquina ataca em %d,%d\n", l, c);

    if(tab_jogador[l][c] == NAVIO) {
        tab_jogador[l][c] = ACERTO;
        printf("Maquina acertou!\n");
    } else {
        tab_jogador[l][c] = ERRO;
        printf("Maquina errou.\n");
    }
}

int main() {
    srand(time(NULL));

    inicializarTabuleiro(tab_jogador);
    inicializarTabuleiro(tab_maquina);
    posicionarNaviosJogador();
    posicionarNaviosMaquina();

    int tipo, l, c;

    while(1) {
        printf("\nSeu tabuleiro:\n");
        exibirTabuleiro(tab_jogador, 1);

        printf("\nTabuleiro da mquina:\n");
        exibirTabuleiro(tab_maquina, 0);

        printf("\nEscolha o tipo de ataque:\n");
        printf("1 - Simples\n2 - Especial\nOpcao: ");
        scanf("%d", &tipo);

        printf("Digite a linha e coluna do ataque: ");
        scanf("%d %d", &l, &c);

        int acertos = 0;
        if(tipo == 1) {
            acertos = ataqueSimples(l, c);
            if(acertos) printf("Voce acertou um navio!\n");
            else printf("Voce errou.\n");
        } else if(tipo == 2) {
            printf("Habilidade:\n1 - Cone\n2 - Cruz\n3 - Octaedro\nEscolha: ");
            int habilidade;
            scanf("%d", &habilidade);

            marcarAreaHabilidade(habilidade, l, c);
            printf("\nArea de ataque marcada com '*':\n");
            exibirTabuleiro(tab_maquina, 0);

            acertos = aplicarAtaqueEspecial(l, c, habilidade);
            printf("Ataque especial causou %d acertos!\n", acertos);
        } else {
            printf("Opcao invalida.\n");
            continue;
        }

        ataqueMaquina();
    }

    return 0;
}
