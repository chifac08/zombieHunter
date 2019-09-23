//defines

//vars
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

//Methods
int* getProcessList(char* cpDir);
void checkProcessState(int* processList);
