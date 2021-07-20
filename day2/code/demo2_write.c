#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	// 1.打开文件
	int fd;	
	fd = open("1.txt", O_RDWR);
	if(fd == -1)
	{
		perror("open failed");
		return -1;
	}

	printf("open susscessful!\n");

	// 2.写入数据到文件中
	char buf[10] = "nihao";
		// 每个字符串都有结束符 '\0'，剩余的4个也是'\0'

	write(fd, buf, 10);
		// 数组名就是数组首元素地址
	
	return 0;
}

