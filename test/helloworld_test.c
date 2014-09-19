/*
 *  Version 1.0
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/helloworld.h>

#define __test_print(__pr, prefix, fmt, arg...)  __pr("helloworld-testing: " prefix fmt "\n", ## arg)

#define test_debug(arg...)  __test_print(pr_debug, "debug - ", ## arg)
#define test_error(arg...)  __test_print(pr_err, "error - ", ## arg)
#define test_info(arg...)   __test_print(pr_info, "", ## arg)

static unsigned int test;

static int test_func(const char *val, struct kernel_param *kp)
{
	test_info("test func");
        return 0;
}

module_param_call(test, test_func, param_get_uint, &test, S_IRUGO | S_IWUSR);

static int testing_init(void)
{
	test_debug("loading");
	return 0;
}

static void testing_exit(void)
{
	test_debug("unloading");
}

module_init(testing_init);
module_exit(testing_exit);

MODULE_AUTHOR("Dan Aloni <dan@kernelim.com>");
MODULE_LICENSE("GPL");
