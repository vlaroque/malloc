#ifndef PRINTING_H
#define PRINTING_H

#define COLOR_BLACK         "\x1b[30m"
#define COLOR_RED           "\x1b[31m"
#define COLOR_GREEN         "\x1b[32m"
#define COLOR_YELLOW        "\x1b[33m"
#define COLOR_YELLOW_BOLD   "\x1b[1;33m"
#define COLOR_YELLOW_FAINT  "\x1b[2;33m"
#define COLOR_BLUE          "\x1b[34m"
#define COLOR_BLUE_BOLD     "\x1b[1;34m"
#define COLOR_MAGENTA       "\x1b[35m"
#define COLOR_CYAN          "\x1b[36m"
#define COLOR_CYAN_BOLD     "\x1b[1;36m"
#define COLOR_WHITE         "\x1b[37m"
#define COLOR_RESET         "\x1b[0m"
#define COLOR_BOLD          "\x1b[1m"

void str_out(char *str);
void str_out(char *str);
void putnbr_out(int n);
void puthex_out(unsigned long n);
void putptr_out(void* n);


#define DUMP_PTR(obj, field) \
	do { \
		str_out(COLOR_CYAN_BOLD #field COLOR_RESET " = " COLOR_BLUE);\
		putptr_out(obj->field);\
		if (obj->field == NULL)\
		{\
			str_out("           ");\
		}\
		str_out(COLOR_RESET);\
	} while (0)

#define DUMP_INT(obj, field) \
	do { \
		str_out(COLOR_CYAN_BOLD #field COLOR_RESET " = " COLOR_BLUE);\
		putnbr_out(obj->field);\
		str_out(COLOR_RESET);\
	} while (0)


#define DUMP_VAR(var) do\
	{\
		str_out(#var " = ");\
		putnbr_out(var);\
		str_out("\n");\
	} while(0)

#endif /* PRINTING_H */
