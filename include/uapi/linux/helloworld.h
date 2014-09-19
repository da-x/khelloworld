#ifndef __LINUX_HELLOWORLD_UAPI_H__
#define __LINUX_HELLOWORLD_UAPI_H__

/* Userspace-facing API */

#include <linux/ioctl.h>
#include <linux/types.h>

#define HELLOWORLD_IOCTL_MAGIC 's'

/* Here we have all the list of ioctls along with their structs */

#define HELLOWORLD_IOCTLS_LIST				       \
	/* name,	    lowname,	    iocmaker,	nr) */ \
	X(CREATE,           create,         _IOWR,	1)     \

enum helloworld_uapi_code {
	HELLOWORLD_UAPI_CODE_OK=0,
};

/*
 * IOCTL: bind
 */

struct helloworld_ioctl_create {
	/* Returned */
	__u32 uapi_code;
};

/*
 * The following generates the ioctl enum:
 */
#define X(name, lowname, iocmaker, nr) \
	HELLOWORLD_IOCTL_##name = iocmaker(HELLOWORLD_IOCTL_MAGIC, nr, \
					struct helloworld_ioctl_##lowname), \

enum helloworld_ioctls {
	HELLOWORLD_IOCTLS_LIST
};

#undef X

#endif
