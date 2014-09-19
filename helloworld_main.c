#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#include <linux/helloworld_priv.h>

#include "helloworld_internal.h"

#define __helloworld_print(__pr, prefix, fmt, arg...)  __pr("helloworld: " prefix fmt "\n", ## arg)

#define helloworld_debug(arg...)  __helloworld_print(pr_debug, "debug - ", ## arg)
#define helloworld_error(arg...)  __helloworld_print(pr_err, "error - ", ## arg)
#define helloworld_info(arg...)   __helloworld_print(pr_info, "", ## arg)

struct helloworld_userdev {
	int foo;
};

struct helloworld_globals {
	struct kmem_cache *userdev_cache;
} helloworld_global;

/* User device */

static int helloworld_userdev_open(struct inode *inode, struct file *file)
{
	struct helloworld_userdev *helloworlddev;

	helloworld_debug("userdev open");

	helloworlddev = kmem_cache_zalloc(helloworld_global.userdev_cache,
					  GFP_KERNEL);
	if (!helloworlddev)
		return -ENOMEM;

	file->private_data = helloworlddev;
	return 0;
}

static int helloworld_userdev_release(struct inode *inode, struct file *file)
{
	struct helloworld_userdev *helloworlddev = file->private_data;

	helloworld_debug("userdev release");
	kmem_cache_free(helloworld_global.userdev_cache, helloworlddev);
	return 0;
}

static int helloworld_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
	return -EPERM;
}

static long helloworld_ioctl_create(struct helloworld_userdev *helloworlddev,
			     struct helloworld_ioctl_create *params)
{
	return 0;
}

static long helloworld_userdev_ioctl(struct file *filp, unsigned int command,
				  unsigned long arg)
{
	struct helloworld_userdev *helloworlddev = filp->private_data;

	helloworld_debug("ioctl (%x)", command);

	switch (command & 0xff) {
#define X(name, lowname, iocmaker, nr)					\
	case HELLOWORLD_IOCTL_##name & 0xff: {				\
		struct helloworld_ioctl_##lowname var = {};                \
		long ret;						\
									\
                if (_IOC_DIR(HELLOWORLD_IOCTL_##name) & _IOC_READ) {	\
			if (copy_from_user(&var, (void *)arg, sizeof(var)))	\
				return -EFAULT;				\
                }							\
									\
		ret = helloworld_ioctl_##lowname(helloworlddev, &var);		\
									\
                if (_IOC_DIR(HELLOWORLD_IOCTL_##name) & _IOC_WRITE) {	\
			if (copy_to_user((void *)arg, &var, sizeof(var))) \
				return -EFAULT;				\
                }							\
									\
		return ret;						\
	}								\

	HELLOWORLD_IOCTLS_LIST;
#undef X
	};

	return -EINVAL;
}

static struct file_operations helloworld_userdev_fops = {
	.mmap = helloworld_userdev_mmap,
	.open = helloworld_userdev_open,
	.release = helloworld_userdev_release,
	.unlocked_ioctl = helloworld_userdev_ioctl,
};

struct miscdevice helloworld_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "helloworld",
	.fops = &helloworld_userdev_fops
};


static int helloworld_init(void)
{
	int ret = -ENOMEM;

	helloworld_info("loading");

	helloworld_global.userdev_cache = KMEM_CACHE(helloworld_userdev, SLAB_HWCACHE_ALIGN);
	if (!helloworld_global.userdev_cache)
		goto error;

	ret = misc_register(&helloworld_device);
	if (ret)
		goto error;

	/* - more initialziation stuff here - */
	helloworld_info("loaded");

error:
	if (ret  &&  helloworld_global.userdev_cache) {
		kmem_cache_destroy(helloworld_global.userdev_cache);
	}

	return ret;
}

static void helloworld_exit(void)
{
	helloworld_debug("unloading");
	misc_deregister(&helloworld_device);
	kmem_cache_destroy(helloworld_global.userdev_cache);
	helloworld_info("unloaded");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
 
MODULE_AUTHOR("Dan Aloni <dan@kernelim.com>");
MODULE_LICENSE("GPL");
