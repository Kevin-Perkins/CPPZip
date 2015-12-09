//
//  CZipWriteStream.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef __CZipWriteStream__
#define __CZipWriteStream__

#include <string>
#include "zip.h"

class CZipWriteStream {
public:
    CZipWriteStream(zipFile zipFile, std::string fileNameInZip);
    
    int WriteData(const unsigned char *dataBuf, unsigned int length);
    void FinishedWriting();
    
    const std::string GetFileNameInZip();
    
private:
    std::string m_fileNameInZip;
    zipFile m_zipFile;
};

#endif /* defined(__CZipWriteStream__) */
