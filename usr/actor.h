/*
 * iSCSI usermode single-threaded scheduler
 *
 * Copyright (C) 2004 Dmitry Yusupov, Alex Aizman
 * maintained by open-iscsi@googlegroups.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * See the file COPYING included with this distribution for more details.
 */
#ifndef ACTOR_H
#define ACTOR_H

#include <stdio.h>
#include "types.h"
#include "list.h"

#define ACTOR_NAME_LEN 128

typedef enum actor_state_e {
    ACTOR_INVALID,
    ACTOR_WAITING,
    ACTOR_SCHEDULED,
    ACTOR_NOTSCHEDULED,
} actor_state_e;

typedef struct actor {
	char name[ACTOR_NAME_LEN];
	struct list_head list;
	actor_state_e state;
	void *data;
	void (*callback)(void * );
	time_t ttschedule;
} actor_t;

extern void __actor_init(actor_t *thread, void (*callback)(void *), void * data);
extern void actor_delete(actor_t *thread);
extern void actor_schedule_head(actor_t *thread);
extern void actor_schedule(actor_t *thread);
extern void __actor_timer(actor_t *thread, uint32_t delay_secs,
			void (*callback)(void *), void *data);
extern void actor_timer_mod(actor_t *thread, uint32_t new_delay_secs,
			    void *data);
extern void actor_poll(void);

#define actor_init(thread, callback, data) \
do { \
	snprintf((thread)->name, ACTOR_NAME_LEN, #callback); \
	__actor_init(thread, callback, data); \
} while (0)

#define actor_timer(thread, timeout_secs, callback, data) \
do { \
	snprintf((thread)->name, ACTOR_NAME_LEN, #callback); \
	__actor_timer(thread, timeout_secs, callback, data); \
} while (0)

#endif /* ACTOR_H */
