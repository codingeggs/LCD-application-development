#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
	char write_buf[10] = "Rose\n";
	ret = write(fd, write_buf, 5);
	printf("%d writen\n", ret);

	// 2.5 修改文件的偏移量
	lseek(fd, 0, SEEK_SET);

	// 3.读取文件
	char read_buf[10] = {0};
	ret = read(fd, read_buf, sizeof(read_buf));
	printf("Get(%d): %s\n", ret, read_buf);

	// 3.关闭文件
	close(fd);

	return 0;
}












