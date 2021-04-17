#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

void getArg(int);
void make_directory(char *);
void downloadPhotos(char *, char *);
void makeKillersh(char *argv[]);

void makeStatustxt(char *);
char *getCaesarCypher(int, char *);
void zipDirectory(char *);

char *getTimeName();

int main(int argc, char *argv[]) {

    getArg(argc);

    pid_t pid, sid;
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    sid = setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    makeKillersh(argv);

    time_t curr_time;

    while (1) {
        curr_time = time(NULL);

        pid_t c_id;
        c_id = fork();

        char *dir_name = getTimeName();
        if (c_id < 0){
            exit(EXIT_FAILURE);
        }
        if (c_id == 0) {
            if (fork() == 0) {
                make_directory(dir_name);
            }
            else {
                wait(NULL);
                    for (int i = 0; i < 10; i++) {
                        if (fork() == 0) {

                            char *linkinet = malloc(32 * sizeof(char));
                            int size_jpg = ((int)time(NULL) % 1000) + 50;
                            sprintf(linkinet, "https://picsum.photos/%d/%d", size_jpg, size_jpg);

                            downloadPhotos(linkinet, dir_name);

                        }
                        sleep(5);
                    }

                    makeStatustxt(dir_name);
                    zipDirectory(dir_name);
            }
    }
    else
      sleep(40);
  }
}

void getArg(int argc){
        if(argc != 2){
            puts("Argument tidak ada");
            exit(EXIT_FAILURE);
        }
}

/// Subsoal A (membuat folder)
void make_directory(char *location){
        //printf("Creating Directory -> %s\n", location);
        execl("/bin/mkdir", "mkdir", location, NULL);
}

/// Subsoal B (mendownload foto tiap 5 detik)
void downloadPhotos(char *linkinet, char *location){

    char *p_loc = malloc(32 * sizeof(char));
    sprintf(p_loc, "%s/%s.jpg", location, getTimeName());

    execl("/usr/bin/wget", "wget", "-q", linkinet, "-O", p_loc, NULL);

    free(location);
}

/// Subsoal C (membuat status.txt)
void makeStatustxt(char *file_name){
    char *c_result = getCaesarCypher(5, "Download Success");
    char *text = malloc(32 * sizeof(char));
	char *location = malloc(32 * sizeof(char));

	sprintf(text, "%s", c_result);
	free(c_result);

	sprintf(location, "%s/status.txt", file_name);
	FILE *statusfile = fopen(location, "w");

	if (statusfile){
		fprintf(statusfile, "%s\n", text);
		fclose(statusfile);
	}
	free(location);
    free(text);
}
/// Caesar Cypher Func
char *getCaesarCypher(int shift, char *text){
	int length = strlen(text);
	char *res_text;

	res_text = malloc(length * sizeof(char));
    int i = 0;
	while(i < length){
        if (text[i] >= 'a' && text[i] <= 'z')
			res_text[i] = 'a' + (text[i] + shift - 'a') % 26;
		else if (text[i] >= 'A' && text[i]+shift <= 'Z')
			res_text[i] = 'A' + (text[i] + shift - 'A') % 26;
		else
			res_text[i] = text[i];

		i++;
	}
	return res_text;
}
/// Subsoal C (menZIP folder)
void zipDirectory(char *file_name){
	char *zip_name = malloc(32 * sizeof(char));
	char *location = malloc(32 * sizeof(char));

	sprintf(zip_name, "%s.zip" ,file_name);
	sprintf(location, "./%s" ,file_name);

    printf("Making Zip File -> %s from %s\n", zip_name, location);
	execl("/usr/bin/zip", "zip", "-q", "-rm", zip_name, location, NULL);

	free(zip_name);
	free(location);
}

/// Subsoal E dan D(inisialisasi mode)
void makeKillersh(char *argv[])
{
    FILE *killer_file = fopen("./killer.sh", "w");

    if(strcmp(argv[1], "-z") == 0)
        fprintf(killer_file, "#!/bin/bash\nkill -9 -%d\nrm Killer", getpid());

    if(strcmp(argv[1], "-x") == 0)
        fprintf(killer_file, "#!/bin/bash\nkill %d\nrm Killer", getpid());

    if(fork() == 0) {
        if (fork() == 0) {
            char *argv[] = {"chmod", "u+x", "killer.sh", NULL};
            execv("/bin/chmod", argv);
        }
        else  {
            wait(NULL);
            char *argv[] = {"mv", "killer.sh", "Killer", NULL};
            execv("/bin/mv", argv);
        }
    }
    fclose(killer_file);
}

char *getTimeName(){
    char *name = malloc(32 * sizeof(char));
	time_t curr_time = time(NULL);
	struct tm* p_time = localtime(&curr_time);

	strftime(name, 32, "%Y-%m-%d_%H:%M:%S", p_time);
	return name;
}
