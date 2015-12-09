//
//  CZipWriteStream.cpp
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#include "ZipWriteStream.h"
#include "ZipException.h"

CZipWriteStream::CZipWriteStream(zipFile zipFile, std::string fileNameInZip) {
    m_zipFile = zipFile;
    m_fileNameInZip = fileNameInZip;
}

int CZipWriteStream::WriteData(const unsigned char *dataBuf, unsigned int length) {
    int bytes = zipWriteInFileInZip(m_zipFile, dataBuf, length);
    if (bytes < 0) {
        std::string reason = "Error writing \'" + m_fileNameInZip + "\' in the zipfile";
        throw CZipException(reason);
    }
    return bytes;
}

void CZipWriteStream::FinishedWriting() {
    int err = zipCloseFileInZip(m_zipFile);
    if (ZIP_OK != err) {
        std::string reason = "Error closing \'" + m_fileNameInZip + "\' in the zipfile";
        throw CZipException(reason);
    }
}

const std::string CZipWriteStream::GetFileNameInZip() {
    return m_fileNameInZip;
}