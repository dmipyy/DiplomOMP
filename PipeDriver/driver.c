#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/fcntl.h>

#define maxMessageLength 1024
#define WR_DATA _IOW('a','a',char*)
#define RD_DATA _IOR('a','b',char*)

struct IoctlBuffer //структура данных для функции ioctl
{
	char message[maxMessageLength];
};

char *ioBuffer = NULL;
int ioBufferLen = 0;
dev_t device = 0; //устройстро символьного драйвера

int driverOpenCounter = 0;
int readersCounter = 0;
int writersCounter = 0;
volatile bool dataAvailable;

static struct class *DeviceClass;
static struct cdev CharDevice;
static struct IoctlBuffer data;

uint8_t *kernel_buffer;

static int __init CharDriverInit(void);
static void __exit CharDriverExit(void);
static int CharDriverOpen(struct inode *inode, struct file *file);
static int CharDriverRelease(struct inode *inode , struct file *file);
static ssize_t CharDriverRead(struct file *filp, char *userBuffer, size_t messageLength, loff_t *off);
static ssize_t CharDriverWrite(struct file *filp, const char *userBuffer,size_t maxLength, loff_t *off);
static long CharDriverIoctl(struct file *filp, unsigned int cmd, unsigned long address);

static DECLARE_WAIT_QUEUE_HEAD(wait_q);	//очередь отправленных "поспать" процессов

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .read = CharDriverRead,
    .write = CharDriverWrite,
    .open = CharDriverOpen,
    .unlocked_ioctl = CharDriverIoctl,
    .release = CharDriverRelease,
};

static int __init CharDriverInit(void)
{
    //выделяем major
    if((alloc_chrdev_region(&device,0,1,"CharDevice")) < 0)
	{
        pr_alert("Cannot allocate the major number..\n");
        return -1;
    }

    pr_alert("Major = %d , Minor = %d..\n",MAJOR(device),MINOR(device));

    //Создаем структуру CharDevice
    cdev_init(&CharDevice,&fops);
	
    //Добавляем символьное устройство в систему
    if((cdev_add(&CharDevice,device,1)) < 0)
	{
        pr_alert("Cannot add the device to the system...\n");
        goto r_class;
    }
	
    //Создаем структуру класса
    if((DeviceClass = class_create(THIS_MODULE,"my_class")) == NULL)
	{
        pr_alert("cannot create struct class...\n");
        goto r_class;
    }
	
    //Создаем устройство
    if((device_create(DeviceClass,NULL,device,NULL,"my_device"))== NULL)
	{
        pr_alert("cannot create the device..\n");
        goto r_device;
    }

	//Выделяем память под буфер
    if((ioBuffer = kmalloc(maxMessageLength, GFP_KERNEL)) == 0)
	{
        pr_alert("cannot allocate memory in kernel\n");
        return -1;
    }

	dataAvailable = false;
	
    pr_alert("Device driver insert...done properly...\n");
    return 0;

r_device:
    class_destroy(DeviceClass);

r_class:
        unregister_chrdev_region(device,1);
        return -1;
}

static void __exit CharDriverExit(void)
{
    device_destroy(DeviceClass,device);
    class_destroy(DeviceClass);
    cdev_del(&CharDevice);
    unregister_chrdev_region(device,1);
    pr_alert("Device driver is removed successfully..\n");
}

static int CharDriverOpen(struct inode *inode, struct file *file)
{
	driverOpenCounter++;
    pr_alert("Device file opened...!!!!\n");
    return 0;
}

static int CharDriverRelease(struct inode *inode , struct file *file)
{
	if(driverOpenCounter == 1)
	{
		kfree(ioBuffer);
	}
    pr_alert("Device file closed.....!!!\n");
	driverOpenCounter--;
    return 0;
}

static ssize_t CharDriverRead(struct file *filp, char __user *userBuffer, size_t messageLength, loff_t *off)
{
	//сделать проверку на количество открытий
    pr_alert("Reading to %c\n", &userBuffer);
    //char localBuffer[1024] = {"Hello World"};
    pr_alert("Sending message: %s\n", ioBuffer);
    int lostBytes = copy_to_user(userBuffer, ioBuffer, messageLength);
    pr_alert("Lost bytes %d\n",lostBytes);

    return messageLength;
}

static ssize_t CharDriverWrite(struct file *filp, const char __user *userBuffer,size_t maxLength, loff_t *off)
{
    int realMessageLength = 0;
	
    pr_alert("Writing");
    copy_from_user(&(data.message), userBuffer, maxLength);
    pr_alert("AFTER COPY FROM USER");
	
	//считаем настоящую длину сообщения
    char *ptr;
    ptr = data.message;
	pr_alert("CONTINUED WRITING");
    for(int i = 0; i <= maxLength ; ++i)
	{
		realMessageLength++;
        ++ptr;
        if(*ptr == '\0') break;	
    }
    		
    if((ioBuffer = krealloc(ioBuffer, (realMessageLength + 1)*sizeof(char), GFP_KERNEL))== 0)
	{
     	pr_alert("cannot allocate memory in kernel\n");
        return -1;
    }
        	
    ptr = data.message;
  
    for(int i = 0; i < realMessageLength; ++i)
    {
    	ioBuffer[i] = *ptr;
    	++ptr;
    }
    ioBuffer[realMessageLength] = '\0';
    
	
    pr_alert("Message: %s\n", ioBuffer);
    pr_alert("Message length = %d",realMessageLength);
    
    return realMessageLength + 1;
}

static long CharDriverIoctl(struct file *filp, unsigned int cmd, unsigned long address)
{
	pr_alert("IOCTL\n");
	
    switch(cmd)
	{
    	case WR_DATA:
    		writersCounter++;
        	ioBufferLen = CharDriverWrite(filp, (char*)address, sizeof(data), NULL); 
			WRITE_ONCE(dataAvailable,true);	
			wake_up_interruptible(&wait_q);
			pr_alert("Writer sleeping\n");
        	wait_event_interruptible(wait_q, (dataAvailable == false)); 
        	pr_alert("Writer is back!\n");
        	writersCounter--; 
        break;
			
        case RD_DATA:
        	readersCounter++;
        	if (dataAvailable == false)
        	{
        		pr_alert("Reader sleeping\n");
        		wait_event_interruptible(wait_q, (dataAvailable == true));	//ждём, когда можно будет спать
        	}else
        	{
        		pr_alert("Reader is going to read %d bytes\n", ioBufferLen);
        		CharDriverRead(filp, (char*)address, (size_t)(ioBufferLen*sizeof(char)),NULL);
				WRITE_ONCE(dataAvailable,false);	
				wake_up_interruptible(&wait_q);
			}
			readersCounter--;
        break;
    }
    return 0;
}

module_init(CharDriverInit); 
module_exit(CharDriverExit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borisov, Voropanov, Podlesnyy");
MODULE_DESCRIPTION("Pipe device driver");
