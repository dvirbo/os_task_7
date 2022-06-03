// Creadit: https://www.youtube.com/watch?v=n2AAhiujAqs&ab_channel=drdelhart

#include "ufs.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

int find_empty_inode()
{
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        if (inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
}
int find_empty_block()
{
    int i;
    for (i = 0; i < sb.num_blocks; i++)
    {
        if (dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}
int allocate_file(char name[8]){
    int in = find_empty_inode();
    int block = find_empty_block();
    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy((inodes[in]).name, name);
    // return the file descriptor
    return in;
}

void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if (nn >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}

void create_fs()
{
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof(struct disk_block);
    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
    }
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
    }
}

void mount_fs()
{
    FILE *file;
    file = fopen("fs_data", "r");
    fread(&sb, sizeof(struct superblock), 1, file);
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block)* sb.num_blocks);
    
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}

void sync_fs()
{
    FILE *file;
    file = fopen("fs_data", "w+");
    fwrite(&sb, sizeof(struct superblock), 1, file);
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}
int get_block_num(int file, int offset){
    int togo= offset;
    int bn = inodes[file].first_block;
    while (togo > 0)
    {
        bn = dbs[bn].next_block_num;
        togo--;
    }
    return bn;  
}

void set_filesize(int filenum, int size){
    int temp = size + BLOCKSIZE -1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    num--;
    while (num > 0)
    {
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2)
        {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[num].next_block_num;
        num--;
    }
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
}

void write_byte(int filenum,int pos, char* data){
    int relative_block = pos / BLOCKSIZE;
    int bn = get_block_num(filenum, relative_block);
    int offset = pos % BLOCKSIZE;
    for (int i = 0; i < strlen(data); i++)
    {
        dbs[bn].data[offset +i] = data[i]; 
    }
    
    // dbs[bn].data[offset] = *data;
}


void print_fs()
{
    printf("SuperBlock Info:\n");
    printf("\tnum inodes %d\n", sb.num_inodes);
    printf("\tnum blocks %d\n", sb.num_blocks);
    printf("\tsize blocks %d\n", sb.size_blocks);
    printf("inodes\n");
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        printf("\tsize: %d block: %d name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }
    for (i = 0; i < sb.num_blocks; i++)
    {
        printf("\tblock_num: %d next block %d\n", i, dbs[i].next_block_num);
    }
}