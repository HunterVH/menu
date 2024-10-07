#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "menu.h"

int rootPermissions(){	
	if(setregid(rgid,egid)){
		printf("ERROR: Unable to properly set GID\n");
		return 1;
	}
	if(setreuid(rgid,egid)){
		printf("ERROR: Unable to properly set UID\n");
		return 1;
	}
}

int defaultPermissions(){	
	if(setregid(egid,rgid)){
		printf("ERROR: Unable to properly set GID\n");
		return 1;
	}
	if(setreuid(euid,ruid)){
		printf("ERROR: Unable to properly set UID\n");
		return 1;
	}
}
