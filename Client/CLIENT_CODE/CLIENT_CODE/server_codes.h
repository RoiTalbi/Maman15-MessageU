#pragma once



/* Server Request Codes */
#define  REQUEST_CODE_REGISTER   			   (1000)
#define  REQUEST_CODE_GET_CLIENTS_LIST   	   (1001)
#define  REQUEST_CODE_GET_CLIENT_PUBLIC_KEY    (1002)
#define  REQUEST_CODE_SEND_MESSAGE  		   (1003)
#define  REQUEST_CODE_GET_PENDING_MESSAGES     (1004)


/* Server Response Codes */
#define  RESPONSE_CODE_REGISTER   		 	  (2000)
#define  RESPONSE_CODE_GET_CLIENTS_LIST   	  (2001)
#define  RESPONSE_CODE_GET_CLIENT_PUBLIC_KEY  (2002)
#define  RESPONSE_CODE_SEND_MESSAGE   		  (2003)
#define  RESPONSE_CODE_GET_PENDING_MESSAGES   (2004)



/* Server Error Codes */
#define  SERVER_ERROR_CODE   (9000)