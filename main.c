#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <zconf.h>

void *findMinMmapedAddress() {
	for (int i = 0; i < 256; i++) { // 256 is arbitrary
		void *expected_addr = (void *)(i*4096);
		int *real_addr = mmap(expected_addr, 4096, PROT_READ|PROT_WRITE,
				MAP_ANONYMOUS|MAP_SHARED|MAP_FIXED, 0, 0);
		if (real_addr = expected_addr) {
			return real_addr;
		}
	}
}

void *getSpecifiedMinAddress() {
	FILE* fp = fopen("/proc/sys/vm/mmap_min_addr", "r");
	int minAddr = -1;
	fscanf(fp, "%d", &minAddr);
	return (void *)minAddr;
}

int main() {
	int a = getuid();
	if (a == 0) {
		printf("running as root - expect nullptr write to succeed\n");
	} else {
		printf("not running as root - expect nullptr write to fail\n");
	}

	int *p = mmap((void *)NULL, 4096, PROT_READ|PROT_WRITE,
			MAP_ANONYMOUS|MAP_SHARED|MAP_FIXED, 0, 0);


	if (errno != 0) {
		printf("error: %s - continuing anyways\n", strerror(errno));
		// this for learning purposes so don't stop execution after error
	}

	if (p == NULL) {
		printf("\nresult: *p = 0x%x\n", *p);
		printf("p == nullptr\n");
		printf("attempting to write 0xDEADBEEF\n");
		*p = 0xDEADBEEF;
		printf("result: *p = 0x%x\n", *p);
	} else {
		printf("\np = 0x%x\n", p);

		printf("\nattempting to find lowest address that can be allocated\n");
		printf("lowest mmapable address (bruteforce mmap) is 0x%x\n", findMinMmapedAddress());
		printf("lowest mmapable address (read /proc/sys/vm/mmap_min_addr) is 0x%x\n", getSpecifiedMinAddress());
	}

	printf("\n\"accidentally\" reading from null (expect segfault if p != nullptr)\n");
	fflush(stdout); // if null mmap failed, program likely segfaults before 
	int *q = NULL;
	printf("result: q = 0x%x\n", *q);
}

