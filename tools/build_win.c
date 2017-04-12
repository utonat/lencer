/*
 * Small/tools/build_win.c
 *
 * (C) 2012-2013 Yafei Zheng
 * V0.0 2012-12-7 19:53:15
 *
 * Email: e9999e@163.com, QQ: 1039332004
 */

/*
 * 此文件是在windows下使用，利用此文件在VC6.0下编译生成的*.exe文件，将OS各模块组装到一块。
 */

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024			// 缓冲区字节数

#define OFFSET_BOOT 32			// 从文件开始处的偏移量(字节数),用来去除boot.s被as86编译之后的MINIX头(32B)
#define OFFSET_HEAD 0x1000		// 去除head模块的0x1000B的GCC头。在Linux-0.11中，是1024B，因为当时的GCC生
								// 成的可执行文件是 a.out 格式的，而我用的GCC(gcc 4.6.3 for Start OS)生成的
								// 是 ELF 格式。关于ELF文件头，请参考ELF相关内容。

int main(void)
{
	char buff[BUFF_SIZE] = {0};
	FILE *fp_head = NULL, *fp_boot = NULL, *fp_Image = NULL;
	int count = 0, size_os = 0;

	if(! ((fp_head=fopen("head","rb")) && (fp_boot=fopen("boot","rb")) && (fp_Image=fopen("Image","wb"))))
	{
		printf("Error: can't open some file!!!\n");
		printf("\npress Enter to exit...");
		getchar();
		return -1;
	}

	fseek(fp_boot,OFFSET_BOOT,SEEK_SET);
	fseek(fp_head,OFFSET_HEAD,SEEK_SET);

	for(; (count=fread(buff,1,sizeof(buff),fp_boot))>0; size_os+=count,fwrite(buff,count,1,fp_Image)) {}
	fclose(fp_boot);
	for(; (count=fread(buff,1,sizeof(buff),fp_head))>0; size_os+=count,fwrite(buff,count,1,fp_Image)) {}
	fclose(fp_head);
	fclose(fp_Image);
	
	printf("===OK!\nSize of OS is %d Bytes.\n",size_os);

	printf("\npress Enter to exit...");
	getchar();
	return 0;
}
