// log.h
#ifndef LOG_H
#define LOG_H
#include "opcodes.h"
#include <stdio.h>
#include <string>
#include <stdarg.h> // 添加对可变参数的支持

// 定义日志级别
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// 函数声明
void log_messages(LogLevel level, const char *file, int line, const char *format, ...);
void log_address(LogLevel level, const char *file, int line, const char *format, ...);
void log_buffer(LogLevel level, const char *file, int line, const char *format, ...);
void log_IMA(LogLevel level, const char *file, int line, const char *format, ...);
void log_calculate(LogLevel level, const char *file, int line, const char *format, ...);
void log_missmatch(LogLevel level, const char *file, int line, const char *format, ...);
void log_IndexArray(LogLevel level, const char *file, int line, const char *format, ...);
// 宏定义，用于简化日志记录
#define LOG_TXT(level, format, ...) log_messages(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_load_store(level,format,...) log_address(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_Buffer(level,format,...) log_buffer(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_IMA(level,format,...) log_IMA(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_CALCULATE(level,format,...) log_calculate(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_Missmatch(level,format,...) log_missmatch(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_Index(level,format,...) log_IndexArray(level, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif // LOG_H