#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>

#define COUNT_DIR 3

void daemonProgram();
bool isNowUltah(int, int, int, int);

void makePreUltah(pid_t, char *argv[], char *argv2[]);
void makeFolders(pid_t, char *argv[]);
void downloadFileRAR(pid_t, char *argv[]);
void unzipFileRAR(pid_t, char *argv[]);

void findFolderPos(char[], char[]);
void moveDonwloadedFiles(struct dirent*, char[], char[]);
void removeZipFolder(pid_t, char *argv[]);

void zipFolders(pid_t, char *argv[], char[]);

int main() {
    pid_t c_id, sid;

    c_id = fork();
    if(c_id > 0)
        exit(EXIT_SUCCESS);

    sid = setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    umask(0);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    daemonProgram();
    return 0;
}

void daemonProgram() {
    pid_t c_id;
    char *stevany_folder[] = {"Musyik", "Fylm", "Pyoto"};
    char *zip_name[] = {"Musik_for_Stevany.zip", "Film_for_Stevany.zip", "Foto_for_Stevany.zip"};
    int jam_ultah = 22;
    time_t curr_time;

    while (1) {
        curr_time = time(NULL);
        struct tm * p_time = localtime(&curr_time);

        if(isNowUltah(p_time->tm_mday, p_time->tm_mon, p_time->tm_min, p_time->tm_sec) &&
            jam_ultah - 6 == p_time->tm_hour){

            makePreUltah(c_id, stevany_folder, zip_name);
        }
        else if(isNowUltah(p_time->tm_mday, p_time->tm_mon, p_time->tm_min, p_time->tm_sec) &&
            jam_ultah == p_time->tm_hour){
            
            removeZipFolder(c_id, zip_name);
            zipFolders(c_id, stevany_folder, "Lopyu_Stevany.zip");
        }
        wait(NULL);

        sleep(1);
    }
}
bool isNowUltah(int day, int month, int minute, int second) {
    int hari_ultah = 9, bulan_ultah = 3, menit_ultah = 22;

    return hari_ultah == day && bulan_ultah == month &&
           menit_ultah == minute && second == 0;
}

///Subsoal e (6 jam sebelum ultah)
void makePreUltah(pid_t c_id, char *stevany_folder[], char *zip_name[]) {

    char *dir_name[] = {"MUSIK", "FILM", "FOTO"};

    makeFolders(c_id, stevany_folder);
    downloadFileRAR(c_id, zip_name);
    unzipFileRAR(c_id, zip_name);

    for(int i=0; i<COUNT_DIR; i++)
        findFolderPos(dir_name[i], stevany_folder[i]);

    removeZipFolder(c_id, dir_name);
}
///Subsoal a (membuat 3 folder)
void makeFolders(pid_t c_id, char *stevany_folder[]) {
    c_id = fork();
    if(c_id == 0) {
        execl("/bin/mkdir", "mkdir", "-p", stevany_folder[0], stevany_folder[1], stevany_folder[2], NULL);
    }
    wait(NULL);
}
///Subsoal b (mendownload 3 zip file)
void downloadFileRAR(pid_t c_id, char *zip_name[]) {
    char *download_links[] = {
        "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
        "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};

    for(int i=0; i<COUNT_DIR; i++) {
        c_id = fork();
        if(c_id == 0){
            execl("/usr/bin/wget", "wget", "--no-check-certificate", download_links[i], "-O", zip_name[i], NULL);
        }
        wait(NULL);
    }
}
///Subsoal c (unzip 3 zip file)
void unzipFileRAR(pid_t c_id, char *zip_name[]) {
    for(int i=0; i<COUNT_DIR; i++) {
        c_id = fork();
        if(c_id  == 0){
            execl("/usr/bin/unzip", "unzip", "-qq", zip_name[i], NULL);
        }
        wait(NULL);
    }
}
///Subsoal d (memindahkan file dalam zip dan menghapus folder dari zip)
void findFolderPos(char dir_name[], char stevany_folder[]) {
    DIR *dir = opendir(dir_name);
    struct dirent *p_dir;
    if (dir != NULL) {
        while ((p_dir = readdir(dir))){
            moveDonwloadedFiles(p_dir, dir_name, stevany_folder);
        }
        (void) closedir (dir);
    } else perror ("Directory not found");
}
void moveDonwloadedFiles(struct dirent * p_dir, char dir_name[], char stevany_folder[]) {
    pid_t c_id;
    c_id = fork();
    if(c_id == 0 && (strcmp(p_dir->d_name, ".") == 0 || strcmp(p_dir->d_name, "..") == 0))
        exit(EXIT_SUCCESS);
    if(c_id == 0) {
        char path[256];
        strcpy(path, dir_name);
        strcat(path, "/");
        strcat(path, p_dir->d_name);

        execl("/bin/mv", "mv", path, stevany_folder, NULL);
    }
    wait(NULL);
}
void removeZipFolder(pid_t c_id, char *dir_name[]) {
    c_id = fork();
    if(c_id == 0) {
        execl("/bin/rm", "rm", "-rf", dir_name[0], dir_name[1], dir_name[2], NULL);
    }
    wait(NULL);
}

///Subsoal f (zip semua folder)
void zipFolders(pid_t c_id, char *stevany_folder[], char zip_name[]) {
    c_id = fork();
    if(c_id == 0) {
        execl("/usr/bin/zip", "zip", "-rmvq", zip_name, stevany_folder[0], stevany_folder[1], stevany_folder[2], NULL);
    }
    wait(NULL);
}

