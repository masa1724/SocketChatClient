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

	// winsock2�̏�����
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
		errorlog(TEXT("winsock2�̏������Ɏ��s���܂����B"));
		return -1;
	}

	// �R�l�N�V�����\�P�b�g�̍쐬
	consock = socket(AF_INET, SOCK_STREAM, 0);
	if (consock == INVALID_SOCKET) {
		errCode = WSAGetLastError();
		errmsg = geterrormessage(errCode);
		errorlog(TEXT("�R�l�N�V����Socket�̐����Ɏ��s���܂����B�G���[�ڍ�:[%s] �G���[�R�[�h:[%d]"), errmsg, errCode);
		return -1;
	}

	/*** �ڑ���T�[�o�̃A�h���X����ݒ� ***/
	memset(&srvaddr, 0, sizeof(srvaddr));
	// �t�@�~���[
	srvaddr.sin_family = AF_INET;
	// �|�[�g�ԍ�
	srvaddr.sin_port = htons(SRV_PORT);
	// IP�A�h���X
	srvaddr.sin_addr.S_un.S_addr = inet_addr(SRV_IP_ADDR);

	// �T�[�o�ɐڑ�
	if (connect(consock, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) != 0) {
		int errCode = WSAGetLastError();
		errCode = WSAGetLastError();
		errmsg = geterrormessage(errCode);
		errorlog(TEXT("�T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B�G���[�ڍ�:[%s] �G���[�R�[�h:[%d] IPAddress:[%s] Port:[%d]"),
				errmsg, errCode, inet_ntoa(srvaddr.sin_addr), ntohs(srvaddr.sin_port));
		return -1;
	}

	/*
	// �T�[�o����f�[�^����M
	 memset(recvmsg, 0, sizeof(recvmsg));
	 if (recv(consock, recvmsg, sizeof(recvmsg), 0) <= 0) {
		 errCode = WSAGetLastError();
		 errmsg = geterrormessage(errCode);
		 errorlog(TEXT("�T�[�o�[����̃��b�Z�[�W��M�Ɏ��s���܂����B�G���[�ڍ�:[%s] �G���[�R�[�h:[%d]"),
				 errmsg, errCode);
		 return -1;
	 }

	 infolog(TEXT("�T�[�o�[���烁�b�Z�[�W����M���܂����BMessage:[%s] Length:[%d]"),
			 recvmsg , _tcslen(recvmsg));

	 Sleep(1);
	*/
	 // 5�������M
	 _tcscpy(sendmsg, msg);

	 // �T�[�o�[�փ��b�Z�[�W�𑗐M
	 if (send(consock, sendmsg, sizeof(sendmsg), 0) <= 0) {
		 errCode = WSAGetLastError();
		 errmsg = geterrormessage(errCode);
		 errorlog(TEXT("�T�[�o�[�ւ̃��b�Z�[�W���M�Ɏ��s���܂����B�G���[�ڍ�:[%s] �G���[�R�[�h:[%d]"), errmsg, errCode);
		 return -1;
	 }

	 infolog(TEXT("�T�[�o�[�փ��b�Z�[�W�𑗐M���܂����BMessage:[%s] Length:[%d]"),
			 sendmsg , _tcslen(sendmsg));

	// winsock2�̏I������
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
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ����̌���
	    (LPTSTR)&msg,
	    0,
	    NULL
	);

	return (TCHAR*)msg;
}
