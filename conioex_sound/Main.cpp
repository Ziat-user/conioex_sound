//#define CONIOEX
//#include "conioex.h"
#include <chrono>
#include <cstdio>
//#include <thread>
#include "sound.h"
#include <conio.h>
#include <windows.h> //Windowsの機能を呼ぶため

#pragma comment(lib, "winmm.lib")

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}//conioexにおけるgotoxy

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // FALSEで非表示、TRUEで表示
    SetConsoleCursorInfo(consoleHandle, &info);
}//conioexにおけるsetcursoltype

int main() {
    auto fps_s = std::chrono::milliseconds(1000 / 60);
    auto last = std::chrono::steady_clock::now();
    hideCursor();
    
    int frame_count = 0;
    int current_fps = 0;
    auto last_fps_time = std::chrono::steady_clock::now();
    /*unsigned long long handle = opensound("test_sound_stock.wav");*/

    bool is_space_pressed = false;
    char ch = 0;
    while (ch != 0x1b) {
        
        auto now = std::chrono::steady_clock::now();

        // 前回から指定した時間が経過しているか判定
        if (now - last >= fps_s) {
            gotoxy(0, 0);
            last = now;

            // FPSの計算（1秒ごとに更新）
            frame_count++;
            if (now - last_fps_time >= std::chrono::seconds(1)) {
                current_fps = frame_count;
                frame_count = 0;
                last_fps_time = now;
            }
            printf("FPS: %d    \n", current_fps);
            // スペースキーの入力判定
            // _kbhit/_getch のブロックを丸ごとこれに置き換え
            if (_kbhit()) ch = _getch(); // ESC検出用だけ残す

            bool space_now = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

            if (space_now) {
                printf("enter space\n");
                if (!is_space_pressed) {
                    is_space_pressed = true;
                    PlaySoundW((LPCWSTR)my_wav_data, NULL, SND_MEMORY | SND_ASYNC);
                }
            }
            else {
                if (is_space_pressed) {
                    is_space_pressed = false;
                    printf("            \n");
                }
            }
        }
    }
    /*closesound(handle);*/
    return 0;
}