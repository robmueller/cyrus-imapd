/* jmap_task.c -- Routines for handling JMAP Tasks requests
 * SPDX-License-Identifier: BSD-3-Clause-CMU
 * See COPYING file at the root of the distribution for more details.
 */

#include <config.h>

#include "caldav_db.h"
#include "jmap_api.h"
#include "jmap_calendar.h"

/* generated headers are not necessarily in current directory */
#include "imap/imap_err.h"

static int jmap_task_get(struct jmap_req *req);
static int jmap_task_set(struct jmap_req *req);
static int jmap_task_changes(struct jmap_req *req);
static int jmap_task_query(struct jmap_req *req);
static int jmap_task_copy(struct jmap_req *req);
static int jmap_tasklist_get(struct jmap_req *req);
static int jmap_tasklist_set(struct jmap_req *req);
static int jmap_tasklist_changes(struct jmap_req *req);

// clang-format off
static jmap_method_t jmap_task_methods_standard[] = {
    {
        "Task/get",
        JMAP_URN_TASKS,
        &jmap_task_get,
        JMAP_NEED_CSTATE
    },
    {
        "Task/set",
        JMAP_URN_TASKS,
        &jmap_task_set,
        JMAP_NEED_CSTATE | JMAP_READ_WRITE
    },
    {
        "Task/changes",
        JMAP_URN_TASKS,
        &jmap_task_changes,
        JMAP_NEED_CSTATE
    },
    {
        "Task/query",
        JMAP_URN_TASKS,
        &jmap_task_query,
        JMAP_NEED_CSTATE
    },
    {
        "Task/copy",
        JMAP_URN_TASKS,
        &jmap_task_copy,
        JMAP_NEED_CSTATE | JMAP_READ_WRITE
    },
    {
        "TaskList/get",
        JMAP_URN_TASKS,
        &jmap_tasklist_get,
        JMAP_NEED_CSTATE
    },
    {
        "TaskList/set",
        JMAP_URN_TASKS,
        &jmap_tasklist_set,
        JMAP_NEED_CSTATE | JMAP_READ_WRITE
    },
    {
        "TaskList/changes",
        JMAP_URN_TASKS,
        &jmap_tasklist_changes,
        JMAP_NEED_CSTATE
    },
    { NULL, NULL, NULL, 0 }
};
// clang-format on

HIDDEN void jmap_task_init(jmap_settings_t *settings)
{
    jmap_add_methods(jmap_task_methods_standard, settings);

    json_object_set_new(settings->server_capabilities,
            JMAP_URN_TASKS, json_object());
}

HIDDEN void jmap_task_capabilities(json_t *account_capabilities,
                                   struct auth_state *authstate __attribute__((unused)),
                                   const char *authuserid __attribute__((unused)),
                                   const char *accountid __attribute__((unused)))
{
    json_object_set_new(account_capabilities, JMAP_URN_TASKS, json_object());
}

/*
 * Task/get
 */
static int jmap_task_get(struct jmap_req *req)
{
    return jmap_calendarevent_get_impl(req, CAL_COMP_VTODO, task_props);
}

/*
 * Task/set
 */
static int jmap_task_set(struct jmap_req *req)
{
    return jmap_calendarevent_set_impl(req, CAL_COMP_VTODO, task_props);
}

/*
 * Task/changes
 */
static int jmap_task_changes(struct jmap_req *req)
{
    return jmap_calendarevent_changes_impl(req, CAL_COMP_VTODO);
}

/*
 * Task/query
 */
static int jmap_task_query(struct jmap_req *req)
{
    return jmap_calendarevent_query_impl(req, CAL_COMP_VTODO);
}

/*
 * Task/copy
 */
static int jmap_task_copy(struct jmap_req *req)
{
    return jmap_calendarevent_copy_impl(req, CAL_COMP_VTODO);
}

/*
 * TaskList/get
 */
static int jmap_tasklist_get(struct jmap_req *req)
{
    return jmap_calendar_get_impl(req, CAL_COMP_VTODO);
}

/*
 * TaskList/set
 */
static int jmap_tasklist_set(struct jmap_req *req)
{
    return jmap_calendar_set_impl(req, CAL_COMP_VTODO);
}

/*
 * TaskList/changes
 */
static int jmap_tasklist_changes(struct jmap_req *req)
{
    return jmap_calendar_changes_impl(req, CAL_COMP_VTODO);
}
