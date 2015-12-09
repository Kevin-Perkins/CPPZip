//
//  CZipException.cpp
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#include "ZipException.h"

CZipException::CZipException(std::string reason) {
    m_error = 0;
}

CZipException::CZipException(int error, std::string reason) {
    m_error = error;
    m_reason = reason;
}

const int CZipException::GetError() {
    return m_error;
}

const std::string CZipException::GetReason() {
    return m_reason;
}