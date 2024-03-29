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

/**  \ingroup testlibrary
*
*/

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <inttypes.h>

#include <sys/stat.h>

#include "opof.h"
#include "opof_test.h"
#include "opof_error.h"




/*
 * Declare functions
 */
void signal_handler(int sig);
void opof_list_tests();

void opof_run_tests(const char *address, int number, unsigned int pageSize, unsigned short port, const char *cert, char *filename, int test_id, bool verbose);
/*
 * Main routine
 */
int main(int argc, char ** argv){
    /*
     * Command Line Arguments
     */
    char cert[2048];

#ifdef FORK
    pid_t pid,sid;
#endif
    char test_config[FILENAME_MAX];
    int c;
    int status = SUCCESS;
    int test_number=1;
    struct sigaction newSigAction;
    unsigned short port = 3443;
    char address[64];
    int number = 1;
    unsigned int pageSize = BUFFER_MAX;
    char *str_part;
    char *default_address ="localhost";
    bool fullTestSuite = false;
    bool verbose = false;
    
    strncpy(address,default_address,strlen(default_address)+1);
    
    test_config[0]='\0';
    //
    static struct option longopts[] = {
        {"version", no_argument,0,'v'},
        {"address", no_argument, 0, 'a'},
        {"port", no_argument, 0 ,'p'},
        {"verbose",no_argument, 0,'v'},
        {"number", no_argument,0,'n'},
        {"buffersize", no_argument,0,'b'},
        {"config", no_argument,0,'c'},
        {"test", no_argument, 0, 't'},
        {"full", no_argument, 0,'f'},
        {"list", no_argument, 0, 'l'},
        {"help",no_argument,0,'h'},
    };
    /*
     * Loop over input
     */
    while (( c = getopt_long(argc,argv, "a:p:n:b:t:c:flvh",longopts,NULL))!=    -1){
        switch(c) {
            case 'v':
                verbose = true;
                break;    
             case 'p':
                port = strtoul(optarg, &str_part,10);
                break;  
            case 'a':
                strncpy(address, optarg,63);
                break;
            case 'n':
                number = strtoul(optarg, &str_part,10);
                break;
            case 'b':
                pageSize = (unsigned int)strtoul(optarg, &str_part,10);
                break;
            case 'f':
                fullTestSuite = true;
                break;
            case 'c':
                strncpy(test_config, optarg, FILENAME_MAX-1);
                break;
            case 't':
                test_number = strtoul(optarg, &str_part,10);
                //printf("Enabling config tests and disabling performance tests\n");
                break;
            case 'l':
                opof_list_tests();
                exit(1);
                break;
            case 'h':
                printf("\nCommand line arguments for OpenOffload tests version: %s \n", OPOF_VERSION);
                printf("\t-p, --port            gRPC Port \n");
                printf("\t-a, --address         Address of gRPC Server\n");
                printf("\t-n, --number          Number of sessions\n");
                printf("\t-b, --buffersize      Streaming buffer size\n");
                printf("\t-v, --verbose         Verbose output\n");
                printf("\t-c, --config          Configuration file\n");
                printf("\t-t, --test            Test to run\n");
                printf("\t-f, --full            Run full test suite\n");
                printf("\t-l, --list            List all tests\n");
                printf("\t-h, --help:           Command line help \n\n");
                exit(1);
            default:
                printf("Ignoring unrecognized command line option:%d\n ",c);
                break;
        }
    }
    /*
    * Open syslog
    */
    setlogmask(LOG_UPTO(LOG_INFO));
    openlog("OPOF", LOG_CONS | LOG_PERROR, LOG_USER);
    syslog(LOG_INFO,"INFO: Starting OPOF Test Client Version  %s", OPOF_VERSION);
    /*
     * Set up a signal handler 
     */
    newSigAction.sa_handler = signal_handler;
    sigemptyset(&newSigAction.sa_mask);
    newSigAction.sa_flags = 0;
   /* Signals to handle */
    sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
    sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
    sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */
    sigaction(SIGILL, &newSigAction, NULL);     /* catch illegal instruction signal */
    sigaction(SIGABRT, &newSigAction, NULL);     /* catch abort signal */
    sigaction(SIGSEGV, &newSigAction, NULL);     /* catch segv signal */

#ifdef FORK
    /*
    * Fork process
    */
    pid = fork();
    if (pid < 0){
        perror("Forking VNF");
        exit(EXIT_FAILURE);
    }
    /*
    * Exit parent
    */
    if (pid > 0){
        exit(EXIT_SUCCESS);
    }
    /*
    * Change file mode mask
    */
    umask(027);
    /* 
    * Create a new SID for the child process 
    */
    sid = setsid();
    if (sid < 0) {
         syslog(LOG_WARNING,"Setting sid");
         exit(EXIT_FAILURE);
    }
#endif /* FORK */
    /*
    * Create log and working directories
    */
    struct stat lst = {0};
    if (stat(LOG_DIR,&lst) == -1){
        if (mkdir(LOG_DIR,0755) == -1) {
            syslog(LOG_WARNING,"Error creating log dir");
            exit(EXIT_FAILURE);
        }
    }
   
#ifdef FORK
    /* 
    * Change the current working directory 
    */
    if (chdir(RUN_DIR) < 0) {
        fprintf(stderr,"Changing to RUN_DIR: %s", RUN_DIR);    
    }
#endif
#ifdef FORK
    /* 
    * Close out the standard file descriptors 
    */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#endif
    /*
     *  run application
     */
#ifdef INTERACTIVE
    print_config(&config_info);
#endif
    

#ifdef SSL
        status = get_key(CERT_FILE, cert);
#endif
        if (status != FAILURE){
            if (fullTestSuite){
                opof_run_tests(address, number, pageSize, port, cert,test_config, -1, verbose); 
            } else {
                opof_run_tests(address, number, pageSize, port, cert,test_config, test_number, verbose); 
            }   
        } else {
            printf("Error: could not read client credentials\n");
            exit (-1);
        } 
    return 1;
}

