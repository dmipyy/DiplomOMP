#include "libsDriver.h"

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
	
	/*
	//Выделяем память под буфер
    if((circBuffer->buf = kmalloc(circBufferLen, GFP_KERNEL)) == 0)
	{
        pr_alert("cannot allocate memory in kernel\n");
		kfree(circBuffer);
        return -1;
    }*/
	
	circBuffer->buf = kmalloc(circBufferLen, GFP_KERNEL);
	if (circBuffer->buf == 0)
	{
		//ret = -ENOMEM;
		pr_alert("driver | Error allocationg mem for circle buffer\n");
		kfree(circBuffer);
		return -1;
	}
	
	circBuffer->head = 0;
	circBuffer->tail = 0;
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
	kfree(circBuffer->buf);
	kfree(circBuffer);
    device_destroy(DeviceClass, device);
    class_destroy(DeviceClass);
    cdev_del(&CharDevice);
    unregister_chrdev_region(device,1);
    pr_alert("Device driver is removed successfully..\n");
}

static int CharDriverOpen(struct inode *inode, struct file *file)
{
	driverOpenCounter++;
	try_module_get(THIS_MODULE);
    pr_alert("Device file opened...!!!!\n");
    return 0;
}

static int CharDriverRelease(struct inode *inode , struct file *file)
{
	driverOpenCounter--;
	pr_alert("Device file closed.....!!!\n");
    return 0;
}

static ssize_t CharDriverRead(struct file *filp, char __user *userBuffer, size_t messageLength, loff_t *off)
{
	int head, tail, byteCounter, outCounter,
	remainder, seqLength, newTail, newEnd;
	
	head = READ_ONCE(circBuffer->head);
	tail = READ_ONCE(circBuffer->tail);
	byteCounter = CIRC_CNT(head, tail, circBufferLen);
	pr_alert("driver | Read | Head: %d, Tail: %d, Bytes: %d\n", head, tail, byteCounter);
	
	if (byteCounter >= messageLength)
	{
		outCounter = messageLength;
	}
	else if (byteCounter > 0)
	{
		outCounter = byteCounter;
		//WRITE_ONCE(dataAvailable, false);
	}
	else
	{
		//WRITE_ONCE(dataAvailable, false);
		return 0;
	}
	
	newEnd = CIRC_CNT_TO_END(head, tail, circBufferLen) + 1;
	remainder = outCounter % newEnd;
	seqLength = outCounter - remainder;
	newTail = (tail + outCounter) & (circBufferLen - 1);
	pr_alert("driver | Read | End: %d, Rem: %d, Seq: %d, Out: %d, Tail: %d\n", newEnd, remainder, seqLength, outCounter, newTail);
	
	outCounter = outCounter - copy_to_user(userBuffer, circBuffer->buf + tail, remainder); //сколько байтов на самом деле записали
	outCounter = outCounter - copy_to_user(userBuffer + remainder, circBuffer->buf, seqLength);
	newTail = (tail + outCounter) & (circBufferLen - 1);
	WRITE_ONCE(circBuffer->tail, newTail);
	
	return outCounter;
	
	/*
	//сделать проверку на количество открытий
    pr_alert("Reading to %c\n", &userBuffer);
    //char localBuffer[1024] = {"Hello World"};
    pr_alert("Sending message: %s\n", ioBuffer);
    int lostBytes = copy_to_user(userBuffer, ioBuffer, messageLength);
    pr_alert("Lost bytes %d\n",lostBytes);

    return messageLength;
	*/
}

static ssize_t CharDriverWrite(struct file *filp, const char __user *userBuffer,size_t maxLength, loff_t *off)
{
	int head, tail, space, newHead;
	int realMessageLength = 0;
	
    copy_from_user(&(data.message), userBuffer, maxLength);
	//считаем настоящую длину сообщения
    char *ptr;
    ptr = data.message;
    for(int i = 0; i <= maxLength ; ++i)
	{
		realMessageLength++;
        ++ptr;
        if(*ptr == '\0') break;	
    }
	
	head = READ_ONCE(circBuffer->head);
	tail = READ_ONCE(circBuffer->tail);
	space = CIRC_SPACE(head, READ_ONCE(circBuffer->tail), circBufferLen);
	pr_alert("driver | Write | Writing. Head: %d, Tail: %d, Space: %d, Bytes: %lu\n", head, tail, space, realMessageLength);
	
	if (space >= realMessageLength)
	{
		int remainder = realMessageLength % (CIRC_SPACE_TO_END(head, tail, circBufferLen) + 1);
		int seqLength = realMessageLength - remainder;
		newHead = (head + realMessageLength) & (circBufferLen - 1);
		pr_alert("drivier : Write : Rem %d, Seq %d, New Head: %d\n", remainder, seqLength, newHead);
		
		copy_from_user(circBuffer->buf + head, userBuffer, remainder);
		copy_from_user(circBuffer->buf, userBuffer + remainder, seqLength);
		WRITE_ONCE(circBuffer->head, newHead);
		//WRITE_ONCE(dataAvailable,true);
		
		//wake_up_interruptible(&wait_q);
	}
	else
	{
		int remainder, seqLength;
		head = 0;
		tail = 0;
		
		WRITE_ONCE(circBuffer->tail, tail);
		WRITE_ONCE(circBuffer->head, head);
		
		remainder = realMessageLength % (CIRC_SPACE_TO_END(head, tail, circBufferLen) + 1);
		seqLength = realMessageLength - remainder;
		newHead = (head + realMessageLength) & (circBufferLen - 1);
		printk("driver | Flush | Rem: %d, Seq: %d, New Head: %d\n", remainder, seqLength, newHead);
		
		copy_from_user(circBuffer->buf + head, userBuffer, remainder);
		copy_from_user(circBuffer->buf, userBuffer + remainder, seqLength);
		WRITE_ONCE(circBuffer->head, newHead);
		//WRITE_ONCE(dataAvailable,true);
		
	}
	
	return realMessageLength;
		
		
    /*		
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
	*/
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
        	if (dataAvailable == false || writersCounter > 0)
        	{
        		pr_alert("Reader sleeping\n");
        		wait_event_interruptible(wait_q, (dataAvailable == true));	//ждём, когда можно будет читать		
        	}
        	pr_alert("Reader is going to read %d bytes\n", ioBufferLen);
        	CharDriverRead(filp, (char*)address, (size_t)(ioBufferLen*sizeof(char)),NULL);
			WRITE_ONCE(dataAvailable,false);	
			wake_up_interruptible(&wait_q);
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
