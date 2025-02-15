/**
 * @file utility.h
 * @brief
 * @author Yu Kimura
 * @date 2021/04/10
 */

#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

namespace Simple {
    // vectorのクリア
    template<class T>
    void VecClear(std::vector<T>& vec) {
        if (!vec.empty()) {
            vec.clear();
            vec.shrink_to_fit();
        }
    }

    // vectorの開放
    template <class T>
    void VecRelease(std::vector<T*>& vec) {
        if (!vec.empty()) {
            for (auto i = 0; i < (int)vec.size(); ++i) {
                delete vec[i];
                vec[i] = nullptr;
            }
            VecClear(vec);
        }
    }

    void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pStr, const char* fileName);

    void StringConvertToWchar(const std::string& str, std::wstring& destStr);

    void WStringConvertToStrig(const std::wstring& srcStr, std::string& destStr);

    void UTF16ConvertToUTF8(const std::wstring& srcWstr, std::string& destStr);

    void UTF8ConvertToUTF16(const std::string& srcStr, std::wstring& destWstr);

    void GuidConvertToString(const GUID guid, std::string& destStr);

    void StringConvertToGuid(const std::string& str, GUID destGuid);

} // namespace

#endif