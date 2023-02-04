/**
 * @file string.h
 * @brief 文字列関連
 *
 */
#if !defined(_STRING_H)
#define _STRING_H

#include "common.h"

typedef u16 charcode;
typedef struct
{
    u8 dummy[8];
    charcode buffer[1];
} String;

#define HEAP_UNUSED 2 // 未使用ヒープのID
#define _nweString ((String * (*)(u32, u32))0x02023131)
#define _deleteString ((void (*)(String *))0x02023105)

enum
{
    CHAR_SPACE = 1,
    CHAR_ZERO = 0xa2,
    CHAR_ONE,
    CHAR_TWO,
    CHAR_THREE,
    CHAR_FOUR,
    CHAR_FIVE,
    CHAR_SIX,
    CHAR_SEVEN,
    CHAR_EIGHT,
    CHAR_NINE,
    CHAR_TEN,
    CHAR_A,
    CHAR_B,
    CHAR_C,
    CHAR_D,
    CHAR_E,
    CHAR_F,
    CHAR_CR = 0xe000, // 改行
    CHAR_EOM = 0xffff,
};

// プロトタイプ宣言
static inline String *newString(u32 length);
static inline void deleteString(String *str);
static inline void u8toStrcode(u8 num, charcode *chars);
static inline void u32toStrcode(u32 num, charcode *chars);
static inline String *NumsToString(const u8 nums[], u32 size);
static inline String *NumsToStringWithSpace(const u8 nums[], u32 size);

/**
 * @brief 文字列を生成する
 * @param length 最大文字数
 * @return 生成された文字列のポインタ
 */
static inline String *newString(u32 length)
{
    return _nweString(length, HEAP_UNUSED);
}

/**
 * @brief 文字列を削除する
 * @param str 削除する文字列のポインタ
 */
static inline void deleteString(String *str)
{
    _deleteString(str);
}

/**
 * @brief u8の数値を16進数で表したcharcodeに変換する
 *
 * @param num 変換する数値
 * @param chars 変換された文字列を受け取るバッファ(2文字分)
 */
static inline void u8toStrcode(u8 num, charcode *chars)
{
    u8 low = num & 0xF;
    u8 high = num >> 4;
    chars[0] = (high + CHAR_ZERO);
    chars[1] = (low + CHAR_ZERO);
}

/**
 * @brief u32の数値を16進数で表したcharcodeに変換する
 *
 * @param num 変換する数値
 * @param chars 変換された文字列を受け取るバッファ(8文字分)
 */
static inline void u32toStrcode(u32 num, charcode *chars)
{
    u8 *_num = (u8 *)&num;
    for (int i = 0; i < 4; i++)
    {
        u8toStrcode(_num[3 - i], &chars[2 * i]);
    }
}

/**
 * @brief メモリ領域を16進数の文字列に変換
 *
 * @param nums 変換するメモリ領域の先頭
 * @param str 変換した数値を格納する文字列
 * @param size 変換するメモリ領域のサイズ (バイト)
 * @details 文字数のチェックは行わないので注意
 */
static inline String *NumsToString(const u8 nums[], u32 size)
{
    String *str = newString(2 * size + 1);
    for (int i = 0; i < size; i++)
    {
        u8toStrcode(nums[i], &str->buffer[2 * i]);
    }
    str->buffer[2 * size] = CHAR_EOM;
    return str;
}

/**
 * @brief メモリ領域を16進数の文字列に変換（空白入）
 *
 * @param nums 変換するメモリ領域の先頭
 * @param str 変換した数値を格納する文字列
 * @param size 変換するメモリ領域のサイズ (バイト)
 * @details 文字数のチェックは行わないので注意
 */
static inline String *NumsToStringWithSpace(const u8 nums[], u32 size)
{
    String *str = newString(3 * size);
    for (int i = 0; i < size; i++)
    {
        u8toStrcode(nums[i], &str->buffer[3 * i]);
        str->buffer[3 * i + 2] = CHAR_SPACE;
    }
    str->buffer[3 * size - 1] = CHAR_EOM;
    return str;
}

#endif // _STRING_H
