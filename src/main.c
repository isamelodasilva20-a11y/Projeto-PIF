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

    Jogo jogo = {0}; //estado do jogo

    // Comeca no andar 1. O mapa, a posicao do jogador ('P'), as baterias ('I')
    // e a escada ('S') vem todos do proprio arquivo andares/andar1.txt
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
                // Regra: so sobe quando a lista de baterias esta vazia E o jogador
                // esta em cima da escada. Antes disso a escada nao faz nada.
                int naEscada = jogo.jogador.posicao.linha == jogo.mapa.saida.linha &&
                               jogo.jogador.posicao.coluna == jogo.mapa.saida.coluna;

                if(verificar_vitoria(jogo.itens)){
                    if(jogo.andarAtual < TOTAL_ANDARES){
                        // Ainda ha andares acima: carrega o proximo (se o arquivo falhar, encerra o jogo)
                        if(!mapaCarregarAndar(&jogo, jogo.andarAtual + 1)) break;
                    }else{
                        // Subiu a escada do ultimo andar: fim de jogo
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
                desenharSaida2D(&jogo.mapa, 24, verificar_vitoria(jogo.itens)); // verde = liberada
                desenharJogador2D(&jogo.jogador, 24);
                desenhar_professor(&jogo.professor, 24);
                desenhar_item(jogo.itens, 24);

                // HUD na faixa abaixo do mapa (25 linhas * 24px = 600px, a janela tem 640px)
                // Mostra o andar atual e quantas baterias faltam; quando zera, avisa para ir a escada
                int restantes = contar_itens(jogo.itens);
                if(restantes > 0){
                    DrawText(TextFormat("Andar %d/%d   Baterias restantes: %d", jogo.andarAtual, TOTAL_ANDARES, restantes), 10, 610, 20, WHITE);
                }else{
                    DrawText(TextFormat("Andar %d/%d   Va ate a escada!", jogo.andarAtual, TOTAL_ANDARES), 10, 610, 20, GREEN);
                }

                if (jogo.telaAtual == TELA_QUIZ)desenhar_quiz(&jogo);

                if(venceu){
                    DrawRectangle(0, 0, 1024, 640, Fade(BLACK, 0.7f)); // Fundo semi-transparente
                    DrawText("BACKUP CONCLUÍDO! TODOS OS PENDRIVES COLETADOS!", 200, 300, 30, GREEN);
                    DrawText("Pressione ESC para sair", 380, 350, 20, WHITE);
                }

        EndDrawing();
    }
    

    // Libera o que sobrou do ultimo andar carregado (os anteriores ja foram
    // liberados pelo mapaCarregarAndar a cada troca)
    liberar_todos_itens(&jogo.itens);
    mapaLiberar(&jogo.mapa);

    memRelatorio();
    CloseWindow();
    return 0;
}
