#define CONIOEX
#include "conioex.h"
#include <chrono>
#include <cstdio>
#include <thread>
#include "sound.h"
#include <windows.h> //Windowsの機能を呼ぶため

int main() {
    auto fps_s = std::chrono::milliseconds(1000 / 60);
    auto last = std::chrono::steady_clock::now();
    setcursortype(NOCURSOR);
    
    int frame_count = 0;
    int current_fps = 0;
    auto last_fps_time = std::chrono::steady_clock::now();
    /*unsigned long long handle = opensound("test_sound_stock.wav");*/

    bool is_space_pressed = false;

    while (!inport(PK_ESC)) {
        auto now = std::chrono::steady_clock::now();

        // 前回から指定した時間が経過しているか判定
        if (now - last >= fps_s) {
            gotoxy(1, 1);
            last = now;

            // FPSの計算（1秒ごとに更新）
            frame_count++;
            if (now - last_fps_time >= std::chrono::seconds(1)) {
                current_fps = frame_count;
                frame_count = 0;
                last_fps_time = now;
            }

            // スペースキーの入力判定
            if (inport(VK_SPACE)) {
                printf("enter space\n");

                // 前のフレームで押されていなかった場合のみ処理する
                if (!is_space_pressed) {
                    is_space_pressed = true;
                    PlaySoundW((LPCWSTR)my_wav_data, NULL, SND_MEMORY | SND_ASYNC);
                    // SND_MEMORY : ファイルではなく、メモリ(配列)のデータを使う
                    // SND_ASYNC  : 処理を止めず、裏側で勝手に再生させる(FPS低下を防ぐ)
                    //conioexのplaysoundよりも、軽い
                    {
                        /*
                        * 以下オプション
                        * SND_MEMORY   : C言語の配列（unsigned char[]）などに変換したWAVデータを指定します。
                        * SND_FILENAME : "sound.wav" のように直接ファイルパスを指定して再生します。
                        * SND_RESOURCE : Visual Studioなどで .rc ファイルを使ってexeに埋め込んだ音声を再生します。（配列化の別ルートのようなものです）
                        * SND_ALIAS    : Windowsのシステム音（"SystemStart" など）を鳴らしたい時に使います。
                        * 
                        * SND_ASYNC    : 音の再生をWindowsに任せ、プログラムはすぐに次の行へ進みます。FPSが落ちません。
                        * SND_SYNC     : （デフォルト設定）音が鳴り終わるまで、プログラムがそこでフリーズしたように止まります。
                        * SND_LOOP     : 音が最後までいったら自動で最初から再生します。BGMに最適です。※必ず SND_ASYNC とセットで使います。（SND_SYNCと混ぜると一生プログラムが止まります）
                        * 
                        * SND_NODEFAULT: 指定したファイルや配列が見つからなかった時、Windows標準の「ポーン」という警告音を勝手に鳴らさないようにします。
                        * SND_NOSTOP   : すでに PlaySound で別の音が鳴っている場合、後から呼ばれたこの音をキャンセル（鳴らさない）します。（連打による音の途切れを防ぐ時に使います）
                        * SND_PURGE    : 再生中の音を止めます。
                        * 
                        * オプションフラグは `|` (ビットor演算子)でつなげます。
                        * PlaySound(NULL, NULL, 0); でSND_PURGEと同じ事ができます。
                        * PlaySound は「同時に1つの音」しか鳴らせないというWindowsの仕様があります。 もし「BGMを鳴らしながら、ジャンプ音も鳴らしたい（和音・複数再生）」という場合は、別の方法が必要です。
                        */
                    }
                }
            }
            else {
                // スペースキーを離したらフラグを戻す
                is_space_pressed = false;
                printf("            \n");
            }
            printf("FPS: %d    \n", current_fps);
        }
    }
    /*closesound(handle);*/
    return 0;
}