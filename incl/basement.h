#include <stdio.h>
#include <sys/stat.h>
#include "typvars.h"

#define DEFAULT_LOG_CACHE 5
#define PROCESS_DIR "/proc"
#define PROCESS_ARRAY_SIZE 1000
#define PROCESS_STATE_LINE 3

//methods
int loadFileInMemory(char* cpFileName, char** szResult);
int createDir(char* cpDir, mode_t mode);
int copyFile(char* cpSourcePath, char* cpDestPath);
bool checkStopFlag(const char* cpFileName);
int removeFile(const char* cpFileName);
void test(void* arg);
