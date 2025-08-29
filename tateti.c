/**
 * @file tateti.c
 * @brief Juego clásico Ta Te Ti (Tic Tac Toe) con raylib.
 *
 * Proyecto educativo para aprender a usar la librería gráfica raylib.
 * El jugador humano (X) juega contra un bot (O) con inteligencia media:
 * - Intenta ganar si puede.
 * - Bloquea al jugador si está por ganar.
 * - Si no, elige una casilla libre al azar.
 *
 * Incluye un pequeño retraso para simular que el bot "piensa".
 *
 * @author Aylen Cantuer
 */

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define BOARD_SIZE 3
#define CELL_SIZE (SCREEN_WIDTH / BOARD_SIZE)

// 0 = vacío, 1 = jugador (X), 2 = bot (O)
int board[BOARD_SIZE][BOARD_SIZE] = {0};

/**
 * @brief Dibuja el tablero y las marcas (X y O).
 */
void DrawBoard() {
    ClearBackground(RAYWHITE);

    int cellSize = SCREEN_HEIGHT / BOARD_SIZE;

    // Líneas del tablero
    for (int i = 1; i < BOARD_SIZE; i++) {
        DrawLineEx((Vector2){i * cellSize, 0}, (Vector2){i * cellSize, SCREEN_HEIGHT}, 4, BLACK);
        DrawLineEx((Vector2){0, i * cellSize}, (Vector2){SCREEN_WIDTH, i * cellSize}, 4, BLACK);
    }

    // Dibujar X y O
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = j * cellSize + cellSize / 2;
            int y = i * cellSize + cellSize / 2;

            if (board[i][j] == 1) {  
                // X = dos líneas cruzadas
                DrawLineEx((Vector2){x - 40, y - 40}, (Vector2){x + 40, y + 40}, 6, RED);
                DrawLineEx((Vector2){x + 40, y - 40}, (Vector2){x - 40, y + 40}, 6, RED);
            } 
            else if (board[i][j] == 2) {  
                // O = círculo con trazo doble
                DrawCircleLines(x, y, 45, BLUE);
                DrawCircleLines(x, y, 44, BLUE);
            }
        }
    }
}

/**
 * @brief Verifica si hay un ganador.
 * @return 0 si no hay ganador, 1 si gana el jugador, 2 si gana el bot.
 */
int CheckWinner() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] != 0 &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        if (board[0][i] != 0 &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    if (board[0][0] != 0 &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != 0 &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return 0;
}

/**
 * @brief Verifica si el tablero está lleno (empate).
 * @return true si todas las casillas están ocupadas.
 */
bool IsBoardFull() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == 0) return false;
    return true;
}

/**
 * @brief Movimiento del bot con estrategia intermedia:
 * 1. Gana si puede.
 * 2. Bloquea al jugador si está por ganar.
 * 3. Si no, elige una casilla aleatoria vacía.
 */
void BotMove() {
    // 1. Intentar ganar
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = 2;
                if (CheckWinner() == 2) return;
                board[i][j] = 0;
            }
        }
    }

    // 2. Bloquear al jugador
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = 1;
                if (CheckWinner() == 1) {
                    board[i][j] = 2;
                    return;
                }
                board[i][j] = 0;
            }
        }
    }

    // 3. Elegir una casilla aleatoria
    srand(time(NULL));
    int emptyCells[9][2];
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int r = rand() % count;
        board[emptyCells[r][0]][emptyCells[r][1]] = 2;
    }
}

/**
 * @brief Función principal del juego.
 * 
 * Maneja el ciclo principal de ejecución:
 * - Detecta jugadas del jugador.
 * - Llama al bot después de 1 segundo.
 * - Verifica condiciones de victoria o empate.
 */
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe - Raylib");
    SetTargetFPS(60);

    int winner = 0;

    while (!WindowShouldClose()) {
        if (winner == 0 && !IsBoardFull()) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int x = GetMouseX() / CELL_SIZE;
                int y = GetMouseY() / CELL_SIZE;

                if (board[y][x] == 0) {
                    board[y][x] = 1;
                    winner = CheckWinner();
                    
                    // Dibujar de inmediato la jugada del jugador
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawBoard();
                    EndDrawing();

                    if (winner == 0 && !IsBoardFull()) {
                        WaitTime(1.0f); // Espera 1 segundo para simular "pensamiento"
                        BotMove();
                        winner = CheckWinner();
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBoard();

        if (winner == 1) DrawText("Ganaste!", 200, 280, 40, GREEN);
        else if (winner == 2) DrawText("Perdiste!", 200, 280, 40, RED);
        else if (IsBoardFull()) DrawText("Empate!", 220, 280, 40, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
