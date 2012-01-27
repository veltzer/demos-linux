#ifndef __shared_h
#define __shared_h

typedef struct _BufferStruct {
	void         *pointer;
	unsigned int size;
} BufferStruct;

// randomly chosen
#define DEMO_MAGIC                      'd'

#define IOCTL_DEMO_MAP                  _IO(DEMO_MAGIC, 0)
#define IOCTL_DEMO_UNMAP                _IO(DEMO_MAGIC, 1)
#define IOCTL_DEMO_READ                 _IO(DEMO_MAGIC, 2)
#define IOCTL_DEMO_WRITE                _IO(DEMO_MAGIC, 3)
#define IOCTL_DEMO_CHECK                _IO(DEMO_MAGIC, 4)
#define IOCTL_DEMO_COPY                 _IO(DEMO_MAGIC, 5)
#define IOCTL_DEMO_KMALLOC              _IO(DEMO_MAGIC, 6)
#define IOCTL_DEMO_GET_FREE_PAGES       _IO(DEMO_MAGIC, 7)
#define IOCTL_DEMO_PCI_ALLOC_CONSISTENT _IO(DEMO_MAGIC, 8)
#define IOCTL_DEMO_DMA_ALLOC_COHERENT   _IO(DEMO_MAGIC, 9)

// timing ioctls
#define IOCTL_TIMING_CLOCK		_IO(DEMO_MAGIC, 10)
#define IOCTL_TIMING_TSC		_IO(DEMO_MAGIC, 11)
#define IOCTL_TIMING_JIFFIES		_IO(DEMO_MAGIC, 12)
#define IOCTL_TIMING_EMPTY		_IO(DEMO_MAGIC, 13)

typedef struct _buffer
{
	unsigned long long u1;
	unsigned long long u2;
	unsigned long long udiv;
	unsigned long long umul;
	unsigned long long uadd;
	unsigned long long usub;
	long long d1;
	long long d2;
	long long ddiv;
	long long dmul;
	long long dadd;
	long long dsub;
} buffer;

#endif // __shared_h
