#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"

#define FONTSIZE 10
int PrintMenuOS();

int Quit(int argc, char *argv[]);

int Time(int argc, char *argv[]);

int TimeAsm(int argc, char *argv[]);

int Fork(int argc, char *argv[]);

int Exec(int argc, char *argv[]);

#include<stdio.h> 
#include<arpa/inet.h> /* internet socket */
#include<string.h>
//#define NDEBUG
#include<assert.h>

#define PORT                5001
#define IP_ADDR             "127.0.0.1"
#define MAX_BUF_LEN         1024
#define MAX_CONNECT_QUEUE   1024
int Replyhi();

int StartReplyhi(int argc, char *argv[]);

int Hello(int argc, char *argv[]);


