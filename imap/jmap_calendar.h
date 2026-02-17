/* jmap_calendar.h -- Routines for handling JMAP calendars */
/* SPDX-License-Identifier: BSD-3-Clause-CMU */
/* See COPYING file at the root of the distribution for more details. */

#ifndef JMAP_CALENDAR_H
#define JMAP_CALENDAR_H

#include <config.h>

#include "jmap_api.h"
#include "json_support.h"
#include "ptrarray.h"
#include "util.h"

extern json_t *jmap_calendar_events_from_msg(jmap_req_t *req,
                                             const char *mboxid, uint32_t uid,
                                             hash_table *icsbody_by_partid,
                                             unsigned allow_max_uids,
                                             const struct buf *mime);

/* Task/TaskList reuse: _impl functions parameterized by comp_type */
extern int jmap_calendarevent_get_impl(struct jmap_req *req,
                                       unsigned int comp_type,
                                       const jmap_property_t *props_spec);
extern int jmap_calendarevent_set_impl(struct jmap_req *req,
                                       unsigned int comp_type,
                                       const jmap_property_t *props_spec);
extern int jmap_calendarevent_changes_impl(struct jmap_req *req,
                                           unsigned int comp_type);
extern int jmap_calendarevent_query_impl(struct jmap_req *req,
                                         unsigned int comp_type);
extern int jmap_calendarevent_copy_impl(struct jmap_req *req,
                                        unsigned int comp_type);
extern int jmap_calendar_get_impl(struct jmap_req *req,
                                  unsigned int comp_type);
extern int jmap_calendar_set_impl(struct jmap_req *req,
                                  unsigned int comp_type);
extern int jmap_calendar_changes_impl(struct jmap_req *req,
                                      unsigned int comp_type);

/* Task property spec for Task/get and Task/set */
extern const jmap_property_t task_props[];

#endif /* JMAP_CALENDAR_H */
