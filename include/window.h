/**
 * @file window.h
 * @brief ウィンドウ描画関連
 *
 */

#if !defined(_WINDOW_H)
#define _WINDOW_H

#include "acehelper.h"
#include "string.h"

// 型・定数・マクロ定義
typedef struct _BGSetting BGSetting;
typedef struct
{
    BGSetting *ini;
    u8 dummy[0xc];
} Window;

#define PALLET_13 13
#define COLOR_WHITE 15
#define BG0 0
#define BG1 1
#define BG2 2
#define BG3 3
#define HEAP_FIELD 11
#define HEAP_UNUSED 2

#define fsys (*(void **)pp_fsys)
#define fsysBgl (*(BGSetting **)((u32)fsys + 8))

#define _fillWindowArea ((void (*)(Window*, u8, u16, u16, u16, u16))0x02019051)
#define _newWindow ((Window * (*)(u32, u32))0x020197c5)
#define _deleteWindow ((void (*)(Window *, u8))0x020195fd)
#define _registerWindow ((void (*)(BGSetting *, Window *, u8, u8, u8, u8, u8, u8, u16))0x020196e9)
#define _fillWindow ((void (*)(Window *, u8))0x02019149)
#define _printStr ((void (*)(Window *, u32, String *, u32, u32, u32, void *))0x0201ce65)
#define _hideWindow ((void (*)(Window *))0x02019201)
#define _clearWindowArea ((void (*)(Window *))0x020194f1)
#define _showWindow ((void (*)(Window *))0x0201955d)

// プロトタイプ宣言
static inline Window *newWindow(u32 x, u32 y, u32 width, u32 height);
static inline void printStr(Window *win, String *str);
static inline void fillWindow(Window *win, u8 color);

/**
 * @brief ウィンドウを生成する
 *
 * @param x ウィンドウの左上隅のx座標
 * @param y ウィンドウの左上隅のy座標
 * @param width ウィンドウの幅（文字数）
 * @param height ウィンドウの高さ（文字数）
 * @return Window* 生成したウィンドウへのポインタ
 * @details 設定ファイルをfsysから流用しているので、PC画面などでは利用できない
 */
inline static Window *newWindow(u32 x, u32 y, u32 width, u32 height)
{
    Window *win = _newWindow(HEAP_UNUSED, 1);
    _registerWindow(fsysBgl, win, BG3, x, y, width, height, PALLET_13, 1);
    _fillWindow(win, COLOR_WHITE);
    return win;
}

/**
 * @brief ウィンドウを削除する
 *
 * @param win 削除するウィンドウへのポインタ
 */
inline static void deleteWindow(Window *win)
{
    _deleteWindow(win, 1);
}

/**
 * @brief ウィンドウに文字列を描画する
 *
 * @param win ウィンドウへのポインタ
 * @param str 文字列へのポインタ
 */
inline static void printStr(Window *win, String *str)
{
    _printStr(win, 0, str, 0, 0, 0xff, NULL);
}

/**
 * @brief ウィンドウを単色で塗りつぶす
 *
 * @param win ウィンドウへのポインタ
 * @param color 色番号。パレット内の番号(0~255)で指定
 */
inline static void fillWindow(Window *win, u8 color)
{
    _fillWindow(win, color);
}

/**
 * @brief ウィンドウを非表示にする
 *
 * @param win 非表示にするウィンドウへのポインタ
 */
inline static void hideWindow(Window *win)
{
    _hideWindow(win);
}

/**
 * @brief ウィンドウの描画される領域をクリアする
 *
 * @param win クリアするウィンドウへのポインタ
 */
inline static void clearWindowArea(Window *win)
{
    _clearWindowArea(win);
}

/**
 * @brief ウィンドウを描画する
 *
 * @param win 描画するウィンドウへのポインタ
 */
inline static void showWindow(Window *win)
{
    _showWindow(win);
}

/**
 * @brief 指定範囲を塗りつぶす
 *
 * @param   win         BMPデータ
 * @param   col         塗りつぶしカラー
 * @param   px          開始X座標
 * @param   py          開始Y座標
 * @param   sx          Xサイズ
 * @param   sy          Yサイズ
 *
 * @return  none
 */
inline static void fillWindowArea(Window *win, u8 col, u16 px, u16 py, u16 sx, u16 sy )
{
    _fillWindowArea(win, col, px, py, sx, sy);
}

#endif // _WINDOW_H
