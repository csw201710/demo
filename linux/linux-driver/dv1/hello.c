#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>

#define    HELLO_MAJOR     231
#define    DEVICE_NAME     "HelloModule"

static int hello_open(struct inode *inode, struct file *file){
    printk(KERN_EMERG "hello open.\n");
    return 0;
}

static int hello_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos){
    //int i=0;
    printk(KERN_EMERG "hello write.\n");
    printk(KERN_EMERG "[%s] buf:%x\n"
                   "   count:%d"
                     ,__FUNCTION__, buf,count);
    printk(KERN_EMERG "buf[0]: %x\n", ((int*)buf)[0]);
    printk(KERN_EMERG "\n");
    return 0;
}

static struct file_operations hello_flops = {
    .owner  =   THIS_MODULE,
    .open   =   hello_open,     
    .write  =   hello_write,
};

static int __init hello_init(void){
    int ret;
    
    ret = register_chrdev(HELLO_MAJOR,DEVICE_NAME, &hello_flops);
    if (ret < 0) {
      printk(KERN_EMERG DEVICE_NAME " can't register major number.\n");
      return ret;
    }
    printk(KERN_EMERG DEVICE_NAME " initialized.\n");
    return 0;
}

static void __exit hello_exit(void){
    unregister_chrdev(HELLO_MAJOR, DEVICE_NAME);
    printk(KERN_EMERG DEVICE_NAME " removed.\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
