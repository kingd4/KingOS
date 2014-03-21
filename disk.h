#ifndef DISK_H_
#define DISK_H_

#define MAX_FILES 64

#pragma pack(push,1)
struct VBR
{
	char data1[13];
	unsigned char sectors_per_cluster;
	unsigned short vbr_sectors;
	unsigned char num_fats;
	unsigned short num_root_dir_entries;
	unsigned short num_sectors_small;
	unsigned char data2;
	unsigned short sectors_per_fat;
	char data3[4];
	unsigned int first_sector;
	unsigned int num_sectors_big;
	char data4[476];
};
#pragma pack(pop)

#pragma pack(push,1)
struct DirEntry
{
	char fname[8]; //NOT null terminated!
	char ext[3]; //NOT null terminated!
	unsigned char attribs;
	unsigned char reserved[10];
	unsigned short ctime,cdate;
	unsigned short cluster;
	unsigned int size;
};
#pragma pack(pop)

struct File
{
	int in_use;
	int offset, first_cluster, size;
	int flags; //0=read, 1=write, 2=r/w
};



int disk_read_sectors(void* buf, unsigned sec, unsigned num);
int disk_write_sectors(void* buf,int first_sector, int num);
void disk_init();
void list_files();
void remove_file(char * x);
int file_test(char * x);

#endif /* HDD_H_ */
