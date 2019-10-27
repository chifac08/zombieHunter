#include <stdio.h>
#include <sys/stat.h>
#include "typvars.h"

#define DEFAULT_LOG_CACHE 5
#define PROCESS_DIR "/proc"
#define PROCESS_ARRAY_SIZE 1000
#define PROCESS_STATE_LINE 3

//methods
ZOMBIE_NODE* create(char* cpFile, ZOMBIE_NODE* next);
ZOMBIE_NODE* prepend(ZOMBIE_NODE* head, char* cpFile);
int count(ZOMBIE_NODE* head);
ZOMBIE_NODE* removeBack(ZOMBIE_NODE* head);
int loadFileInMemory(char* cpFileName, char** szResult);
int createDir(char* cpDir, mode_t mode);
int copyFile(char* cpSourcePath, char* cpDestPath);
bool checkStopFlag(const char* cpFileName);
int removeFile(const char* cpFileName);
//TODO: remove test method
void test(void* arg);
