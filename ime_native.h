/*****************************************************************************
*
* Infinite Memory Engine - Native interface
*
* Copyright (c) 2018, DataDirect Networks.
*
******************************************************************************/

#ifndef _IME_NATIVE_H
#define _IME_NATIVE_H

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

/* Verion of the IME native interface */
#define IME_NATIVE_API_VERSION 130

/* Default prefix for IME-specific files */
#define DEFAULT_IME_FILE_PREFIX     "ime://"
/* Default prefix for IME-specific files (without the trailing slash characters) */
#define DEFAULT_IME_PREFIX_NO_FWD_SLASH "ime:"
#define DEFAULT_IME_FILE_PREFIX_LEN (sizeof(DEFAULT_IME_FILE_PREFIX) - 1)
#define IME_FILE_PREFIX_LEN_NO_FWD_SLASH (sizeof(DEFAULT_IME_PREFIX_NO_FWD_SLASH) - 1)

/*
 * Initialization and Finalization
 */

/* Prepare the IME native client interface for use. The call takes no explicit
 * parameters. Configuration information is obtained through several
 * environment variables:
 * - IM_CLIENT_CFG_FILE=<path>  : Path to the IME configuration file.
 * - IM_CLIENT_DEBUG_LEVEL=<int>: Level of verbosity */
void    ime_native_init(void);
/* Finalize the IME native client interface */
int     ime_native_finalize(void);

/*
 * Path Name Operations
 */

/* IME equivalent for the open() system call */
int     ime_native_open(const char *pathname, int flags, mode_t mode);
/* IME equivalent for the access() system call */
int     ime_native_access(const char *pathname, int mode);
/* IME equivalent for the stat() system call */
int     ime_native_stat(const char *pathname, struct stat *buf);
/* IME equivalent for the unlink() system call */
int     ime_native_unlink(const char *pathname);
/* IME equivalent for the mkdir() system call */
int     ime_native_mkdir(const char *pathname, mode_t mode);
/* IME equivalent for the rmdir() system call */
int     ime_native_rmdir(const char *pathname);
/* IME equivalent for the statvfs system call */
int     ime_native_statvfs(const char *pathname, struct statvfs *buf);

/*
 * File Descriptor Operations
 */

/* IME equivalent for the close() system call */
int     ime_native_close(int fd);
/* IME equivalent for the write() system call */
ssize_t ime_native_write(int fd, const char *buf, size_t count);
/* IME equivalent for the read() system call */
ssize_t ime_native_read(int fd, char *buf, size_t count);
/* IME equivalent for the pwrite() system call */
ssize_t ime_native_pwrite(int fd, const char *buf, size_t count, off_t offset);
/* IME equivalent for the pread() system call */
ssize_t ime_native_pread(int fd, char *buf, size_t count, off_t offset);
/* IME equivalent for the pwritev() system call */
ssize_t ime_native_pwritev(int fd, const struct iovec *iov,
                           int iovcnt, off_t offset);
/* IME equivalent for the preadv() system call */
ssize_t ime_native_preadv(int fd, const struct iovec *iov,
                          int iovcnt, off_t offset);
/* IME equivalent for the fsync() system call */
int     ime_native_fsync(int fd);
/* IME equivalent for the ftruncate() system call */
int     ime_native_ftruncate(int fd, off_t off);
/* IME equivalent for the lseek() system call */
off_t   ime_native_lseek(int fd, off_t offset, int whence);
/* IME equivalent for the dup2 system call */
int     ime_native_dup2(int oldfd, int newfd);

/*
 * Non-blocking/Asynchronous operations
 */

struct ime_aiocb
{
    int           fd;          /* File descriptor */
    int           iovcnt;      /* Number of IOVs entries in the iov[] array */
    uint32_t      flags;       /* Deprecated */
    const struct iovec *iov;   /* Array of IOVs */
    off_t         file_offset; /* Offset of the file pointed by fd */
    /* Callback function called when operation completes */
    void        (*complete_cb)(struct ime_aiocb *aiocb, int err, ssize_t bytes);
    intptr_t      user_context; /* Private to IME, shouldn't be modified */
};

/* Non blocking/Asynchronous write operation */
int     ime_native_aio_write(struct ime_aiocb *aiocb);
/* Non blocking/Asynchronous read operation */
int     ime_native_aio_read(struct ime_aiocb *aiocb);

/*
 * IME Non-Standard Operations
 */

/* Return a null-terminated string that contains the IME version */
const char *ime_native_version(void);

#endif

