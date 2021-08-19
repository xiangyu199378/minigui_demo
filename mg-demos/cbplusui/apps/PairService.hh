///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
** This file is a part of mg-demos package.
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
** Copyright (C) 2018 Beijing Joobot Technologies Co., Ltd.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
/*!============================================================================
 * @file PairService.hh
 * @Synopsis
 * @author Vincent Wei
 * @version 1.0
 * @date 25/05/2018
 */

#ifndef _PAIR_SERVICE_HH_
#define _PAIR_SERVICE_HH_

#include <string>
#include <vector>

#include "AsyncTask.hh"

#define TASKID_PAIR_MAGIC        0x0300
#define TASKID_PAIR_CHECK        (TASKID_PAIR_MAGIC | 0x0000)
#define TASKID_PAIR_GETCODE      (TASKID_PAIR_MAGIC | 0x0001)
#define TASKID_PAIR_EXECUTE      (TASKID_PAIR_MAGIC | 0x0002)

#define LEN_PART_NO             32
#define LEN_DEVICE_NO           16
#define LEN_URL_PATH            1024

#define PS_SYSTEM_ERR           0
#define PS_CHECK_PAIRED         1
#define PS_CHECK_UNPAIRED       2
#define PS_GETCODE_OK           3
#define PS_EXECUTE_OK           4
#define PS_EXECUTE_TIMEOUT      5
#define PS_ERR_NETWORK          6

typedef struct _PAIR_INFO {
    int status;

    char* user_id;
    char* nickname;
    char* code;
    char* qrcode_path;
} PAIR_INFO;

class AsyncTaskPairCheck : public AsyncTask
{
public:
    virtual void* execute_task (pthread_t thread_id, int task_id, DWORD add_data);
    virtual void cleanup_task (pthread_t thread_id, void* res);
};

class AsyncTaskPairGetCode : public AsyncTask
{
public:
    virtual void* execute_task (pthread_t thread_id, int task_id, DWORD add_data);
    virtual void cleanup_task (pthread_t thread_id, void* res);
};

class AsyncTaskPairExecute : public AsyncTask
{
public:
    virtual bool init_task (pthread_t thread_id, int task_id, DWORD add_data);
    virtual void* execute_task (pthread_t thread_id, int task_id, DWORD add_data);
    virtual void cleanup_task (pthread_t thread_id, void* res);

private:
    char m_args [64];

};

class PairService
{
public:
    static PairService* singleton ();

    AsyncTaskPairCheck* checkPair (HWND hwnd);
    AsyncTaskPairGetCode* getCode (HWND hwnd);
    AsyncTaskPairExecute* executePair (HWND hwnd, const char* code);

private:

    PairService ();
    ~PairService ();

    static PairService* s_service;

    class Gc
    {
    public:
        ~Gc() {
            if (PairService::s_service)
                delete PairService::s_service;
        }
    };

    static Gc gc;
};

#define PAIRSERVICE PairService::singleton()

#endif

