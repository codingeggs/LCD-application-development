#include <stdio.h>

int main(void)
{
	int data;
	scanf("%d", &data);	

	int i, j;

	// i:行数
	for(i=1; i<=data; i++)
	{
		// 1.空格
		for(j=1; j<=data-i; j++)
			printf(" ");

		// 2.星号*
		for(j=1; j<=2*i-1; j++)
			printf("*");

		// 3.换行符
		printf("\n");
	}
	
	return 0;
}

