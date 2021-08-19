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
** Copyright (C) 2010 ~ 2019 FMSoft (http://www.fmsoft.cn).
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
 * @file ActivityStack.hh
 * @Synopsis
 * @author DongKai
 * @version 1.0
 *  Company: Beijing FMSoft Technology Co., Ltd.
 */

// refactored @ 2010-11-22 by min

#ifndef  _ACTIVITYSTACK_H
#define  _ACTIVITYSTACK_H

/**
 * @synopsis  A stack for activities manager.
 */
class ActivityStack
{
public:
    typedef std::pair<std::string, Activity*> ActivityEntry;
    typedef std::vector<ActivityEntry> ActivityCollection;

    // get the single instance of ActivityStack
    static ActivityStack* singleton();

    // get the top activity in stack
    Activity* top() const;

    // get the specific activity in stack
    Activity* getActivity (const char* appName);

    // return activity name of current app
    const char* currentAppName() const;

    // Create a new activity and bring it to the top of the stack,
    // using appName to specify which Activity will be created,
    // using intentPtr to specify your intend (optionally).
    bool push(const char* appName, Intent* intentPtr = NULL);

    // Remove the top activity from the stack, and send MSG_CLOSE message to
    // the window of the activity, instead of destroying it directly.
    bool pop(DWORD res = 0);

    // Back to previous view
    bool back();

    // Return to home
    bool home();

    // get depth of the stack
    int depth() const { return m_activities.size(); }

    // This method will find the first Activity named 'appName' in the stack from top to bottom,
    // if the Activity is existed, then pop to that Activity by calling popTo method;
    // if the Activity is not existed, then create a new Activity named appName, and push it into the stack.
    void switchTo(const char* appName, Intent* intentPtr = NULL, DWORD res = 0);

private:

    // constructor & desctructor, for internal use only
    ActivityStack();
    virtual ~ActivityStack();

    Activity* innerPush(const char* appName, Intent* intentPtr);
    bool innerPop(DWORD res);

    // Do the same thing as pop(), but supply a parameter "which" to specify
    // which activity you want to show up, and all activities above it will be poped.
    // Notice that, if you call this function with parameter NULL, the stack will be cleared.
    // return how much activities has been poped up
    int popTo(Activity* which, Intent* intentPtr = NULL, DWORD res = 0);

    // get the activity under the top
    Activity* _underTop() const;

    void _doSwitchEffect(Activity* prev, Activity* next, BOOL switchTo=TRUE);

    // search the first Activity named 'appName' in the stack in the order from top to bottom
    Activity* searchActivityByName(std::string appName);

    // store all activities
    ActivityCollection m_activities;

    int m_status;
    enum STATUS {
        READY = 0,
        PUSH,
        POP,
    };

    // store the single instance of ActivityStack
    static ActivityStack* s_single;
};

#define ACTIVITYSTACK   ActivityStack::singleton()

#endif   /* ----- #ifndef ACTIVITYSTACK_INC  ----- */

