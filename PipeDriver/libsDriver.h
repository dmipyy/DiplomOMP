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
#include <linux/string.h>

#define maxMessageLength 1024
#define WR_DATA _IOW('a','a',char*)
#define RD_DATA _IOR('a','b',char*)

#define bufferSize 10 // размер буфера
//const int bufferSize = 8;
char *buffer; // выделение памяти под буфер
char *bufferStart; // указатель на начало буфера
char *bufferEnd; // указатель на конец буфера
char *currentPosition; // указатель на текущую позицию в буфере
char *tempBuffer;

int remainder = 0;
bool lastLap;

struct IoctlBuffer //структура данных для функции ioctl
{
	char message[maxMessageLength];
};

int ioBufferLen = 0;
dev_t device = 0; //устройстро символьного драйвера

int realMessageLength = 0;
volatile bool dataAvailable;

static struct class *DeviceClass;
static struct cdev CharDevice;
static struct IoctlBuffer data;

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
