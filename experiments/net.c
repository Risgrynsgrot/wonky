#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint64_t scratch = 1;
	uint32_t value = 100;
	scratch |= value << 10;
	printf("%#010lx\n", scratch);
}
