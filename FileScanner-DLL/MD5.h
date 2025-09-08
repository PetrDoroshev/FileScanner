#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string MD5(const std::string& filename) {

    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        
        throw std::runtime_error("Cannot open file");
    }

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        
        CloseHandle(hFile);
        throw std::runtime_error("CryptAcquireContext failed");
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {

        CloseHandle(hFile);
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("CryptCreateHash failed");
    }

    BYTE buffer[4096];
    DWORD bytesRead = 0;
    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        CryptHashData(hHash, buffer, bytesRead, 0);
    }

    BYTE rgbHash[16];
    DWORD cbHash = 16;
    CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0);

    std::ostringstream oss;
    for (DWORD i = 0; i < cbHash; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)rgbHash[i];
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    return oss.str();

}