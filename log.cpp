// log.c
#include "log.h"
#include <time.h> // 用于获取当前时间
#include <stdarg.h> // 用于可变参数
#include <stdio.h>
#include <string.h>

// 实现日志记录函数
void log_messages(LogLevel level, const char *file, int line, const char *format, ...) {
    // 获取当前时间
    time_t now = time(NULL);
    char timeBuffer[26];
    strftime(timeBuffer, 26, "%Y-%m-%d %H", localtime(&now));
    char logFileName[50];
    snprintf(logFileName, sizeof(logFileName), "log_%s.txt", timeBuffer);
    FILE *logFile = fopen(logFileName, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }
    // 根据日志级别添加前缀
    const char *level_str;
    switch (level) {
        case LOG_INFO:   level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:  level_str = "ERROR"; break;
        default:         level_str = "UNKNOWN"; break;
    }

    // 写入日志信息
    va_list args;
    va_start(args, format);
    // fprintf(logFile, "[%s] %s (%s:%d): ", level_str, timeBuffer, file, line);
    fprintf(logFile, "(%s:%d): ",  file, line);
    vfprintf(logFile, format, args);
    fprintf(logFile, "\n");
    va_end(args);

    fclose(logFile);
}
void log_address(LogLevel level, const char *file, int line, const char *format, ...) {
    // 获取当前时间
    time_t now = time(NULL);
    char timeBuffer[26];
    strftime(timeBuffer, 26, "%Y-%m-%d %H", localtime(&now));
    char logFileName[50];
    snprintf(logFileName, sizeof(logFileName), "log_address_%s.txt", timeBuffer);
    FILE *logFile = fopen(logFileName, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }
    // 根据日志级别添加前缀
    const char *level_str;
    switch (level) {
        case LOG_INFO:   level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:  level_str = "ERROR"; break;
        default:         level_str = "UNKNOWN"; break;
    }

    // 写入日志信息
    va_list args;
    va_start(args, format);
    // fprintf(logFile, "[%s] %s (%s:%d): ", level_str, timeBuffer, file, line);
    // fprintf(logFile, "(%s:%d): ",  file, line);
    fprintf(logFile, "(%d): ",  line);
    vfprintf(logFile, format, args);
    fprintf(logFile, "\n");
    va_end(args);

    fclose(logFile);
}
std::string  opcodeStr(uint8_t code){
     switch (code) {
        case NOP:    return "NOP\0";
        case ADD:    return "ADD\0";
        case SUB:    return "SUB\0";
        case MUL:    return "MUL\0";
        case SEXT:   return "SEXT\0";
        case DIV:    return "DIV\0";
        case LS:     return "LS\0";
        case RS:     return "RS\0";
        case ARS:    return "ARS\0";
        case AND:    return "AND\0";
        case OR:     return "OR\0";
        case XOR:    return "XOR\0";
        case SELECT: return "SELECT\0";
        case CMERGE: return "CMERGE\0";
        case CMP:    return "CMP\0";
        case CLT:    return "CLT\0";
        case BR:     return "BR\0";
        case CGT:    return "CGT\0";
        case LOADCL: return "LOADCL\0";
        case MOVCL:  return "MOVCL\0";
        case LOAD:   return "LOAD\0";
        case LOADH:  return "LOADH\0";
        case LOADB:  return "LOADB\0";
        case STORE:  return "STORE\0";
        case STOREH: return "STOREH\0";
        case STOREB: return "STOREB\0";
        case JUMPL:  return "JUMPL\0";
        case MOVC:   return "MOVC\0";
        default:     return "UNKNOWN_OPCODE\0";
    }

}

// 实现日志记录函数
void log_buffer(LogLevel level, const char *file, int line, const char *format, ...) 
{
    // 获取当前时间
    time_t now = time(NULL);
    char timeBuffer[26];
    strftime(timeBuffer, 26, "%Y-%m-%d %H", localtime(&now));
    char logFileName[50];
    snprintf(logFileName, sizeof(logFileName), "Buffer_log_%s.txt", timeBuffer);
    FILE *logFile = fopen(logFileName, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }
    // 根据日志级别添加前缀
    const char *level_str;
    switch (level) {
        case LOG_INFO:   level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:  level_str = "ERROR"; break;
        default:         level_str = "UNKNOWN"; break;
    }

    // 写入日志信息
    va_list args;
    va_start(args, format);
    // fprintf(logFile, "[%s] %s (%s:%d): ", level_str, timeBuffer, file, line);
    fprintf(logFile, "(%d): ", line);
    // fprintf(logFile, "(%s:%d): ",  file, line);
    vfprintf(logFile, format, args);
    fprintf(logFile, "\n");
    va_end(args);

    fclose(logFile);
}

void log_IMA(LogLevel level, const char *file, int line, const char *format, ...){
    // 获取当前时间
    time_t now = time(NULL);
    char timeBuffer[26];
    strftime(timeBuffer, 26, "%Y-%m-%d %H", localtime(&now));
    char logFileName[50];
    snprintf(logFileName, sizeof(logFileName), "IMA_log_%s.txt", timeBuffer);
    FILE *logFile = fopen(logFileName, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }
    // 根据日志级别添加前缀
    const char *level_str;
    switch (level) {
        case LOG_INFO:   level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:  level_str = "ERROR"; break;
        default:         level_str = "UNKNOWN"; break;
    }

    // 写入日志信息
    va_list args;
    va_start(args, format);
    // fprintf(logFile, "[%s] %s (%s:%d): ", level_str, timeBuffer, file, line);
    fprintf(logFile, "(%d): ", line);
    // fprintf(logFile, "(%s:%d): ",  file, line);
    vfprintf(logFile, format, args);
    fprintf(logFile, "\n");
    va_end(args);

    fclose(logFile);
}


void log_calculate(LogLevel level, const char *file, int line, const char *format, ...){
    // 获取当前时间
    time_t now = time(NULL);
    char timeBuffer[26];
    strftime(timeBuffer, 26, "%Y-%m-%d %H", localtime(&now));
    char logFileName[50];
    snprintf(logFileName, sizeof(logFileName), "calculate_log_%s.txt", timeBuffer);
    FILE *logFile = fopen(logFileName, "a");
    if (logFile == NULL) {
        perror("Failed to open log file");
        return;
    }
    // 根据日志级别添加前缀
    const char *level_str;
    switch (level) {
        case LOG_INFO:   level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:  level_str = "ERROR"; break;
        default:         level_str = "UNKNOWN"; break;
    }

    // 写入日志信息
    va_list args;
    va_start(args, format);
    // fprintf(logFile, "[%s] %s (%s:%d): ", level_str, timeBuffer, file, line);
    fprintf(logFile, "(%d): ", line);
    // fprintf(logFile, "(%s:%d): ",  file, line);
    vfprintf(logFile, format, args);
    fprintf(logFile, "\n");
    va_end(args);

    fclose(logFile);
}