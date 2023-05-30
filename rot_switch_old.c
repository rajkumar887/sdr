#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/irq.h>
#include <linux/clk.h>
#include <linux/moduleparam.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/preempt.h>
#include <linux/gpio.h>
#include <asm/ioctl.h>

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/
#define ONE			241
#define TWO			239
#define THREE			237
#define FOUR			235
#define FIVE			225
#define SIX			115
#define SEVEN			243
#define EIGHT			244
#define NINE			246
#define TEN			227

#define ICS_SWITCH_MAGIC           	'K'
#define ICS_SWITCH_MAJOR_NO        	241	
#define ICS_SWITCH_DEVNAME		"rotary" 

/*----------------------------------------------------------------------------
 *        Static variables
 *----------------------------------------------------------------------------
*/
static int SCAN_START = 0;
static struct class *ics_switch_class;
static int deviceopen = 0;      /* For preventing multiple device opens */
static irqreturn_t ISR_switch(int irq, void *dev_id);

/*----------------------------------------------------------------------------
 *        Global variables
 *----------------------------------------------------------------------------
*/
int code;
int major;
int sec_int =0;
int irq_cnt_g=0;
int oneirq=0,twoirq=0,threeirq=0,fourirq=0,ret=0 ,fiveirq=0, sixirq=0, sevenirq=0, eightirq=0, nineirq=0, tenirq=0 ;
int chgscan_code(int row);
wait_queue_head_t  kq; /* waitQ for blocking implementation */

MODULE_AUTHOR("BHARGAVI NAIDU");
MODULE_DESCRIPTION("ROTARY SWITCH  DRIVER");
MODULE_LICENSE("GPL");
 
static int ics_scan_switch_gpio(void)
{
	static int sw_stat1=0,sw_stat2=0,sw_stat3=0, sw_stat4=0,sw_stat5=0,
		 sw_stat6=0,sw_stat7=0,sw_stat8=0,sw_stat9=0,sw_stat10=0;

                /* Reading the status of the Rows*/
	      	code=0;
                sw_stat1=gpio_get_value(ONE);
                sw_stat2=gpio_get_value(TWO);
                sw_stat3=gpio_get_value(THREE);
                sw_stat4=gpio_get_value(FOUR);
                sw_stat5=gpio_get_value(FIVE);
                sw_stat6=gpio_get_value(SIX);
                sw_stat7=gpio_get_value(SEVEN);
                sw_stat8=gpio_get_value(EIGHT);
                sw_stat9=gpio_get_value(NINE);
                sw_stat10=gpio_get_value(TEN);
		
	      mdelay(100);
	      
                if(sw_stat1==0)
                {
                       printk("ONE\n\r");
                       code=1;
                }

                if(sw_stat2==0)
                {
                     printk("TWO\n\r");
                       code=2;
                }
               
                if(sw_stat3==0)
                {
                     printk("THREE\n\r");
                       code=3;
                }
                
                if(sw_stat4==0)
                {
                   printk("FOUR\n\r");
                       code=4;
                }
                
                if(sw_stat5==0)
                {
                    printk("FIVE\n\r");
                       code=5;
                }
                
                if(sw_stat6==0)
                {
                    printk("SIX\n\r");
                      code=6;
                }
                
                if(sw_stat7==0)
                {
                    printk("SEVEN\n\r");
                      code=7;
                }
                
	      	if(sw_stat8== 0)
                {
                    printk("EIGHT\n\r");
                      code=8;
                }
                
                if(sw_stat9==0)
                {
                    printk("NINE");
                       code=9;
                }
                if(sw_stat10==0)
                {
                    printk("TEN");
                       code=10;
                }
				
	return code;
}	
/* This function will read the user key state.
 * 
 *
 */
 
 int ics_switch_dev_open(struct inode *inode, struct file *filp)
{
	printk("ICS Rotary Switch open \n");
	return 0;
}

//static int ics_switch_dev_close(struct inode *inode, struct file *file);


static int ics_switch_dev_close(struct inode *inode, struct file *file)
{
	printk("ICS Rotary Switch closed\n");
	deviceopen --;
	return 0;
}

static ssize_t ics_switch_dev_read(struct file *file, char *buf, size_t lbuf, loff_t *ppos)
{

	int read_buf[2];
        wait_event_interruptible(kq, (SCAN_START != 0));
	SCAN_START = 0;
	printk("ICS Rotary Switch READ FUNCTION>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

        read_buf[0] = ics_scan_switch_gpio();
		
	lbuf=1;
        ret = copy_to_user( buf,        /* to */
                        read_buf,       /* from */
                        lbuf);          /* how many bytes */

	return 0;
}

static ssize_t ics_switch_dev_write(struct file *file, const char *buf, size_t lbuf, loff_t *ppos)
{

	return 0;
}
/* this will implements the ioctl calls from user 
 * @filep : indicates the file pointer 
 * cmd:  ioctl magic command
 * arg :  argumentrs
 */
static long ics_switch_dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	int direction;
	int size;
	int *buffer = NULL;
	//void __user *ioargp = (void __user *)arg;

	/* make sure it is a valid command */
	if (_IOC_TYPE (cmd) != ICS_SWITCH_MAGIC) {
		printk (KERN_WARNING "invalid case, CMD=%d\n", cmd);
		return -EINVAL;
	}

	/* get the size of the buffer and kmalloc it */
	size = _IOC_SIZE (cmd);
	buffer = kmalloc ((size_t) size, GFP_KERNEL);
	if (!buffer) {
		printk (KERN_ERR "Kmalloc failed for buffer\n");
		return -ENOMEM;
	}
	direction = _IOC_DIR (cmd);
	switch (direction) {
		case _IOC_WRITE:
			break;
		case _IOC_READ:
			break;
		default:
			printk (KERN_WARNING " got invalid case, CMD=%d\n", cmd);
			return -EINVAL;
	}
	if (buffer)
		kfree (buffer);
	return 0;
}

struct file_operations ics_switch_fops =
{
	.open = ics_switch_dev_open,
	.release = ics_switch_dev_close,
	.read = ics_switch_dev_read,
	.write = ics_switch_dev_write,
	.unlocked_ioctl = ics_switch_dev_ioctl,
};

static int ics_init_device_node(void)
{
	int ret;

	ret = register_chrdev(ICS_SWITCH_MAJOR_NO, ICS_SWITCH_DEVNAME, &ics_switch_fops);
	if (ret)
	{
		printk(KERN_INFO "test: Device number allocation failed\n");
		ret = (-EIO);
		goto exit_1;
	}
	ics_switch_class = class_create(THIS_MODULE, ICS_SWITCH_DEVNAME);
	if (IS_ERR(ics_switch_class))
	{
		printk("No udev support.\n");
		ret = PTR_ERR(ics_switch_class);
		goto exit_2;
	}
	device_create(ics_switch_class, NULL, MKDEV(ICS_SWITCH_MAJOR_NO, 0), NULL, ICS_SWITCH_DEVNAME);
	printk("FPGADRIVER:  major number  %d\n", major);
	return 0;

exit_2:
	class_destroy(ics_switch_class);
exit_1:
	unregister_chrdev(ICS_SWITCH_MAJOR_NO, ICS_SWITCH_DEVNAME);
	return ret;
}

int ics_init_switch_hw_init(void )
{

    gpio_request(ONE,NULL);
    gpio_request(TWO,NULL);
    gpio_request(THREE,NULL);
    gpio_request(FOUR,NULL);
    gpio_request(FIVE,NULL);
    gpio_request(SIX,NULL);
    gpio_request(SEVEN,NULL);
    gpio_request(EIGHT,NULL);
    gpio_request(NINE,NULL);
    gpio_request(TEN,NULL);

    gpio_direction_input(ONE);
    gpio_direction_input(TWO);
    gpio_direction_input(THREE);
    gpio_direction_input(FOUR);
    gpio_direction_input(FIVE);
    gpio_direction_input(SIX);
    gpio_direction_input(SEVEN);
    gpio_direction_input(EIGHT);
    gpio_direction_input(NINE);
    gpio_direction_input(TEN);

    gpio_set_debounce(ONE,50);
    gpio_set_debounce(TWO,50);
    gpio_set_debounce(THREE,50);
    gpio_set_debounce(FOUR,50);
    gpio_set_debounce(FIVE,50);
    gpio_set_debounce(SIX,50);
    gpio_set_debounce(SEVEN,50);
    gpio_set_debounce(EIGHT,50);
    gpio_set_debounce(NINE,50);
    gpio_set_debounce(TEN,50);

#if 1
	oneirq = gpio_to_irq(ONE);
	ret = request_irq(oneirq, ISR_switch ,IRQF_TRIGGER_RISING, "oneirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  oneirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	twoirq = gpio_to_irq(TWO);
	ret = request_irq(twoirq, ISR_switch ,IRQF_TRIGGER_RISING, "twoirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  twoirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	threeirq = gpio_to_irq(THREE);
	ret = request_irq(threeirq, ISR_switch ,IRQF_TRIGGER_RISING, "threeirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  threeirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	fourirq = gpio_to_irq(FOUR);
	ret = request_irq(fourirq, ISR_switch ,IRQF_TRIGGER_RISING, "fourirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  fourirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	fiveirq = gpio_to_irq(FIVE);
	ret = request_irq(fiveirq, ISR_switch ,IRQF_TRIGGER_RISING, "fiveirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  fiveirq, (unsigned long)ISR_switch);
		goto out;
	}
	sixirq = gpio_to_irq(SIX);
	ret = request_irq(sixirq, ISR_switch ,IRQF_TRIGGER_RISING, "sixirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  sixirq, (unsigned long)ISR_switch);
		goto out;
	}
	sevenirq = gpio_to_irq(SEVEN);
	ret = request_irq(sevenirq, ISR_switch ,IRQF_TRIGGER_RISING, "sevenirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  sevenirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	eightirq = gpio_to_irq(EIGHT);
	ret = request_irq(eightirq, ISR_switch ,IRQF_TRIGGER_RISING, "eightirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  eightirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	nineirq = gpio_to_irq(NINE);
	ret = request_irq(nineirq, ISR_switch ,IRQF_TRIGGER_RISING, "nineirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  nineirq, (unsigned long)ISR_switch);
		goto out;
	}
	
	tenirq = gpio_to_irq(TEN);
	ret = request_irq(tenirq, ISR_switch ,IRQF_TRIGGER_RISING, "tenirq", NULL);
	if(ret!= 0)
	{
		printk("Switch Requested IRQ: %d @ 0x%lx",  tenirq, (unsigned long)ISR_switch);
		goto out;
	}
	
#endif
out:
	return ret;
}


static irqreturn_t ISR_switch(int irq, void *dev_id)
{

	//	printk("ISR\n\r");

        SCAN_START = 1;
        wake_up_interruptible(&kq);
	return IRQ_HANDLED;
}

int init_switch_device(void)
{
	printk(KERN_INFO"Rotary Switch\n\r");
	printk("Switch initialization\n");
      //  printk("GPIOS modified\n");
        	ics_init_switch_hw_init();
                  
        	ics_init_device_node();

        	init_waitqueue_head(&kq);
	while(1)
	{
		ics_scan_switch_gpio();
		mdelay(5);       

	}
	return 0;
}

void exit_switch_device(void)
{
	printk("Switch De-initialization\n");
	device_destroy(ics_switch_class, MKDEV(ICS_SWITCH_MAJOR_NO, 0));
	class_destroy(ics_switch_class);
	unregister_chrdev(ICS_SWITCH_MAJOR_NO, ICS_SWITCH_DEVNAME);

	if(oneirq > 0)
		free_irq(oneirq, NULL);
	if(twoirq > 0)
		free_irq(twoirq, NULL);
	if(threeirq > 0)
		free_irq(threeirq, NULL);
	if(fourirq > 0)
		free_irq(fourirq, NULL); 
	if(fiveirq > 0)
		free_irq(fiveirq, NULL);
	if(sixirq > 0)
		free_irq(sixirq, NULL);
	if(sevenirq > 0)
		free_irq(sevenirq, NULL); 
	if(eightirq > 0)
		free_irq(eightirq, NULL);
	if(nineirq > 0)
		free_irq(nineirq, NULL);
	if(tenirq > 0)
		free_irq(tenirq, NULL); 

        gpio_free(ONE);
        gpio_free(TWO);
        gpio_free(THREE);
        gpio_free(FOUR);
        gpio_free(FIVE);
        gpio_free(SIX);
        gpio_free(SEVEN);
        gpio_free(EIGHT);        
        gpio_free(NINE);        
        gpio_free(TEN);        
    
	printk("\n Module removed Successfully\n");
}

module_init(init_switch_device); /* Module init function */
module_exit(exit_switch_device); /* Module exit function */

