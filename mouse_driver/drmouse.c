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


#define DEVICE_NAME "random_mouse"
#define CLASS_NAME "random"

static dev_t dev;
static struct cdev CharDevice;
static struct class *DeviceClass;
static struct device *DevFile;

static int irqNum;

static irqreturn_t mouseInterrupt(int irq, void *devId) {
	//ktime_t ktime_get(void);
	unsigned int randomNumber;
	//struct timespec currentTime = current_kernel_time();
	//unsigned int timeValue = currentTime.tv_sec;
	
	get_random_bytes(&randomNumber, sizeof(randomNumber));
	//randomNumber ^= timeValue;
	unsigned int timeValue = ktime_get_boottime_seconds();
	randomNumber ^= timeValue;
	pr_alert("Random number generated: %u, Time: %u \n", randomNumber, timeValue);
	
	return IRQ_HANDLED;
}

static ssize_t driverRead(struct file *filp, char __user *userBuffer, size_t len, loff_t *off) {
	
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
	
