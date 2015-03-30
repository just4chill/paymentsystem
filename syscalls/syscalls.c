/* Don't need anything here. */

#include <stdlib.h>
#include <sys/stat.h>

int _read (int file, char * ptr, int len)
{
	( void ) file;
	( void ) ptr;
	( void ) len;
	return -1;
}

/***************************************************************************/

int _lseek (int file, int ptr, int dir)
{
	( void ) file;
	( void ) ptr;
	( void ) dir;
	
	return 0;
}

int _write (int file, char * ptr, int len)
{  
	return 0;
}

caddr_t _sbrk (int incr)
{
	( void ) incr;
	return 0;
}

int _close(int file)
{
	(void)file;
	return -1;
}

int _isatty(int file)
{
	(void)file;
	return 0;	
}

int _fstat(int file, struct stat *st)
{
	return -1;
}



