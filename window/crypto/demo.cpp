#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#include <windows.h>
#include <wincrypt.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char uint8;

static BYTE PrivateKeyWithExponentOfOne[] = {
    0x07, 0x02, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00,
    0x52, 0x53, 0x41, 0x32, 0x00, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xAB, 0xEF, 0xFA, 0xC6,
    0x7D, 0xE8, 0xDE, 0xFB, 0x68, 0x38, 0x09, 0x92,
    0xD9, 0x42, 0x7E, 0x6B, 0x89, 0x9E, 0x21, 0xD7,
    0x52, 0x1C, 0x99, 0x3C, 0x17, 0x48, 0x4E, 0x3A,
    0x44, 0x02, 0xF2, 0xFA, 0x74, 0x57, 0xDA, 0xE4,
    0xD3, 0xC0, 0x35, 0x67, 0xFA, 0x6E, 0xDF, 0x78,
    0x4C, 0x75, 0x35, 0x1C, 0xA0, 0x74, 0x49, 0xE3,
    0x20, 0x13, 0x71, 0x35, 0x65, 0xDF, 0x12, 0x20,
    0xF5, 0xF5, 0xF5, 0xC1, 0xED, 0x5C, 0x91, 0x36,
    0x75, 0xB0, 0xA9, 0x9C, 0x04, 0xDB, 0x0C, 0x8C,
    0xBF, 0x99, 0x75, 0x13, 0x7E, 0x87, 0x80, 0x4B,
    0x71, 0x94, 0xB8, 0x00, 0xA0, 0x7D, 0xB7, 0x53,
    0xDD, 0x20, 0x63, 0xEE, 0xF7, 0x83, 0x41, 0xFE,
    0x16, 0xA7, 0x6E, 0xDF, 0x21, 0x7D, 0x76, 0xC0,
    0x85, 0xD5, 0x65, 0x7F, 0x00, 0x23, 0x57, 0x45,
    0x52, 0x02, 0x9D, 0xEA, 0x69, 0xAC, 0x1F, 0xFD,
    0x3F, 0x8C, 0x4A, 0xD0,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x64, 0xD5, 0xAA, 0xB1,
    0xA6, 0x03, 0x18, 0x92, 0x03, 0xAA, 0x31, 0x2E,
    0x48, 0x4B, 0x65, 0x20, 0x99, 0xCD, 0xC6, 0x0C,
    0x15, 0x0C, 0xBF, 0x3E, 0xFF, 0x78, 0x95, 0x67,
    0xB1, 0x74, 0x5B, 0x60,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};





int testEnc()
{
    HCRYPTPROV hProv = NULL;
    HCRYPTKEY hPrivateKey = NULL;
    HCRYPTKEY hPublicKey = NULL;
    HCRYPTKEY hSessionKey = NULL;
    BOOL bStatus = FALSE;
    const char* szPassword = "password";
    DWORD dwPasswordLen = (DWORD) strlen(szPassword);
    LPBYTE pEncryptedData = NULL;
    DWORD i, dwKeyLen = 0, dwValLen = 0;
    DWORD dwEncryptedDataLen = 0;
    /*
     * We suppose here that the default container exists and
     * that it contains an RSA exchange key pair .
     */
    bStatus = CryptAcquireContext(&hProv,
                                  NULL, /* default container */
                                  MS_DEF_PROV,
                                  PROV_RSA_FULL,
                                  0);

    if (!bStatus) {
        printf("CryptAcquireContext failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    // 导入密钥
    //if (!CryptImportKey(hProv, PrivateKeyWithExponentOfOne, sizeof(PrivateKeyWithExponentOfOne), 0, 0, &hPrivateKey)) {
    //    printf("Error CryptImportKey() failed. 0x%.8X\n", GetLastError());
    //    goto error;
    //}

    
    // 产生会话密钥
    if (!CryptGenKey(hProv, CALG_RSA_KEYX, CRYPT_EXPORTABLE, &hSessionKey)) {
        printf("Error CryptGenKey() failed. 0x%.8X\n", GetLastError());
        goto error;
    }
    
    // 得到公钥
    bStatus = CryptGetUserKey(hProv,
                              AT_KEYEXCHANGE,
                              &hPublicKey);

    if (!bStatus) {
        printf("CryptGetUserKey failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    /*
     * get the size of the key
     */
    dwValLen = sizeof(DWORD);
    bStatus = CryptGetKeyParam(hPublicKey,
                               KP_KEYLEN,
                               (LPBYTE) &dwKeyLen,
                               &dwValLen,
                               0);

    if (!bStatus) {
        printf("CryptGetKeyParam failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    /*
     * Allocate input/output buffer
     */
    dwKeyLen = (dwKeyLen + 7) / 8; /* tranform to bytes length */
    pEncryptedData = (LPBYTE) LocalAlloc(0, dwKeyLen);

    if (!pEncryptedData) {
        printf("LocalAlloc failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    /*
     * copy password to the buffer
     */
    ZeroMemory(pEncryptedData, dwKeyLen);
    CopyMemory(pEncryptedData, szPassword, dwPasswordLen);
    dwEncryptedDataLen = dwPasswordLen;
    bStatus = CryptEncrypt(hSessionKey,
                           NULL,
                           TRUE,
                           0,
                           pEncryptedData,
                           &dwEncryptedDataLen,
                           dwKeyLen);

    if (!bStatus) {
        printf("CryptEncrypt failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    printf("Password encrypted successfully :\n\tlength = %d bytes.\n\tValue = ", (int) dwEncryptedDataLen);

    for (i = 0; i < dwEncryptedDataLen; i++) {
        printf("%.2X", pEncryptedData[i]);
    }

    printf("\n\n");
    /*
     * verifying encryption result
     */
    printf("Verifying encryption result...\r\n");
    // 换成 hPublicKey 也能解开, 对称加密么?
	
    bStatus = CryptDecrypt(hSessionKey,
                           NULL,
                           TRUE,
                           0,
                           pEncryptedData,
                           &dwEncryptedDataLen);

    if (!bStatus) {
        printf("CryptDecrypt failed with error 0x%.8X\n", GetLastError());
        goto error;
    }

    if ((dwEncryptedDataLen != dwPasswordLen) ||
        (0 != memcmp(pEncryptedData, szPassword, dwPasswordLen))) {
        printf("\nVerification failed!!\n");
    } else {
		pEncryptedData[dwEncryptedDataLen] = '\0';
        printf("Decrypt result = %s\r\n", (char*)pEncryptedData);
        printf("\nSucess.\n");
    }

error:

    if (NULL != pEncryptedData) {
        LocalFree(pEncryptedData);
        pEncryptedData = NULL;
    }

    if (NULL != hPublicKey) {
        CryptDestroyKey(hPublicKey);
        hPublicKey = NULL;
    }

    if (NULL != hPrivateKey) {
        CryptDestroyKey(hPrivateKey);
        hPrivateKey = NULL;
    }

    if (NULL != hProv) {
        CryptReleaseContext(hProv, 0);
        hProv = NULL;
    }

    /** run result
    Password encrypted successfully :
    length = 64 bytes.
    Value = 64726F777373617000E030312B9C252536C83F34F7EC3E47D554E9B188DD25E8
    1BDAF9CB18F3ABE3693389BCDB1C224741A858E35954170115E82FC16C530200

    Verifying encryption result...
    Decrypt result = password

    Sucess.
    请按任意键继续. . .
    */
    return 0;
}

int testInfo(){
	unsigned char* pbX509Data; // 证书数据
	unsigned long ulX509DataLen; // 证书数据长度
    FILE* fpCert = NULL;
	const char* certFilePath ="D:\\systemp\\cert\\baiducom.cer";

    fopen_s(&fpCert, certFilePath, "rb");
    if (!fpCert) {
      return false;
    }
    fseek(fpCert, 0, SEEK_END);
    long cbData = ftell(fpCert);
    uint8* pbData = new uint8[cbData];
    fseek(fpCert, 0, SEEK_SET);
    fread(pbData, sizeof(char), cbData, fpCert);
    fclose(fpCert);
	pbX509Data = pbData;
	ulX509DataLen = cbData;

	PCCERT_CONTEXT pCertContext = CertCreateCertificateContext(X509_ASN_ENCODING, pbX509Data, ulX509DataLen);
    if (!pCertContext)
      return 1;
	//获取证书信息
	pCertContext->pCertInfo->dwVersion; // 证书版本号
	CRYPT_INTEGER_BLOB snBlob = pCertContext->pCertInfo->SerialNumber; // 证书SN
	CERT_NAME_BLOB issuerBlob = pCertContext->pCertInfo->Issuer; // 证书颁发者
	CERT_NAME_BLOB subjectBlob = pCertContext->pCertInfo->Subject; // 证书主题
	// 证书有效起始日期
	SYSTEMTIME sysTime;
	FILETIME localFileTime;
	memset(&sysTime, 0, sizeof(sysTime));
	memset(&localFileTime, 0, sizeof(localFileTime));
	FileTimeToLocalFileTime(&pCertContext->pCertInfo->NotBefore,&localFileTime);
	FileTimeToSystemTime(&localFileTime, &sysTime);
	char szTime[128] = {0};
	sprintf_s(szTime, 128, "%d年%d月%d日 %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	// 证书有效终止日期
	memset(&sysTime, 0, sizeof(sysTime));
	memset(&localFileTime, 0, sizeof(localFileTime));
	FileTimeToLocalFileTime(&pCertContext->pCertInfo->NotBefore,&localFileTime);
	FileTimeToSystemTime(&localFileTime, &sysTime);
	memset(szTime, 0, sizeof(szTime));
	sprintf_s(szTime, 128, "%d年%d月%d日 %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	//创建临时密钥容器
	HCRYPTPROV hTmpProv = NULL;
	if (RCRYPT_FAILED(CryptAcquireContext(&hTmpProv, "My_Temporary_Container", NULL, PROV_RSA_AES, CRYPT_NEWKEYSET))) // NULL表示使用系统默认CSP
	{
		if (RCRYPT_FAILED(CryptAcquireContext(&hTmpProv, "My_Temporary_Container", NULL, PROV_RSA_AES, 0))) // NULL表示使用系统默认CSP
		{
			CertFreeCertificateContext(pCertContext);
			return NTE_FAIL;
		}
	}
	//向容器中导入公钥，获取公钥句柄
	HCRYPTKEY hKey = NULL;
	CERT_PUBLIC_KEY_INFO certPubKeyInfo = pCertContext->pCertInfo->SubjectPublicKeyInfo;
	CryptImportPublicKeyInfo(hTmpProv, X509_ASN_ENCODING|PKCS_7_ASN_ENCODING, &certPubKeyInfo, &hKey);

	//导出公钥（最好采用二次调用方式）
	unsigned char* pBuf = NULL;
	unsigned long ulBufLen = 0;
	CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, pBuf, &ulBufLen);
	pBuf = new unsigned char[ulBufLen];
	memset(pBuf, 0, ulBufLen);
	CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, pBuf, &ulBufLen);

	//获取公钥信息
	unsigned char* p = pBuf + sizeof(PUBLICKEYSTRUC);
	(*(RSAPUBKEY*)p).bitlen; // 公钥模长（以bit为单位）
	(*(RSAPUBKEY*)p).pubexp; // 公钥的e（注意字节顺序）
	p += sizeof(RSAPUBKEY); // 公钥的n（注意字节顺序）

	//清理工作
	delete[] pBuf;
	pBuf = NULL;
	CryptDestroyKey(hKey);
	CryptReleaseContext(hTmpProv, 0);
	CertFreeCertificateContext(pCertContext);

	return 0;
}

int main(){
	testInfo();
	//testEnc();

}

