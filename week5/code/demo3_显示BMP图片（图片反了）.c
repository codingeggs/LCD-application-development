#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(void)
{
	//（一个问题：把大象从冰箱1塞进冰箱2，分几步？）
	//1.	打开冰箱2门：打开LCD屏幕文件”/dev/fb0”	
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

	// 2.打开冰箱1（BMP文件）
	int bmp_fd = open("school.bmp", O_RDONLY);
	if(bmp_fd == -1)
	{
		perror("open bmp failed");
		return -2;
	}

	// 3.将大象从冰箱1拿出来（读取RGB）
	char bmp_buf[800*480*3] = {0};
	char bmp_info[54] = {0};
		//读取文件头（无意义，为了偏移文件到RGB位置）
	read(bmp_fd, bmp_info, 54);

		//读取RGB数据
	read(bmp_fd, bmp_buf, 800*480*3);

	// 4.将大象塞到冰箱2（将RGB写入到aRGB的LCD中）
		// 将3字节RGB转换为4字节的aRGB		
	int i;
	int lcd_buf[800*480];
	for(i=0; i<800*480; i++)
		lcd_buf[i] = bmp_buf[3*i] | bmp_buf[3*i+1]<<8 | bmp_buf[3*i+2]<<16;
		//（思考一下上述代码）

		// 将数据写入到LCD
	int x, y;		
	for(y=0; y<480; y++)
	{
		for(x=0; x<800; x++)	
		{
			*(FB+x+800*y) = lcd_buf[x+800*y];
		}
	}

	// 5.关闭冰箱1的门
	close(bmp_fd);

	// 6.	关闭冰箱2门：将lcd关闭			
	close(lcd_fd);

	return 0;
}












