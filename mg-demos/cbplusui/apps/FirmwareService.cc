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
 * @file FirmwareService.cc
 * @Synopsis
 * @author Vincent Wei
 * @version 1.0
 * @date 17/05/2018
 */

#include <unistd.h>
#include <jbus.h>

#include "global.h"
#include "resource.h"

#include "cJSON.h"
#include "FirmwareService.hh"

FirmwareService* FirmwareService::s_service = NULL;

// get the single instance of FirmwareService
FirmwareService* FirmwareService::singleton()
{
    if (NULL == FirmwareService::s_service) {
        FirmwareService::s_service = new FirmwareService();
    }
    return FirmwareService::s_service;
}

// private:
FirmwareService::FirmwareService ()
{
}

FirmwareService::~FirmwareService ()
{
}

void FirmwareService::checkFirmware (HWND hwnd)
{
    AsyncTaskCheckFirmware* task = new AsyncTaskCheckFirmware ();

    task->createTask (hwnd, TASKID_FIRMWARE_CHECK, 0);
}

void FirmwareService::downloadFirmware (HWND hwnd, const char* url)
{
    AsyncTaskDownloadFirmware* task = new AsyncTaskDownloadFirmware ();

    task->createTask (hwnd, TASKID_FIRMWARE_DOWNLOAD, 0);
}

void FirmwareService::upgradeFirmware (HWND hwnd, const char* path)
{
    AsyncTaskUpgradeFirmware* task = new AsyncTaskUpgradeFirmware ();

    task->createTask (hwnd, TASKID_FIRMWARE_UPGRADE, 0);
}

void* AsyncTaskCheckFirmware::execute_task (pthread_t thread_id, int task_id, DWORD add_data)
{
    memset (&m_info, 0, sizeof (FIRMWARE_INFO));
    m_info.status = FWS_ERR_SYSTEM;

    char* res = jbus_invoke ("cbplus.firmware", "check", "{}");
    if (res == NULL) {
        return &m_info;
    }

    /* convert to C structure here */
    cJSON* root = cJSON_Parse (res);
    jbus_free (res);

    if (root == NULL) {
        goto error;
    }
    else {
        cJSON* retCode = cJSON_GetObjectItem (root, "retCode");
        if (retCode == NULL || retCode->valueint != 0) {
            goto error;
        }

        cJSON* data = cJSON_GetObjectItem (root, "data");
        if (data == NULL) {
            goto error;
        }

        cJSON* json_update = cJSON_GetObjectItem (data, "update");
        if (json_update == NULL || json_update->type != cJSON_Number) {
            goto error;
        }

        if (json_update->valueint == 0) {
            m_info.status = FWS_ERR_UPTODATE;
            goto error;
        }

        cJSON* json_verCode = cJSON_GetObjectItem (data, "verCode");
        if (json_verCode == NULL || json_verCode->type != cJSON_Number) {
            goto error;
        }
        m_info.verCode = json_verCode->valueint;

        cJSON* json_size = cJSON_GetObjectItem (data, "size");
        if (json_size == NULL || json_size->type != cJSON_Number) {
            goto error;
        }
        m_info.size = json_size->valueint;

        cJSON* json_url = cJSON_GetObjectItem (data, "url");
        if (json_url == NULL || json_url->type != cJSON_String) {
            goto error;
        }

        m_info.url = strdup (json_url->valuestring);

        cJSON* json_verName = cJSON_GetObjectItem (data, "verName");
        if (json_verName == NULL || json_verName->type != cJSON_String) {
            goto error;
        }

        m_info.verName = strdup (json_verName->valuestring);

        cJSON* json_verDesc = cJSON_GetObjectItem (data, "verDesc");
        if (json_verDesc == NULL || json_verDesc->type != cJSON_String) {
            goto error;
        }

        m_info.verDesc = strdup (json_verDesc->valuestring);

        cJSON* json_md5 = cJSON_GetObjectItem (data, "md5");
        if (json_md5 == NULL || json_md5->type != cJSON_String) {
            goto error;
        }

        m_info.md5 = strdup (json_md5->valuestring);
    }

    m_info.status = FWS_OK;

error:
    cJSON_Delete (root);

    return (void*) &m_info;
}

void AsyncTaskCheckFirmware::cleanup_task (pthread_t thread_id, void* res)
{
    if (m_info.verName) free (m_info.verName);
    if (m_info.verDesc) free (m_info.verDesc);
    if (m_info.url) free (m_info.url);
    if (m_info.md5) free (m_info.md5);

    delete this;
}

void* AsyncTaskDownloadFirmware::execute_task (pthread_t thread_id, int task_id, DWORD add_data)
{
    memset (&m_info, 0, sizeof (FIRMWARE_INFO));
    m_info.status = FWS_ERR_SYSTEM;

    char* res = jbus_invoke ("cbplus.firmware", "download", "{}");
    if (res == NULL) {
        return &m_info;
    }

    /* convert to C structure here */
    cJSON* root = cJSON_Parse (res);
    jbus_free (res);

    if (root == NULL) {
        goto error;
    }
    else {
        cJSON* retCode = cJSON_GetObjectItem (root, "retCode");
        if (retCode == NULL || retCode->valueint != 0) {
            goto error;
        }
    }

    m_info.status = FWS_OK;

error:
    cJSON_Delete (root);

    return (void*) &m_info;
}

void AsyncTaskDownloadFirmware::cleanup_task (pthread_t thread_id, void* res)
{
    delete this;
}

void* AsyncTaskUpgradeFirmware::execute_task (pthread_t thread_id, int task_id, DWORD add_data)
{
    memset (&m_info, 0, sizeof (FIRMWARE_INFO));
    m_info.status = FWS_ERR_SYSTEM;

    char* res = jbus_invoke ("cbplus.firmware", "upgrade", "{}");
    if (res == NULL) {
        return &m_info;
    }

    /* convert to C structure here */
    cJSON* root = cJSON_Parse (res);
    jbus_free (res);

    if (root == NULL) {
        goto error;
    }
    else {
        cJSON* retCode = cJSON_GetObjectItem (root, "retCode");
        if (retCode == NULL || retCode->valueint != 0) {
            goto error;
        }
    }

    m_info.status = FWS_OK;

error:
    cJSON_Delete (root);

    return (void*) &m_info;
}

void AsyncTaskUpgradeFirmware::cleanup_task (pthread_t thread_id, void* res)
{
    delete this;
}

