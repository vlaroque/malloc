#include "includes/libft_malloc.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define COLOR_BLACK        "\x1b[30m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_YELLOW_BOLD  "\x1b[1;33m"
#define COLOR_YELLOW_FAINT "\x1b[2;33m"
#define COLOR_BLUE         "\x1b[34m"
#define COLOR_BLUE_BOLD    "\x1b[1;34m"
#define COLOR_MAGENTA      "\x1b[35m"
#define COLOR_CYAN         "\x1b[36m"
#define COLOR_CYAN_BOLD    "\x1b[1;36m"
#define COLOR_WHITE        "\x1b[37m"
#define COLOR_RESET        "\x1b[0m"
#define COLOR_BOLD         "\x1b[1m"

void to_stdout(char *str)
{
	assert(str != NULL);

	ssize_t ignored = write(STDOUT_FILENO, str, strlen(str));
	(void)ignored;
}


void tiny_putnbr(int n)
{
	static char buf[32]; // big enough for int
	int i = 30;          // index to fill backwards
	int neg = 0;

	if (n == 0)
	{
		ssize_t ignored = write(STDOUT_FILENO, "0", 1);
		(void) ignored;
		return;
	}

	if (n < 0) {
		neg = 1;
		// careful with INT_MIN: use long
		long ln = -(long)n;
		while (ln > 0 && i >= 0) {
			buf[i--] = '0' + (ln % 10);
			ln /= 10;
		}
	}
	else
	{
		while (n > 0 && i >= 0) {
			buf[i--] = '0' + (n % 10);
			n /= 10;
		}
	}

	if (neg)
		buf[i--] = '-';

	ssize_t ignored = write(STDOUT_FILENO, buf + i + 1, 30 - i);
	(void)ignored;

}

void tiny_puthex(unsigned long n)
{
	static char buf[32]; // plenty for 64-bit hex (max 16 digits + "0x")
	const char *digits = "0123456789ABCDEF";
	int i = 30;

	if (n == 0) {
		ssize_t ignored = write(STDOUT_FILENO, "0x0", 3);
		(void)ignored;

		return;
	}

	while (n > 0 && i >= 0) {
		buf[i--] = digits[n & 0xF]; // n % 16
		n >>= 4;                    // n /= 16
	}

	buf[i--] = 'x';
	buf[i--] = '0';

	ssize_t ignored = write(STDOUT_FILENO, buf + i + 1, 30 - i);
	(void)ignored;
}

void tiny_putptr(void* n)
{
	tiny_puthex((unsigned long)n);
}

#define DEBUG 0

void *test_malloc_and_write(size_t size, char c)
{
	void *ptr = malloc(size);
	char *buff = ptr;

	size_t i = 0;
	while( i < size )
	{
		buff[i] = c;
		i++;
	}

	return ptr;
}

void test_verify_and_free(void *ptr, size_t size, char c)
{
	size_t i = 0;
	char *buff = ptr;

	while(i < size)
	{
		assert(buff[i] == c);
		i++;
	}
	free(ptr);
}

void test_remove_ordored()
{
	to_stdout(COLOR_RED "TEST REMOVE ORDER\n" COLOR_RESET);

	void *ptr[5];
	int i = 0;
	while (i < 1000)
	{
		ptr[0] = test_malloc_and_write(10, 'a');
		ptr[1] = test_malloc_and_write(20, 'b');
		test_verify_and_free(ptr[0], 10, 'a');
		test_verify_and_free(ptr[1], 20, 'b');
		show_alloc_mem();
		i++;
	}
}

void test_remove_unordored()
{
	to_stdout(COLOR_RED "TEST REMOVE UN ORDER\n" COLOR_RESET);

	void *ptr[5];
	int i = 0;
	while (i < 1000)
	{
		ptr[0] = test_malloc_and_write(10, 'a');
		ptr[1] = test_malloc_and_write(20, 'b');
		test_verify_and_free(ptr[0], 10, 'a');
		test_verify_and_free(ptr[1], 20, 'b');
		show_alloc_mem();
		i++;
	}
}


void first_test()
{
	to_stdout(COLOR_RED "TEST 1\n" COLOR_RESET);

	void *ptr[5];

	ptr[0] = test_malloc_and_write(10, 'a');
	ptr[1] = test_malloc_and_write(20, 'b');
	ptr[2] = test_malloc_and_write(30, 'c');
	ptr[3] = test_malloc_and_write(40, 'd');
	ptr[4] = test_malloc_and_write(50, 'e');
	//show_alloc_mem();
	test_verify_and_free(ptr[2], 30, 'c');
	test_verify_and_free(ptr[3], 40, 'd');
	//show_alloc_mem();
	ptr[2] = test_malloc_and_write(40, 'f');
	show_alloc_mem();
	test_verify_and_free(ptr[0], 10, 'a');
	test_verify_and_free(ptr[1], 20, 'b');
	test_verify_and_free(ptr[4], 50, 'e');
	test_verify_and_free(ptr[2], 40, 'f');
	show_alloc_mem();
}

void test__alloc_even_dealloc(size_t size, int nbr_alloc)
{
	assert(nbr_alloc < 2000);

	to_stdout(COLOR_RED "TEST 2 - MAX TINY alloc - unorder dealloc\n" COLOR_RESET);
	void *ptr[2000];

	int i = 0;

	while ( i < nbr_alloc )
	{
		ptr[i] = test_malloc_and_write(size, (char)(i % 255));
		show_alloc_mem();
		i++;
	}

	to_stdout(COLOR_RED "TEST 2 - allocated\n" COLOR_RESET);
	show_alloc_mem();
	to_stdout(COLOR_RED "TEST 2 - let's free\n" COLOR_RESET);

	i = 1;
	while ( i < nbr_alloc )
	{
		test_verify_and_free(ptr[i], size, (char)(i % 255) );
		show_alloc_mem();

		i+=2;
	}

	i = 0;
	while ( i < nbr_alloc )
	{
		test_verify_and_free(ptr[i], size, (char)(i % 255) );
		show_alloc_mem();

		i+=2;
	}
	show_alloc_mem();
}


void test__alloc_unorder_dealloc(size_t size, int nbr_alloc)
{
	assert(nbr_alloc < 2000);

	to_stdout(COLOR_RED "TEST 2 - MAX TINY alloc - unorder dealloc\n" COLOR_RESET);
	void *ptr[2000];

	int i = 0;

	while ( i < nbr_alloc )
	{
		ptr[i] = test_malloc_and_write(size, (char)(i % 255));
		show_alloc_mem();
		i++;
	}

	to_stdout(COLOR_RED "TEST 2 - allocated\n" COLOR_RESET);
	show_alloc_mem();
	to_stdout(COLOR_RED "TEST 2 - let's free\n" COLOR_RESET);

	i -= 1;

	while ( i >= 0)
	{
		test_verify_and_free(ptr[i], size, (char)(i % 255) );
		show_alloc_mem();

		i--;
	}
	show_alloc_mem();
}

void test__alloc_order_dealloc(size_t size, int nbr_alloc)
{
	assert(nbr_alloc < 2000);

	to_stdout(COLOR_RED "TEST 2 - MAX TINY alloc - unorder dealloc\n" COLOR_RESET);
	void *ptr[2000];

	int i = 0;

	while ( i < nbr_alloc )
	{
		ptr[i] = test_malloc_and_write(size, (char)i % 254 );
		show_alloc_mem();
		i++;
	}

	to_stdout(COLOR_RED "TEST 2 - allocated\n" COLOR_RESET);
	show_alloc_mem();
	to_stdout(COLOR_RED "TEST 2 - let's free\n" COLOR_RESET);

	i = 0;

	while ( i < nbr_alloc )
	{
		test_verify_and_free(ptr[i], size, (char)i % 254 );
		show_alloc_mem();

		i++;
	}
	show_alloc_mem();
}

int main(void)
{
	to_stdout("/////////TESTING\\\\\\\\\\\n");

	test_remove_ordored();
	test_remove_unordored();
	first_test();
	test__alloc_order_dealloc(128, 100);
	test__alloc_unorder_dealloc(128, 100);

	test__alloc_order_dealloc(128, 254);
	test__alloc_unorder_dealloc(128, 254);
	test__alloc_even_dealloc(128, 254);


	//test__alloc_order_dealloc(1, 300); //300 to have more than 1 block (17680 / (48 + 16))
	//test__alloc_unorder_dealloc(1, 300);

	test__alloc_order_dealloc(129, 254);
	test__alloc_unorder_dealloc(129, 254);

	test__alloc_order_dealloc(1024, 254);
	test__alloc_unorder_dealloc(1024, 254);

	//test__alloc_order_dealloc(2048, 40);
	//test__alloc_unorder_dealloc(2048, 40);
	test__alloc_even_dealloc(2048, 20);

	return 0;

	void *ptr[1000];

	for (size_t i = 0; i < 400; i++)
	{
		ptr[i] = malloc(128);
	}
	for (size_t i = 0; i < 200; i++)
	{
		free(ptr[i]);
	}
	show_alloc_mem();

	return 0;
}
