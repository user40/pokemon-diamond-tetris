/**
 * @file memory_viewer.c
 * @brief eggloader用テトリスプログラム
 *
 * @details テトリス本体のロジックは https://github.com/taylorconor/tinytetris
 * を改変して使用しています。
 */

#include "acehelper.h"
#include "window.h"
#include "input.h"
#include "job.h"

#define winPosX 1
#define winPosY 1
#define winWidth 10
#define winHeight 20
#define SCORE_LENGTH 8
#define RECT_COLOR 10
#define BOX_SIZE 6
#define BOX_MARGIN 2
#define GRID_SIZE (BOX_SIZE + BOX_MARGIN)

enum
{
    WAIT = 1,
    PLAYING,
    GAMEOVER,
};

// テトリス本体用の定数
// block layout is: {w-1,h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each)
const u32 X = 431424, Y = 598356, R = 427089, PX = 247872, PY = 799248, PR,
          C = 348480, P = 615696;
const u32 block[7][4] = {{X, Y, X, Y},
                         {R, P, R, P},
                         {C, C, C, C},
                         {599636, 431376, 598336, 432192},
                         {411985, 610832, 415808, 595540},
                         {PX, PY, PX, PY},
                         {614928, 399424, 615744, 428369}};

// グローバル変数
int state;
Window *win;
String *str;
Job *job;

u8 x, y, r, px, py, pr, p, tick;
u32 c, score;
u8 board[20][10];

// プロトタイプ宣言
void job_func();
inline static void init();
inline static void init_tetris();
inline static void terminate();
inline static void draw();
inline static bool do_tick();
inline static void control();
inline static void new_piece();

void main()
{
    state = WAIT;
    job = addJob((jobFunc)&job_func, 0, 0);
}

void job_func()
{
    switch (state)
    {
    case WAIT:
        // Lボタンが押されていたら開始
        if (pad_state_keydown & pad_keyL)
        {
            init();
            init_tetris();
            state = PLAYING;
        }
        break;

    case PLAYING:
        // Lボタンが押されていたら終了
        if (pad_state_keydown & pad_keyL)
        {
            terminate();
            return;
        }
        tick++;
        fillWindow(win, COLOR_WHITE);
        if (!do_tick())
        {
            state = GAMEOVER;
            return;
        }
        control();
        draw();
        showWindow(win);
        break;

    case GAMEOVER:
        // Lボタンが押されたら終了
        if (pad_state_keydown & pad_keyL)
        {
            terminate();
            return;
        }
        // Aボタンが押されたらリスタート
        if (pad_state_keydown & pad_keyA)
        {
            init_tetris();
            state = PLAYING;
            return;
        }
        fillWindow(win, COLOR_WHITE);
        draw();
        showWindow(win);
        break;

    default:
        state = WAIT;
    }
}

inline static void init()
{
    win = newWindow(winPosX, winPosY, winWidth, winHeight);
    str = newString(SCORE_LENGTH);
}

inline static void init_tetris()
{
    score = 0;
    memset(&board[0][0], 0, 10*20);
    new_piece();
}

inline static void terminate()
{
    hideWindow(win);
    deleteWindow(win);
    deleteString(str);
    deleteJob(job);
}

inline static void draw()
{
    // ブロックの描画
    for (u32 i = 0; i < 20; i++)
    {
        for (u32 j = 0; j < 10; j++)
        {
            if (board[i][j] > 0)
            {
                u16 px = BOX_MARGIN + GRID_SIZE * j;
                u16 py = BOX_MARGIN + GRID_SIZE * i;
                // TEST 矩形領域の塗りつぶし
                fillWindowArea(win, board[i][j], px, py, BOX_SIZE, BOX_SIZE);
            }
        }
    }

    // スコアの描画
    // u32toStrcode(score, str->buffer);
    // printStr(win, str);
}

// extract a 2-bit number from a block entry
inline static u8 NUM(u8 x, u8 y) { return 3 & block[p][x] >> y; }

// create a new piece, don't remove old one (it has landed and should stick)
inline static void new_piece()
{
    y = py = 0;
    p = rand() % 7;
    r = pr = rand() % 4;
    x = px = rand() % (10 - NUM(r, 16));
}

// set the value fo the board for a particular (x,y,r) piece
inline static void set_piece(u8 x, u8 y, u8 r, u8 v)
{
    for (u8 i = 0; i < 8; i += 2)
    {
        board[NUM(r, i * 2) + y][NUM(r, (i * 2) + 2) + x] = v;
    }
}

// move a piece from old (p*) coords to new
inline static void update_piece()
{
    set_piece(px, py, pr, 0);
    set_piece(px = x, py = y, pr = r, p + 1);
}

// remove line(s) from the board if they're full
inline static void remove_line()
{
    for (u8 row = y; row <= y + NUM(r, 18); row++)
    {
        c = 1;
        for (u8 i = 0; i < 10; i++)
        {
            c *= board[row][i];
        }
        if (!c)
        {
            continue;
        }
        for (u8 i = row - 1; i > 0; i--)
        {
            memcpy(&board[i + 1][0], &board[i][0], 10);
        }
        memset(&board[0][0], 0, 10);
        score++;
    }
}

// check if placing p at (x,y,r) will be a collision
inline static u8 check_hit(u8 x, u8 y, u8 r)
{
    if (y + NUM(r, 18) > 19)
    {
        return 1;
    }
    set_piece(px, py, pr, 0);
    c = 0;
    for (u8 i = 0; i < 8; i += 2)
    {
        board[y + NUM(r, i * 2)][x + NUM(r, (i * 2) + 2)] && c++;
    }
    set_piece(px, py, pr, p + 1);
    return c;
}

// slowly tick the piece y position down so the piece falls
inline static bool do_tick()
{
    if (++tick > 30)
    {
        tick = 0;
        if (check_hit(x, y + 1, r))
        {
            if (!y)
            {
                return 0;
            }
            remove_line();
            new_piece();
        }
        else
        {
            y++;
            update_piece();
        }
    }
    return 1;
}

inline static void control()
{
    if ((pad_state_keydown & pad_keyLeft) && x > 0 && !check_hit(x - 1, y, r))
    {
        x--;
    }
    if ((pad_state_keydown & pad_keyRight) && x + NUM(r, 16) < 9 && !check_hit(x + 1, y, r))
    {
        x++;
    }
    if (pad_state_keydown & pad_keyDown)
    {
        while (!check_hit(x, y + 1, r))
        {
            y++;
            update_piece();
        }
        remove_line();
        new_piece();
    }
    if (pad_state_keydown & pad_keyUp)
    {
        r++;
        r %= 4;
        while (x + NUM(r, 16) > 9)
        {
            x--;
        }
        if (check_hit(x, y, r))
        {
            x = px;
            r = pr;
        }
    }
    if (c == 'q')
    {
        return;
    }
    update_piece();
}