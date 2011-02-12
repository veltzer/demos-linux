after the device is insmod'ed you can see various bits of information about the device:

The various parameters:
cat /sys/module/pipe/parameters/debug_param
cat /sys/module/pipe/parameters/pipe_size_param
cat /sys/module/pipe/parameters/pipe_count_param

module information:
cat /sys/module/pipe/srcversion # source version
cat /sys/module/pipe/initstate # current state
cat /sys/module/pipe/refcnt # how many modules are making use of this one
more information is in the [/sys/module/pipe] folder.

The information from the /proc file system:
cat /proc/driver/pipe

The fact that the module is in:
cat /proc/modules | grep pipe
lsmod | grep pipe

The major number assigned to the module:
cat /proc/devices | grep pipe
