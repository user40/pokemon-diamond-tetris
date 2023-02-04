/**
 * @file eggcommon.h
 * @brief eggsmaker/loader関連の型定義
 *
 */

#if !defined(_EGGCOMMON_H)
#define _EGGCOMMON_H

#include "acehelper.h"

#define EGG_DATA1_SIZE 0x1c
#define EGG_DATA2_SIZE 0x58
#define EGG_DATA_SIZE (EGG_DATA1_SIZE+EGG_DATA2_SIZE)
#define EGG_DATA1_OFFSET 0x4
#define EGG_DATA2_OFFSET 0x28
#define EGG_DATA_SIZE (EGG_DATA1_SIZE+EGG_DATA2_SIZE)

typedef u8 data80[0x80];
// タマゴ１つに保存できるデータ
typedef u8 data74[0x74];
typedef struct
{
    u8 eggNum; // 展開したいタマゴの個数
    u8 dummy[2];
    u8 isAutoRun; // 展開と同時に実行する(1), 展開のみする(0)
    u16 isEncrypted0 : 1;
    u16 isEncrypted1 : 1;
    u16 isBadEgg : 1;
    u16 : 13;
    u16 checksum;
} Header;

typedef struct
{
    Header header;
    data80 data;
} BoxPokemonAsData;

typedef struct
{
    Header header;
    data74 data[1];
} EggsData;

#define headersize sizeof(Header)

#endif // _EGGCOMMON_H
