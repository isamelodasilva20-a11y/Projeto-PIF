#include "robos.h"
#include "memoria.h"
#include <stdlib.h>

void robosAdicionar(Robo **cabeca, Posicao posicao, TipoRobo tipo) {
    Robo *novo = (Robo *)memAlocar(sizeof(Robo));
    if (novo == NULL) return;
    
    novo->posicao = posicao;
    novo->posicaoInicial = posicao;
    novo->tipo = tipo;
    novo->estado = ESTADO_NORMAL;
    novo->next = *cabeca;
    *cabeca = novo;
}

void robosAtualizar(Jogo *jogo) {
    if (jogo == NULL) return;

    Robo *atual = jogo->robos;
    while (atual != NULL) {
        if (atual->estado != ESTADO_DESLIGADO) {
            // Sorteia uma das 4 direcoes (0: Cima, 1: Baixo, 2: Esquerda, 3: Direita)
            int dir = rand() % 4;
            Posicao novaPos = atual->posicao;

            if (dir == 0) novaPos.linha--;
            else if (dir == 1) novaPos.linha++;
            else if (dir == 2) novaPos.coluna--;
            else if (dir == 3) novaPos.coluna++;

            // Só move se a casa de destino nao for parede
            if (novaPos.linha >= 0 && novaPos.linha < jogo->mapa.linhas &&
                novaPos.coluna >= 0 && novaPos.coluna < jogo->mapa.colunas) {
                if (jogo->mapa.matriz[novaPos.linha][novaPos.coluna] != '#') {
                    atual->posicao = novaPos;
                }
            }
        }
        atual = atual->next;
    }
}

void checarColisoes(Jogo *jogo) {
    if (jogo == NULL) return;

    Robo *atual = jogo->robos;
    while (atual != NULL) {
        if (atual->estado != ESTADO_DESLIGADO) {
            // Verifica se a posicao do jogador é a mesma do robo
            if (jogo->jogador.posicao.linha == atual->posicao.linha &&
                jogo->jogador.posicao.coluna == atual->posicao.coluna) {
                
                jogo->jogador.vidas--;
                
                // Reseta jogador e robos para a posicao inicial
                jogo->jogador.posicao = jogo->jogador.posicaoInicial;
                
                Robo *r = jogo->robos;
                while (r != NULL) {
                    r->posicao = r->posicaoInicial;
                    r = r->next;
                }
                break;
            }
        }
        atual = atual->next;
    }
}

void robosLiberar(Robo **cabeca) {
    if (cabeca == NULL || *cabeca == NULL) return;

    Robo *atual = *cabeca;
    while (atual != NULL) {
        Robo *proximo = atual->next;
        memLiberar(atual);
        atual = proximo;
    }
    *cabeca = NULL;
}