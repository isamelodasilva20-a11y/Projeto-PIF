#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "jogo.h"

void inicializar_professor(Professor *professor, Posicao posicao, const char *nome, const char *pergunta, const char *resposta);

bool verificar_interacao_professor(Jogador *jogador, Professor *professor);

void desenhar_professor(Professor *professor, int tamanhoCasa);

void concluir_ajuda_professor(Jogador *jogador, Professor *professor);

void desenhar_quiz(Jogo *jogo);

#endif
