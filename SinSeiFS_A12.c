#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

int x = 0;

static  const  char *dirpath = "/home/kelvin/Downloads";

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    if (x != 24) x++;
	else logging("READDIR", fpath);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    logging("READ", path);

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}


int check(char string1[], char string2[]){
    int j=0;
    for(int i=0; i<strlen(string1); i++){
        if(string1[i]==string2[j]) j++;
        else j=0;
        if(j==strlen(string2)) return 1;
    }
    return 0;
}



static int xmp_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

	int res;
	res = mkdir(fpath, mode);
    logging("MKDIR", fpath);
	if (res == -1)
		return -errno;
    char newname[2000];
    char filename[1000];
    sprintf(filename,"%s",path);
    char prefix[10]="/AtoZ_";
    if(check(fpath,prefix)==1){
        char nama[1000];
        sprintf(nama,"%s",filename);
        for(int i=0;nama[i]!='\0';i++){
            if(nama[i]>='A'&&nama[i]<='Z') nama[i]='Z'-nama[i]+'A';
            if(nama[i]>='a'&&nama[i]<='z') nama[i]='z'-nama[i]+'a';
        }
        sprintf(newname,"%s%s",dirpath,nama);
        printf("%s\n",fpath);
        printf("%s\n",newname);
        rename(fpath, newname); 
    }
	return 0;
}



static int xmp_rename(const char *from, const char *to)
{
    char fpath[1000];
    char tpath[1000];
    if(strcmp(from,"/") == 0)
    {
        from=dirpath;

        sprintf(fpath,"%s",from);
        sprintf(tpath,"%s",to);
    }
    else
    {
        sprintf(fpath, "%s%s",dirpath,from);
        sprintf(tpath, "%s%s",dirpath,to);
    } 
	int res;
	res = rename(fpath, tpath);
	if (res == -1)
		return -errno;
    DIR *dir;
    struct dirent *ent;
    char oldname[2000], newname[2000];
    char prefix[10]="/AtoZ_";
    if(check(tpath,prefix)==1){
        if ((dir = opendir (tpath)) != NULL) 
        {
            while ((ent = readdir (dir)) != NULL) 
            {
                char nama[1000];
                sprintf(nama,"%s",ent->d_name);
                for(int i=0;nama[i]!='\0';i++){
                    if(nama[i]>='A'&&nama[i]<='Z') nama[i]='Z'-nama[i]+'A';
                    if(nama[i]>='a'&&nama[i]<='z') nama[i]='z'-nama[i]+'a';
                }
                sprintf(oldname,"%s/%s",tpath,ent->d_name);
                sprintf(newname,"%s/%s",tpath,nama);
                rename(oldname, newname);         
            }
        closedir (dir);
        } 
    }
    return 0;
}

static int xmp_unlink(const char *path)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{

    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
	int res;

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

void logging(char *nama, char *fpath)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char text[1000];

	FILE *file;
	file = fopen("/home/kelvin/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(text, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);
	else
		sprintf(text, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);

	fputs(text, file);
	fclose(file);
	return;
}

void logging2(char *nama, char *fpath, char *ke)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char text[1000];

	FILE *file;
	file = fopen("/home/kelvin/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(text, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath, ke);
	else
		sprintf(text, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath, ke);

	fputs(text, file);
	fclose(file);
	return;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
};



int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
