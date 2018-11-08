/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <reent.h>
#include <unistd.h>
#include <sys/wait.h>


extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));


register char * stack_ptr asm("sp");

static char __dram_ok = 0, __dram_set = 0;
static char *heap_end, *dram_addr, *dram_end;
static char* heap_end_backup;

void __DRAM_RESET_()
{
	if (heap_end_backup == 0)
		return;
	heap_end = heap_end_backup;
	__dram_ok = 0;
	__dram_set = 0;
}

void __DRAM_SET_()
{
	__dram_ok = 1;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;
	if (dram_addr == 0)
		dram_addr = (char*) 0xC0000000;
	if (dram_end == 0)
		dram_end = (char*) 0xC1000000;

	if (__dram_ok == 1 && __dram_set == 0)
	{
		__dram_set = 1;
		heap_end_backup = heap_end;
		heap_end = dram_addr;
	}

	prev_heap_end = heap_end;

	if (__dram_ok == 0)
	{
		if (heap_end + incr > stack_ptr)
		{
			errno = ENOMEM;
			return (caddr_t) -1;
		}
	}
	else
	{
		if (heap_end + incr > dram_end)
		{
			errno = ENOMEM;
			return (caddr_t) -1;
		}
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}
/*
caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end, *dram_addr, *dram_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;
	if (dram_addr == 0)
		dram_addr = (char*) 0xC0000800;
	if (dram_end == 0)
		dram_end = (char*) 0xC1000000;

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr)
	{
		if (heap_end + incr > dram_end)
		{
			errno = ENOMEM;
			return (caddr_t) -1;
		}
		else if (heap_end < dram_addr)
		{
			prev_heap_end = heap_end = dram_addr;
		}
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}
*/

/*
 * _gettimeofday primitive (Stub function)
 * */
int _gettimeofday (struct timeval * tp, struct timezone * tzp)
{
  /* Return fixed data for the timezone.  */
  if (tzp)
    {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
}
void initialise_monitor_handles()
{
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

		for (DataIdx = 0; DataIdx < len; DataIdx++)
		{
		   __io_putchar( *ptr++ );
		}
	return len;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
	  *ptr++ = __io_getchar();
	}

   return len;
}

int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}
