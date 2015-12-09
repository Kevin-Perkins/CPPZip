//
//  CZipReadStream.cpp
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#include "ZipReadStream.h"
#include "ZipException.h"

CZipReadStream::CZipReadStream(unzFile unzFile, std::string fileNameInZip) {
    m_unzFile = unzFile;
    m_fileNameInZip = fileNameInZip;
}

int CZipReadStream::ReadDataWithBuffer(unsigned char *buffer, unsigned int length) {
    int bytes = unzReadCurrentFile(m_unzFile, buffer, length);
    if (bytes < 0) {
        std::string reason = "Error reading \'" + m_fileNameInZip + "\' in the zipfile";
        throw CZipException(reason);
    }
    return bytes;
}

void CZipReadStream::FinishedReading() {
    int err = unzCloseCurrentFile(m_unzFile);
    if (UNZ_OK != err) {
        std::string reason = "Error closing \'" + m_fileNameInZip + "\' in the zipfile";
        throw CZipException(reason);
    }
}

const std::string CZipReadStream::GetFileNameInZip() {
    return m_fileNameInZip;
}