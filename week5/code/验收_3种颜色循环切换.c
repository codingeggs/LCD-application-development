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
	int color_red =   0x00FF0000;	
	int color_green = 0x0000FF00;	
	int color_blue =  0x000000FF;

	// 全屏红色
	int x, y;
	
	while(1)
	{
		lseek(lcd_fd, 0, SEEK_SET);
		for(y=0; y<480; y++)
		{
			for(x=0; x<800; x++)
			{
				write(lcd_fd, &color_red, 4);
			}
		}
		sleep(1);

		// 全屏绿色
		lseek(lcd_fd, 0, SEEK_SET);
		for(y=0; y<480; y++)
		{
			for(x=0; x<800; x++)
			{
				write(lcd_fd, &color_green, 4);
			}
		}
		sleep(1);
	
		// 全屏蓝色
		lseek(lcd_fd, 0, SEEK_SET);
		for(y=0; y<480; y++)
		{
			for(x=0; x<800; x++)
			{
				write(lcd_fd, &color_blue, 4);
			}
		}
		sleep(1);
	}

	//3.	关闭冰箱门：将lcd关闭			
	close(lcd_fd);

	return 0;
}












