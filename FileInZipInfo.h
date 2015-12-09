//
//  CFileInZipInfo.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef __CFileInZipInfo__
#define __CFileInZipInfo__

#include "ZipTypeDefine.h"

#include <ctime>
#include <string>

class CFileInZipInfo {
public:
    CFileInZipInfo(std::string name, unsigned long long length, ZipCompressionLevel level, bool crypted, unsigned long long size, time_t date, unsigned long long crc32);
    
    const std::string GetName();
    const unsigned long long GetLength();
    const ZipCompressionLevel GetCompressionLevel();
    const bool IsCrypted();
    const unsigned long long GetSize();
    const time_t GetDate();
    const unsigned long long GetCRC32();
    
private:
    unsigned long long  m_length;
    ZipCompressionLevel m_level;
    bool                m_crypted;
    unsigned long long  m_size;
    time_t              m_date;
    unsigned long long  m_crc32;
    std::string         m_name;
};

#endif /* defined(__CFileInZipInfo__) */
