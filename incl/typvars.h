/*
 * typvars.h
 *
 * Includes all global variables, structs, etc.
 *
 *  Created on: Sep 25, 2019
 *      Author: chifac08
 */

#ifndef INCL_TYPVARS_H_
#define INCL_TYPVARS_H_

#include <pthread.h>
#include "LogTypes.h"


#define LOG_FILE_DELIMITER "="
#define TEMP_FILE_DIR "/dev/shm/zombieHunter"
#define STOP_FLAG_FILE "/tmp/zombieHunter.flg"

//defualt values
#define DEFAULT_CONFIG_FILE "/etc/opt/zombieHunter/zombieHunter.cfg"
#define DEFAULT_LOG_FILE "/var/log/zombieHunter/log.log"
#define DEFAULT_LISTEN_PORT 5557
#define DEFAULT_LOG_ROTATE 10
#define DEFAULT_CHECK_INTERVALL 30
#define DEFAULT_PROCESS_DIR "/proc"
#define DEFAULT_QUEUE_SIZE 500

typedef enum {
	false=0,
	true=1
} bool;

typedef struct
{
	LOG_CONFIG logConfig;
	unsigned int listenPort;
	char host[16];
	unsigned int port;
	unsigned int sendTimeout;
	unsigned int checkIntervall;
} CONFIG;

typedef struct
{
	char name[1024];
	char umask[5];
	char state[64];
	int tgid;
	int ngid;
	int pid;
	int ppid;
	int tracerPid;
	char uid[512];
	char gid[512];
	int fdsize;
	char groups[512];
	int nstgid;
	int nspid;
	int nspgid;
	int nssid;
	char vmpeak[32];
	char vmsize[32];
	char vmlck[32];
	char vmpint[32];
	char vmhwm[32];
	char vmrss[32];
	char rssanon[32];
	char rssfile[32];
	char rssshmem[32];
	char vmdata[32];
	char vmstk[32];
	char vmexe[32];
	char vmlib[32];
	char vmpte[32];
	char vmswap[32];
	char hugetlbpages[32];
	int coredumping;
	int thp_enabled;
	int threads;
	char sigq[16];
	char sigpnd[17];
	char shdpnd[17];
	char sigblk[17];
	char sigign[17];
	char sigcgt[17];
	char capinh[17];
	char capprm[17];
	char capeff[17];
	char capbnd[17];
	char capamb[17];
	int nonewprivs;
	int ceccomp;
	char speculation_store_bypass[32];
	char cpus_allowed[5];
	char cpus_allowed_list[5];
	char mem_allowed[2048];
	int mems_allowed_list;
	int voluntary_ctxt_switches;
	int nonvoluntary_ctxt_switches;
} PROCESS_STATUS;

typedef struct zombie_queue
{
	unsigned int capacity;
	unsigned int size;
	int front;
	int rear;
	char **elements;
} Queue;

typedef struct file_watcher_arg
{
	int iWatcher;
	Queue* zombie_queue;
	pthread_mutex_t mutex;
} FILE_WATCHER_ARG;

#endif /* INCL_TYPVARS_H_ */
