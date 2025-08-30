#include "includes/libft_malloc.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>

void to_stdout(char *str)
{
	assert(str != NULL);

	(void)write(STDOUT_FILENO, str, strlen(str));
}

int main(void)
{
	to_stdout("before\n");
	void *ptr = malloc(100);
	void *ptr2 = malloc(100);
	void *ptr3 = malloc(100);
	return 0;
}
