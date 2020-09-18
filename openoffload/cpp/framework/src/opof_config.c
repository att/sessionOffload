
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <libconfig.h>

#include <net/if.h>
#include <arpa/inet.h>

#include "opof.h"
#include "opof_error.h"

//
//void display_session_request(sessionRequest_t *request);

sessionRequest_t **read_config(char *filename,int *nsessions){
  	sessionRequest_t *request;
	sessionRequest_t **sessionRequests = NULL;
	config_t cfg;
  	config_setting_t *requests;
  	struct in6_addr addr6;
  	struct in_addr addr4;
  	const char *str;
	int i;
	int value;
	const char *srcaddr, *dstaddr, *nexthopaddr;
  	config_init(&cfg);

  	/* Read the file. If there is an error, report it and exit. */
  	if (! config_read_file(&cfg, filename))
  	{
    	fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    	config_destroy(&cfg);
    	return NULL;
  	}

	/* Get the store name. */
	if(config_lookup_string(&cfg, "name", &str))
		printf("Test File Name: %s\n\n", str);
	else
		fprintf(stderr, "No 'name' setting in configuration file.\n");
	

	requests = config_lookup(&cfg, "testdata.requests");
	if(requests != NULL){

	    *nsessions = config_setting_length(requests);
	    sessionRequests = (sessionRequest_t **)malloc(*nsessions * (sizeof(request)));

	 	for(i = 0; i < *nsessions; ++i){
	 		request = (sessionRequest_t *)malloc(sizeof(sessionRequest_t));
	      	config_setting_t *session = config_setting_get_elem(requests, i);
	      	config_setting_lookup_int(session, "sessionid", &value);
	      	request->sessId = value;
	      	 config_setting_lookup_int(session, "actiontype",&value);
		    request->actType = value;
		    config_setting_lookup_int(session, "inlif",&value);
		    request->inlif = value;
		    config_setting_lookup_int(session, "outlif",&value);
		    request->outlif = value;
		   	config_setting_lookup_int(session, "ipversion",&value);
		    request->ipver = value;
		    if (request->ipver == _IPV6){
 				config_setting_lookup_string(session, "sourceip",&srcaddr);
		    	inet_pton(AF_INET6, srcaddr, &addr6);
		    	request->srcIPV6 = addr6;
		    } else {
		    	 config_setting_lookup_string(session, "sourceip",&srcaddr);
		    	inet_pton(AF_INET, srcaddr, &addr4);
		    	request->srcIP = addr4;
		    }
		    config_setting_lookup_string(session, "nexthop",&nexthopaddr);
		    inet_pton(AF_INET,nexthopaddr, &value);
		    request->nextHop = value;
		    config_setting_lookup_int(session, "protocolid",&value);
		    request->proto = value;
		    config_setting_lookup_int(session, "sourceport",&value);
		    request->srcPort = value;
		     if (request->ipver == _IPV6){
 				config_setting_lookup_string(session, "destinationip",&dstaddr);
		    	inet_pton(AF_INET6, dstaddr, &addr6);
		    	request->dstIPV6 = addr6;
		    } else {
		    	 config_setting_lookup_string(session, "destinationip",&srcaddr);
		    	inet_pton(AF_INET, srcaddr, &addr4);
		    	request->dstIP = addr4;
		    }
 			config_setting_lookup_int(session, "destinationport",&value);
		    request->dstPort = value;		   
		    sessionRequests[i] = request;
	    }
	}
	config_destroy(&cfg);
	return sessionRequests;
}