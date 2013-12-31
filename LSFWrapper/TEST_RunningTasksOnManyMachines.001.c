// Running tasks on many machines

//MLB
#ifdef _MSC_VER
# pragma warning(disable:4115 4255 4820)
#endif // #ifdef _MSC_VER

#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <lsf/lsf.h>

//MLB
#ifdef _MSC_VER
# pragma warning(default:4115 4255 4820)
#endif // #ifdef _MSC_VER

int main (int argc, char **argv)
{
	char *command[3];
	int numHosts;
	int i;
	int tid;

	if (argc <= 1) {
		printf("Usage: %s host1 [host2 ...]\n",argv[0]);
		exit(-1);
	}

	numHosts = argc - 1;
/*
	command[0] = "rm";
	command[1] = "-f";
	command[2] = "/tmp/core";
	command[3] = NULL;
*/
	command[0] = "C:\\WINNT\\system32\\ping.exe";
	command[1] = "localhost";
	command[2] = NULL;

	if (ls_initrex(numHosts, 0) < 0) {
		ls_perror("ls_initrex");
		exit(-1);
	}

//	signal(SIGUSR1, SIG_IGN);

	/* Run command on the specified hosts */
	for (i=1; i<=numHosts; i++) {
		if ((tid = ls_rtask(argv[i], command, 0)) < 0) {
			fprintf(stderr, "lsrtask failed for host %s: %s\n", 
				argv[i], ls_sysmsg());
			exit(-1);
		}
		printf("Task %d started on %s\n", tid, argv[i]);
	}

	while (numHosts) {
		LS_WAIT_T status;

		tid = ls_rwait(&status, 0, NULL);
		if (tid < 0) {
			ls_perror("ls_rwait");
			exit(-1);
		}
		
		printf("task %d finished\n", tid);
		numHosts--;
	}

	exit(0);
}

