LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := uart_multitouch
LOCAL_SRC_FILES := umt.cpp umt_fd.cpp umt_packet.cpp umt_uart.cpp umt_uinput.cpp umt_watchdog.cpp

include $(BUILD_EXECUTABLE)
