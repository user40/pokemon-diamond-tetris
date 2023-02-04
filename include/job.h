#ifndef _JOB_H
#define _JOB_H

#include "common.h"

#define pp_jobsys (0x021c6118 + 0x18)
#define fp_addJob 0x0201c26d
#define fp_deleteJob 0x0201c1c1

typedef struct _Job
{
    u8 data[0x1c];
} Job;

/**
 * @brief ジョブに登録される関数の型。
 * 登録したジョブへのポインタと利用するワーキングエリアのポインタを受け取る。
 */
typedef void (*jobFunc)(Job *, void *);

/**
 * ジョブを追加する
 * @param func 実行する関数
 * @param work funcが利用するワーキングエリアへのポインタ
 * @param priority 実行優先度　小さいほど先に実行される
 * @return 作成されたジョブ構造体へのポインタ
 * @details 関数をジョブに登録すると1フレームにつき1回呼び出されるようになる。
 */
static inline Job *addJob(jobFunc func, void *work, u32 priority)
{
    return ((Job * (*)(void *, jobFunc, void *, u32)) fp_addJob)(*((void **)pp_jobsys), func, work, priority);
}

/**
 * ジョブを削除する
 * @param job 削除するジョブへのポインタ
 */
static inline void deleteJob(Job *job)
{
    ((void (*)(Job *))fp_deleteJob)(job);
}

#endif // _JOB_H
