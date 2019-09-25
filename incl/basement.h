#include <stdio.h>
#include "typvars.h"

#define DEFAULT_CONFIG_FILE "/opt/etc/zombieHunter/hunter.cfg"
#define DEFAULT_LOG_FILE "/var/log/zombieHunter/zombieHunter.log"
#define DEFAULT_LOG_CACHE 5
#define PROCESS_DIR "/proc"
#define PROCESS_ARRAY_SIZE 1000
#define PROCESS_STATE_LINE 3

//methods
ZOMBIE_NODE* prepend(ZOMBIE_NODE* head, PROCESS_STATUS proc_stat);
int count(ZOMBIE_NODE* head);
ZOMBIE_NODE* removeBack(ZOMBIE_NODE* head);
int loadFileInMemory(char* cpFileName, char** szResult);
