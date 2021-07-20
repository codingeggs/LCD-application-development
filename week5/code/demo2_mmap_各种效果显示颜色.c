#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

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

	// 映射显存
	int *FB;	
	FB = mmap(NULL, 
		4*800*480, 
		PROT_READ |PROT_WRITE, 
		MAP_SHARED, 
		lcd_fd,
		0);

	//2.	把大象塞进冰箱	：将颜色写入到lcd		
	int color_red =   0x00FF0000;	
	int color_green = 0x0000FF00;	
	int color_blue =  0x000000FF;

	int x, y;

	while(1)
	{
		// 全屏红色（指针前加星号×，解引用，表示操作指针对应的数据）
			//从上到下			
		for(y=0; y<480; y++)
		{
			for(x=0; x<800; x++)	
			{
				*(FB+x+800*y) = color_red;
			}
			usleep(1*1000);	//us微秒级延时 1s=1 000ms= 1 000 000us
		}
		sleep(1);
	
			// 从下到上
		for(y=479; y>=0; y--)
		{
			for(x=0; x<800; x++)	
			{
				*(FB+x+800*y) = color_green;
			}
			usleep(1*1000);	//us微秒级延时 1s=1 000ms= 1 000 000us
		}
		sleep(1);

			// 从左到右
		for(x=0; x<800; x++)	
		{
			for(y=0; y<480; y++)
			{
				*(FB+x+800*y) = color_blue;
			}
			usleep(1*1000);	//us微秒级延时 1s=1 000ms= 1 000 000us
		}
		sleep(1);
	}

	//3.	关闭冰箱门：将lcd关闭			
	close(lcd_fd);

	return 0;
}












