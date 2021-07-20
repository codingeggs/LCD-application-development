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
	
	while(1)
	{
		read(ts_fd, &ts_buf, sizeof(ts_buf));
		//printf("type:0x%02x code:0x%02x value:%d\n", ts_buf.type, ts_buf.code, ts_buf.value);
		if(ts_buf.type==EV_ABS && ts_buf.code==ABS_X)
			printf("(%d, ", ts_buf.value);
		else if(ts_buf.type==EV_ABS && ts_buf.code==ABS_Y)
			printf("%d)\n", ts_buf.value);
	}

	// 3.关闭触摸屏
	close(ts_fd);

	return 0;
}

