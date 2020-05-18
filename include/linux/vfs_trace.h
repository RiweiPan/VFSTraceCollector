#ifndef INCLUDE_LINUX_VFS_TRACE_H_
#define INCLUDE_LINUX_VFS_TRACE_H_


#define CONFIG_VFS_FILETRACER
#ifdef CONFIG_VFS_FILETRACER

#define VT_PATH_LEN 256   // VFS Trace path name length

//#define CONFIG_VFS_TRACE_MMAP

#define CONFIG_VFS_TRACE_WRITE
#define CONFIG_VFS_TRACE_READ

//#define CONFIG_VFS_TRACE_WRITE_PAGEIDX // do not support now
//#define CONFIG_VFS_TRACE_READ_PAGEIDX

//#define CONFIG_VFS_TRACE_READAHEAD

//#define CONFIG_VFS_TRACE_BIO_READ
//#define CONFIG_VFS_TRACE_BIO_WRITE

//#define CONFIG_VFS_TRACE_UNLINK

#define TRACE_CONDITION (strstr(ss, "/data/") != NULL || strstr(ss, "/app/") != NULL) //(strstr(ss, "com.facebook.katana") != NULL)

#endif

#endif /* INCLUDE_LINUX_VFS_TRACE_H_ */
