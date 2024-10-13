#ifndef __UMT_H__
#define __UMT_H__

#include <stdio.h>

#define UMT_NO_LOG	0
#define UMT_DBG_LOG	1
#define UMT_INFO_LOG	2
#define UMT_ERR_LOG	3

#define UMT_LOG_LEVEL	UMT_INFO_LOG

#define UMT_LOGGER	fprintf

#if UMT_LOG_LEVEL == UMT_NO_LOG
#define UMT_LOG(lvl, fmt, ...)
#elif UMT_LOG_LEVEL == UMT_DBG_LOG
#define UMT_LOG(lvl, fmt, ...) ({								\
	do {											\
		switch ((lvl)) {								\
		case UMT_DBG_LOG:								\
			UMT_LOGGER(stdout, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		case UMT_INFO_LOG:								\
			UMT_LOGGER(stdout, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		case UMT_ERR_LOG:								\
			UMT_LOGGER(stderr, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		default:									\
			break;									\
		}										\
	} while (0);										\
})
#elif UMT_LOG_LEVEL == UMT_INFO_LOG
#define UMT_LOG(lvl, fmt, ...) ({								\
	do {											\
		switch ((lvl)) {								\
		case UMT_INFO_LOG:								\
			UMT_LOGGER(stdout, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		case UMT_ERR_LOG:								\
			UMT_LOGGER(stderr, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		default:									\
			break;									\
		}										\
	} while (0);										\
})
#elif UMT_LOG_LEVEL == UMT_ERR_LOG
#define UMT_LOG(lvl, fmt, ...) ({								\
	do {											\
		switch ((lvl)) {								\
		case UMT_ERR_LOG:								\
			UMT_LOGGER(stderr, (fmt) __VA_OPT__(,) __VA_ARGS__);			\
			break;									\
		default:									\
			break;									\
		}										\
	} while (0);										\
})
#endif

#define UMT_BUFF_DUMP(b, s) ({									\
	do {											\
		int i;										\
		for (i = 0; i < (s); ++i)							\
			UMT_LOG(UMT_DBG_LOG, "buffer[%d] \t= 0x%X\n",				\
				i, (int)(b)[i]);						\
	} while (0);										\
})

#define UMT_BOOL_TO_STR(v) ((v) ? "true" : "false")

#define UMT_UDEV_RES_X_MAX 1920
#define UMT_UDEV_RES_Y_MAX 1200
#define UMT_MAX_FINGER_CNT 9

#endif /* __UMT_H__ */
