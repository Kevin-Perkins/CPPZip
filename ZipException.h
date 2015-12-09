//
//  CZipException.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef __CZipException__
#define __CZipException__

#include <string>

class CZipException {
public:
    CZipException(std::string reason);
    CZipException(int error, std::string reason);
    
    const int GetError();
    const std::string GetReason();
    
private:
    int m_error;
    std::string m_reason;
};

#endif /* defined(__CZipException__) */
