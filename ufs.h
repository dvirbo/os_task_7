#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BLOCKSIZE 512

struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode {
    int size;
    int first_block;
    char name[8];
};

struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
};

void create_fs();
void mount_fs();
void sync_fs();

int allocate_file(char name[8]);
void set_filesize(int filenum, int size);
void write_byte(int filenum,int pos, char* data);
int get_block_num(int file, int offset);
void shorten_file(int bn);
void print_fs();






// struct mydirent{

// };
// void mymkfs(int x);
// int mymount(const char *source, const char *target,const char *filesystemtype, unsigned longmountflags, const void *data);
// int myopen(const char *pathname, int flags);
// int myclose(int myfd);
// ssize_t myread(int myfd, void *buf, size_t count);
// ssize_t mywrite(int myfd, const void *buf, size_t count);
// off_t mylseek(int myfd, off_t offset, int whence);
// myDIR *myopendir(const char *name);
// struct mydirent *myreaddir(myDIR *dirp);
// int myclosedir(myDIR *dirp);