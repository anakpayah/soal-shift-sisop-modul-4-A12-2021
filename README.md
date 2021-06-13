# soal-shift-sisop-modul-4-A12-2021
Praktikum Sisop Modul 4 Semester genap 2021

<hr>

## Soal nomor 1
### Penjelasan Soal
Untuk soal nomor 1, kita perlu membuat metode encoding dimana jika terdapat file atau folder di dalam folder ter-encode, maka nama file tersebut harus ter-encode dengan menggunakan atbash cipher, yaitu alfabet dipetakan ke kebalikannya (a menjadi z).  
### Soal 1a
Untuk soal 1a, jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.  
### Kendala
```c
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
```
Dengan kode yang sudah dibuat ini, file dengan awalan "AtoZ_" akan langsung diencode padahal seharusnya file atau folder dalam folder tersebut yang seharusnya diencode. Disini, fpath dari file yang dibuat dicek jika memiliki awalah "AtoZ_" dan jika iya, maka setiap alfabet dipetakan mnejadi kebalikannya dan nama folder tersebut direname.  
![image](https://user-images.githubusercontent.com/7587945/121809512-40845e00-cc87-11eb-88c1-6777e746e7c5.png)
![image](https://user-images.githubusercontent.com/7587945/121809530-4e39e380-cc87-11eb-96a9-3248e293c98a.png)
### Soal 1b
