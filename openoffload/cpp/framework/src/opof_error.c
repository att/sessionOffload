/*
 *  Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
 *
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


/**
* \ingroup testlibrary
*
* \brief gRPC Test Library for C/C++
*
*/

#include "opof_error.h"



void opof_shutdown(){
  syslog(LOG_INFO, "Shutting down OPOF");
}
/*
*  Signal Handler
*/
void signal_handler(int signo)
{
   switch(signo)
        {
            case SIGHUP:
                syslog(LOG_WARNING, "Received SIGHUP signal.");
                break;
            case SIGINT:
             	syslog(LOG_INFO, "Daemon exiting due to control C");
                opof_shutdown();
                exit(1);
                break;
            case SIGTERM:
            	syslog(LOG_INFO, "Daemon exiting due to kill command");
                opof_shutdown();
                exit(2);
                break;
            case SIGSEGV:
                syslog(LOG_INFO, "Daemon exiting due to segmentation violation");
                opof_shutdown();
                exit(3);
                break;
            default:
                syslog(LOG_WARNING, "Unhandled signal %s", strsignal(signo));
                exit(4);
                break;
        }
}

static void err_handler(FILE *fp,int level, const char *fmt, va_list ap){
	unsigned long errno_save, n;
	char buf[MAXLINE + 1];
	errno_save = errno;
	n = strlen(buf);

	switch 	(level) {
		case OPOF_LOG_INFO:
			strcpy(buf,"\nINFORMATIONAL: ");
			n = strlen(buf);
			vsnprintf(buf+n, MAXLINE - n,fmt,ap);
			break;
		case OPOF_LOG_WARN:
			strcpy(buf,"\nWARNING: ");
			n = strlen(buf);
			vsnprintf(buf+n, MAXLINE - n,fmt,ap);
			break;
		case OPOF_LOG_ERR:
			strcpy(buf,"\nERROR: ");
			n = strlen(buf);
			vsnprintf(buf+n, MAXLINE - n,fmt,ap);
			n = strlen(buf);
			snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
			break;
		default:
			break;
	}
	fputs(buf,fp);
	fflush(fp);
	return;
}
void err_fatal(FILE *fp, const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_handler(fp,OPOF_LOG_ERR, fmt,ap);
	va_end(ap);
	fclose(fp);
	exit(EXIT_FAILURE);
}

void err_error(FILE *fp, const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_handler(fp,OPOF_LOG_ERR, fmt,ap);
	va_end(ap);
	fclose(fp);
	exit(EXIT_FAILURE);
}

void err_warn(FILE *fp, const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_handler(fp,OPOF_LOG_WARN, fmt,ap);
	va_end(ap);
	return;
}
void err_info(FILE *fp, const char *fmt, ...){
	va_list ap;
	va_start(ap,fmt);
	err_handler(fp,OPOF_LOG_INFO, fmt,ap);
	va_end(ap);
	return;
}

