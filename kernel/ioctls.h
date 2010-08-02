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
