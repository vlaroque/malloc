# Chapter 1 Foreword

Here is what Wikipedia has to say about sensory memory.

During every moment of an organism’s life, sensory information is being taken in by sensory receptors and processed by the nervous system. Sensory information is stored in sensory memory just long enough to be transferred to short-term memory.[1] Humans have five traditional senses: sight, hearing, taste, smell, touch.

Sensory memory (SM) allows individuals to retain impressions of sensory information after the original stimulus has ceased.[2] A common demonstration of SM is a child’s ability to write letters and make circles by twirling a sparkler at night. When the sparkler is spun fast enough, it appears to leave a trail which forms a continuous image. This “light trail” is the image that is represented in the visual sensory store known as iconic memory.

The other two types of SM that have been most extensively studied are echoic memory,and haptic memory; however, it is reasonable to assume that each physiological sense has a corresponding memory store. Children for example have been shown to remember specific “sweet” tastes during incidental learning trials but the nature of this gustatory store is still unclear.

# Chapter 2 General Instructions

This project will be corrected by humans only. You’re allowed to organise and name your files as you see fit, but you must follow the following rules.

* The library must be named libft_malloc_$HOSTTYPE.so.

* A Makefile or something similar must compile the project and must contain the usual rules. It must recompile and re-link the program only if necessary.

* Your Makefile will have to check the existence of the environment variable $HOST-TYPE. If it is empty or non-existant, to assign the following value: ‘uname -m‘_‘uname -s‘

```bash
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
```

* Your Makefile will have to create a symbolic link libft_malloc.so pointing to libft_malloc_$HOSTTYPE.so so for example: `libft_malloc.so -> libft_malloc_intel-mac.so`

* If you are clever, you will use your library for your malloc. Submit also your folder libft including its own Makefile at the root of your repository. Your Makefile will have to compile the library, and then compile your project.

* You are allowed a global variable to manage your allocations and one for the thread- safe.

* Your project must be clean code even without norm if it’s ugly you will get 0.

* You have to handle errors carefully. In no way can your function leads to undefined behaviour or segv.

* Within the mandatory part, you are allowed to use the following functions:
	* mmap(2)
	* munmap(2)
	* getpagesize under OSX or sysconf(_SC_PAGESIZE) under linux
	* getrlimit(2)
	* The authorized functions within your libft (write(2) for exemple ;-) )
	* The functions from libpthread

* You are allowed to use other functions to complete the bonus part as long as their use is justified during your defence. Be smart!

You can ask your questions on the forum, on slack...

# Chapter 3 Mandatory part

This mini project is about writing a dynamic allocation memory management library. So that you can use it with some programs already in use without modifying them or recompiling, you must rewrite the following libc functions malloc(3), free(3) and realloc(3).

Your functions will be prototyped like the sytems ones:

```c
#include <stdlib.h>
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
```

The malloc() function allocates “size” bytes of memory and returns a pointer to the allocated memory.

The realloc() function tries to change the size of the allocation pointed to by “ptr” to “size”, and returns “ptr”. If there is not enough room to enlarge the memory allocation pointed to by ptr, realloc() creates a new allocation, copies as much of the old data pointed to by “ptr” as will fit to the new allocation, frees the old allocation, and returns a pointer to the allocated memory.

The free() function deallocates the memory allocation pointed to by ptr. If ptr is a NULL pointer, no operation is performed.

If there is an error, the malloc() and realloc() functions return a NULL pointer.

You must use the mmap(2) and munmap(2) syscall to claim and return the memory zones to the system.

You must manage your own memory allocations for the internal functioning of your project without using the libc malloc function.

With performance in mind, you must limit the number of calls to mmap(), but also to munmap(). You have to “pre-allocate” some memory zones to store your “small” and “medium” malloc.

The size of these zones must be a multiple of getpagesize() under osX or sysconf(_SC_PAGESIZE) under linux.

• Each zone must contain at least 100 allocations.
* “TINY” mallocs, from 1 to n bytes, will be stored in N bytes big zones.
* “SMALL” mallocs, from (n+1) to m bytes, will be stored in M bytes big zones.
* “LARGE” mallocs, fron (m+1) bytes and more, will be stored out of zone, which simply means with mmap(), they will be in a zone on their own.

It’s up to you to define the size of n, m, N and M so that you find a good compromise between speed (saving on system recall) and saving memory.

You also must write a function that allows visual on the state of the allocated memory zones. It needs to be prototyped as follows:

```c
void show_alloc_mem();
```

The visual will be formatted by increasing addresses such as:

```bash
TINY : 0xA0000
0xA0020 - 0xA004A : 42 bytes
0xA006A - 0xA00BE : 84 bytes
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 bytes
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 bytes
Total : 52698 bytes
```

⚠️ You must align the memory given by your malloc

# Chapter 4 Bonus part

The following is the first bonus in this subject:

• Manage the use of your malloc in a multi-threaded program (so to be “thread safe” using the pthread lib).

In order to get the maximum score you have to implement some additional functions (non exhaustive list) such as:

• Manage the malloc debug environment variables. You can imitate those from malloc system or invent your own.

• Create a show_alloc_mem_ex() function that displays more details, for example, a history of allocations, or an hexa dump of the allocated zones.

• “Defragment” the freed memory.

⚠️ The bonus part will only be assessed if the mandatory part is PERFECT. Perfect means the mandatory part has been integrally done and works without malfunctioning. If you have not passed ALL the mandatory requirements, your bonus part will not be evaluated at all
