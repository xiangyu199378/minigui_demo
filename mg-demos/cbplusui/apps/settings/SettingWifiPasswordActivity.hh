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
 * @file SettingWifiPasswordActivity.hh
 * @Synopsis
 * @author Vincent Wei
 * @version 1.0
 * @date 23/05/2018
 */

#ifndef _SETTING_WIFI_PASSWROD_ACTIVITY_HH_
#define _SETTING_WIFI_PASSWROD_ACTIVITY_HH_

typedef struct _WIFI_PASSWORD_INFO {
    char* ssid;
    char* password;
} WIFI_PASSWORD_INFO;

class SettingWifiPasswordActivity : public NCSActivity {

public:
    SettingWifiPasswordActivity ();

private:
    ~SettingWifiPasswordActivity() {};
};

#endif /* _SETTING_WIFI_PASSWROD_ACTIVITY_HH_ */
