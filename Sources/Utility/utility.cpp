/**
 * @file utility.cpp
 * @brief
 * @author Yu Kimura
 * @date 2021/04/10
 */

#include "utility.h"

namespace Simple {
    // BMPファイルの保存
    void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pStr, const char* fileName) 
    {
        HANDLE handle = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handle != INVALID_HANDLE_VALUE) {

            BITMAPFILEHEADER bmpHead;
            ZeroMemory(&bmpHead, sizeof(bmpHead));
            DWORD writeSize;

            bmpHead.bfType = ('M' << 8) | 'B';
            bmpHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_bmpInfo.biSizeImage;
            bmpHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

            WriteFile(handle, &bmpHead, sizeof(BITMAPFILEHEADER), &writeSize, NULL);
            WriteFile(handle, &m_bmpInfo, sizeof(BITMAPINFOHEADER), &writeSize, NULL);
            WriteFile(handle, pStr, m_bmpInfo.biSizeImage, &writeSize, NULL);
        }
        else {
            MessageBox(NULL, "BMPファイルの保存:失敗", "MessageBox", MB_OK);
        }
        CloseHandle(handle);
    }

    // char型からwstring型に変換
    void StringConvertToWchar(const std::string& srcStr, std::wstring& destStr) 
    {
        size_t size = srcStr.size() + 1;
        destStr.reserve(size);

        std::wstring outStr;
        outStr.reserve(size);

        mbstowcs_s(&size, outStr.data(), size, srcStr.c_str(), _TRUNCATE);

        destStr = outStr.c_str();
    }

    // wstring型からstring型に変換
    void WStringConvertToStrig(const std::wstring& srcStr, std::string& destStr) 
    {
        size_t size = srcStr.size() + 1;
        destStr.reserve(size);

        std::string outStr;
        outStr.reserve(size);

        wcstombs_s(&size, outStr.data(), size, srcStr.c_str(), _TRUNCATE);

        destStr = outStr.c_str();
    }

    // UTF16(一文字2byte)→UTF8(一文字1byte)に変換
    void UTF16ConvertToUTF8(const std::wstring& srcWstr, std::string& destStr) 
    {
        size_t size = srcWstr.size() + 1;
        destStr.reserve(size);

        std::string outStr;
        outStr.reserve(size);

        _wcstombs_s_l(&size, outStr.data(), size, srcWstr.data(), _TRUNCATE, _create_locale(LC_ALL, "jpn"));

        destStr = outStr.c_str();
    }

    // UTF8(一文字1byte)→UTF16(一文字2byte)に変換
    void UTF8ConvertToUTF16(const std::string& srcStr, std::wstring& destWstr)
    {
        size_t size = srcStr.size() + 1;

        std::wstring outStr;
        outStr.reserve(size);

        _mbstowcs_s_l(&size, outStr.data(), size, srcStr.c_str(), _TRUNCATE, _create_locale(LC_ALL, "jpn"));

        destWstr = outStr.c_str();
    }

    // GUIDからstring型に変換
    void GuidConvertToString(const GUID guid, std::string& destStr)
    {
        RPC_WSTR wString;
        if (RPC_S_OK == UuidToStringW(&guid, &wString))
        {
            WStringConvertToStrig((WCHAR*)wString, destStr);

            RpcStringFreeW(&wString);
        }
    }

    // string型からGUIDに変換
    void StringConvertToGuid(const std::string& str, GUID destGuid)
    {
        std::wstring wstr;
        StringConvertToWchar(str, wstr);

        if (RPC_S_OK == UuidFromString((RPC_CSTR)wstr.c_str(), &destGuid))
        {

        }
    }

} // namespace