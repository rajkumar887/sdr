

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <malloc.h>


struct display_write
{   
        unsigned int len;
        unsigned int mode;
        int row_idx;
        int col_idx;
        char wbuff[ICS_LCD_BUFF_SIZE];
};
struct display_write lcd_obj;

void ics_lcd_write(int row, int col, char *data)
{
	int status;
	lcd_obj.row_idx = row;                
       	lcd_obj.col_idx = col;
	strcpy(lcd_obj.wbuff, data);
	lcd_obj.len = strlen(data); 
	lcd_obj.wbuff[lcd_obj.len-1]='\0';
	status=ioctl(ics_lcd_fd,WRITE_IOCTL_WITHNEXTLINE,&lcd_obj);
	if(status < 0)	
	{
		printf("LCD write failed\n");
	}
}
