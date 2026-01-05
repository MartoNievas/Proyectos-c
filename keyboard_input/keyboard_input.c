#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#define WIDTH 1600
#define HEIGHT 480
// Estructura solicitada
typedef struct {
    int key_id;
    char* label;
} KeyVisual;
KeyVisual keys[87];

static void set_layout(void) {
    // Inicializar todo a 0
    for (int i = 0; i < 168; i++) {
        keys[i].key_id = 0;
        keys[i].label = 0;
    }

    int i = 0;

    // --- FILA 1: Esc, Funciones, Navegación ---
    keys[i++] = (KeyVisual){ KEY_ESCAPE, "Esc" };
    keys[i++] = (KeyVisual){ KEY_F1, "F1" };   keys[i++] = (KeyVisual){ KEY_F2, "F2" };
    keys[i++] = (KeyVisual){ KEY_F3, "F3" };   keys[i++] = (KeyVisual){ KEY_F4, "F4" };
    keys[i++] = (KeyVisual){ KEY_F5, "F5" };   keys[i++] = (KeyVisual){ KEY_F6, "F6" };
    keys[i++] = (KeyVisual){ KEY_F7, "F7" };   keys[i++] = (KeyVisual){ KEY_F8, "F8" };
    keys[i++] = (KeyVisual){ KEY_F9, "F9" };   keys[i++] = (KeyVisual){ KEY_F10, "F10" };
    keys[i++] = (KeyVisual){ KEY_F11, "F11" }; keys[i++] = (KeyVisual){ KEY_F12, "F12" };
    keys[i++] = (KeyVisual){ KEY_HOME, "Home" }; keys[i++] = (KeyVisual){ KEY_END, "End" };
    keys[i++] = (KeyVisual){ KEY_INSERT, "Ins" }; keys[i++] = (KeyVisual){ KEY_DELETE, "Del" };

    // --- FILA 2: Números y Símbolos ---
    keys[i++] = (KeyVisual){ KEY_GRAVE, "~" };  keys[i++] = (KeyVisual){ KEY_ONE, "1" };
    keys[i++] = (KeyVisual){ KEY_TWO, "2" };    keys[i++] = (KeyVisual){ KEY_THREE, "3" };
    keys[i++] = (KeyVisual){ KEY_FOUR, "4" };   keys[i++] = (KeyVisual){ KEY_FIVE, "5" };
    keys[i++] = (KeyVisual){ KEY_SIX, "6" };    keys[i++] = (KeyVisual){ KEY_SEVEN, "7" };
    keys[i++] = (KeyVisual){ KEY_EIGHT, "8" };  keys[i++] = (KeyVisual){ KEY_NINE, "9" };
    keys[i++] = (KeyVisual){ KEY_ZERO, "0" };   keys[i++] = (KeyVisual){ KEY_MINUS, "-" };
    keys[i++] = (KeyVisual){ KEY_EQUAL, "=" };  keys[i++] = (KeyVisual){ KEY_BACKSPACE, "Back" };

    // --- FILA 3: Tab y QWERTY ---
    keys[i++] = (KeyVisual){ KEY_TAB, "Tab" };  keys[i++] = (KeyVisual){ KEY_Q, "Q" };
    keys[i++] = (KeyVisual){ KEY_W, "W" };      keys[i++] = (KeyVisual){ KEY_E, "E" };
    keys[i++] = (KeyVisual){ KEY_R, "R" };      keys[i++] = (KeyVisual){ KEY_T, "T" };
    keys[i++] = (KeyVisual){ KEY_Y, "Y" };      keys[i++] = (KeyVisual){ KEY_U, "U" };
    keys[i++] = (KeyVisual){ KEY_I, "I" };      keys[i++] = (KeyVisual){ KEY_O, "O" };
    keys[i++] = (KeyVisual){ KEY_P, "P" };      keys[i++] = (KeyVisual){ KEY_LEFT_BRACKET, "[" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_BRACKET, "]" }; 

    // --- FILA 4: Caps y ASDF ---
    keys[i++] = (KeyVisual){ KEY_CAPS_LOCK, "Caps" }; keys[i++] = (KeyVisual){ KEY_A, "A" };
    keys[i++] = (KeyVisual){ KEY_S, "S" };      keys[i++] = (KeyVisual){ KEY_D, "D" };
    keys[i++] = (KeyVisual){ KEY_F, "F" };      keys[i++] = (KeyVisual){ KEY_G, "G" };
    keys[i++] = (KeyVisual){ KEY_H, "H" };      keys[i++] = (KeyVisual){ KEY_J, "J" };
    keys[i++] = (KeyVisual){ KEY_K, "K" };      keys[i++] = (KeyVisual){ KEY_L, "L" };
    keys[i++] = (KeyVisual){ KEY_SEMICOLON, ";" }; keys[i++] = (KeyVisual){ KEY_APOSTROPHE, "'" };
    keys[i++] = (KeyVisual){ KEY_BACKSLASH, "\\" };keys[i++] = (KeyVisual){ KEY_ENTER, "Enter" };

    // --- FILA 5: Shift y ZXCV ---
    keys[i++] = (KeyVisual){ KEY_LEFT_SHIFT, "Shift" };  keys[i++] = (KeyVisual){161, "<>" };
    keys[i++] = (KeyVisual){ KEY_Z, "Z" }; 
    keys[i++] = (KeyVisual){ KEY_X, "X" };      keys[i++] = (KeyVisual){ KEY_C, "C" };
    keys[i++] = (KeyVisual){ KEY_V, "V" };      keys[i++] = (KeyVisual){ KEY_B, "B" };
    keys[i++] = (KeyVisual){ KEY_N, "N" };      keys[i++] = (KeyVisual){ KEY_M, "M" };
    keys[i++] = (KeyVisual){ KEY_COMMA, "," };  keys[i++] = (KeyVisual){ KEY_PERIOD, "." };
    keys[i++] = (KeyVisual){ KEY_SLASH, "/" };  keys[i++] = (KeyVisual){ KEY_RIGHT_SHIFT, "Shift" };
    keys[i++] = (KeyVisual){ KEY_UP, "^" }; 

    // --- FILA 6: Control, Space, Flechas ---
    keys[i++] = (KeyVisual){KEY_KP_0, "Fn"};
    keys[i++] = (KeyVisual){ KEY_LEFT_CONTROL, "Ctrl" };  keys[i++] = (KeyVisual){ KEY_LEFT_SUPER, "Win" };
    keys[i++] = (KeyVisual){ KEY_LEFT_ALT, "Alt" };     keys[i++] = (KeyVisual){ KEY_SPACE, "Space" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_ALT, "Alt" };    keys[i++] = (KeyVisual){ 283, "PrtSc" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_CONTROL, "Ctrl" }; keys[i++] = (KeyVisual){ 266, "RePag" };
    keys[i++] = (KeyVisual){ KEY_LEFT, "<" };             keys[i++] = (KeyVisual){ KEY_DOWN, "v" };
    keys[i++] = (KeyVisual){ KEY_RIGHT,">"};              keys[i++] = (KeyVisual){267, "AvPag"};
}

void draw_keyboard(void) 
{
    ClearBackground(BLACK);
    BeginDrawing();

    // Configuraciones base
    float base_w = 94.11f; // Ancho base de una tecla cuadrada
    float base_h = 80.0f; // Alto de la tecla
    float gap = 0.0f;     // Espacio entre teclas
    float start_x = 0.0f;
    float start_y = 0.0f;

    // Cuántas teclas hay en cada fila 
    int row_counts[] = { 17, 14, 13, 14, 14, 13 }; 
    int current_key = 0;

    for (int row = 0; row < 6; row++) {
        float x_offset = start_x;
        
        for (int k = 0; k < row_counts[row]; k++) {
            if (current_key >= 168) break;
            
            KeyVisual key = keys[current_key];
            
            // Lógica para anchos especiales (Opcional)
            float width = base_w;
            if (key.key_id == KEY_RIGHT_SHIFT) width = base_w * 3.0f;
            if (key.key_id == KEY_BACKSPACE || key.key_id == KEY_ENTER) width = base_w * 2.0f;
            if (key.key_id == KEY_SPACE) width = base_w * 5.0f;
            if (key.key_id == KEY_TAB) width = base_w * 1.33f;
            if (key.key_id == KEY_CAPS_LOCK) width = base_w * 1.5f;
            Rectangle rect = { x_offset, start_y + row * (base_h + gap), width, base_h };
            
            // Dibujo
            bool pressed = (key.key_id != 0) && IsKeyDown(key.key_id);
            DrawRectangleRec(rect, pressed ? RED : RAYWHITE);
            DrawRectangleLinesEx(rect, 2, GRAY);
            if (key.label != NULL) {
                // Centrar texto (aproximado)
                int text_w = MeasureText(key.label, 10);
                DrawText(key.label, rect.x + (width/2 - text_w/2), rect.y + (base_h/2 - 5), 20, BLACK);
            }

            x_offset += width + gap;
            current_key++;
        }
    }

    EndDrawing();
}


int main(int argc, char *argv[])
{
    printf("Welcome to the keyboard input\n");
    set_layout();
    InitWindow(WIDTH, HEIGHT, "Keyboard Input");
    SetTargetFPS(60);
    SetExitKey(0);    
    //draw keyboard
      
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);           
        draw_keyboard();
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
