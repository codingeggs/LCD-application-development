#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	// 1.打开文件
	int fd = open("1.txt", O_RDWR);
	if(fd == -1)
	{
		perror("open failed");
		return -1;
	}

	// 2.写入文件
	int ret;
	char write_buf[10] = "hello,123\n";
	ret = write(fd, write_buf, strlen(write_buf));
	printf("%d writen\n", ret);

	// 2.5 修改文件的偏移量（偏移到开头）
	lseek(fd, 0, SEEK_SET);

	// 3.第一次读取文件
	char read_buf[50] = {0};
	ret = read(fd, read_buf, sizeof(read_buf));
	printf("1 Get(%d): %s\n", ret, read_buf);

	// 输入你的名字
	bzero(write_buf, sizeof(write_buf));	//清空
	scanf("%s", write_buf);
	
	// 写入名字到指定位置
	lseek(fd, 6, SEEK_SET);	//偏移到指定位置
	write(fd, write_buf, strlen(write_buf));

	// 第二次读取文件
	lseek(fd, 0, SEEK_SET);	//偏移到开头
	bzero(read_buf, sizeof(read_buf));	//清空
	ret = read(fd, read_buf, sizeof(read_buf));
	printf("2 Get(%d): %s\n", ret, read_buf);

	// 关闭文件
	close(fd);

	return 0;
}












