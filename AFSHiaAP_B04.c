#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <grp.h>
#include <pwd.h>

#define MAX_PATH 299
#define KEY 17

const char cd[]= {"qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV\']jcp5JZ&Xl|\\8s;g<{3.u*W-0"};
static const char *dirpath = "/home/aeris/shift4";

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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
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
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
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
	int res = 0;

	DIR *dp;
	struct dirent *de;


	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));

        char data[1000];
        memcpy(data, fpath, sizeof(data));
        strcat(data, "/");
        strcat(data, de->d_name);

        memcpy(new, de->d_name, sizeof(new));
        dekrip(new);
        
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

        if((readGrup == 0 || readOther == 0 || readUser == 0) && rusak == 0 && (ic == 0 || chipset == 0)){
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
        }
        else {
            st.st_ino = de->d_ino;
            st.st_mode = de->d_type << 12;
            res = (filler(buf, new, &st, 0));
                if(res!=0) break;
        }
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
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
	int res = 0;
    int fd = 0;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char new[1000];
	sprintf(new,"%s",path);
	if(strlen(new)>9 && strncmp(new,"/YOUTUBER",9)==0)
	{
		enkrip(new);
		sprintf(fpath, "%s%s",dirpath,new);
		res = mkdir(fpath, 0750);	
	}
	else{
    	enkrip(new);
		sprintf(fpath, "%s%s",dirpath,new);
		res = mkdir(fpath, mode);
		if (res == -1)
			return -errno;
	}
	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char new[1000];
    // char cb[1000];
	sprintf(new,"%s",path);
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
	else{
    	enkrip(new);
		sprintf(fpath, "%s%s",dirpath,new);
		res = chmod(fpath, mode);
	}
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
    enkrip(name);
	sprintf(fpath, "%s%s",dirpath,name);
	res = lchown(fpath, uid, gid);
	if (res == -1) 
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	char fpath[1000];
    char new[1000];
	sprintf(new,"%s",path);
    enkrip(new);
	sprintf(fpath, "%s%s",dirpath,new);
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
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
	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{

    (void) fi;
    char fpath[1000];
    char new[1000];
	sprintf(new,"%s",path);
    int res;
	if(strlen(new)>9 && strncmp(new,"/YOUTUBER",9)==0)
	{
		strcat(new,".iz1");
		enkrip(new);
		sprintf(fpath, "%s%s",dirpath,new);
    	res = creat(fpath, 0640);
	}
	else{
    	enkrip(new);
		sprintf(fpath, "%s%s",dirpath,new);
    	res = creat(fpath, mode);
	}
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;

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

	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
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
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mkdir      = xmp_mkdir,
    .write      = xmp_write,
    .create     = xmp_create,
    .utimens    = xmp_utimens,
    .truncate   = xmp_truncate,
    .chmod		= xmp_chmod,
    .chown		= xmp_chown,
    .mknod		= xmp_mknod,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
