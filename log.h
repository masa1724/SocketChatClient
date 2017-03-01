
#ifndef LIB_LOG_H_
#define LIB_LOG_H_

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <tchar.h>
#include <windows.h>
#include <stdarg.h>


#define infolog(format, ...) _infolog(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define warnlog(format, ...) _warnlog(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define errorlog(format, ...) _errorlog(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

/*
 *  ログファイルへメッセージを出力する.
 *  ※下記関数は直接呼び出さず、上記のマクロを介して呼び出して下さい。
 *
 *  @param *fileName : ファイル名
 *  @param *funcName : 関数名
 *  @param lineNum   : 行番号
 *  @param *format   : 書式化文字列
 *  @param ...       : 可変長パラメータ
 *
 */
void _infolog(const TCHAR *fileName, const TCHAR *funcName, int line, const TCHAR *format, ...);
void _warnlog(const TCHAR *fileName, const TCHAR *funcName, int line, const TCHAR *format, ...);
void _errorlog(const TCHAR *fileName, const TCHAR *funcName, int line, const TCHAR *format, ...);

#endif /* LIB_LOG_H_ */
