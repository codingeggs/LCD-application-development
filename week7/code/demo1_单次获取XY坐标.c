#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/input.h>	// 输入子系统头文件

// 获取触摸屏坐标
int main(void)
{
	// 1.打开触摸屏设备
	int ts_fd;	
	ts_fd = open("/dev/input/event0", O_RDONLY);
	if(ts_fd == -1)
	{
		perror("open ts failed");
		return 1;
	}

	// 2.从触摸屏读取数据
	struct input_event ts_buf;
	int pos_x, pos_y;
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
			break;
	}

	printf("pos: (%d, %d)\n", pos_x, pos_y);

	if(pos_x <= 512 )
		printf("left\n");
	else if(pos_x > 512 )
		printf("right\n");

	// 3.关闭触摸屏
	close(ts_fd);

	return 0;
}

