#include <linux/helloworld.h>

#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <exception>
#include <memory>
#include <thread>
#include <string>
#include <vector>
#include <map>

/*** HelloWorld ***/

class HelloWorld {
private:
    int fd;

public:
    HelloWorld();
    ~HelloWorld();

    class error: public std::exception {
    public:
        long retval;
        error(long retval) : retval(retval) {};
    };

    int get_fd() const { return fd; };

#define X(name, lowname, iocmaker, nr) \
    long lowname(struct helloworld_ioctl_##lowname &params) { \
        return ioctl(fd, HELLOWORLD_IOCTL_##name, &params); \
    }  \
    void lowname##_CHECK(struct helloworld_ioctl_##lowname &params) {    \
        long retval = lowname(params); \
        if (retval != 0) throw error(retval);   \
    }  \

    HELLOWORLD_IOCTLS_LIST
#undef X
};

HelloWorld::HelloWorld()
{
    fd = open("/dev/helloworld", O_RDWR);
    if (fd < 0)
        throw std::exception();

    int ret = fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
    assert(!ret);
}

HelloWorld::~HelloWorld()
{
    close(fd);
}


/*** HelloWorldTest ***/

class HelloWorldTest {
public:
    HelloWorldTest();
    void test();
    ~HelloWorldTest();
};

HelloWorldTest::HelloWorldTest()
{
    int ret = system("insmod helloworld-test.ko dyndbg='+pt'");
    assert(!ret);
}

void HelloWorldTest::test()
{
    int fd = open("/sys/module/helloworld_test/parameters/test", O_RDWR);
    assert(fd >= 0 );
    write(fd, "0\n", 2);
    close(fd);
}

HelloWorldTest::~HelloWorldTest()
{
    int ret = system("rmmod helloworld-test");
    assert(!ret);
}

/*** Controller ***/

class Controller {
public:
    static const int helloworld_count = 4;
    static const unsigned long helloworld_part_size = 0x400000;
    std::shared_ptr<HelloWorld> helloworlds[helloworld_count];

    Controller() {}
    void run();
};

void Controller::run(void)
{
    std::shared_ptr<HelloWorld> helloworld(new HelloWorld());

    struct helloworld_ioctl_create params = {};
    long ret = helloworld->create(params);
    if (ret) {
        printf("helloworld create failed: %ld\n", ret);
    }
    assert(ret == 0);
}

/* Main */

int main(int argc, char *argv[])
{
    Controller controller;
    controller.run();

    return 0;
}
