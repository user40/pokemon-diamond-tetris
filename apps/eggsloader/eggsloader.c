/*
 * 複数のタマゴに保存されたデータを連結し、指定のアドレスにコピーする。また指定があればそれを実行する。
 *
 * マーキングACEで使用するプログラム。このプログラムが書き込まれたタマゴにマークをつけると、
 * その右に続くタマゴのデータを0x0229008にコピーする。
 * コピーするタマゴの数と、コピー後に実行するか否かは1つ目のタマゴのヘッダで指定する;
 * eggNum はコピーするタマゴの数を指定する。
 * isAutRun が1のときはコピー後にそれを実行する。
 */

#include "acehelper.h"
#include "eggcommon.h"

#define defalutTarget 0x02290000
#define offsetScale 0x100

#define call(funcAddr, mark) (((void (*)(u8))funcAddr)(mark))
static inline void _memcpy(void *target, const void *source, u32 size);

void main(BoxPokemon *selected, u8 mark)
{
    // データを展開するアドレス
    EggsData *target = (EggsData *)defalutTarget;
    // sourceはマークしたポケモンの次のポケモンを指す
    BoxPokemonAsData *source = (BoxPokemonAsData *)(selected + 1);
    u8 eggNum = source[0].header.eggNum;

    // 卵のデータを指定したアドレスにコピー
    for (int i = 0; i < eggNum; i++)
    {
        // シャッフルタイプは0(ABCD)を想定している。
        // クラッシュをさけるために種族ID(0x8~0xc)と技ID(0x28~0x30)は使用しない。
        // 0xc~0x28をコピー
        _memcpy(
            (u8 *)&target->data[i],
            (u8 *)&source[i].data + EGG_DATA1_OFFSET,
            EGG_DATA1_SIZE);
        // 0x30~0x88 をコピー
        _memcpy(
            (u8 *)&target->data[i] + EGG_DATA1_SIZE,
            (u8 *)&source[i].data + EGG_DATA2_OFFSET,
            EGG_DATA2_SIZE);
    }

    // 実行
    if (source[0].header.isAutoRun == 1)
    {
        // キャッシュラインを書き戻す
        DC_StoreAll();
        IC_InvalidateAll();
        
        call(((u32)target->data) + 1, mark);    // +1 for thumb mode
    }
}

static inline void _memcpy(void *target, const void *source, u32 size)
{
    register const void *_source __asm__("r0") = source;
    register void *_target __asm__("r1") = target;
    register u32 _hwordCount __asm__("r2") = size / 2;
    __asm__ volatile(
        "swi #0xb"
        :
        : "r"(_source), "r"(_target), "r"(_hwordCount)
        : "r3");
}