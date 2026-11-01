#include "raylib.h"
#include "jogo.h"
#include "jogador.h"
#include "render_2d.h"
#include "mapa.h"
#include "memoria.h"
#include "itens.h"
#include "professor.h"

int main(void) {
    InitWindow(1024, 640, "Blackout");
    SetTargetFPS(60);

    Jogo jogo = {0}; // estado do jogo

    // Começa no andar 1.
    if (!mapaCarregarAndar(&jogo, 1)) {
        CloseWindow();
        return 1;
    }

    float acumuladorTempo = 0.0f;
    int venceu = 0;

    inicializar_professor(&jogo.professor, (Posicao){ .linha = 5, .coluna = 10 }, "Prof. Thiaguinho", "Qual especificador imprime um int em C?\nA) %s  B) %d  C) %f", "B");

    while (!WindowShouldClose()) {
        if (!venceu && jogo.telaAtual == TELA_NORMAL) {
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    movimentar_jogador(&jogo.jogador, &jogo.mapa, DIR_CIMA);
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  movimentar_jogador(&jogo.jogador, &jogo.mapa, DIR_BAIXO);
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  movimentar_jogador(&jogo.jogador, &jogo.mapa, DIR_ESQUERDA);
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) movimentar_jogador(&jogo.jogador, &jogo.mapa, DIR_DIREITA);

            coletar_item(&jogo.itens, &jogo.jogador);

            if (verificar_interacao_professor(&jogo.jogador, &jogo.professor)) {
                jogo.telaAtual = TELA_QUIZ;
            }

            // --- Troca de andar ---
            int naEscada = (jogo.jogador.posicao.linha == jogo.mapa.saida.linha &&
                            jogo.jogador.posicao.coluna == jogo.mapa.saida.coluna);

            // Muda de andar apenas se o professor foi ajudado E o jogador subiu na escada
            if (jogo.professor.ajudado && naEscada) {
                if (jogo.andarAtual < TOTAL_ANDARES) {
                    if (!mapaCarregarAndar(&jogo, jogo.andarAtual + 1)) break;
                } else {
                    venceu = 1;
                }
            }
        }

        acumuladorTempo += GetFrameTime();
        if (acumuladorTempo >= 0.05f) {
            acumuladorTempo -= 0.05f;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            desenharMapa2D(&jogo.mapa, 24);
            desenharSaida2D(&jogo.mapa, 24, jogo.professor.ajudado); // verde = liberada pelo professor
            desenharJogador2D(&jogo.jogador, 24);
            desenhar_professor(&jogo.professor, 24);
            desenhar_item(jogo.itens, 24);

            // HUD na faixa abaixo do mapa
            if (!jogo.professor.ajudado) {
                DrawText(TextFormat("Andar %d/%d   Ajude o professor para liberar a saída!", jogo.andarAtual, TOTAL_ANDARES), 10, 610, 20, WHITE);
            } else {
                DrawText(TextFormat("Andar %d/%d   Professor ajudado! Vá até a escada!", jogo.andarAtual, TOTAL_ANDARES), 10, 610, 20, GREEN);
            }

            if (jogo.telaAtual == TELA_QUIZ) desenhar_quiz(&jogo);

            if (venceu) {
                DrawRectangle(0, 0, 1024, 640, Fade(BLACK, 0.7f));
                DrawText("BACKUP CONCLUÍDO! TODOS OS ANDARES CONCLUÍDOS!", 180, 300, 30, GREEN);
                DrawText("Pressione ESC para sair", 380, 350, 20, WHITE);
            }

        EndDrawing();
    }

    liberar_todos_itens(&jogo.itens);
    mapaLiberar(&jogo.mapa);

    memRelatorio();
    CloseWindow();
    return 0;
}
