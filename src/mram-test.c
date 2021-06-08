#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
 
#define EIM_CS0 0x08000000
#define MRAM_SIZE (1 << 17)

int main(int argc, char **argv)
{
	uint8_t *base;
	int mem;
	int ret = 0;
	uint8_t wbuf[MRAM_SIZE];
	uint8_t rbuf;

	int i;

	mem = open("/dev/mem", O_RDWR|O_SYNC);
	if(!mem) {
		perror("Can't open memory");
		return 1;
	}

	base = mmap(0, 
		    MRAM_SIZE, 
		    PROT_READ|PROT_WRITE, 
		    MAP_SHARED, 
		    mem, 
		    EIM_CS0);
	if(!base) {
		perror("Can't open memory");
		return 1;
	}

	srand(0);
	for (i = 0; i < MRAM_SIZE; i++) {
		wbuf[i] = (uint8_t)rand();
    		base[i] = wbuf[i];
	}
 
    	for (i = 0; i < MRAM_SIZE; i++) {
    		rbuf = base[i];
    		if(wbuf[i] != rbuf){
    			fprintf(stderr, "At offset %d, expected 0x%X, got 0x%X\n", i, wbuf[i], rbuf);
    			ret = 1;
    		}
    	}
 
	return ret;
}