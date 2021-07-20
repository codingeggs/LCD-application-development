#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	//（一个问题：把大象塞进冰箱，分几步？）
	//1.	打开冰箱门：打开LCD屏幕文件”/dev/fb0”	
	int lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("lcd open failed");
		return -1;
	}

	//2.	把大象塞进冰箱	：将颜色写入到lcd		
	int color_red = 0x00FF0000;

	write(lcd_fd, &color_red, 4);


	//3.	关闭冰箱门		：将lcd关闭			close

	return 0;
}












