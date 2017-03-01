#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <unistd.h>
#include <WInerror.h>
#include "log.h"

#define SRV_IP_ADDR "127.0.0.1"

#define SRV_PORT 12345

int socketSend(TCHAR *msg);
char* geterrormessage(int errcd);

int
main() {
	TCHAR msg[1024] = "testaaa";
	socketSend(msg);
	return 0;
}

int
socketSend(TCHAR *msg) {
	WSADATA wsaData;
	SOCKET consock;
	struct sockaddr_in srvaddr;
	TCHAR sendmsg[1024];
	int errCode;
	TCHAR *errmsg;

	// winsock2の初期化
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
		errorlog(TEXT("winsock2の初期化に失敗しました。"));
		return -1;
	}

	// コネクションソケットの作成
	consock = socket(AF_INET, SOCK_STREAM, 0);
	if (consock == INVALID_SOCKET) {
		errCode = WSAGetLastError();
		errmsg = geterrormessage(errCode);
		errorlog(TEXT("コネクションSocketの生成に失敗しました。エラー詳細:[%s] エラーコード:[%d]"), errmsg, errCode);
		return -1;
	}

	/*** 接続先サーバのアドレス情報を設定 ***/
	memset(&srvaddr, 0, sizeof(srvaddr));
	// ファミリー
	srvaddr.sin_family = AF_INET;
	// ポート番号
	srvaddr.sin_port = htons(SRV_PORT);
	// IPアドレス
	srvaddr.sin_addr.S_un.S_addr = inet_addr(SRV_IP_ADDR);

	// サーバに接続
	if (connect(consock, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) != 0) {
		int errCode = WSAGetLastError();
		errCode = WSAGetLastError();
		errmsg = geterrormessage(errCode);
		errorlog(TEXT("サーバーへの接続に失敗しました。エラー詳細:[%s] エラーコード:[%d] IPAddress:[%s] Port:[%d]"),
				errmsg, errCode, inet_ntoa(srvaddr.sin_addr), ntohs(srvaddr.sin_port));
		return -1;
	}

	/*
	// サーバからデータを受信
	 memset(recvmsg, 0, sizeof(recvmsg));
	 if (recv(consock, recvmsg, sizeof(recvmsg), 0) <= 0) {
		 errCode = WSAGetLastError();
		 errmsg = geterrormessage(errCode);
		 errorlog(TEXT("サーバーからのメッセージ受信に失敗しました。エラー詳細:[%s] エラーコード:[%d]"),
				 errmsg, errCode);
		 return -1;
	 }

	 infolog(TEXT("サーバーからメッセージを受信しました。Message:[%s] Length:[%d]"),
			 recvmsg , _tcslen(recvmsg));

	 Sleep(1);
	*/
	 // 5文字送信
	 _tcscpy(sendmsg, msg);

	 // サーバーへメッセージを送信
	 if (send(consock, sendmsg, sizeof(sendmsg), 0) <= 0) {
		 errCode = WSAGetLastError();
		 errmsg = geterrormessage(errCode);
		 errorlog(TEXT("サーバーへのメッセージ送信に失敗しました。エラー詳細:[%s] エラーコード:[%d]"), errmsg, errCode);
		 return -1;
	 }

	 infolog(TEXT("サーバーへメッセージを送信しました。Message:[%s] Length:[%d]"),
			 sendmsg , _tcslen(sendmsg));

	// winsock2の終了処理
	WSACleanup();

	return 0;
}

TCHAR* geterrormessage(int errcd){
	LPVOID msg;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	    FORMAT_MESSAGE_FROM_SYSTEM |
	    FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
		errcd,
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
	    (LPTSTR)&msg,
	    0,
	    NULL
	);

	return (TCHAR*)msg;
}
