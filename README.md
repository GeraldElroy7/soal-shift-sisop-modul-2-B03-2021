# soal-shift-sisop-modul-2-B03-2021

### Anggota Kelompok
1. Nur Putra Khanafi     05111940000020
2. Gerald Elroy Van Ryan 05111940000187
3. Husnan                05111940007002

## Soal 1

Pada soal 2, *user* diminta untuk membantu Steven memberikan suprise pada Stevany lewat program Daemon dalam membuat folder, mendownload, unzip, remove dan zip.

Soal ini dikerjakan menggunakan `fork` dan `execl` agar bisa menjalankan banyak proses secara bersamaan. Main menunya adalah sebagai berikut:

```c
#define COUNT_DIR 3

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
```

- melakukan fork() pada child_id dan set sid
- mengubah permission file dengan umask(0)
- menyembunyikan program(berjalan sebagai background)
- menjalankan program Daemon utama

Untuk Program Daemon sendiri berfungsi untuk menjalankan fungsi-fungsi pengolahan file dan folder serta mengolah waktu terkait ulang tahun stevany. 

![prea](https://user-images.githubusercontent.com/65794806/115995419-b10ae900-a60d-11eb-9929-4b999c1975c0.png)

Untuk mengambil waktu menggunakan tipe `time_t` dan `struct *tm` yang mana akan mendapatkan local time yang berupa bulan, hari, jam, menit, dan detik.
Action pertama akan berjalan saat 6 jam sebelum ulang tahun Stevany(jam 16.22) dengan syarat jamnya,
```c
jam_ultah - 6 == p_time->tm_hour
```

Action kedua akan berjalan saat ulang tahun Stevany(jam 22.22) dengan syarat jamnya,
```c
jam_ultah == p_time->tm_hour
```

### Cara Pengerjaan 1A

```c
///Subsoal a (membuat 3 folder)
void makeFolders(pid_t c_id, char *stevany_folder[]) {
    c_id = fork();
    if(c_id == 0) {
        execl("/bin/mkdir", "mkdir", "-p", stevany_folder[0], stevany_folder[1], stevany_folder[2], NULL);
    }
    wait(NULL);
}
```

Untuk membuat folder "Musyik", "Fylm" dan "Pyoto", menggunakan sumber variabel `stevany_folder` dan argumen asal `/bin/mkdir`, setelah itu di jeda `wait()`.

### Cara Pengerjaan 1B

```c
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
```

Untuk mendownload FileRAR, menggunakan link yang disimpan pada `download_links` kemudia men-loop sebanyak 3 kali sesuai dengan jumlah folder yang dibuat sebelumnya lalu menggunakan argumen asal `/usr/bin/wget` dengan tambahan `--no-check-certificate`, setelah itu di jeda `wait()`.

### Cara Pengerjaan 1C

```c
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
```

Untuk meng-ekstrak FileRAR, menggunakan variabel `zip_name` sesuai dengan 3 nama zip file yang telah didownload setelah itu di unzip dengan argumen asal `/usr/bin/unzip`, kemudian di jeda `wait()`.

### Cara Pengerjaan 1D

```c
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
```

Untuk memindahkan hasil dari ekstak sendiri harus mengetahui letak folder yang dituju, sehingga menggunakan variabel `dir_name` yang akan di buka dengan `opendir(dir_name)`, setelah itu memproses fungsi `moveDonwloadedFiles()`.

Pada fungsi kedua, nama folder dicompare terlebih dahulu, karena jika tidak lokasinya tidak berdampingan dengan lokasi zip file, maka akan `exit()`. Jika folder tidak berdala diluar posisi, maka akan diambil nama folder yang akan dijadikan sebagai `path` untuk menjalankan argumen asal `/bin/mv`, kemudian di jeda `wait()`.

Terakhir, folder yang isinya telah dipindah(folder kosong) akan dihapus sesuai namanya dari variabel `dir_name` dengan argumen asal  `/bin/rm`, setelah itu di jeda `wait()`.

### Cara Pengerjaan 1E

```c
///Subsoal e (Action 1, 6 jam sebelum ultah)
void makePreUltah(pid_t c_id, char *stevany_folder[], char *zip_name[]) {

    char *dir_name[] = {"MUSIK", "FILM", "FOTO"};

    makeFolders(c_id, stevany_folder);
    downloadFileRAR(c_id, zip_name);
    unzipFileRAR(c_id, zip_name);

    for(int i=0; i<COUNT_DIR; i++)
        findFolderPos(dir_name[i], stevany_folder[i]);

    removeZipFolder(c_id, dir_name);
}
```

Pada action 1, program akan menjalankan fungsi:
1. `makeFolders()`
2. `downloadFileRAR()`
3. `unzipFileRAR()`
4. `findFolderPos()`
5. `removeZipFolder()`

### Cara Pengerjaan 1F

```c
///Subsoal f (zip semua folder, saat ultah Stevany)
void zipFolders(pid_t c_id, char *stevany_folder[], char zip_name[]) {
    c_id = fork();
    if(c_id == 0) {
        execl("/usr/bin/zip", "zip", "-rmvq", zip_name, stevany_folder[0], stevany_folder[1], stevany_folder[2], NULL);
    }
    wait(NULL);
}
```
Pada action 2, program akan menjalankan fungsi `removeZipFolder()` dan `zipFolders()` yang berada pada Program Daemon. Untuk zip menggunakan argumen asal `/usr/bin/zip`, kemudian di jeda `wait()`.

### Output

1. Isi Halaman Folder Sebelum Program Dijalankan
![no1a](https://user-images.githubusercontent.com/65794806/115996295-4c518d80-a611-11eb-9974-bc5750585053.png)

2. Hasil Output ketika Waktunya 6 Jam Sebelum Ulang Tahun Stevany
![no1b](https://user-images.githubusercontent.com/65794806/115996352-786d0e80-a611-11eb-8ea1-534ec600a636.png)

3. Hasil Output ketika Waktunya Ulang Tahun Stevany
![no1c](https://user-images.githubusercontent.com/65794806/115996373-8753c100-a611-11eb-9b76-aedad87938c1.png)

### Kendala Selama Pengerjaan

1. Awalnya masih kurang paham tentang cara menjalankan proses background dengan fork() yang berada pada setiap fungsi, sehingga mencari referensi lebih banyak terlebih dulu.

## Soal 2

Pada soal 2, *user* diminta untuk membantu Loba mengategorikan foto-foto peliharaannya yang sudah ada pada zip. Ada beberapa kriteria yang terbagi dalam beberapa sub soal.

Soal ini dikerjakan menggunakan `fork` dan `execv` agar bisa menjalankan banyak proses secara bersamaan. Global variabel yang akan digunakan adalah:

```c
char download[] = "/home/frozz/Downloads/pets.zip";
char loc[] = "/home/frozz/modul2/petshop";
char keterangan[100] = "";
```

- download[ ] : Menyimpan lokasi pets.zip berada
- loc[ ] : Menyimpan lokasi folder petshop yang nantinya menjadi tempat menyimpan foto-foto peliharaan
- keterangan[ ] : Akan dipakai ketika membuat keterangan.txt

### Cara Pengerjaan 2A

![carbon (2)](https://user-images.githubusercontent.com/64303057/115360716-187a0080-a1ea-11eb-882f-2ce215845221.png)

Setiap proses yang dibuat akan diinisiasi terlebih dulu dengan `fork()`.

Sebelum *unzip* **Pets.zip**, buat terlebih dahulu folder untuk menyimpan foto-foto tersebut. Foto akan disimpan di **loc**. Lalu, memakai argumen `mkdir` agar bisa membuat direktori baru. `-p` ditambahkan pada argumen agar langsung membuat keseluruhan direktori tanpa memanggil banyak `mkdir`.

Setelah selesai membuat direktori, lakukan proses baru dengan `fork` kembali. Pets.zip yang sudah didownload akan di-*unzip* menggunakan `execv(unzip...)`. Lokasi zip sudah disimpan ke variabel `download`, cukup panggil variabelnya agar simpel. 

Tidak semua item perlu untuk di-*unzip*. Untuk menghindari *unzip* pada item/folder yang tidak diinginkan, maka tambahkan `-x`, agar program meng-*exclude*-kan folder yang tidak diperlukan. 

Setelah item yang diinginkan berhasil di-*unzip*, panggil `listFilesRecursively(loc)` untuk melakukan travers rekursif terhadap direktori `loc` untuk melanjutkan sub-soal selanjutnya.

### Cara Pengerjaan 2B (Untuk yang fotonya hanya ada satu hewan)

![carbon (3)](https://user-images.githubusercontent.com/64303057/115986381-1349f680-a5da-11eb-9440-f7615e44aed3.png)

Sebelumnya,  `listFilesRecursively()` akan dijalankan sesuai dengan yang ada pada modul, seperti memakai `struct dirent` dan `while ((dp = readdir(dir)) != NULL)` agar travers file dapat berjalan terus. Program akan mengambil string dari nama file hingga bertemu tanda ";" pertama, lalu lakukan `mkdir` agar membuat folder sesuai dengan jenis hewannya.

### Cara Pengerjaan 2C (Untuk yang fotonya hanya ada satu hewan)

![carbon (4)](https://user-images.githubusercontent.com/64303057/115987123-e5ff4780-a5dd-11eb-8fb9-ad9fde573397.png)

Untuk 2C, user diminta untuk bisa memindahkan tiap foto ke folder sesuai dengan jenis hewannya. Awalnya, inisiasikan dulu dua char yang akan menyimpan alamat.

```c
char mfolder[100] = "/home/frozz/modul2/petshop/";
strcat(mfolder, dp->d_name);
```

**mfolder** akan berfungsi untuk menyimpan alamat direktori petshop, ditambah dengan nama folder untuk tiap hewan yang baru saja dibuat. 

```c
char petfile[100] = "";
strcpy(petfile, dp->d_name);
```
Lalu, ada **petfile** yang akan menyimpan nama file dari setiap foto yang nanti akan diiterasi.

Cara pengerjaan soal nomor 2 ini akan dipisah, satu untuk foto yang hewannya hanya  ada satu dan foto yang hewannya ada dua. Kedua hal ini dibedakan oleh nama file-nya, di mana jika ada dua hewan, maka nama filenya akan mengandung simbol "_".

Untuk yang fotonya hanya satu hewan, maka jalankan permisalan : 
```c
if (!(strstr(dp->d_name, delimiter)))
```

`Delimiter` ini berisikan karakter "_".

Jadi, **if** ini akan berjalan jika ada file yang **tidak** memiliki _

Lakukan command `mv` untuk memindahkan foto ke folder yang sesuai dengan jenisnya.

### Lanjutan 2C (Mengganti nama file menjadi namapeliharaan.jpg)

![carbon (5)](https://user-images.githubusercontent.com/64303057/115989697-c66e1c00-a5e9-11eb-8c57-6015b1677051.png)

Buatlah string rm yang akan menyimpan nama peliharaan lalu menambahkannya dengan alamat direktori. Lalu lakukan proses baru, agar memindahkan foto-foto tersebut sesuai dengan folder yang sesuai.

### Cara Pengerjaan 2E (Untuk yang fotonya hanya ada satu hewan)

![carbon (6)](https://user-images.githubusercontent.com/64303057/115989940-ff5ac080-a5ea-11eb-83cf-306db6190d88.png)

Nama hewan akan disimpan di char nama_hewan[]. Awalnya, `keterangan.txt` akan di-printkan sebuah tulisan, dan akan "dibuka" terlebih dulu dengan fungsi `fopen(...,"a")`, di mana a ini berfungsi untuk *append* setiap kata yang ada ke `keterangan.txt`.

Inisiasi dulu check = 2, lalu lakukan perulangan. Setiap kali program menemukan ";" pada nama file (`a[0]`), maka nilai check akan berkurang, dan print **nama :**, sesuai dengan yang diminta pada soal. Jika sudah, maka akan di-*continue* langsung ke bawahnya.

Begitu juga jika check sudah bernilai 1, periksa apakah program menemui karakter ";" lagi. Jika iya, maka print **umur :** pada akhir keterangan.txt.

Dan untuk yang terakhir, jika sudah menemui akhiran ".jpg", maka print **tahun :** pada akhir txt. 

Jika semua kondisi ini belum terpenuhi, maka print nama hewan dan data lainnya ke keterangan.txt hingga perulangan telah selesai.

### Cara Pengerjaan 2B (Untuk yang fotonya ada dua hewan)

![carbon (7)](https://user-images.githubusercontent.com/64303057/115990499-c7a14800-a5ed-11eb-9cce-cd0ec9a8587f.png)

Lalu, untuk foto yang hewannya ada dua, cara penyelesaiannya juga hampir sama. Untuk pertama, jika ditemukan adanya karakter ";" pada nama filenya, maka buatlah folder sesuai dengan karakter yang sudah diambil (jenis hewan) dengan command `mkdir`.


### Cara Pengerjaan 2B (Untuk yang fotonya ada dua hewan)

![carbon (8)](https://user-images.githubusercontent.com/64303057/115991062-bf96d780-a5f0-11eb-90e3-7a3c993e3c89.png)

Pada proses ini, dibuatkan sebuah char `keterangan_new[100]` yang akan menyimpan alamat dari direktori ditambah dengan keterangan.txt, lalu lakukan `fopen` agar dapat menambahkan teks pada keterangan.txt.

```c
strncat(temp, &petfile[j], 1);
```

Ini yang akan dilakukan program jika belum menemuka karakter ";" pada nama file. Setiap char pada petfile akan dimasukkan ke variabel `temp` satu per satu.

### Cara Pengerjaan 2C (Untuk yang fotonya ada dua hewan)

![carbon (9)](https://user-images.githubusercontent.com/64303057/115991439-9aa36400-a5f2-11eb-97a7-092cdab50831.png)

Ketika count2 sudah menjadi 1, maka lakukan lagi permisalan. Jika program kembali menemukan ";" lainnya, maka nama file tersebut akan diganti namanya dan ditambahkan ".jpg", dan pindahkan foto hewan tersebut ke folder yang sesuai dengan jenisnya.

Untuk langkah berikutnya, prosesnya hampir sama dengan cara yang foto hewannya ada satu, lakukan terus pengulangan hingga bertemu akhiran ".jpg", lalu tambahkan teks "tahun" pada akhir keterangan.txt. 

Lalu, lakukan `closedir(dir)` untuk mengakhiri travers direktori.

### Output

1. Direktori modul2/petshop sudah terbentuk beserta folder-folder untuk tiap jenis hewan
![Screenshot 2021-04-25 183559](https://user-images.githubusercontent.com/64303057/115991930-28804e80-a5f5-11eb-826e-d6164a9d340f.png)

2. Setiap foto sudah menempati folder yang sesuai, contohnya seperti foto-foto anjing yang masuk pada folder "dog". Nama file juga sudah diubah menjadi "namahewan.jpg".
![Screenshot 2021-04-25 183805](https://user-images.githubusercontent.com/64303057/115992016-9cbaf200-a5f5-11eb-8961-4f06769e262c.png)

3. Contoh isi dari keterangan.txt pada folder "dog"
![Screenshot 2021-04-25 183823](https://user-images.githubusercontent.com/64303057/115992028-aba1a480-a5f5-11eb-9d3a-4ec00d1a3054.png)

### Kendala Selama Pengerjaan

1. Awalnya masih kurang paham tentang cara menjalankan proses yang banyak dalam satu file (fork), sehingga mencari referensi lebih banyak terlebih dulu.
2. Kesulitan untuk memisahkan foto yang ada dua hewan untuk diletakkan pada folder, terutama yang hewannya berbeda.
3. Sempat melakukan revisi lagi karena lupa memanggil salah satu fork(), yang menyebabkan sebagian program jadi tidak berjalan.


