#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/circ_buf.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>

#define maxMessageLength 1024
#define WR_DATA _IOW('a','a',char*)
#define RD_DATA _IOR('a','b',char*)
#define circBufferLen 16

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

struct circ_buf *circBuffer;
//uint8_t *kernel_buffer;

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



