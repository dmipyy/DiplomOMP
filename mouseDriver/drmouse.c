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
long randomValue;
bool binaryArray[randomBits];
long bootTime;
long bitValue;
int index;
volatile bool dataAvailable;

static irqreturn_t mouseInterrupt(int irq, void *devId) {
	//ktime_t ktime_get(void);
	unsigned int randomNumber;

	bootTime = ktime_get_boottime();
	bitValue = (bootTime >> bitBasic) &1;
	if (index < randomBits){
		binaryArray[index] = bitValue;
		//randomValue += binaryArray[index] * (1<<(randomBits - index - 1));
		randomValue = index;
		++index;
	} else if (index == randomBits){
		WRITE_ONCE(dataAvailable, true);
		wake_up_interruptible(&wait_q);
	}

	pr_alert("Bite from time: %u, Time: %u , RandomValue: %u \n", bitValue, bootTime, randomValue);
	
	return IRQ_HANDLED;
}

static ssize_t driverRead(struct file *filp, char __user *userBuffer, size_t len, loff_t *off) {
	if (dataAvailable == false){
		pr_alert("Reader sleeping\n");
        wait_event_interruptible(wait_q, (dataAvailable == true));	//ждём, когда можно будет читать
	}
	
	pr_alert("Reader is back!\n");
	randomValue = 0;
	
	for (int i = 0; i < randomBits; i++){
		//randomValue += binaryArray[i] * (1<<(randomBits - i - 1));
		binaryArray[i];
		sprintf("NICE %d", i);
	}
	//sprintf ("Our random value: %i\n", binaryArray);

	WRITE_ONCE(dataAvailable, false);
	index = 0;
	
    return len;
	
}

static int driverOpen(struct inode *inode, struct file *filp) {
    return 0;
}

static int driverClose(struct inode *inode, struct file *filp){
	return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = driverRead,
    .open = driverOpen,
    .release = driverClose
};

static int __init driverInit(void) {
    // Резервирование мажорного и минорного номеров устройства
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        pr_alert("Cannot allocate the major number..\n");
        return -1;
        
        pr_alert("Major = %d , Minor = %d..\n",MAJOR(dev),MINOR(dev));
    }
    
    // Создание класса устройства
    if ((DeviceClass = class_create(THIS_MODULE, CLASS_NAME)) == NULL) {
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    
     // Создание символьного устройства
    if (device_create(DeviceClass, NULL, dev, NULL, DEVICE_NAME) == NULL) {
        class_destroy(DeviceClass);
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    
    // Инициализация символьного устройства
    cdev_init(&CharDevice, &fops);
    
        // Добавление символьного устройства в систему
    if (cdev_add(&CharDevice, dev, 1) == -1) {
        device_destroy(DeviceClass, dev);
        class_destroy(DeviceClass);
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    
       // Получение номера прерывания мыши
    irqNum = 12; // Пример
    
    // Регистрация обработчика прерывания мыши
    if (request_irq(irqNum, mouseInterrupt, IRQF_SHARED, "random_mouse", &dev) != 0) {
        cdev_del(&CharDevice);
        device_destroy(DeviceClass, dev);
        class_destroy(DeviceClass);
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    
    //инициализация переменных
    index = 0;
    dataAvailable = false;
    
    pr_alert("Random mouse driver registered\n");
    return 0;
}    

static void __exit driverExit(void) {
	free_irq(irqNum, &dev);
	cdev_del(&CharDevice);
	device_destroy(DeviceClass, dev);
	class_destroy(DeviceClass);
	unregister_chrdev_region(dev,1);
	
	pr_alert("Random mouse driver unregistered\n");
}

module_init(driverInit);
module_exit(driverExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me & CO");
MODULE_DESCRIPTION("Random mouse driver");
	
