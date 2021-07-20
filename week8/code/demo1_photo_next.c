#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include <linux/input.h>	// 输入子系统头文件

// 函数声明
// 1.打开触摸屏
void open_ts(void);	
// 2.获取xy坐标
void get_xy(void);	
// 3.关闭触摸屏
void close_ts(void);		

// 显示全屏图片
int show_bmp_full(char *pic_name);
// 打开LCD
void open_lcd(void);
// 关闭LCD
void close_lcd(void);

// 功能1
void func_store(void);
void func_photo(void);

// 全局变量
int pos_x, pos_y;	// XY坐标
int ts_fd;			// 触摸屏文件描述符
int lcd_fd;			// LCD文件描述符
int *FB;			// LCD显存


// 获取触摸屏坐标
int main(void)
{
	open_ts();	// 1.打开触摸屏
	open_lcd();	// 打开LCD

	// 逻辑代码
		// 显示主界面
	show_bmp_full("ui_main.bmp");

	while(1)
	{
		get_xy();	// 2.获取xy坐标
		printf("pos: (%d, %d)\n", pos_x, pos_y);

		if(pos_x>515 && pos_x<=590 && pos_y>140 && pos_y<215)
		{
			printf("wangzhe\n");
			func_store();
		}
		else if(pos_x>75 && pos_x<=130 && pos_y>145 && pos_y<200)
		{
			printf("photo\n");
			func_photo();
		}
	}


	close_lcd();	// 关闭LCD
	close_ts();		// 3.关闭触摸屏
}

void func_store(void)
{
	// 显示功能界面
	show_bmp_full("ui_store.bmp");

	while(1)
	{
		get_xy();	// 2.获取xy坐标
		printf("pos: (%d, %d)\n", pos_x, pos_y);

		if(pos_x>0 && pos_x<=100 && pos_y>0 && pos_y<40)
		{
			printf("return\n");
			// 显示主界面
			show_bmp_full("ui_main.bmp");
			return;
		}
	}
}


void func_photo(void)
{
	// 字符串数组（存放一系列的图片名）
	char *pic_list[5] = {"1.bmp", "2.bmp", "3.bmp", "4.bmp", "5.bmp"};

	// 显示功能界面
	show_bmp_full("ui_photo.bmp");

	int i=-1;
	while(1)
	{
		get_xy();	// 2.获取xy坐标
		printf("pos: (%d, %d)\n", pos_x, pos_y);

		if(pos_x>200 && pos_x<=600 && pos_y>0 && pos_y<480)
		{
			// 返回
			printf("return\n");
			// 显示主界面
			show_bmp_full("ui_main.bmp");
			return;
		}
		else if(pos_x>0 && pos_x<=200 && pos_y>0 && pos_y<480)
		{
			// 上一张
			printf("prev\n");
			
		}
		else if(pos_x>600 && pos_x<=800 && pos_y>0 && pos_y<480)
		{
			// 下一张
			printf("next\n");
			i++;
			if(i >= 5)
				i=0;

			show_bmp_full(pic_list[i]);	// "1.bmp"
		}
	}
}



// 1.打开触摸屏
void open_ts(void)
{
	ts_fd = open("/dev/input/event0", O_RDONLY);
	if(ts_fd == -1)
	{
		perror("open ts failed");
		return;
	}
}

// 2.获取xy坐标
void get_xy(void)
{
	struct input_event ts_buf;

	int x_ready=0, y_ready=0;

	while(1)
	{
		read(ts_fd, &ts_buf, sizeof(ts_buf));
		//printf("type:0x%02x code:0x%02x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);

		if(ts_buf.type==EV_ABS && ts_buf.code==ABS_X)
		{
			pos_x = ts_buf.value;
			x_ready = 1;
			y_ready = 0;	// 确保先获取到X坐标
		}
		else if(ts_buf.type==EV_ABS && ts_buf.code==ABS_Y)
		{
			pos_y = ts_buf.value;
			y_ready = 1;
		}
	
		if(x_ready==1 && y_ready==1)
		{
			// 对坐标进行处理 1024-600  --->  800-480
			pos_x = pos_x*800/1024;
			pos_y = pos_y*480/600;
			break;
		}
	}
}

// 3.关闭触摸屏
void close_ts(void)
{
	close(ts_fd);
}		

// 打开LCD
void open_lcd(void)
{	
	//（一个问题：把大象从冰箱1塞进冰箱2，分几步？）
	//1.	打开冰箱2门：打开LCD屏幕文件”/dev/fb0”	
	lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("lcd open failed");
		return;
	}
	// 映射显存
	FB = mmap(NULL, 
		4*800*480, 
		PROT_READ |PROT_WRITE, 
		MAP_SHARED, 
		lcd_fd,
		0);
}

// 关闭LCD
void close_lcd(void)
{
	// 6.	关闭冰箱2门：将lcd关闭			
	close(lcd_fd);
}

// 显示一张全屏图片
int show_bmp_full(char *pic_name)
{
	// 2.打开冰箱1（BMP文件）
	int bmp_fd = open(pic_name, O_RDONLY);
	if(bmp_fd == -1)
	{
		perror("open bmp failed");
		printf("pic_name: %s\n", pic_name);
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
			*(FB+x+800*y) = lcd_buf[x+800*(479-y)];
		}
	}

	// 5.关闭冰箱1的门
	close(bmp_fd);

	return 0;
}








