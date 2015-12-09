//
//  CFileInZipInfo.cpp
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#include "FileInZipInfo.h"

CFileInZipInfo::CFileInZipInfo(std::string name, unsigned long long length, ZipCompressionLevel level, bool crypted, unsigned long long size, time_t date, unsigned long long crc32) {
    m_name = name;
    m_length = length;
    m_level = level;
    m_crypted = crypted;
    m_size = size;
    m_date = date;
    m_crc32 = crc32;
}

const std::string CFileInZipInfo::GetName() {
    return m_name;
}

const unsigned long long CFileInZipInfo::GetLength() {
    return m_length;
}

const ZipCompressionLevel CFileInZipInfo::GetCompressionLevel() {
    return m_level;
}

const bool CFileInZipInfo::IsCrypted() {
    return m_crypted;
}

const unsigned long long CFileInZipInfo::GetSize() {
    return m_size;
}

const time_t CFileInZipInfo::GetDate() {
    return m_date;
}

const unsigned long long CFileInZipInfo::GetCRC32() {
    return m_crc32;
}