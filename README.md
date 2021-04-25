# soal-shift-sisop-modul-2-B03-2021

### Anggota Kelompok
1. Nur Putra Khanafi     05111940000020
2. Gerald Elroy Van Ryan 05111940000187
3. Husnan                05111940007002

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


