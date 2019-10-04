#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Piskun");
MODULE_DESCRIPTION("Say hello and goodbye");
MODULE_VERSION("0.22");

static char *name = "user";
static int int1 = 0;
static int int2 = 0;
static unsigned char byte1 = 0;
module_param(name, charp, S_IRUGO); //just can be read
module_param(int1, int, S_IRUGO);   //just can be read
module_param(int2, int, S_IRUGO);   //just can be read
module_param(byte1, byte, S_IRUGO);   //just can be read
MODULE_PARM_DESC(name, "The name to display");  ///< parameter description

static int __init hello_init(void)
{
   printk(KERN_INFO "Hello, %s!\n", name);
   printk(KERN_INFO "int1 * int2 = %d\n", (int1 * int2));

   if (int2 != 0)
   {
       printk(KERN_INFO "int1 / int2 = %d\n", (int1 / int2));
   }
   else
   {
       printk(KERN_INFO "int1 / int2 = ERROR; int2 == 0\n");
   }

   printk(KERN_INFO "byte1 = %d\n", byte1);

   return -2;
}

static void __exit hello_exit(void)
{
   printk(KERN_INFO "Goodbye, %s!\n", name);
}

module_init(hello_init);
module_exit(hello_exit);