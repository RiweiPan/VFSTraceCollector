# VFS-Trace-Collector

用于采集内核VFS层信息的工具



## 使用方法

**第一步: 源码复制**

1. 复制本仓库的`include/linux/vfs_trace.h`到你的内核源码`include/linux`目录。

2.　根据本仓库的各个采集模块的实现，复制到你的内核源码的对应的位置。
3.　添加头文件`#include<linux/vfs_trace.h>`到需要采集的函数所在的ｃ文件的头部。

**第二步: 使用**

我们看到`include/linux/vfs_trace.h`的实现如下。通过不同的宏作为开关，采集不同函数的信息。同时也可以通过`TRACE_CONDITION`设定各个模块的采集条件。

```c
#ifndef INCLUDE_LINUX_VFS_TRACE_H_
#define INCLUDE_LINUX_VFS_TRACE_H_


#define CONFIG_VFS_FILETRACER
#ifdef CONFIG_VFS_FILETRACER

#define VT_PATH_LEN 256   // VFS Trace path name length

//#define CONFIG_VFS_TRACE_MMAP // 采集mmap

#define CONFIG_VFS_TRACE_WRITE // 采集vfs_read和vfs_write
#define CONFIG_VFS_TRACE_READ

//#define CONFIG_VFS_TRACE_WRITE_PAGEIDX // do not support now
//#define CONFIG_VFS_TRACE_READ_PAGEIDX // 采集 do_generic_read里面的page index

//#define CONFIG_VFS_TRACE_READAHEAD // 采集readahead信息

//#define CONFIG_VFS_TRACE_BIO_READ // 采集读bio
//#define CONFIG_VFS_TRACE_BIO_WRITE // 采集写bio

//#define CONFIG_VFS_TRACE_UNLINK // 采集unlink信息

// 这里设置条件，只要在这里设置好，所有的采集函数都会根据这个条件进行采集
#define TRACE_CONDITION (strstr(ss, "/data/") != NULL || strstr(ss, "/app/") != NULL) //(strstr(ss, "com.facebook.katana") != NULL)

#endif

#endif /* INCLUDE_LINUX_VFS_TRACE_H_ */
```

**第三步: 分析**

为了方便一个trace有固定的格式，约定了如下的trace格式。前三个变量分别是时间，标志位和路径，之后的信息根据各自的采集功能不一样，记录的数据的方式也不一样。

```
[vfs]时间，标志位，路径，...
```

下面说明当前功能的信息:

**vfs_read**使用`r`作为标志位

```
[vfs]时间戳,r标志位,路径,偏移量(offset),读尺寸(count),文件大小(file size),文件ino(file ino)
[   47.927354] [vfs]1535910406994,r,/data/xxxx/int.db,0,4096,229376,98523
```

**vfs_write**使用`w`作为标志位

```c
[vfs]时间戳,w标志位,路径,偏移量(offset),读尺寸(count),文件大小(file size),文件ino,是否为append写
[   47.927910] [vfs]1535910406994,w,/data/xxx/internal.db-shm,4095,1,0,98555,1
```

**do_generic_read**使用`pr`作为标志位

```c
[vfs]时间戳,pr标志位,路径,Page Index,文件大小,文件ino
[   43.286667] [vfs]1535910402920,pr,/data/xxxx/launcher.db-wal,19,119512,98465
```

**readahead**使用`rd`作为标志位

```c
[vfs]时间戳,rd标志位,路径,预读页数,文件大小,文件ino,预读index[0~31]
[   43.286667] [vfs]1535910402872,rd,/data/xxxx/icons.db,9,126976,98467,21,22,23,24,26,27,28,29,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
```

**bio-read**和**bio-write**分别使用`br`和`bw`作为标志位，可以同时打开开关，也可以只打开一个开关

```
[vfs]时间戳,br或者bw标志位,路径,bio尺寸,bio包含页数,Sector,文件大小,文件ino
[   44.757882][vfs]1535910404346,bw,/data/xxxx/Prov.db-journal,8192,2,53715160,4616,98513
[   44.795848][vfs]1535910404384,br,/data/xxxx/refs/oti.xml,4096,1,36108000,115,98521
```

## 增加新的Tracer

只需要在`include/linux/vfs_trace.h`添加开关，然后将这个头文件inlucde需要采集的函数所在的文件，然后实现就可以。最后更新这个`README`说明文件，定义了trace的含义和信息，然后配套开发处理程序。