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

	// 全屏红色（指针前加星号×，解引用，表示操作指针对应的数据）
	*FB = color_red;	//第1个像素点
	*(FB+1) = color_red;	//第2个像素点
	//....

	//3.	关闭冰箱门：将lcd关闭			
	close(lcd_fd);

	return 0;
}












