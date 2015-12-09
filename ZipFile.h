//
//  CZipFile.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef __CZipFile__
#define __CZipFile__

#include "ZipTypeDefine.h"
#include "ZipException.h"
#include "ZipReadStream.h"
#include "ZipWriteStream.h"
#include "FileInZipInfo.h"

#include <stdio.h>
#include <string>
#include <vector>

#include "zip.h"
#include "unzip.h"

class CZipFile {
public:
    CZipFile(std::string fileName, ZipFileMode mode);
    ~CZipFile();
    
public:
    CZipWriteStream WriteFileInZip(std::string fileNameInZip, ZipCompressionLevel compressionLevel);
    CZipWriteStream WriteFileInZip(std::string fileNameInZip, time_t fileDate, ZipCompressionLevel compressionLevel);
    CZipWriteStream WriteFileInZip(std::string fileNameInZip, time_t fileDate, ZipCompressionLevel compressionLevel, std::string password, unsigned long crc32);
    
    std::string GetFileName();
    unsigned long GetNumberFilesInZip();
    std::vector<CFileInZipInfo> ListFileInZipInfos();
    
    void GoToFirstFileInZip();
    bool GoToNextFileInZip();
    bool LocateFileInZip(std::string fileNameInZip);
    
    CFileInZipInfo GetCurrentFileInZipInfo();
    
    CZipReadStream ReadCurrentFileInZip();
    CZipReadStream ReadCurrentFileInZipWithPassword(std::string password);
    
    void Close();
    
private:
    std::string m_zipFileName;
    ZipFileMode m_zipFileMode;
    
    zipFile m_zipFile;
    unzFile m_unzFile;
};

#endif /* defined(__CZipFile__) */
