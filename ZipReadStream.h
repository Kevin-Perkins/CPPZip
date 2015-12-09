//
//  CZipReadStream.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef __CZipReadStream__
#define __CZipReadStream__

#include <string>
#include "unzip.h"

class CZipReadStream {
public:
    CZipReadStream(unzFile unzFile, std::string fileNameInZip);
    
    int ReadDataWithBuffer(unsigned char *buffer, unsigned int length);
    void FinishedReading();
    
    const std::string GetFileNameInZip();
    
private:
    std::string m_fileNameInZip;
    unzFile m_unzFile;
};

#endif /* defined(__CZipReadStream__) */
