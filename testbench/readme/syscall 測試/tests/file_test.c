
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utime.h>
#include <utime.h>
#include <string.h>
#include <sys/stat.h>


int main() {
    FILE *file;
    FILE *file2;

    file = fopen("test.txt", "w+");  
    file2 = fopen("test2.txt", "w+");  
    
    char *buffer = "abcdef!?? \ngh*IJK()\nlmn\nopqr#$%";
    char *buffer2 = "dfsd\nsadjas\nsss\n\nksdjaskd\n";
    
    fwrite(buffer2,1,strlen(buffer2), file2);
    fwrite(buffer,1,strlen(buffer), file);
    
    fflush(file);
    fflush(file2);
    fseek(file,0,SEEK_SET);
    fseek(file2,0,SEEK_SET);
    
    if (file == NULL) {
        printf("無法開啟檔案！\n");
        exit(1);
    }
    if (file2 == NULL) {
        printf("無法開啟檔案！\n");
        exit(1); 
    }
 
    int ch; //不能用char  
    ch = fgetc(file2);
    printf("%d\n", ch);
    while ((ch = fgetc(file2)) != EOF) {
        putchar(ch);
    }

    printf("\n");    
    
    char b[100] = {0};
    while(fgets(b,sizeof(b), file) != NULL){
    	printf("%s", b);
    }

    /*
    struct stat fileStat;
    printf("\n");
    fstat(fileno(file), &fileStat);
    printf("---------------------------\n");
    printf("Device: \t\t%ld\n", (long)fileStat.st_dev);
    printf("Inode: \t\t\t%ld\n", (long)fileStat.st_ino);
    printf("Mode: \t\t\t%o (octal)\n", (unsigned int)fileStat.st_mode);
    printf("Number of Links: \t%ld\n", (long)fileStat.st_nlink);
    printf("User ID: \t\t%ld\n", (long)fileStat.st_uid);
    printf("Group ID: \t\t%ld\n", (long)fileStat.st_gid);
    printf("Device ID (if special file): \t%ld\n", (long)fileStat.st_rdev);
    printf("Total Size: \t\t%ld bytes\n", (long)fileStat.st_size);
    printf("Block Size: \t\t%ld bytes\n", (long)fileStat.st_blksize);
    printf("Number of Blocks: \t%ld\n", (long)fileStat.st_blocks);
    printf("Last Access Time: \t%s", ctime(&fileStat.st_atim.tv_sec));
    printf("Last Modification Time: \t%s", ctime(&fileStat.st_mtim.tv_sec));
    printf("Last Status Change Time: \t%s", ctime(&fileStat.st_ctim.tv_sec));
    
    
printf("Offset of st_dev: %ld\n", (long)offsetof(struct stat, st_dev));
    printf("Offset of st_ino: %ld\n", (long)offsetof(struct stat, st_ino));
    printf("Offset of st_mode: %ld\n", (long)offsetof(struct stat, st_mode));
    printf("Offset of st_nlink: %ld\n", (long)offsetof(struct stat, st_nlink));
    printf("Offset of st_uid: %ld\n", (long)offsetof(struct stat, st_uid));
    printf("Offset of st_gid: %ld\n", (long)offsetof(struct stat, st_gid));
    printf("Offset of st_rdev: %ld\n", (long)offsetof(struct stat, st_rdev));
    printf("Offset of st_size: %ld\n", (long)offsetof(struct stat, st_size));
    // 对于条件编译的部分，确保它们在你的环境中有效
    printf("Offset of st_atim: %ld\n", (long)offsetof(struct stat, st_atim));
    printf("Offset of st_mtim: %ld\n", (long)offsetof(struct stat, st_mtim));
    printf("Offset of st_ctim: %ld\n", (long)offsetof(struct stat, st_ctim));
    printf("Offset of st_blksize: %ld\n", (long)offsetof(struct stat, st_blksize));
    printf("Offset of st_blocks: %ld\n", (long)offsetof(struct stat, st_blocks));
    */
//printf("Offset of actime: %ld\n", (long)offsetof(struct utimbuf, actime));
//printf("Offset of modtime: %ld\n", (long)offsetof(struct utimbuf, modtime));
 //alarm(5);
    //struct utimbuf new_times;
    //struct stat file_stat;

    //new_times.actime = time(NULL);  // 设置访问时间为当前时间
    //new_times.modtime = time(NULL); // 设置修改时间为当前时间

    //utime("test.txt", &new_times);
/*
    fstat(fileno(file), &file_stat);
    printf("atime:  %s", ctime(&file_stat.st_atime));
    printf("mtime:  %s", ctime(&file_stat.st_mtime));
    
    fclose(file);
    fclose(file2);
    */
    return 0;
}

