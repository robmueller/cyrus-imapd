/* notify_mailto.c -- email notification method
 *
 * Copyright (c) 1994-2008 Carnegie Mellon University.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any legal
 *    details, please contact
 *      Carnegie Mellon University
 *      Center for Technology Transfer and Enterprise Creation
 *      4615 Forbes Avenue
 *      Suite 302
 *      Pittsburgh, PA  15213
 *      (412) 268-7393, fax: (412) 268-7395
 *      innovation@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <config.h>

#include "notify_mailto.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "imap/global.h"
#include "imap/smtpclient.h"
#include "libconfig.h"
#include "sieve/sieve_interface.h"
#include "times.h"
#include "util.h"

static int contains_8bit(const char *msg);

static int global_outgoing_count = 0;

char* notify_mailto(const char *class,
                    const char *priority __attribute__((unused)),
                    const char *user __attribute__((unused)),
                    const char *mailbox __attribute__((unused)),
                    int nopt, char **options,
                    const char *message,
                    const char *fname __attribute__((unused)))
{
    struct buf msgbuf = BUF_INITIALIZER;
    smtp_envelope_t sm_env = SMTP_ENVELOPE_INITIALIZER;
    smtpclient_t *sm = NULL;
    char outmsgid[256];
    time_t t;
    char datestr[RFC5322_DATETIME_MAX+1];
    int r;

    /* XXX check/parse options (mailto URI) */
    if (nopt < 1)
        return strdup("NO mailto URI not specified");

    /* Initialize SMTP envelope */
    smtp_envelope_set_from(&sm_env, "");
    smtp_envelope_add_rcpt(&sm_env, options[0]);

    /* Build message */
    t = time(NULL);
    snprintf(outmsgid, sizeof(outmsgid), "<cmu-sieve-%d-" TIME_T_FMT "-%d@%s>",
             (int) getpid(), t, global_outgoing_count++, config_servername);

    buf_printf(&msgbuf, "Message-ID: %s\r\n", outmsgid);

    time_to_rfc5322(t, datestr, sizeof(datestr));
    buf_printf(&msgbuf, "Date: %s\r\n", datestr);

    buf_printf(&msgbuf, "X-Sieve: %s\r\n", SIEVE_VERSION);
    buf_printf(&msgbuf, "From: Mail Sieve Subsystem <%s>\r\n",
               config_getstring(IMAPOPT_POSTMASTER));
    buf_printf(&msgbuf, "To: <%s>\r\n", options[0]);
    buf_printf(&msgbuf, "Subject: [%s] New mail notification\r\n", class);
    if (contains_8bit(message)) {
        buf_appendcstr(&msgbuf, "MIME-Version: 1.0\r\n");
        buf_appendcstr(&msgbuf, "Content-Type: text/plain; charset=UTF-8\r\n");
        buf_appendcstr(&msgbuf, "Content-Transfer-Encoding: 8BIT\r\n");
    }
    buf_appendcstr(&msgbuf, "\r\n");
    buf_printf(&msgbuf, "%s\r\n", message);

    /* Send message using smtpclient API */
    r = smtpclient_open(&sm);
    if (!r) {
        r = smtpclient_send(sm, &sm_env, &msgbuf);
    }
    smtpclient_close(&sm);

    /* Clean up */
    smtp_envelope_fini(&sm_env);
    buf_free(&msgbuf);

    /* XXX add outmsgid to duplicate delivery database to prevent loop */

    if (r) {
        return strdup("NO mailto notification failed");
    }

    return strdup("OK mailto notification successful");
}

static int contains_8bit(const char * msg)
{
    int result = 0;

    if (msg) {
        const unsigned char *s = (const unsigned char *)msg;

        while (*s) {
            if (0 != (*s & 0x80)) {
                result = 1;
                break ;
            }
            s++;
        }
    }
    return result;
}
