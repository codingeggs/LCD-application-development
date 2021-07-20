#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(void)
{
	// 1.打开目录
	DIR *dp;
	dp = opendir("/mnt/udisk");
	if(dp == NULL)
	{
		perror("open dir failed");
		return -1;
	}

	// 2.读取目录信息
	struct dirent *p;
	int len;
	while(1)
	{
		p = readdir(dp);
		if(p == NULL)
		{
			// 读取完毕
			printf("done!\n");
			break;
		}
/*
		01234
		1.bmp	长度5
		[1] == "."
		[2] == "b"
		[3] == "m"
		[4] == "p"

		0123456
		123.bmp		长度7
		[3] == "."
		[4] == "b"
		[5] == "m"
		[6] == "p"

		.bmp
*/
		// 计算文件名长度， 判断最后的四个字符 . b m p
		len = strlen(p->d_name);
		if(len > 4
			&& p->d_name[len-4] == '.' 
			&& p->d_name[len-3] == 'b' 
			&& p->d_name[len-2] == 'm' 
			&& p->d_name[len-1] == 'p')
		{
			// 读取成功
			printf("%s\n", p->d_name);
		}
	}

	// 3.关闭目录
	// closedir

	return 0;
}
























