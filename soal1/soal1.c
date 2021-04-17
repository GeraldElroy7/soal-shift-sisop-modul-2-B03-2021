#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <errno.h>
#include <dirent.h>

int main() {

    pid_t pid, sid;
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    time_t t = time(NULL);
    struct tm* p_time;

    while (1) {


            /// (No. B)
            c_id = fork();

            if (c_id == 0) {
                execl("/bin/wget", "wget", "-q", "--no-check-certificate", "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"
                      , "-O", "fotoForStevany.zip", NULL);
            } else {
                wait(NULL);
            }

            c_id = fork();

            if (c_id == 0) {
                execl("/bin/wget", "wget", "-q", "--no-check-certificate", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"
                      , "-O", "fotoForStevany.zip", NULL);
            } else {
                wait(NULL);
            }

            ///(No. C)

            c_id = fork();

            if (c_id == 0) {
                execl("/bin/unzip", "unzip", "-q", "fotoForStevany.zip", NULL);
            } else {

            }




            // Extract zip filmForStevany.zip (No. C) (sama kayak fotoForStevany cuma beda file)

            c_id = fork();

            if (c_id == 0) {
                execl("/bin/unzip", "unzip", "-q", "filmForStevany.zip", NULL);
            } else {
                while ((wait(&status)) > 0);
            }


        sleep(1);
    }
}
