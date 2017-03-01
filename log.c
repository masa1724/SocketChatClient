#include "log.h"

// アプリログファイル
#define LOG_DIR_PATH TEXT("C:\\var\\log\\clang\\")

// アプリログファイル
#define APL_LOG_NAME TEXT("apl.log")

// エラーログファイル
#define ERROR_LOG_NAME TEXT("error.log")

// アプリログファイル
#define LOG_BACKUP_DIR_PATH TEXT("C:\\var\\log\\clang\\backup\\")

// ログサイズの最大サイズ 2M (byte単位)
#define LOG_FILE_SIZE_MAX 20000000


/* ログ出力情報 */
typedef struct __LOG_INFO_T__ {
	TCHAR title[5];      // タイトル
	TCHAR filePath[200]; // ファイルパス
} LOG_INFO_T;


void writelog(const TCHAR *msg, LOG_INFO_T *info, const TCHAR *fileName, const TCHAR *funcName, int lineNum);
int iscyclelog(const TCHAR *filePath);
int backuplog(const TCHAR *filePath);


void
_infolog(const TCHAR *fileName, const TCHAR *funcName, int lineNum, const TCHAR *format, ...) {
	LOG_INFO_T info;
	_tcscpy(info.title, TEXT("Info"));
	_stprintf(info.filePath, TEXT("%s%s"), LOG_DIR_PATH, APL_LOG_NAME);

	TCHAR msg[1024];
	va_list args;
	va_start(args, format);
	_vstprintf(msg, format, args);
	writelog(msg, &info, fileName, funcName, lineNum);
	va_end(args);
}

void
_warnlog(const TCHAR *fileName, const TCHAR *funcName, int lineNum, const TCHAR *format, ...) {
	LOG_INFO_T info;
	_tcscpy(info.title, TEXT("Warn"));
	_stprintf(info.filePath, TEXT("%s%s"), LOG_DIR_PATH, APL_LOG_NAME);
	TCHAR msg[1024];
	va_list args;
	va_start(args, format);
	_vstprintf(msg, format, args);
	writelog(msg, &info, fileName, funcName, lineNum);
	va_end(args);
}

void
_errorlog(const TCHAR *fileName, const TCHAR *funcName, int lineNum, const TCHAR *format, ...) {
	LOG_INFO_T info;
	_tcscpy(info.title, TEXT("Err "));
	_stprintf(info.filePath, TEXT("%s%s"), LOG_DIR_PATH, ERROR_LOG_NAME);

	TCHAR msg[1024];
	va_list args;
	va_start(args, format);
	_vstprintf(msg, format, args);
	writelog(msg, &info, fileName, funcName, lineNum);
	va_end(args);
}

void
writelog(
		const TCHAR *msg,
		LOG_INFO_T *info,
		const TCHAR *fileName,
		const TCHAR *funcName,
		int lineNum) {

	struct timeval tv;
	time_t t;
	struct tm *ts;
	gettimeofday(&tv, NULL);
	t = (time_t)tv.tv_sec;
	ts = localtime(&t);

	FILE *fp;
	fp = _tfopen(info->filePath, TEXT("a"));
	if(fp == NULL) {
		_tprintf(TEXT("aaaaaaaaaaaaaaaaaaaaaa\n"));
	}

	// ログファイルへメッセージ出力
	_ftprintf(fp,
			TEXT("[%s] %04d-%02d-%02d.%02d:%02d:%02d.%06ld pgname:[%s] funcName:[%s] lineNum:[%d] %s\n"),
			info->title,
			ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec, tv.tv_usec,
			fileName, funcName, lineNum, msg);

	fclose(fp);

	// ログファイルの周期させるか判定
	if (iscyclelog(info->filePath) == 0) {
		backuplog(info->filePath);
	}
}

int
iscyclelog(const TCHAR *filePath) {
	struct _stat st;

	int ret = _tstat(filePath, &st);

	// ファイル存在しない場合
	if(ret != 0) {
		return 0;
	}

	int size = st.st_size;

	// ファイルサイズが最大値超え
	if(size >= LOG_FILE_SIZE_MAX) {
		return 0;
	}

	return -1;
}

int
backuplog(const TCHAR *filePath) {
	TCHAR *fileName;
	time_t timer;
	TCHAR newFilePath[200];

	// ファイル名を取得
	fileName = _tcsrchr(filePath, TEXT('\\')) + 1;

	// 現在日時を取得
	time(&timer);
	struct tm *ts = localtime(&timer);

	// ログファイルパスを生成
	_stprintf(newFilePath,
			 TEXT("%s%04d%02d%02d%02d%02d%02d%s"),
					LOG_BACKUP_DIR_PATH,
					ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec,
					fileName);

	// backupフォルダに移動
	_trename(filePath, newFilePath);

	return 0;
}
