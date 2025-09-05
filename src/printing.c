#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <libft.h>
#include <assert.h>

void c_out(char c)
{
	static char buff[4096];
	static size_t i = 0;

	buff[i++] = c;

	if ( c == '\n' || i >= sizeof(buff) - 1)
	{
		ssize_t ignored = write(STDOUT_FILENO, buff, i);
		(void)ignored;
		i = 0;
		return;
	}

}

void strn_out(char *str, size_t len)
{
	size_t i   = 0;

	while ( i < len )
	{
		c_out(str[i]);
		i++;
	}
}

void str_out(char *str)
{
	size_t len = ft_strlen(str);

	strn_out(str, len);
}

void putnbr_out(int n)
{
	char buf[32]; // big enough for int
	int i = 30;          // index to fill backwards
	int neg = 0;

	if (n == 0)
	{
		str_out("0");
		return;
	}

	if (n < 0)
	{
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

	strn_out((char *)(buf + i + 1), 30 - i);
}

void puthex_out(unsigned long n)
{
	char buf[32]; // plenty for 64-bit hex (max 16 digits + "0x")
	const char *digits = "0123456789ABCDEF";
	int i = 30;

	if (n == 0)
	{
		str_out("0x0");
		return;
	}

	while (n > 0 && i >= 0)
	{
		buf[i--] = digits[n & 0xF]; // n % 16
		n >>= 4;                    // n /= 16
	}

	buf[i--] = 'x';
	buf[i--] = '0';

	strn_out((char *)buf + i + 1, 30 - i);
}

void putptr_out(void* n)
{
	puthex_out((uintptr_t)n);
}


