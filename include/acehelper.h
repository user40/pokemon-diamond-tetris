#ifndef _ACEHELPER_H
#define _ACEHELPER_H

#ifdef __cplusplus
/* Dummy function to avoid linker complaints */
extern "C" void __aeabi_unwind_cpp_pr0(void){};
#endif

#include "common.h"

typedef struct
{
    u32 pid;
    u16 isEncrypted0 : 1;
    u16 isEncrypted1 : 1;
    u16 isBadEgg : 1;
    u16 : 13;
    u16 checksum;
    u8 abcd[0x80];
} BoxPokemon;

#define p_base ((u32 *)0x02108818)
#define pp_fsys 0x021c7270
#define p_sys 0x021c6118

#define pause_flag (*((u32 *)(p_base + 0x22AAC)))   // フィールドでの操作を無効にする

#define fp_malloc 0x02017099
#define fp_free 0x02016fdd
#define fp_gf_rand 0x0201c909
#define fp_decrypt 0x0206ee7d
#define fp_encrypt 0x0206eea5
#define fp_loadOverlay 0x0200635d
#define fp_unloadOverlay 0x02006471
#define fp_DC_StoreAll 0x020ce510
#define fp_IC_InvalidateAll 0x020ce5d8
#define fp_memcpy 0x020e17b0
#define fp___fill_mem 0x020e17d0

#define HEAP_UNUSED 2

static inline void *malloc(u32 size)
{
    return ((void *(*)(u32, u32))fp_malloc)(HEAP_UNUSED, size);
}

static inline void free(void *ptr)
{
    ((void (*)(void *))fp_free)(ptr);
}

#define memcpy(dest, src, n) ((void* (*) (void*, void*, u32))fp_memcpy)(dest, src, n)

static inline void *memset(void *buf, u8 ch, u32 n)
{
    for(int i=0; i<n; i++)
    {
        ((u8*)buf)[i] = ch;
    }
    return buf;
}

static inline u16 rand()
{
    return ((u16 (*)(void))fp_gf_rand)();
}

static inline void decrypt(BoxPokemon *poke)
{
    ((void (*)(BoxPokemon *))fp_decrypt)(poke);
}

static inline void encrypt(BoxPokemon *poke)
{
    ((void (*)(BoxPokemon *, u32))fp_encrypt)(poke, 1);
}

// static inline bool loadOverlay(u32 id) {
//     return ((bool (*)(u32, u32))fp_loadOverlay) (id, 0);
// }

// static inline void unloadOverlay(u32 id) {
//     ((void (*)(u32))fp_unloadOverlay) (id);
// }

/**
 * @brief Clean entire data cache.
 */
static inline void DC_StoreAll()
{
    ((void (*)())fp_DC_StoreAll)();
}

/**
 * @brief Flush entire instruction cache.
 */
static inline void IC_InvalidateAll()
{
    ((void (*)())fp_IC_InvalidateAll)();
}

#endif
