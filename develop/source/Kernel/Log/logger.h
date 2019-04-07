#ifndef __KERNEL_LOGGER_H__
#define __KERNEL_LOGGER_H__

#include <linux/string.h>
#include <linux/stringify.h>

#define __FILENAME(f) (strrchr(f,'/')?(strrchr(f,'/')+1):(f))

#ifndef __KMODULE_NAME
#define __KMODULE_NAME HostMgr
#endif

#define __LOGGER(level, fmt, ...) \
    do { \
        printk(KERN_##level __stringify(__KMODULE_NAME) \
            ": [" __stringify(level) "] [%s:%d] (%s) - " fmt "\n", \
            __FILENAME(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    } while (0)

#define LOG_ERR(fmt, ...)   __LOGGER(ERR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  __LOGGER(WARNING, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  __LOGGER(INFO, fmt, ##__VA_ARGS__)

#ifndef NDEBUG
#define LOG_DEBUG(fmt, ...) __LOGGER(DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#endif /* !__KERNEL_LOGGER_H__ */
