# SoalShift_modul4_B04
Pengerjaan Soal Shift Modul 4 Sisop 2019

# Soal Shift Modul 4

## Kelompok B4
#### ***Ersad Ahmad Ishlahuddin***

#### ***05111740000016***

#### ***Philip Antoni Siahaan***

#### ***05111740000111***

### **Nomor 1**

#### Soal

```
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher.  
Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan.  
Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:  

qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0  

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:  
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .  
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

```

#### Pemahaman Soal 1
```
Inti pada soal nomor 1 adalah melakukan dekripsi pada file yang terenkripsi pada direktori asli  
Jadi file dan folder yang ada di direktori mount akan bisa di baca 
```

#### Jawaban

#### Penjelasan
```c
void enkrip(char kata[]){
    if(strcmp(kata, ".") == 0) return;
    if(strcmp(kata, "..") == 0) return;

    int leng = strlen(kata);
    for(int i = 0; i < leng; i++)
    {
        for(int j = 0; j < 94; j++)
        {
            if(kata[i] == cd[j])
            {
                kata[i] = cd[(j+KEY)%94];
                break;
            }
        }
        
    }
    
    printf("%s\n", kata);
}

void dekrip(char kata[]){
    if(strcmp(kata, ".") == 0) return;
    if(strcmp(kata, "..") == 0) return;
    
    int leng = strlen(kata);
    for(int i = 0; i < leng; i++)
    {
        for(int j = 0; j < 94; j++)
        {
            if(kata[i] == cd[j])
            {
                kata[i] = cd[(j+94-KEY)%94];
                break;
            }
        }
        
    }
    
    printf("%s\n", kata);
}
```
```
Fungsi untuk melakukan enkripsi dan dekripsi menggunakan Caesar Cipher dengan beberapa modifikasi  
Akan melakukan return jika string yang masuk adalah . atau ..  
Langkah awal adalah mencari posisi pada list kode yang digunakan  
Kemudian nilai pada posisi tersebut akan diganti sesuai dengan KEY
```
```c
char fpath[1000];
char new[1000];
if(strcmp(path,"/") == 0)
{
    path=dirpath;
    sprintf(fpath,"%s",path);
}
else
{
    memcpy(new, path, sizeof(new));
    enkrip(new);
    sprintf(fpath, "%s%s", dirpath, new);
}
```
```
Saat melakukan pemanggilan fungsi read, readdir, dan getattribute maka juga akan memanggil fungsi enkripsi  
Untuk mengubah path dari file system, agar bisa dioperasi
```

#### Soal 2

```
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”  
Urutan operasi dari kebutuhan ini adalah:
Tepat saat sebelum file system di-mount  
Secara otomatis folder “Videos” terbuat di root directory file system  
Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”  
Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse  
Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount.  
Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut  
Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”  
Jangan tampilkan file pecahan di direktori manapun  
Tepat saat file system akan di-unmount  
Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system  
Hapus folder “Videos” 

```
#### Pemahaman Soal 2

#### Jawaban 2
#### Source Code

### **Nomor 3**

### Soal

```
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus.  
Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
Owner Name     : ‘chipset’ atau ‘ic_controller’
Group Name    : ‘rusak’
Tidak dapat dibaca
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt”  
(format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

```
### Pemahaman Soal 3

```
Inti pada soal nomor 3 adalah menghapus file dengan kriteria tertentu dan melakukan logging pada file yang telah dihapus
```

#### Jawaban 3
#### Penjelasan
```c
struct stat izin;
stat(data, &izin);
struct group *grup = getgrgid(izin.st_gid);
struct passwd *nama = getpwuid(izin.st_uid);

int chipset = strcmp(nama->pw_name, "chipset");
int ic = strcmp(nama->pw_name, "ic_controller");
int rusak = strcmp(grup->gr_name, "rusak");

int readGrup = izin.st_mode & S_IRGRP;
int readOther = izin.st_mode & S_IROTH;
int readUser = izin.st_mode & S_IRUSR;

if((readGrup == 0 || readOther == 0 || readUser == 0) && rusak == 0 && (ic == 0 || chipset == 0))
```
```
Pada saat
Mengambilkan nilai dari user dan grup, serta file yang hanya bisa diread  
Kemudian jika memenuhi syarat maka akan menjalankan proses selanjutnya
```
```c
char alamat[1000];
char miris[] = {"/filemiris.txt"};
FILE * filem;
char waktu[1000];
char buffer[1000];

enkrip(miris);
memcpy(alamat, dirpath, sizeof(alamat));
strcat(alamat,miris);
filem = fopen (alamat, "a+");
strftime(waktu, 15, "%d %H:%M:%S", localtime(&izin.st_ctime));
sprintf(buffer, "Nama : %s\tOID : %d\tGID : %d\tWaktu : %s", new, nama->pw_uid, grup->gr_gid, waktu);
fprintf(filem,"%s\n",buffer);
remove(data);
fclose(filem);
```
```
Membuat file untuk menyimpan log, kemudian memasukkan Nama File ID Owner dan ID Group serta waktu terakhir diakses ke dalam file log
```

### **Nomor 4**

### Soal

```
Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750.  
Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”.  
File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

```
### Pemahaman Soal 4

```
Inti pada soal nomor 4 adalah mengubah permission pada folder dan file yang dibuat pada directory YOUTUBER. Kemudian pada saat membuat file maka nama file akan ditambah dengan .iz1, dan file tersebut tidak bisa diubah permissionnya.
```

#### Jawaban 4
#### Penjelasan
```c
if(strlen(new)>9 && strncmp(new,"/YOUTUBER",9)==0)
{
    enkrip(new);
    sprintf(fpath, "%s%s",dirpath,new);
    res = mkdir(fpath, 0750);	
}
```
```
Pada saat membuat folder maka melakukan pengecekan, jika pada direktori YOUTUBER maka permissionya diubah menjadi 0750
```
```c
if(strlen(new)>9 && strncmp(new,"/YOUTUBER",9)==0)
{
    strcat(new,".iz1");
    enkrip(new);
    sprintf(fpath, "%s%s",dirpath,new);
    res = creat(fpath, 0640);
}
```
```
Pada saat membuat file maka melakukan pengecekan, jika pada direktori YOUTUBER maka nama file akan ditambah dengan ".iz1" dan permissionya diubah menjadi 0640.
```
```c
if(strlen(new)>9 && strncmp(new,"/YOUTUBER",9)==0 && strcmp(new+strlen(new)-4,".iz1")==0)
{
    pid_t child1;
    child1=fork();
    if(child1==0){
        // sprintf(cb,"notify-send 'File ekstensi iz1 tidak boleh diubah permissionnya.'");
        // system(cb);
        execl("/usr/bin/zenity","/usr/bin/zenity","--error","--text=File ekstensi iz1 tidak boleh diubah permissionnya.","--title=Tidak bisa merubah",NULL);
    }
    else{
        wait(NULL);
    }
}
```
```
Tidak bisa mengubah permission file berekstensi ".iz1", memanfaatkan thread dan ditampilkan dengan memanfaatkan zenity
```

### **Nomor 5**

### Soal

```
Ketika mengedit suatu file dan melakukan save,  
maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi.  
Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin,  
kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip  
dan ditaruh ke dalam folder RecycleBin (file asli dan backup terhapus).  
Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss
```

### Pemahaman Soal 5

#### Jawaban 5

### Penjelasan
