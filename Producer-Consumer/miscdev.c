#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#define WORDLENGTH 10


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay Tapar and Omkar Nibandhe");
MODULE_DESCRIPTION("FIFO queue for Producer Consumer Problem ");
MODULE_VERSION("1.0");

//Declare and define semaphore

static DEFINE_SEMAPHORE(semFull);
static DEFINE_SEMAPHORE(semEmpty);
static DEFINE_MUTEX(mutVar);
static int max;
	

module_param(max,int,0);


          
 

//Queue that stores N strings

static int rear= -1;
static int front= -1;
static char **queue_Array;

static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);



//File operations structure
static const struct file_operations my_fops = {
        .owner                = THIS_MODULE,
   	.read = dev_read,
   	.write = dev_write,
   	        
};


//Miscdevice structure
static struct miscdevice my_misc_device = {
.minor = MISC_DYNAMIC_MINOR,
.name = "miscdev",
.fops = &my_fops
};


//Init Module

static int __init load_init(void)
{
        int ret;
	int i=0;
	ret = misc_register(&my_misc_device);
        if (ret)
                printk(KERN_ERR "Unable to register \"Fifo Queue\" misc device\n");
	else
	 {
        	printk("Device Registered\n");	
        	sema_init(&semFull,0);			//Intializing Semaphores
       		sema_init(&semEmpty,max);
         }       	
	printk("MAX->%d\n",max);	
	queue_Array= kmalloc(sizeof(char*)*WORDLENGTH,GFP_KERNEL);
	if(queue_Array==NULL)
	 {
 		printk("Memory Allocation Error\n");
	 }
	for(i=0;i<max;i++){
	queue_Array[i]=kmalloc(sizeof(char)*max,GFP_KERNEL);	
	if(queue_Array[i]==NULL)
	 {
 		printk("Memory Allocation Error\n");
	 }
	}	
return ret;
}

module_init(load_init);


//Exit Function
static void __exit unload_exit(void)
{
        
	int i;	
	for(i=0;i<max;i++){
	kfree(queue_Array[i]);	
	queue_Array[i]=NULL;
	}
	kfree(queue_Array);
	misc_deregister(&my_misc_device);
}




 
//Read Function


static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   
   int error_count = 0;
   int ret;
   int mutret;	
   int value;
   ret = down_interruptible(&semFull);
   if (ret!= 0) 
   {
	printk("Process Wait on semaphore Full\n");
        return ret;
   }
 
   
   mutret = mutex_lock_interruptible(&mutVar);
   if (mutret!= 0) 
    {
        printk("Process Wait on mutex read\n");
	up(&semFull);
	return mutret;
     }
	//-----------Critical Section---------------------	      
		value=strlen(queue_Array[front]);		
   		error_count = copy_to_user(buffer, queue_Array[front],strlen(queue_Array[front])+1);
		
		if (error_count==0)
		{            
		   
		      if(front== rear)
			{
			  front=-1;
			  rear=-1;				
			}
			else
			{
		          if(front==max-1)//
				front=0;
			  else
				front=front+1;
			}
		}  
		      
	   	else 
		{
	      	      printk(KERN_INFO " Failed to send %d characters to the user\n", error_count);
	      	      mutex_unlock(&mutVar);		//Release locks/mutex
    		      up(&semEmpty);			//Release locks/mutex
		      return -EFAULT;  
		}          
		
		
  	       	      mutex_unlock(&mutVar);		//Release locks/mutex
    		       up(&semEmpty);			//Release locks/mutex
		
	      	      
	return value;  
 }


//Write Function

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
                 
   int ret;
   int mutret;
   static char   message[256] = {0};           
   static short  size_of_message;   
   //sprintf(message, "%s", buffer); 
   int error_count=0;
   
   error_count = copy_from_user(message, buffer,len);
   if(error_count!=0)
   {
	
	printk(KERN_INFO " Failed to send %d characters to the user\n", error_count);    
	return -EFAULT; 	          	
   }
				 
   size_of_message = strlen(message);  
		
   ret = down_interruptible(&semEmpty);
   if (ret!= 0) 
   {
	printk("Process Wait on semaphore Empty\n");
        return ret;
   }
 
   mutret = mutex_lock_interruptible(&mutVar);
   if (mutret!= 0) 
   {
     printk("Process Wait on mutex write\n");
     up(&semFull);							//LOCK
     return mutret;	
   }
 //------------Critical Section-----------------------
	
   if(front==-1 && rear==-1)
	front=rear=0;

   else	
	  if(rear==max-1)//
		rear=0;
	  else		
		rear=rear+1;

   strcpy(queue_Array[rear],message);	
   printk("QUEUE element %s",queue_Array[rear]);
   mutex_unlock(&mutVar);						//UNLOCK
   up(&semFull);							//UNLOCK
   printk(KERN_INFO "Received %zu characters from the user\n", len);	
   return len;
}



module_exit(unload_exit);


