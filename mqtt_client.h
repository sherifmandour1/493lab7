#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_
#include "log.h"
#include <assert.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#define UNUSED(x) (void)(x)

#define QOS 1
#define TIMEOUT 10000L
#define DEFAULT_PORT "1883"
#define DEFAULT_HOST "localhost"
#define STRINGS_MATCH 0
#define SERVER_LISTENNING_ERROR -15
#define LISTENED 0
#define BINEDED 0
#define SERVER_CREATION_ERROR -30
#define TIME_OUT_COUNTER_MAX 500
#define SET_BAD_REQUEST_INVALID "invalid"
#define LINE_READ_ERROR -1
#define GET_LINE_ERROR -2
#define ZERO_VALUE 0
#define SPACE_CHAR ' '
#define ARRAY_INIT_NUM 1000
#define SIZEOF_LONGEST_ACTION 10
#define LAB1_BUFFER_SIZE 1024
#define MAX_ACCEPTABLE_BUFFER_CAPACITY_PERCENTAGE 0.7
#define TWO_VALUE 2
#define ONE_VALUE 1
#define NULL_TERMINATOR '\0'
#define READ_FROM_STDIN '-'
#define LENGTH_OF_LONGEST_ACTION 10
#define HANDLE_RESPONSE_ERROR -13
#define UPPERCASE_CODE 0x1
#define LOWERCASE_CODE 0x2
#define TITLE_CASE_CODE 0x4
#define REVERSE_CODE 0x8
#define SHUFFLE_CODE 0x10
#define DYNAMIC_BUFFER_ERROR -33
#define RECEIVED_CHAR_ERROR -34
#define RESPONSE_ERROR -35
#define SENDING_ERROR -36
#define CONNECT_ERROR -37
#define SENT_COMPLETE 0
#define ZERO_RESET_INIT_VALUE 0



typedef struct Config {
    char *port;
    char *relative_path;
    char* host;
    char* netid;
    char* action;
    char* message;
} Config;



Config mqtt_client_parse_arguments(int argc, char *argv[]);

#endif