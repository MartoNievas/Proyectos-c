#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WIDTH 1600
#define HEIGHT 560

// Estructura solicitada
#define MAX_QUEUE 128
char queue_keys[MAX_QUEUE];
int queue_len = 0;

typedef struct {
    int key_id;
    char* label;
} KeyVisual;

KeyVisual keys[87];

static void set_layout(void)
{
    // Inicializar todo a 0
    for (int i = 0; i < 87; i++) {
        keys[i].key_id = 0;
        keys[i].label = 0;
    }

    int i = 0;

    // --- FILA 1: Esc, Funciones, Navegación ---
    keys[i++] = (KeyVisual){ KEY_ESCAPE, "Esc" };
    keys[i++] = (KeyVisual){ KEY_F1, "F1" };
    keys[i++] = (KeyVisual){ KEY_F2, "F2" };
    keys[i++] = (KeyVisual){ KEY_F3, "F3" };
    keys[i++] = (KeyVisual){ KEY_F4, "F4" };
    keys[i++] = (KeyVisual){ KEY_F5, "F5" };
    keys[i++] = (KeyVisual){ KEY_F6, "F6" };
    keys[i++] = (KeyVisual){ KEY_F7, "F7" };
    keys[i++] = (KeyVisual){ KEY_F8, "F8" };
    keys[i++] = (KeyVisual){ KEY_F9, "F9" };
    keys[i++] = (KeyVisual){ KEY_F10, "F10" };
    keys[i++] = (KeyVisual){ KEY_F11, "F11" };
    keys[i++] = (KeyVisual){ KEY_F12, "F12" };
    keys[i++] = (KeyVisual){ KEY_HOME, "Home" };
    keys[i++] = (KeyVisual){ KEY_END, "End" };
    keys[i++] = (KeyVisual){ KEY_INSERT, "Ins" };
    keys[i++] = (KeyVisual){ KEY_DELETE, "Del" };

    // --- FILA 2: Números y Símbolos ---
    keys[i++] = (KeyVisual){ KEY_GRAVE, "~" };
    keys[i++] = (KeyVisual){ KEY_ONE, "1" };
    keys[i++] = (KeyVisual){ KEY_TWO, "2" };
    keys[i++] = (KeyVisual){ KEY_THREE, "3" };
    keys[i++] = (KeyVisual){ KEY_FOUR, "4" };
    keys[i++] = (KeyVisual){ KEY_FIVE, "5" };
    keys[i++] = (KeyVisual){ KEY_SIX, "6" };
    keys[i++] = (KeyVisual){ KEY_SEVEN, "7" };
    keys[i++] = (KeyVisual){ KEY_EIGHT, "8" };
    keys[i++] = (KeyVisual){ KEY_NINE, "9" };
    keys[i++] = (KeyVisual){ KEY_ZERO, "0" };
    keys[i++] = (KeyVisual){ KEY_MINUS, "-" };
    keys[i++] = (KeyVisual){ KEY_EQUAL, "=" };
    keys[i++] = (KeyVisual){ KEY_BACKSPACE, "Back" };

    // --- FILA 3: Tab y QWERTY ---
    keys[i++] = (KeyVisual){ KEY_TAB, "Tab" };
    keys[i++] = (KeyVisual){ KEY_Q, "Q" };
    keys[i++] = (KeyVisual){ KEY_W, "W" };
    keys[i++] = (KeyVisual){ KEY_E, "E" };
    keys[i++] = (KeyVisual){ KEY_R, "R" };
    keys[i++] = (KeyVisual){ KEY_T, "T" };
    keys[i++] = (KeyVisual){ KEY_Y, "Y" };
    keys[i++] = (KeyVisual){ KEY_U, "U" };
    keys[i++] = (KeyVisual){ KEY_I, "I" };
    keys[i++] = (KeyVisual){ KEY_O, "O" };
    keys[i++] = (KeyVisual){ KEY_P, "P" };
    keys[i++] = (KeyVisual){ KEY_LEFT_BRACKET, "[" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_BRACKET, "]" };

    // --- FILA 4: Caps y ASDF ---
    keys[i++] = (KeyVisual){ KEY_CAPS_LOCK, "Caps" };
    keys[i++] = (KeyVisual){ KEY_A, "A" };
    keys[i++] = (KeyVisual){ KEY_S, "S" };
    keys[i++] = (KeyVisual){ KEY_D, "D" };
    keys[i++] = (KeyVisual){ KEY_F, "F" };
    keys[i++] = (KeyVisual){ KEY_G, "G" };
    keys[i++] = (KeyVisual){ KEY_H, "H" };
    keys[i++] = (KeyVisual){ KEY_J, "J" };
    keys[i++] = (KeyVisual){ KEY_K, "K" };
    keys[i++] = (KeyVisual){ KEY_L, "L" };
    keys[i++] = (KeyVisual){ KEY_SEMICOLON, ";" };
    keys[i++] = (KeyVisual){ KEY_APOSTROPHE, "'" };
    keys[i++] = (KeyVisual){ KEY_BACKSLASH, "\\" };
    keys[i++] = (KeyVisual){ KEY_ENTER, "Enter" };

    // --- FILA 5: Shift y ZXCV ---
    keys[i++] = (KeyVisual){ KEY_LEFT_SHIFT, "Shift" };
    keys[i++] = (KeyVisual){ 161, "<>" };
    keys[i++] = (KeyVisual){ KEY_Z, "Z" };
    keys[i++] = (KeyVisual){ KEY_X, "X" };
    keys[i++] = (KeyVisual){ KEY_C, "C" };
    keys[i++] = (KeyVisual){ KEY_V, "V" };
    keys[i++] = (KeyVisual){ KEY_B, "B" };
    keys[i++] = (KeyVisual){ KEY_N, "N" };
    keys[i++] = (KeyVisual){ KEY_M, "M" };
    keys[i++] = (KeyVisual){ KEY_COMMA, "," };
    keys[i++] = (KeyVisual){ KEY_PERIOD, "." };
    keys[i++] = (KeyVisual){ KEY_SLASH, "/" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_SHIFT, "Shift" };
    keys[i++] = (KeyVisual){ KEY_UP, "^" };

    // --- FILA 6: Control, Space, Flechas ---
    keys[i++] = (KeyVisual){ KEY_KP_0, "Fn" };
    keys[i++] = (KeyVisual){ KEY_LEFT_CONTROL, "Ctrl" };
    keys[i++] = (KeyVisual){ KEY_LEFT_SUPER, "Win" };
    keys[i++] = (KeyVisual){ KEY_LEFT_ALT, "Alt" };
    keys[i++] = (KeyVisual){ KEY_SPACE, "Space" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_ALT, "Alt" };
    keys[i++] = (KeyVisual){ 283, "PrtSc" };
    keys[i++] = (KeyVisual){ KEY_RIGHT_CONTROL, "Ctrl" };
    keys[i++] = (KeyVisual){ 266, "RePag" };
    keys[i++] = (KeyVisual){ KEY_LEFT, "<" };
    keys[i++] = (KeyVisual){ KEY_DOWN, "v" };
    keys[i++] = (KeyVisual){ KEY_RIGHT, ">" };
    keys[i++] = (KeyVisual){ 267, "AvPag" };
}

void draw_keyboard(void)
{
    BeginDrawing();

    float base_w = 94.11f;
    float base_h = 80.0f;
    float gap = 0.0f;
    float start_x = 0.0f;
    float start_y = 80.0f;

    int row_counts[] = { 17, 14, 13, 14, 14, 13 };
    int current_key = 0;

    for (int row = 0; row < 6; row++) {
        float x_offset = start_x;

        for (int k = 0; k < row_counts[row]; k++) {
            if (current_key >= 87) break;

            KeyVisual key = keys[current_key];

            float width = base_w;
            if (key.key_id == KEY_RIGHT_SHIFT) width = base_w * 2.0f;
            if (key.key_id == KEY_BACKSPACE || key.key_id == KEY_ENTER) width = base_w * 2.0f;
            if (key.key_id == KEY_SPACE) width = base_w * 5.0f;
            if (key.key_id == KEY_TAB) width = base_w * 1.33f;
            if (key.key_id == KEY_CAPS_LOCK) width = base_w * 1.5f;

            Rectangle rect = { x_offset, start_y + row * (base_h + gap), width, base_h };

            bool pressed = (key.key_id != 0) && IsKeyDown(key.key_id);
            DrawRectangleRec(rect, pressed ? RED : RAYWHITE);
            DrawRectangleLinesEx(rect, 2, GRAY);

            if (key.label != NULL) {
                int text_w = MeasureText(key.label, 10);
                DrawText(key.label, rect.x + (width/2 - text_w/2), rect.y + (base_h/2 - 5), 20, BLACK);
            }

            x_offset += width + gap;
            current_key++;
        }
    }

    EndDrawing();
}

char* key_code_to_string(int key_code)
{
  switch (key_code) 
  { 
    case KEY_ESCAPE: return " Esc";
    case KEY_F1:  return " F1"; 
    case KEY_F2: return " F2"; 
    case KEY_F3: return " F3";
    case KEY_F4: return " F4"; 
    case KEY_F5: return " F5";
    case KEY_F6: return " F6";
    case KEY_F7: return " F7";
    case KEY_F8: return " F8";
    case KEY_F9: return " F9";
    case KEY_F10: return " F10";
    case KEY_F11: return " F11";
    case KEY_F12: return " F12";
    case KEY_HOME: return " Home";
    case KEY_END: return " End";
    case KEY_INSERT: return " Ins";
    case KEY_DELETE: return " Del";
    case KEY_BACKSPACE: return " Back";
    case KEY_TAB: return " Tab";
    case KEY_ENTER: return " Enter";
    case KEY_CAPS_LOCK: return " Caps";
    case KEY_LEFT_SHIFT: return " Shift";
    case KEY_RIGHT_SHIFT: return " Shift";
    case KEY_LEFT_CONTROL: return " Ctrl";
    case KEY_LEFT_SUPER: return " Win";
    case KEY_LEFT_ALT: return " Alt";
    case KEY_SPACE: return " Space";
    case KEY_RIGHT_ALT: return " Alt";
    case 283: return " PtrSc";
    case KEY_RIGHT_CONTROL: return " Ctrl";
    case 266: return " RePag";
    case 267: return " AvPag";
    case KEY_UP: return " ^";
    case KEY_LEFT: return " <";
    case KEY_DOWN: return  " v";
    case KEY_RIGHT: return " >";
  }
  return "";
}

void draw_queue_keys(void)
{
    int ch;

    // 1️⃣ Caracteres imprimibles
    while ((ch = GetCharPressed()) > 0)
    {
        if (queue_len < MAX_QUEUE - 1)
        {
            queue_keys[queue_len++] = ' ';
            queue_keys[queue_len++] = (char)ch;
            queue_keys[queue_len] = '\0';
        }
    }

    // 2️⃣ Teclas especiales
    int key;
    while ((key = GetKeyPressed()) > 0)
    {
        const char* text = key_code_to_string(key);
        if (!text) continue;

        int len = strlen(text);
        if (queue_len + len < MAX_QUEUE - 1)
        {
            strcat(queue_keys, text);
            queue_len += len;
        }
    }

    // 3️⃣ Limpieza automática (efecto cola)
    if (queue_len > 115)
    {
        queue_len = 0;
        queue_keys[queue_len] = '\0';
    }

    DrawText(queue_keys, 20, 40, 28, RAYWHITE);
}



int main(int argc, char *argv[])
{
    printf("Welcome to the keyboard input\n");

    set_layout();

    InitWindow(WIDTH, HEIGHT, "Keyboard Input");

    ClearBackground(BLACK);

    Rectangle rect_queue = (Rectangle) {0,0,WIDTH,80};
    DrawRectangleRec(rect_queue,GRAY);

    SetTargetFPS(60);

    SetExitKey(0);

    int i = 0;

    while (!WindowShouldClose()) {
        draw_keyboard();
        ClearBackground(GRAY);
        draw_queue_keys();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
