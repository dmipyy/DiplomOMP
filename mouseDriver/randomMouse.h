#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/timekeeping.h>
#include <linux/math.h>

#define DEVICE_NAME "random_mouse"
#define CLASS_NAME "random"
#define randomBits 40
#define bitBasic 1

static dev_t dev;
static struct cdev CharDevice;
static struct class *DeviceClass;
static struct device *DevFile;

static int irqNum;
static DECLARE_WAIT_QUEUE_HEAD(wait_q);	//очередь отправленных "поспать" процессов
unsigned long long randomValue;
bool binaryArray[randomBits];
unsigned long long bootTime;
unsigned long long bitValue;
unsigned long long tempValue;
int index;
volatile bool dataAvailable;

