#include "syscalls.h"

/**
 * Prints formatted output to a file descriptor.
 *
 * @param fd The file descriptor to write the output to.
 * @param format The format string specifying the output format.
 * @param ... Additional arguments to be formatted according to the format string.
 *
 * @returns None
 */
void print(int fd, const char *format, ...) {
    va_list args;
    va_start(args, format); 

    va_list cpy;
    va_copy(cpy, args);
    size_t len = vsnprintf(NULL, 0, format, cpy) + 1;
    char *out = malloc(len);
    vsnprintf(out, len, format, args);
    write(fd, out, len - 1);

    free(out);
    va_end(args);
}

/**
 * Writes a message followed by a newline character to the specified file descriptor.
 *
 * @param fd The file descriptor to write to.
 * @param msg The message to write.
 *
 * @returns None
 */
void println(int fd, const char *msg){
    size_t len = snprintf(NULL, 0, "%s\n", msg) + 1;
    char *out = malloc(len);
    snprintf(out, len, "%s\n", msg);
    write(fd, out, len);
    free(out);
}

/* 
0 for little endian
1 for big endian
*/
void bin_dump(unsigned char *addr, size_t size, int endianess){
    if(!endianess){ //little endian
        for(size_t i = 0; i < size; i ++){
            for(size_t j = 0; j < 8; j ++){
                print(STDOUT_FILENO, "%d", (*(addr + i) >> j) & 1);
            }
            print(STDOUT_FILENO, " ");
        }
        println(STDOUT_FILENO, "");
    }else{ //big endian
        for(size_t i = 0; i < size; i ++){
            for(ssize_t j = 7; j >= 0; j --){
                print(STDOUT_FILENO, "%d", (*(addr + i) >> j) & 1);
            }
            print(STDOUT_FILENO, " ");
        }
        println(STDOUT_FILENO, "");
    }
}

/**
 * Prints an error message along with the corresponding error number and exits the program.
 *
 * @param msg The error message to be printed.
 * @param errnum The error number associated with the error.
 *
 * @returns None
 */
void print_err_exit(const char *msg, int errnum){
    size_t len = snprintf(NULL, 0, "%s: %s\n", msg, strerror(errnum)) + 1;
    char *err = malloc(len);
    snprintf(err, len, "%s: %s\n", msg, strerror(errnum));
    write(STDERR_FILENO, msg, len);
    free(err);
    exit(EXIT_FAILURE);
}

/**
 * Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate.
 *
 * @returns A pointer to the allocated memory block.
 */
void *sec_malloc(size_t size){
    return malloc(size);
}

/**
 * Allocates memory for an array of elements and initializes them to zero.
 *
 * @param nmemb The number of elements to allocate.
 * @param size The size of each element in bytes.
 *
 * @returns A pointer to the allocated memory, or NULL if the allocation fails.
 *          If the allocation fails, an error message is printed and the program exits.
 */
void *sec_calloc(size_t nmemb, size_t size){
    void *res;
    if((res = calloc(nmemb, size)) == NULL){
        print_err_exit("calloc", errno);
    }
    return res;
}

/**
 * Reallocates memory for a given block of memory.
 *
 * @param old A pointer to the old memory block.
 * @param sizeOld The size of the old memory block.
 * @param sizeNew The size of the new memory block.
 *
 * @returns A pointer to the new memory block.
 */
void *sec_realloc(void *old, size_t sizeOld, size_t sizeNew){
    void *new = sec_malloc(sizeNew);
    memcpy(new, old, sizeOld);
    memset(old, 0, sizeOld);
    free(old);
    return new;
}

/**
 * Frees the memory allocated for a pointer.
 *
 * @param ptr A pointer to the memory to be freed.
 *
 * @returns None
 */
void sec_free(void *ptr){
   free(ptr);
}

/**
 * Retrieves the login name of the current user.
 *
 * @returns A pointer to a string containing the login name of the current user.
 *          Returns NULL if an error occurs.
 */
char *sys_getlogin(){
    char *res;
    if ((res = getlogin()) == NULL){
        print_err_exit("getlogin", errno);
    }
    return res;
}

/**
 * Returns the path of the controlling terminal.
 *
 * @param s A pointer to a character array where the result will be stored.
 *
 * @returns A pointer to the path of the controlling terminal.
 *          If an error occurs, NULL is returned.
 */
char *sys_ctermid(char *s){
    char *res;
    if ((res = ctermid(s)) == NULL){
        print_err_exit("ctermid", errno);
    }
    return res;
}

/**
 * Retrieves the current working directory.
 *
 * @param buf A pointer to the buffer where the current working directory will be stored.
 * @param size The size of the buffer.
 *
 * @returns A pointer to the current working directory on success, or NULL on failure.
 */
char *sys_getcwd(char *buf, size_t size){
    char *res;
    if ((res = getcwd(buf, size)) == NULL){
        print_err_exit("getcwd", errno);
    }
    return res;
}

/**
 * Retrieves the value of an environment variable.
 *
 * @param name The name of the environment variable.
 *
 * @returns The value of the environment variable.
 *
 * @throws An error and exits the program if the environment variable does not exist.
 */
char *sys_getenv(const char *name){
    char *res;
    if ((res = getenv(name)) == NULL){
        print_err_exit("getenv", errno);
    }
    return res;
}

/**
 * Retrieves the name of the terminal associated with the given file descriptor.
 *
 * @param desc The file descriptor of the terminal.
 *
 * @returns A pointer to a string containing the name of the terminal.
 *          Returns NULL if an error occurs.
 */
char *sys_ttyname (int desc){
    char *res;
    if ((res = ttyname(desc)) == NULL){
        print_err_exit("ttyname", errno);
    }
    return res;
}

/**
 * Returns the current system time.
 *
 * @returns The current system time.
 */
clock_t sys_times(struct tms *buf){
    clock_t res;
    if ((res = times(buf)) == -1){
        print_err_exit("times", errno);
    }
    return res;
}

/**
 * Opens a directory for reading.
 *
 * @param path The path to the directory.
 *
 * @returns A pointer to the opened directory.
 */
DIR *sys_opendir(const char *name){
    DIR *res;
    if ((res = opendir(name)) == NULL){
        print_err_exit("opendir", errno);
    }
    return res;
}

/**
 * Opens a file descriptor as a stream.
 *
 * @param fd The file descriptor to be opened.
 * @param mode The mode in which the file should be opened.
 *
 * @returns A stream associated with the file descriptor.
 */
FILE *sys_fdopen (int fildes, const char *mode){
    FILE *res;
    if ((res = fdopen(fildes, mode)) == NULL){
        print_err_exit("fdopen", errno);
    }
    return res;
}

/**
 * Checks the accessibility of a file or directory.
 *
 * @param path The path to the file or directory.
 * 
 * @returns 0 if the file or directory is accessible, -1 otherwise.
 */
int sys_access(const char *pathname, int mode){
    int res;
    if ((res = access(pathname, mode)) == -1){
        print_err_exit("access", errno);
    }
    return res;
}

/**
 * Changes the current working directory.
 *
 * @param path The path to the directory to change to.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_chdir(const char *path){
    int res;
    if ((res = chdir(path)) == -1){
        print_err_exit("chdir", errno);
    }
    return res;
}

/**
The `sys_chmod` function is not a standard function in the C++ language. It seems to be a custom function or a function from a specific library. Without more information about the function and its purpose, it is difficult to provide a detailed docstring comment. However, a general docstring comment for a `sys_chmod` function could be:
 */
int sys_chmod(const char *path, mode_t mode){
    int res;
    if ((res = chmod(path, mode)) == -1){
        print_err_exit("chmod", errno);
    }
    return res;
}

/**
 * Changes the ownership of a file or directory.
 *
 * @param path The path to the file or directory.
 * @param owner The new owner of the file or directory.
 * @param group The new group of the file or directory.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_chown(const char *path, uid_t owner, gid_t group){
    int res;
    if ((res = chown(path, owner, group)) == -1){
        print_err_exit("chown", errno);
    }
    return res;
}

/**
 * Closes a file descriptor.
 *
 * @param fd The file descriptor to close.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_close(int fd){
    int res;
    if ((res = close(fd)) == -1){
        print_err_exit("close", errno);
    }
    return res;
}

/**
 * Closes a directory stream.
 *
 * @param dir The directory stream to be closed.
 *
 * @returns None
 */
int sys_closedir(DIR *dir){
    int res;
    if ((res = closedir(dir)) == -1){
        print_err_exit("closedir", errno);
    }
    return res;
}

/**
 * Creates a new file with the specified name and permissions.
 *
 * @param filename The name of the file to be created.
 * @param mode The permissions to be set for the file.
 *
 * @returns On success, the file descriptor of the newly created file. On failure, -1 is returned.
 */
int sys_creat(const char *pathname, mode_t mode){
    int res;
    if ((res = creat(pathname, mode)) == -1){
        print_err_exit("creat", errno);
    }
    return res;
}

/**
 * Duplicates a file descriptor.
 *
 * @param oldfd The file descriptor to duplicate.
 *
 * @returns The new file descriptor, or -1 if an error occurred.
 */
int sys_dup(int oldfd){
    int res;
    if ((res = dup(oldfd)) == -1){
        print_err_exit("dup", errno);
    }
    return res;
}

/**
 * Duplicates a file descriptor to another file descriptor.
 *
 * @param oldfd The file descriptor to be duplicated.
 * @param newfd The file descriptor to duplicate to.
 *
 * @returns On success, returns the new file descriptor. On failure, returns -1.
 */
int sys_dup2(int oldfd, int newfd){
    int res;
    if ((res = dup2(oldfd, newfd)) == -1){
        print_err_exit("dup2", errno);
    }
    return res;
}

/**
 * Executes a program specified by the given path.
 *
 * @param path The path to the program to be executed.
 *
 * @returns None
 */
int sys_execv( const char *path, char *const argv[]){
    int res;
    if ((res = execv(path, argv)) == -1){
        print_err_exit("execv", errno);
    }
    return res;
}

/**
 * Executes a program using the `execvp` system call.
 *
 * @param file The name or path of the program to execute.
 * @param argv An array of null-terminated strings that represent the program arguments.
 *
 * @returns None
 */
int sys_execvp( const char *file, char *const argv[]){
    int res;
    if ((res = execvp(file, argv)) == -1){
        print_err_exit("execvp", errno);
    }
    return res;
}

int sys_fcntl(int fd, int cmd){
    int res;
    if ((res = fcntl(fd, cmd)) == -1){
        print_err_exit("fcntl", errno);
    }
    return res;
}

/**
 * Returns the file descriptor associated with the specified file object.
 *
 * @param fileobj A file object.
 *
 * @returns The file descriptor associated with the file object.
 */
int sys_fileno(FILE *stream){
    int res;
    if ((res = fileno(stream)) == -1){
        print_err_exit("fileno", errno);
    }
    return res;
}

/**
 * Retrieves information about a file or file descriptor.
 *
 * @param fd The file descriptor of the file.
 * @param buf A pointer to a structure where the file information will be stored.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_fstat(int filedes, struct stat *buf){
    int res;
    if ((res = fstat(filedes, buf)) == -1){
        print_err_exit("fstat", errno);
    }
    return res;
}

/**
 * Retrieves the list of group IDs for the current user.
 *
 * @returns An array of group IDs for the current user.
 */
int sys_getgroups(int size, gid_t list[]){
    int res;
    if ((res = getgroups(size, list)) == -1){
        print_err_exit("getgroups", errno);
    }
    return res;
}

/**
 * Checks if the standard output is connected to a terminal.
 *
 * @returns True if the standard output is connected to a terminal, False otherwise.
 */
int sys_isatty (int desc){
    int res;
    if ((res = isatty(desc)) == -1){
        print_err_exit("isatty", errno);
    }
    return res;
}

int sys_kill(pid_t pid, int sig){
    int res;
    if ((res = kill(pid, sig)) == -1){
        print_err_exit("kill", errno);
    }
    return res;
}

/**
 * Creates a hard link between two files.
 *
 * @param source The path to the source file.
 * @param destination The path to the destination file.
 *
 * @returns None
 */
int sys_link(const char *oldpath, const char *newpath){
    int res;
    if ((res = link(oldpath, newpath)) == -1){
        print_err_exit("link", errno);
    }
    return res;
}

/**
 * Creates a new directory with the specified name.
 *
 * @param directory_name The name of the directory to be created.
 *
 * @returns None
 */
int sys_mkdir(const char *pathname, mode_t mode){
    int res;
    if ((res = mkdir(pathname, mode)) == -1){
        print_err_exit("mkdir", errno);
    }
    return res;
}

/**
 * Creates a named pipe (FIFO) with the specified name.
 *
 * @param path The path and name of the FIFO to be created.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_mkfifo(const char *pathname, mode_t mode ){
    int res;
    if ((res = mkfifo(pathname, mode)) == -1){
        print_err_exit("mkfifo", errno);
    }
    return res;
}

/**
 * Opens a file or device.
 *
 * @param filename The name of the file or device to open.
 * @param flags The flags to use when opening the file or device.
 * @param mode The permissions to set on the file or device.
 *
 * @returns The file descriptor of the opened file or device.
 */
int sys_open(const char *pathname, int flags){
    int res;
    if ((res = open(pathname, flags)) == -1){
        print_err_exit("open", errno);
    }
    return res;
}

int sys_pause(){
    int res;
    if ((res = pause()) == -1){
        print_err_exit("pause", errno);
    }
    return res;
}

/**
 * Creates a pipe for interprocess communication.
 *
 * @returns A pair of file descriptors representing the read and write ends of the pipe.
 */
int sys_pipe(int filedes[2]){
    int res;
    if ((res = pipe(filedes)) == -1){
        print_err_exit("pipe", errno);
    }
    return res;
}

int sys_rename(const char *oldpath, const char *newpath){
    int res;
    if ((res = rename(oldpath, newpath)) == -1){
        print_err_exit("rename", errno);
    }
    return res;
}

/**
 * Removes a directory.
 *
 * @param path The path of the directory to be removed.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_rmdir(const char *pathname){
    int res;
    if ((res = rmdir(pathname)) == -1){
        print_err_exit("rmdir", errno);
    }
    return res;
}

/**
 * Sets up a jump point for non-local control flow.
 *
 * @returns The value to be returned by the corresponding sys_longjmp call.
 */
int sys_setjmp(jmp_buf env){
    int res;
    if ((res = setjmp(env)) == -1){
        print_err_exit("setjmp", errno);
    }
    return res;
}

/**
 * Sets the process group ID of the calling process.
 *
 * @param None
 *
 * @returns None
 */
int sys_setpgid(pid_t pid, pid_t pgid){
    int res;
    if ((res = setpgid(pid, pgid)) == -1){
        print_err_exit("setpgid", errno);
    }
    return res;
}

/**
 * Adds a signal to a signal set.
 *
 * @param set A pointer to the signal set.
 * @param signum The signal number to add to the set.
 *
 * @returns None
 */
int sys_sigaddset(sigset_t *set, int signum){
    int res;
    if ((res = sigaddset(set, signum)) == -1){
        print_err_exit("sigaddset", errno);
    }
    return res;
}

/**
 * Removes a signal from a signal set.
 *
 * @param set A pointer to the signal set.
 * @param signum The signal number to be removed.
 *
 * @returns None
 */
int sys_sigdelset(sigset_t *set, int signum){
    int res;
    if ((res = sigdelset(set, signum)) == -1){
        print_err_exit("sigdelset", errno);
    }
    return res;
}

/**
 * Initializes an empty signal set.
 *
 * This function initializes a signal set to be empty, meaning it does not contain any signals.
 *
 * @returns None
 */
int sys_sigemptyset(sigset_t *set){
    int res;
    if ((res = sigemptyset(set)) == -1){
        print_err_exit("sigemptyset", errno);
    }
    return res;
}

/**
 * Fills a signal set with all available signals.
 *
 * @returns None
 */
int sys_sigfillset(sigset_t *set){
    int res;
    if ((res = sigfillset(set)) == -1){
        print_err_exit("sigfillset", errno);
    }
    return res;
}

/**
 * Checks if a specific signal is a member of a signal set.
 *
 * @param set A pointer to the signal set.
 * @param signum The signal number to check.
 *
 * @returns 1 if the signal is a member of the set, 0 if it is not, or -1 if an error occurred.
 */
int sys_sigismember(const sigset_t *set, int signum){
    int res;
    if ((res = sigismember(set, signum)) == -1){
        print_err_exit("sigismember", errno);
    }
    return res;
}

/**
 * Retrieves the set of pending signals for the calling process.
 *
 * @returns A set of pending signals for the calling process.
 */
int sys_sigpending(sigset_t *set){
    int res;
    if ((res = sigpending(set)) == -1){
        print_err_exit("sigpending", errno);
    }
    return res;
}

/**
 * Retrieves and/or modifies the signal mask of the calling thread.
 *
 * @param how Specifies the how argument for the sigprocmask system call.
 *            - SIG_BLOCK: The set of blocked signals is the union of the current set and the set argument.
 *            - SIG_UNBLOCK: The signals in set are removed from the current set of blocked signals.
 *            - SIG_SETMASK: The set of blocked signals is set to the argument set.
 * @param set Points to the new signal mask to be set if how is SIG_SETMASK or SIG_BLOCK.
 *            If set is NULL, the signal mask is not changed.
 * @param oldset If non-NULL, the previous signal mask is stored in old
 */
int sys_sigprocmask(int how, const sigset_t *set, sigset_t *oldset){
    int res;
    if ((res = sigprocmask(how, set, oldset)) == -1){
        print_err_exit("sigprocmask", errno);
    }
    return res;
}

/**
 * Saves the current signal mask and program counter for later use by siglongjmp.
 *
 * @param env The buffer to store the saved signal mask and program counter.
 * @param savesigs A flag indicating whether to save the current signal mask.
 *
 * @returns 0 on success, or a non-zero value on failure.
 */
int sys_sigsetjmp(sigjmp_buf env, int savesigs){
    int res;
    if ((res = sigsetjmp(env, savesigs)) == -1){
        print_err_exit("sigsetjmp", errno);
    }
    return res;
}

/**
 * Suspends the execution of the calling process until a signal is received.
 *
 * @returns None
 */
int sys_sigsuspend(const sigset_t *mask){
    int res;
    if ((res = sigsuspend(mask)) == -1){
        print_err_exit("sigsuspend", errno);
    }
    return res;
}

int sys_stat(const char *file_name, struct stat *buf){
    int res;
    if ((res = stat(file_name, buf)) == -1){
        print_err_exit("stat", errno);
    }
    return res;
}

/**
 * Waits until all output written to the object referred to by the file descriptor has been transmitted.
 *
 * @param fd The file descriptor to wait for.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_tcdrain (int fd){
    int res;
    if ((res = tcdrain(fd)) == -1){
        print_err_exit("tcdrain", errno);
    }
    return res;
}

/**
 * Suspends or resumes the transmission or reception of data on a specified terminal.
 *
 * @param fd The file descriptor of the terminal.
 * @param action The action to be performed on the terminal flow control.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_tcflow (int fd, int action){
    int res;
    if ((res = tcflow(fd, action)) == -1){
        print_err_exit("tcflow", errno);
    }
    return res;
}

/**
 * Flushes the input and/or output buffer of a specified terminal.
 *
 * @param fd The file descriptor of the terminal.
 * @param queue_selector Specifies which buffer(s) to flush. It can be one of the following values:
 *                       - TCIFLUSH: flushes the input queue.
 *                       - TCOFLUSH: flushes the output queue.
 *                       - TCIOFLUSH: flushes both the input and output queues.
 *
 * @returns 0 on success, or -1 on failure.
 */
int sys_tcflush(int fd, int queue_selector){
    int res;
    if ((res = tcflush(fd, queue_selector)) == -1){
        print_err_exit("tcflush", errno);
    }
    return res;
}

/**
 * Retrieves the current terminal attributes.
 *
 * @returns The current terminal attributes.
 */
int sys_tcgetattr(int fd, struct termios *termios_p){
    int res;
    if ((res = tcgetattr(fd, termios_p)) == -1){
        print_err_exit("tcgetattr", errno);
    }
    return res;
}

/**
 * Sends a break signal to the terminal.
 *
 * This function is used to send a break signal to the terminal. It is typically used for serial communication
 * and can be used to generate a break condition on the line.
 *
 * @param sys The system or device to send the break signal to.
 *
 * @returns None
 */
int sys_tcsendbreak(int fd, int duration){
    int res;
    if ((res = tcsendbreak(fd, duration)) == -1){
        print_err_exit("tcsendbreak", errno);
    }
    return res;
}

/**
 * Sets the foreground process group ID associated with the given terminal file descriptor.
 *
 * @param fd The file descriptor of the terminal.
 * @param pgid The process group ID to set as the foreground process group.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_tcsetpgrp(int fd, pid_t pgrpid){
    int res;
    if ((res = tcsetpgrp(fd, pgrpid)) == -1){
        print_err_exit("tcsetpgrp", errno);
    }
    return res;
}

/**
 * Retrieves system information using the uname system call.
 *
 * @returns A structure containing system information such as the operating system name, 
 *          network node hostname, kernel release, kernel version, machine hardware name, 
 *          and processor type.
 */
int sys_uname(struct utsname *buf){
    int res;
    if ((res = uname(buf)) == -1){
        print_err_exit("uname", errno);
    }
    return res;
}

/**
 * Deletes a file from the file system.
 *
 * @param path The path to the file to be deleted.
 *
 * @returns 0 on success, -1 on failure.
 */
int sys_unlink(const char *pathname){
    int res;
    if ((res = unlink(pathname)) == -1){
        print_err_exit("unlink", errno);
    }
    return res;
}

/**
 * Returns the user CPU time in seconds.
 *
 * @returns The user CPU time in seconds.
 */
int sys_utime(const char *filename, struct utimbuf *buf){
    int res;
    if ((res = utime(filename, buf)) == -1){
        print_err_exit("utime", errno);
    }
    return res;
}

/**
 * Retrieves the value of a system file configuration variable.
 *
 * @param None
 *
 * @returns The value of the system file configuration variable.
 */
long sys_fpathconf(int filedes, int name){
    long res;
    if ((res = fpathconf(filedes, name)) == -1){
        print_err_exit("fpathconf", errno);
    }
    return res;
}

/**
 * Retrieves system configuration information.
 *
 * This function retrieves system configuration information using the sysconf() system call.
 * It returns the value of a system configuration variable specified by the argument name.
 *
 * @param name The name of the system configuration variable to retrieve.
 *
 * @returns The value of the system configuration variable.
 */
long sys_sysconf(int name){
    long res;
    if ((res = sysconf(name)) == -1){
        print_err_exit("sysconf", errno);
    }
    return res;
}

/**
 * Retrieves the current umask value of the system.
 *
 * @returns The current umask value.
 */
mode_t sys_umask(mode_t mask){
    mode_t res;
    if ((res = umask(mask)) == (mode_t) -1){
        print_err_exit("umask", errno);
    }
    return res;
}

/**
 * Changes the file offset of the specified file descriptor.
 *
 * @param fd The file descriptor of the file.
 * @param offset The new offset to set.
 * @param whence The reference point for the offset.
 *
 * @returns The new offset from the beginning of the file.
 */
off_t sys_lseek(int fildes, off_t offset, int whence){
    off_t res;
    if ((res = lseek(fildes, offset, whence)) == -1){
        print_err_exit("lseek", errno);
    }
    return res;
}

/**
 * Creates a new process by duplicating the existing process.
 *
 * @returns On success, the process ID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, no child process is created, and an error code is set appropriately.
 */
pid_t sys_fork(){
    pid_t res;
    if ((res = fork()) == -1){
        print_err_exit("fork", errno);
    }
    return res;
}

/**
 * Retrieves the process group ID of the calling process.
 *
 * @returns The process group ID of the calling process.
 */
pid_t sys_getpgrp(){
    pid_t res;
    if ((res = getpgrp()) == -1){
        print_err_exit("getpgrp", errno);
    }
    return res;
}

/**
 * Returns the process ID of the current process.
 *
 * @returns The process ID of the current process.
 */
pid_t sys_getpid(){
    pid_t res;
    if ((res = getpid()) == -1){
        print_err_exit("getpid", errno);
    }
    return res;
}

/**
 * Returns the parent process ID of the current process.
 *
 * @returns The parent process ID.
 */
pid_t sys_getppid(){
    pid_t res;
    if ((res = getppid()) == -1){
        print_err_exit("getppid", errno);
    }
    return res;
}

/**
 * Creates a new session and sets the process group ID of the calling process to the session ID.
 *
 * @returns The session ID of the newly created session.
 */
pid_t sys_setsid(){
    pid_t res;
    if ((res = setsid()) == -1){
        print_err_exit("setsid", errno);
    }
    return res;
}

/**
 * Retrieves the process group ID of the foreground process group associated with the given terminal.
 *
 * @param None
 *
 * @returns The process group ID of the foreground process group associated with the terminal.
 */
pid_t sys_tcgetpgrp(int fd){
    pid_t res;
    if ((res = tcgetpgrp(fd)) == -1){
        print_err_exit("tcgetpgrp", errno);
    }
    return res;
}
pid_t sys_waitpid(pid_t pid, int *status, int options){
    pid_t res;
    if ((res = waitpid(pid, status, options)) == -1){
        print_err_exit("waitpid", errno);
    }
    return res;
}

/**
 * Retrieves the input baud rate of the terminal.
 *
 * @returns The input baud rate of the terminal.
 */
speed_t sys_cfgetispeed (struct termios *termios_p ){
    speed_t res;
    if ((res = cfgetispeed(termios_p)) == (speed_t) -1){
        print_err_exit("cfgetispeed", errno);
    }
    return res;
}

/**
 * Retrieves the output speed of the terminal associated with the file descriptor.
 *
 * @param fd The file descriptor of the terminal.
 *
 * @returns The output speed of the terminal.
 */
speed_t sys_cfgetospeed (struct termios *termios_p){
    speed_t res;
    if ((res = cfgetospeed(termios_p)) == (speed_t) -1){
        print_err_exit("cfgetospeed", errno);
    }
    return res;
}

/**
 * Reads data from a file descriptor into a buffer.
 *
 * @param fd The file descriptor to read from.
 * @param buf The buffer to store the read data.
 * @param count The maximum number of bytes to read.
 *
 * @returns The number of bytes read, or -1 if an error occurred.
 */
ssize_t sys_read(int fd, void *buf, size_t count){
    ssize_t res;
    if ((res = read(fd, buf, count)) == -1){
        print_err_exit("read", errno);
    }
    return res;
}

/**
 * Writes data to the standard output.
 *
 * @param data The data to be written.
 *
 * @returns None
 */
ssize_t sys_write(int fd, const void *buf, size_t count){
    ssize_t res;
    if ((res = write(fd, buf, count)) == -1){
        print_err_exit("write", errno);
    }
    return res;
}

/**
 * Reads the contents of a directory.
 *
 * @param None
 *
 * @returns A list of strings representing the names of the files and directories in the specified directory.
 */
struct dirent *sys_readdir(DIR *dir){
    struct dirent *res;
    if ((res = readdir(dir)) == NULL){
        print_err_exit("readdir", errno);
    }
    return res;
}

/**
 * Retrieves the group entry for a given group ID.
 *
 * @param gid The group ID.
 *
 * @returns A pointer to the group entry structure for the given group ID.
 */
struct group *sys_getgrgid(gid_t gid){
    struct group *res;
    if ((res = getgrgid(gid)) == NULL){
        print_err_exit("getgrgid", errno);
    }
    return res;
}

/**
 * Retrieves the group entry for the given group name.
 *
 * @param name The name of the group.
 *
 * @returns A pointer to the group entry structure if found, NULL otherwise.
 */
struct group *sys_getgrnam(const char *name){
    struct group *res;
    if ((res = getgrnam(name)) == NULL){
        print_err_exit("getgrnam", errno);
    }
    return res;
}

/**
 * Retrieves the password entry for a given username.
 *
 * @param username The username for which to retrieve the password entry.
 *
 * @returns A pointer to the password entry structure for the given username.
 *          If the username is not found, NULL is returned.
 */
struct passwd *sys_getpwnam(const char * name){
    struct passwd *res;
    if ((res = getpwnam(name)) == NULL){
        print_err_exit("getpwnam", errno);
    }
    return res;
}

/**
 * Retrieves the password entry for the current user.
 *
 * @returns A pointer to the password entry for the current user.
 */
struct passwd *sys_getpwuid(uid_t uid){
    struct passwd *res;
    if ((res = getpwuid(uid)) == NULL){
        print_err_exit("getpwuid", errno);
    }
    return res;
}

/**
The `sys_sigaction` function is used to set the action to be taken when a specific signal is received by the process. It allows the process to define a custom handler function for a specific signal.
 */
int sys_sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact){
    int res;
    if ((res = sigaction(sig, act, oact)) == -1){
        print_err_exit("sigaction", errno);
    }
    return res;
}

void sys_exit(int status){
   _exit(status);
}









