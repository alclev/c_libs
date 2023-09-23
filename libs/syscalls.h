#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include <termios.h>
#include <unistd.h>
#include <utime.h>
#include <wait.h>

void print(int fd, const char *format, ...);
void println(int fd, const char *msg);
void *sec_malloc(size_t size);
void *sec_calloc(size_t nmemb, size_t size);
void *sec_realloc(void *old, size_t sizeOld, size_t sizeNew);
void sec_free(void *ptr);
void bin_dump(unsigned char *addr, size_t size, int endianess);
void print_err(const char *msg, int errnum);

char *sys_getlogin (void);
char *sys_ctermid(char *s);
char *sys_getcwd(char *buf, size_t size);
char *sys_getenv(const char *name);
char *sys_ttyname ( int desc );
clock_t sys_times(struct tms *buf);
DIR *sys_opendir(const char *name);
FILE *sys_fdopen (int fildes, const char *mode);
int sys_access(const char *pathname, int mode);
int sys_chdir(const char *path);
int sys_chmod(const char *path, mode_t mode);
int sys_chown(const char *path, uid_t owner, gid_t group);
int sys_close(int fd);
int sys_closedir(DIR *dir);
int sys_creat(const char *pathname, mode_t mode);
int sys_dup(int oldfd);
int sys_dup2(int oldfd, int newfd);
int sys_execv( const char *path, char *const argv[]);
int sys_execvp( const char *file, char *const argv[]);
int sys_fcntl(int fd, int cmd);
int sys_fileno( FILE *stream);
int sys_fstat(int filedes, struct stat *buf);
int sys_getgroups(int size, gid_t list[]);
int sys_isatty (int desc);
int sys_kill(pid_t pid, int sig);
int sys_link(const char *oldpath, const char *newpath);
int sys_mkdir(const char *pathname, mode_t mode);
int sys_mkfifo ( const char *pathname, mode_t mode );
int sys_open(const char *pathname, int flags);
int sys_pause(void);
int sys_pipe(int filedes[2]);
int sys_rename(const char *oldpath, const char *newpath);
int sys_rmdir(const char *pathname);
int sys_setjmp(jmp_buf env);
int sys_setpgid(pid_t pid, pid_t pgid);
int sys_sigaddset(sigset_t *set, int signum);
int sys_sigdelset(sigset_t *set, int signum);
int sys_sigemptyset(sigset_t *set);
int sys_sigfillset(sigset_t *set);
int sys_sigismember(const sigset_t *set, int signum);
int sys_sigpending(sigset_t *set);
int sys_sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sys_sigsetjmp(sigjmp_buf env, int savesigs);
int sys_sigsuspend(const sigset_t *mask);
int sys_stat(const char *file_name, struct stat *buf);
int sys_tcdrain ( int fd );
int sys_tcflow ( int fd, int action );
int sys_tcflush ( int fd, int queue_selector );
int sys_tcgetattr ( int fd, struct termios *termios_p);
int sys_tcsendbreak ( int fd, int duration );
int sys_tcsetpgrp ( int fd, pid_t pgrpid );
int sys_uname(struct utsname *buf);
int sys_unlink(const char *pathname);
int sys_utime(const char *filename, struct utimbuf *buf);
long sys_fpathconf(int filedes, int name);
long sys_sysconf(int name);
mode_t sys_umask(mode_t mask);
off_t sys_lseek(int fildes, off_t offset, int whence);
pid_t sys_fork(void);
pid_t sys_getpgrp(void);
pid_t sys_getpid(void);
pid_t sys_getppid(void);
pid_t sys_setsid(void);
pid_t sys_tcgetpgrp ( int fd );
pid_t sys_waitpid(pid_t pid, int *status, int options);
speed_t sys_cfgetispeed ( struct termios *termios_p );
speed_t sys_cfgetospeed ( struct termios *termios_p );
ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
struct dirent *sys_readdir(DIR *dir);
struct group *sys_getgrgid(gid_t gid);
struct group *sys_getgrnam(const char *name);
struct passwd *sys_getpwnam(const char * name);
struct passwd *sys_getpwuid(uid_t uid);
int sys_sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact);
void sys_exit(int status);

#endif