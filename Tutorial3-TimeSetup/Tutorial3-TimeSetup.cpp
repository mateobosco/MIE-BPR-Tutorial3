// Tutorial3-TimeSetup.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int changeTime(const SYSTEMTIME time) {
	HANDLE      hToken;
	TOKEN_PRIVILEGES tp;
	TOKEN_PRIVILEGES oldtp;
	DWORD    dwSize = sizeof(TOKEN_PRIVILEGES);
	LUID     luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		printf("OpenProcessToken() failed with code %d\n", GetLastError());
		return -1;
	}
	if (!LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid)) {
		printf("LookupPrivilege() failed with code %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	ZeroMemory(&tp, sizeof(tp));
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
		&oldtp, &dwSize))
	{
		printf("AdjustTokenPrivileges() failed with code %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	if (!SetSystemTime(&time)) {
		printf("SetSystemTime() failed with code %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	AdjustTokenPrivileges(hToken, FALSE, &oldtp, dwSize, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS) {
		printf("AdjustTokenPrivileges() failed with code %d\n", GetLastError());
		CloseHandle(hToken);
		return -1;
	}

	CloseHandle(hToken);
	return 0;
}

int showTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
	return 0;
}


int main(int argc, char *argv[]) {

	printf("argc = %d \n", argc);
	for (int i = 0; i < argc; i++) {
		std::cout << "argv["<<i<<"] = " << std::string(argv[i]) << std::endl;
	}

	showTime();

	SYSTEMTIME st;
	GetSystemTime(&st);
	st.wHour++;
	changeTime(st);
	showTime();

	//system("pause");

    return 0;
}

