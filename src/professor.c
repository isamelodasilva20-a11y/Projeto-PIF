#include <stddef.h>
#include <string.h>
#include "professor.h"
#include "raylib.h"



void inicializar_professor(Professor *professor, Posicao posicao, const char *nome, const char *pergunta, const char *resposta){
    if(professor==NULL)return;
    professor->posicao = posicao;
    professor->nome = nome;
    professor->pergunta = pergunta;
    professor->resposta = resposta;
    professor->ajudado = false;
}

bool verificar_interacao_professor(Jogador *jogador, Professor *professor){
    if(jogador==NULL || professor ==NULL || professor->ajudado)return false;
    if(jogador->posicao.linha == professor->posicao.linha && jogador->posicao.coluna == professor->posicao.coluna)return true;
    return false;
}

void desenhar_professor(Professor *professor, int tamanhoCasa){
    if(professor==NULL)return;

    int centroX = (professor->posicao.coluna * tamanhoCasa) + (tamanhoCasa / 2);
    int centroY = (professor->posicao.linha * tamanhoCasa) + (tamanhoCasa / 2);

    Color corProfessor = professor->ajudado ? GRAY : PURPLE;

    DrawCircle(centroX, centroY, tamanhoCasa / 3, corProfessor);

    if (!professor->ajudado) {
        DrawText("!", centroX - 2, centroY - (tamanhoCasa / 2) - 8, 10, YELLOW);
    }
}

void desenhar_quiz(Jogo *jogo) {
    if (jogo == NULL) return;

    //efeito de pause
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    //tela principal
    int larguraModal = 400;
    int alturaModal = 220;
    int posX = (GetScreenWidth() - larguraModal) / 2;
    int posY = (GetScreenHeight() - alturaModal) / 2;

    DrawRectangle(posX, posY, larguraModal, alturaModal, DARKGRAY);
    DrawRectangleLines(posX, posY, larguraModal, alturaModal, RAYWHITE);

    // nome do professor e pergunta
    DrawText(jogo->professor.nome, posX + 20, posY + 15, 20, YELLOW);
    DrawText(jogo->professor.pergunta, posX + 20, posY + 50, 16, WHITE);

    if (jogo->estadoFeedback == FEEDBACK_NENHUM) {
        // Mostra a instrução enquanto aguarda resposta
        DrawText("Pressione [A], [B], [C] ou [D] para responder", posX + 20, posY + 190, 12, LIGHTGRAY);

        const char *respostaJogador = NULL;
        if (IsKeyPressed(KEY_A)) respostaJogador = "A";
        if (IsKeyPressed(KEY_B)) respostaJogador = "B";
        if (IsKeyPressed(KEY_C)) respostaJogador = "C";
        if (IsKeyPressed(KEY_D)) respostaJogador = "D";

        if (respostaJogador != NULL) {
            jogo->tempoInicioFeedback = GetTime();

            if (strcmp(respostaJogador, jogo->professor.resposta) == 0) {
                jogo->estadoFeedback = FEEDBACK_ACERTOU;
                concluir_ajuda_professor(&jogo->jogador, &jogo->professor);
            } else {
                jogo->estadoFeedback = FEEDBACK_ERROU;
            }
        }
    } 
    else {
        // 4. Renderização do Feedback Visual (Acertou / Errou)
        if (jogo->estadoFeedback == FEEDBACK_ACERTOU) {
            DrawRectangle(posX + 20, posY + 180, larguraModal - 40, 40, GREEN);
            DrawText("RESPOSTA CORRETA! Pendrive obtido!", posX + 35, posY + 192, 16, WHITE);
        } 
        else if (jogo->estadoFeedback == FEEDBACK_ERROU) {
            DrawRectangle(posX + 20, posY + 180, larguraModal - 40, 40, RED);
            DrawText("INCORRETO! Tente novamente.", posX + 60, posY + 192, 16, WHITE);
        }

        if (GetTime() - jogo->tempoInicioFeedback >= 1.5) {
            if (jogo->estadoFeedback == FEEDBACK_ACERTOU) {
                jogo->telaAtual = TELA_NORMAL;
            }
            jogo->estadoFeedback = FEEDBACK_NENHUM;
        }
    }
}


void concluir_ajuda_professor(Jogador *jogador, Professor *professor){
    if(jogador == NULL || professor == NULL || professor->ajudado)return;

    professor->ajudado = true;
    jogador->temPendrive = true;
    jogador->bateria = 100;
}