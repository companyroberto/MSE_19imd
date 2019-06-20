/**
 * @archivo	myds3231.c
 * @autor	Roberto Compañy
 * @fecha	12 Junio 2019
 * @version	1.0
 * @basado en	http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
 */

#include <linux/module.h>			// Core header for loading LKMs into the kernel
#include <linux/i2c.h>
#include <linux/fs.h>				// Header for the Linux file system support
#include <linux/uaccess.h>			// Required for the copy to user function


static const struct i2c_device_id myds3231_i2c_id[] = {
    { "myds3231", 0 },
    { }
};

MODULE_DEVICE_TABLE(i2c, myds3231_i2c_id);

static const struct of_device_id myds3231_of_match[] = {
    { .compatible = "mse,myds3231" },
    { }
};

MODULE_DEVICE_TABLE(of, myds3231_of_match);

/******************************************************************************************************/

MODULE_LICENSE("GPL");				         //< The license type -- this affects available functionality
MODULE_AUTHOR("Roberto Compañy");		   //< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Driver para ds3231");	//< The description -- see modinfo
MODULE_VERSION("1.0");				         //< A version number to inform users

#define  DEVICE_NAME "myds3231"			   //< The device will appear at /dev/myds3231Char using this value
#define  CLASS_NAME  "i2c"			         //< The device class -- this is a character device driver

static int    majorNumber;			         //< Stores the device number -- determined automatically
static int    numberOpens = 0;			   //< Counts the number of times the device is opened
static struct class*  i2cClass  = NULL;	//< The device-driver class struct pointer
static struct device* i2cDevice = NULL;	//< The device-driver device struct pointer

static struct i2c_client *modClient;
#define BUFFER_SIZE 16

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/******************************************************************************************************/


/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int myds3231_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
   modClient = client; // usado en i2c_master_send

   printk(KERN_INFO "myds3231Char: Initializing the myds3231Char LKM\n");

   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "myds3231Char failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "myds3231Char: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   i2cClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(i2cClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(i2cClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "ds3231Char: device class registered correctly\n");

   // Register the device driver
   i2cDevice = device_create(i2cClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(i2cDevice)){               // Clean up if there is an error
      class_destroy(i2cClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(i2cDevice);
   }
   printk(KERN_INFO "myds3231Char: device class created correctly\n"); // Made it! device was initialized

   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static int myds3231_remove(struct i2c_client *client)
{
   device_destroy(i2cClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(i2cClass);                          // unregister the device class
   class_destroy(i2cClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);         // unregister the major number
   printk(KERN_INFO "myds3231Char: Goodbye from the LKM!\n");

   return 0;
}

static struct i2c_driver myds3231_i2c_driver = {
    .driver = {
        .name = "myds3231",
        .of_match_table = myds3231_of_match,
    },
    .probe = myds3231_probe,
    .remove = myds3231_remove,
    .id_table = myds3231_i2c_id
};

module_i2c_driver(myds3231_i2c_driver);

/******************************************************************************************************/

/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "myds3231Char: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){

   int rv = 0;
   int error_count = 0;
   char direccion[] = {0x00};
   char ssmmhh[3];
   char hora[] = "La hora actual es: hh:mm:ss\n";

   if (len < 8){
      printk(KERN_INFO "myds3231Char: ERROR - Buffer menor de 8 bytes\n");
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }else{
      rv = i2c_master_send(modClient, direccion, 1);
      rv = i2c_master_recv(modClient, ssmmhh, 3);

      // pr_info("Datos Recibidos: %0d\n", rv);
      sprintf(hora, "%02X:%02X:%02X\n", ssmmhh[2], ssmmhh[1], ssmmhh[0]);
      // pr_info("cadena: %s", hora);

      error_count = copy_to_user(buffer, hora, 8);
      if (error_count==0){            // if true then have success
         printk(KERN_INFO "myds3231Char: Sent %d characters to the user\n", rv);
         return (rv);
      }
      else {
         printk(KERN_INFO "myds3231Char: Failed to send %d characters to the user\n", error_count);
         return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
      }
   }
}

/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   printk(KERN_INFO "myds3231Char: ERROR - No implementado\n");
   return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
}

/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
   numberOpens--;
   printk(KERN_INFO "myds3231Char: Device successfully closed\n");
   return 0;
}
