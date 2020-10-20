/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPOFERROR_H
#define OPOFERROR_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#define OPOF_LOG_INFO 0
#define OPOF_LOG_WARN 1
#define OPOF_LOG_ERR 2

#define MAXLINE 4096

void err_info(FILE *fp, const char *, ...);
void err_warn(FILE *fp, const char *, ...);

void err_error(FILE *fp, const char *, ...);
void err_fatal(FILE *fp, const char *, ...);

#endif  /* OPOFERROR_H */
