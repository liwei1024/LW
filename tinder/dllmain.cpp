// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <tchar.h>

HMODULE g_hModule;
//
//#pragma warning(disable:4996)
//using namespace std;
//#define  NO_MODULE_MARK 0x2018FFFF
//HANDLE g_hMutex = NULL;
//TCHAR dllModuleName[MAX_PATH] = { 0 };
//TCHAR exeModuleName[MAX_PATH] = { 0 };
//
//typedef BOOL(WINAPI *DLL_MAIN)(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
////创建互斥体
//BOOL IsMutexExist(TCHAR* pstrMutex)
//{
//	BOOL bRet = FALSE;
//	HANDLE hMutex = NULL;
//
//	hMutex = CreateMutex(NULL, TRUE, pstrMutex);
//	if (hMutex)
//	{
//		if (GetLastError() == ERROR_ALREADY_EXISTS)
//			bRet = TRUE;
//		ReleaseMutex(hMutex);
//		CloseHandle(hMutex);
//	}
//	else
//	{
//		bRet = TRUE;
//	}
//	return bRet;
//}
//BOOL LoadDll2Mem(PVOID &pAllocMem, DWORD &dwMemSize, TCHAR* strFileName)
//{
//	HANDLE hFile = CreateFile(strFileName, GENERIC_READ,
//		FILE_SHARE_READ, NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL, NULL);
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		return FALSE;
//	}
//	PVOID pFileBuff = NULL;
//
//	int nFileSize = GetFileSize(hFile, NULL);
//	if (nFileSize == 0)
//	{
//		return FALSE;
//	}
//	else
//	{
//		pFileBuff = VirtualAlloc(NULL, nFileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//	}
//	DWORD dwReadSize = 0;
//	if (!ReadFile(hFile, pFileBuff, nFileSize, &dwReadSize, NULL))
//	{
//		return FALSE;
//	}
//
//	PVOID pBase = pFileBuff;
//	PIMAGE_DOS_HEADER pIDH = (PIMAGE_DOS_HEADER)pFileBuff;
//	if (IMAGE_DOS_SIGNATURE != pIDH->e_magic)
//	{
//		return FALSE;
//	}
//	PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)((ULONG)pFileBuff + pIDH->e_lfanew);
//	if (IMAGE_NT_SIGNATURE != pINH->Signature)
//	{
//		return FALSE;
//	}
//	dwMemSize = nFileSize;
//	pAllocMem = pFileBuff;
//	return TRUE;
//}
//BOOL PELoader(char *lpStaticPEBuff, PVOID& pExecuMem)
//{
//	long lPESignOffset = *(long *)(lpStaticPEBuff + 0x3c);
//	IMAGE_NT_HEADERS *pINH = (IMAGE_NT_HEADERS *)(lpStaticPEBuff + lPESignOffset);
//	long lImageSize = pINH->OptionalHeader.SizeOfImage;
//	char *lpDynPEBuff = (char *)VirtualAlloc(NULL, lImageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//	if (lpDynPEBuff == NULL)
//	{
//		return FALSE;
//	}
//	memset(lpDynPEBuff, 0, lImageSize);
//	long lSectionNum = pINH->FileHeader.NumberOfSections;
//	IMAGE_SECTION_HEADER *pISH = (IMAGE_SECTION_HEADER *)((char *)pINH + sizeof(IMAGE_NT_HEADERS));
//	memcpy(lpDynPEBuff, lpStaticPEBuff, pISH->VirtualAddress);
//	long lFileAlignMask = pINH->OptionalHeader.FileAlignment - 1;
//	long lSectionAlignMask = pINH->OptionalHeader.SectionAlignment - 1;
//	for (int nIndex = 0; nIndex < lSectionNum; nIndex++, pISH++)
//	{
//		memcpy(lpDynPEBuff + pISH->VirtualAddress, lpStaticPEBuff + pISH->PointerToRawData, pISH->SizeOfRawData);
//	}
//	if (pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size > 0)
//	{
//		IMAGE_IMPORT_DESCRIPTOR *pIID = (IMAGE_IMPORT_DESCRIPTOR *)(lpDynPEBuff + \
//			pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
//		for (; pIID->Name != NULL; pIID++)
//		{
//			IMAGE_THUNK_DATA *pITD = (IMAGE_THUNK_DATA *)(lpDynPEBuff + pIID->FirstThunk);
//			char* pLoadName = lpDynPEBuff + pIID->Name;
//			HINSTANCE hInstance = LoadLibraryA(pLoadName);
//			if (hInstance == NULL)
//			{
//				VirtualFree(lpDynPEBuff, lImageSize, MEM_DECOMMIT);
//				return FALSE;
//			}
//			for (; pITD->u1.Ordinal != 0; pITD++)
//			{
//				FARPROC fpFun;
//				if (pITD->u1.Ordinal & IMAGE_ORDINAL_FLAG32)
//				{
//					fpFun = GetProcAddress(hInstance, (LPCSTR)(pITD->u1.Ordinal & 0x0000ffff));
//				}
//				else
//				{
//					IMAGE_IMPORT_BY_NAME * pIIBN = (IMAGE_IMPORT_BY_NAME *)(lpDynPEBuff + pITD->u1.Ordinal);
//					fpFun = GetProcAddress(hInstance, (LPCSTR)pIIBN->Name);
//				}
//				if (fpFun == NULL)
//				{
//					delete lpDynPEBuff;
//					return false;
//				}
//				pITD->u1.Ordinal = (long)fpFun;
//			}
//		}
//	}
//	if (pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size > 0)
//	{
//		IMAGE_BASE_RELOCATION *pIBR = (IMAGE_BASE_RELOCATION *)(lpDynPEBuff + \
//			pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
//
//		long lDifference = (long)lpDynPEBuff - pINH->OptionalHeader.ImageBase;
//
//		for (; pIBR->VirtualAddress != 0; )
//		{
//			char *lpMemPage = lpDynPEBuff + pIBR->VirtualAddress;
//			long lCount = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) >> 1;
//
//			short int *pRelocationItem = (short int *)((char *)pIBR + sizeof(IMAGE_BASE_RELOCATION));
//			for (int nIndex = 0; nIndex < lCount; nIndex++)
//			{
//				int nOffset = pRelocationItem[nIndex] & 0x0fff;
//				int nType = pRelocationItem[nIndex] >> 12;
//
//				if (nType == 3)
//				{
//					*(long *)(lpMemPage + nOffset) += lDifference;
//				}
//				else if (nType == 0)
//				{
//				}
//			}
//			pIBR = (IMAGE_BASE_RELOCATION *)(pRelocationItem + lCount);
//		}
//	}
//	pExecuMem = lpDynPEBuff;
//	return true;
//}
//BOOL CallDllMain(PVOID pExecMem, DWORD dwReaseon, TCHAR* pModuleName)
//{
//	PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)((ULONG)pExecMem + ((PIMAGE_DOS_HEADER)pExecMem)->e_lfanew);
//	DWORD dwEP = pINH->OptionalHeader.AddressOfEntryPoint;
//	DLL_MAIN lpDllMain = (DLL_MAIN)((DWORD)pExecMem + dwEP);
//	lpDllMain((HMODULE)pExecMem, dwReaseon, pModuleName);
//	return TRUE;
//}
//BOOL LaunchDll(TCHAR *strName, DWORD dwReason)
//{
//	PVOID pRelocMem = NULL;
//	PVOID pExecuMem = NULL;
//	DWORD dwMemSize = 0;
//	if (LoadDll2Mem(pRelocMem, dwMemSize, strName))
//	{
//		PELoader((char *)pRelocMem, pExecuMem);
//		CallDllMain(pExecuMem, dwReason, strName);
//		ZeroMemory(pRelocMem, dwMemSize);
//		VirtualFree(pRelocMem, dwMemSize, MEM_DECOMMIT);
//	}
//	return TRUE;
//}
//void LaunchNoModule()
//{
//	LaunchDll(dllModuleName, NO_MODULE_MARK);
//}
//void NoModuleEntryCall(HMODULE hModule, DWORD ul_reason_for_call, TCHAR* pstrModuleName)
//{
//	TCHAR szMutexName[MAX_PATH];
//	TCHAR buffer[] = TEXT("092FC6F5-074E-43EE-AB2F-B5E73F7D7EC9-%d");
//	_stprintf_s(szMutexName, buffer, GetCurrentProcessId());
//	g_hMutex = CreateMutex(NULL, TRUE, szMutexName);
//	g_hModule = hModule;
//	return;
//}
//BOOL ChooseSub(HMODULE hModule, DWORD ul_reason_for_call, TCHAR* pstrModuleName)
//{
//	BOOL bRet = FALSE;
//	GetModuleFileName(NULL, exeModuleName, MAX_PATH);
//	if (ul_reason_for_call == NO_MODULE_MARK)_tcscpy_s(dllModuleName, MAX_PATH, pstrModuleName);
//	else GetModuleFileName(hModule, dllModuleName, MAX_PATH);
//	if (ul_reason_for_call == NO_MODULE_MARK)
//	{
//		NoModuleEntryCall(hModule, DLL_PROCESS_ATTACH, 0);
//		bRet = TRUE;
//	}
//	else
//	{
//		LaunchNoModule();
//		bRet = FALSE;
//	}
//	return bRet;
//}
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//	BOOL  Ret = FALSE;
//	if (ul_reason_for_call == DLL_PROCESS_ATTACH || ul_reason_for_call == NO_MODULE_MARK)
//	{
//		TCHAR szMutexName[MAX_PATH];
//		_stprintf_s(szMutexName, TEXT("092FC6F5-074E-43EE-AB2F-B5E73F7D7EC9-%d"), GetCurrentProcessId());
//		if (IsMutexExist(szMutexName)) return FALSE;
//		Ret = ChooseSub(hModule, ul_reason_for_call, (TCHAR *)lpReserved);
//	}
//	else
//	{
//		if (ul_reason_for_call == DLL_PROCESS_DETACH)
//		{
//			ReleaseMutex(g_hMutex);
//			CloseHandle(g_hMutex);
//			Ret = TRUE;
//		}
//	}
//	return Ret;
//}
//
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

