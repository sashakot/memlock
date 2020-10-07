#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
	long size_gb;
	size_t size;
	char *next;
	void *ptr;

	if (argc <2) {
		fprintf(stderr, "Usage: %s <mem size in GB>\n", argv[0]);
		exit (1);
	}

	size_gb = strtol (argv[1], &next, 10);
	if (errno == EINVAL) {
		fprintf(stderr, "%s invalid\n", argv[0]);
		exit (1);
	} else if (errno == ERANGE) {
		fprintf(stderr, "%s is out of range\n", argv[0]);
		exit (1);
	} else if (*next != '\0') {
		fprintf(stderr, "%s has invalid char %c\n", argv[0], *next);
		exit (1);
	}


	size = size_gb * 1024 * 1024 * 1024;
	ptr = mmap(NULL, size,  PROT_NONE,
		                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	int alloc_size = 100 * 1024 * 1024;
	mprotect(ptr, alloc_size, PROT_READ | PROT_WRITE);
	memset(ptr, '\0', alloc_size);

	if (ptr == MAP_FAILED) {
		fprintf(stderr, "mmap call failed: %m\n");
		exit (2);
	}

	munmap(ptr, size);

	return 0;
}
