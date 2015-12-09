//
//  CZipFile.cpp
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#include "ZipFile.h"

#include <ctime>
#include <cstdlib>
#include <sstream>

#define FILE_IN_ZIP_MAX_NAME_LENGTH (256)
#define IS_NOT_UNZIP_FILE_MODE                                              \
    if (ZipFileModeUnzip == m_zipFileMode) {                                \
        throw CZipException("Operation not permitted with Unzip mode");     \
    }
#define IS_UNZIP_FILE_MODE                                                  \
    if (ZipFileModeUnzip != m_zipFileMode) {                                \
        throw CZipException("Operation not permitted without Unzip mode");  \
    }

CZipFile::CZipFile(std::string fileName, ZipFileMode mode) {
    m_zipFileName = fileName;
    m_zipFileMode = mode;
    
    switch (mode) {
        case ZipFileModeUnzip:
            m_unzFile = unzOpen64(fileName.c_str());
            if (0 == m_unzFile) {
                std::string reason = "Can't open \'" + m_zipFileName + "\'";
                throw CZipException(reason);
            }
            break;
        case ZipFileModeCreate:
            m_zipFile = zipOpen64(fileName.c_str(), APPEND_STATUS_CREATE);
            if (0 == m_zipFile) {
                std::string reason = "Can't open \'" + m_zipFileName + "\'";
                throw CZipException(reason);
            }
            break;
        case ZipFileModeAppend:
            m_zipFile = zipOpen64(fileName.c_str(), APPEND_STATUS_ADDINZIP);
            if (0 == m_zipFile) {
                std::string reason = "Can't open \'" + m_zipFileName + "\'";
                throw CZipException(reason);
            }
            break;
        default:
            std::stringstream strStream;
            strStream << "Unknown mode \'" << m_zipFileMode << "\'" ;
            std::string reason = strStream.str();
            throw CZipException(reason);
            break;
    }
}

CZipFile::~CZipFile() {
    
}

CZipWriteStream CZipFile::WriteFileInZip(std::string fileNameInZip, ZipCompressionLevel compressionLevel) {
    time_t rawtime;
    time(&rawtime);
    return WriteFileInZip(fileNameInZip, rawtime, compressionLevel);
}

CZipWriteStream CZipFile::WriteFileInZip(std::string fileNameInZip, time_t fileDate, ZipCompressionLevel compressionLevel) {
    return WriteFileInZip(fileNameInZip, fileDate, compressionLevel, "", 0);
}

CZipWriteStream CZipFile::WriteFileInZip(std::string fileNameInZip, time_t fileDate, ZipCompressionLevel compressionLevel, std::string password, unsigned long crc32) {
    IS_NOT_UNZIP_FILE_MODE
    
    struct tm *localTime = localtime(&fileDate);
    zip_fileinfo zi;
    zi.tmz_date.tm_sec = localTime->tm_sec;
    zi.tmz_date.tm_min = localTime->tm_min;
    zi.tmz_date.tm_hour = localTime->tm_hour;
    zi.tmz_date.tm_mday = localTime->tm_mday;
    zi.tmz_date.tm_mon = localTime->tm_mon - 1;
    zi.tmz_date.tm_year = localTime->tm_year;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    zi.dosDate = 0;
    
    int err = zipOpenNewFileInZip3_64(m_zipFile, fileNameInZip.c_str(), &zi,
                                      NULL, 0, NULL, 0, NULL,
                                      (ZipCompressionLevelNone != compressionLevel) ? Z_DEFLATED : 0,
                                      compressionLevel, 0,
                                      -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                      password.c_str(), crc32, 1);
    if (ZIP_OK != err) {
        std::string reason = "Error opening \'" + m_zipFileName + "\' in zipfile";
        throw CZipException(reason);
    }
    return CZipWriteStream(m_zipFile, fileNameInZip);
}

std::string CZipFile::GetFileName() {
    return m_zipFileName;
}

unsigned long CZipFile::GetNumberFilesInZip() {
    IS_UNZIP_FILE_MODE
    
    unz_global_info64 gi;
    int err = unzGetGlobalInfo64(m_unzFile, &gi);
    if (UNZ_OK != err) {
        std::string reason = "Error getting global info in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    return gi.number_entry;
}

std::vector<CFileInZipInfo> CZipFile::ListFileInZipInfos() {
    std::vector<CFileInZipInfo> files;
    unsigned long num = GetNumberFilesInZip();
    GoToFirstFileInZip();
    for (unsigned long index = 0; index < num; index++) {
        CFileInZipInfo info = GetCurrentFileInZipInfo();
        files.push_back(info);
        if (index + 1 < num) {
            GoToNextFileInZip();
        }
    }
    return files;
}

void CZipFile::GoToFirstFileInZip() {
    IS_UNZIP_FILE_MODE
    
    int err = unzGoToFirstFile(m_unzFile);
    if (UNZ_OK != err) {
        std::string reason = "Error going to first file in zip in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
}

bool CZipFile::GoToNextFileInZip() {
    IS_UNZIP_FILE_MODE
    
    int err = unzGoToNextFile(m_unzFile);
    if (UNZ_END_OF_LIST_OF_FILE == err) {
        return false;
    }
    if (UNZ_OK != err) {
        std::string reason = "Error going to next file in zip in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    return true;
}

bool CZipFile::LocateFileInZip(std::string fileNameInZip) {
    IS_UNZIP_FILE_MODE
    
    int err = unzLocateFile(m_unzFile, fileNameInZip.c_str(), NULL);
    if (UNZ_END_OF_LIST_OF_FILE == err) {
        return false;
    }
    if (UNZ_OK != err) {
        std::string reason = "Error localting file in zip in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    return true;
}

CFileInZipInfo CZipFile::GetCurrentFileInZipInfo() {
    IS_UNZIP_FILE_MODE
    
    char filenameInZip[FILE_IN_ZIP_MAX_NAME_LENGTH] = {0};
    unz_file_info64 file_info;
    int err = unzGetCurrentFileInfo64(m_unzFile, &file_info, filenameInZip, sizeof(filenameInZip), NULL, 0, NULL, 0);
    if (UNZ_OK != err) {
        std::string reason = "Error getting current file info in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    
    ZipCompressionLevel level = ZipCompressionLevelNone;
    if (0 != file_info.compression_method) {
        switch ((file_info.flag & 0x6) / 2) {
            case 0:
                level = ZipCompressionLevelDefault;
                break;
            case 1:
                level = ZipCompressionLevelBest;
                break;
            case 2:
                level = ZipCompressionLevelFastest;
                break;
            default:
                break;
        }
    }
    
    bool crypted = (0 != (file_info.flag & 1));
    
    struct tm fileTime;
    fileTime.tm_mday = file_info.tmu_date.tm_mday;
    fileTime.tm_mon = file_info.tmu_date.tm_mon + 1;
    fileTime.tm_year = file_info.tmu_date.tm_year;
    fileTime.tm_hour = file_info.tmu_date.tm_hour;
    fileTime.tm_min = file_info.tmu_date.tm_min;
    fileTime.tm_sec = file_info.tmu_date.tm_sec;
    time_t rawTime = mktime(&fileTime);
    CFileInZipInfo info = CFileInZipInfo(std::string(filenameInZip), file_info.uncompressed_size, level, crypted, file_info.compressed_size, rawTime, file_info.crc);
    return info;
}

CZipReadStream CZipFile::ReadCurrentFileInZip() {
    return ReadCurrentFileInZipWithPassword("");
}

CZipReadStream CZipFile::ReadCurrentFileInZipWithPassword(std::string password) {
    IS_UNZIP_FILE_MODE
    
    char filenameInZip[FILE_IN_ZIP_MAX_NAME_LENGTH] = {0};
    unz_file_info64 file_info;
    int err = unzGetCurrentFileInfo64(m_unzFile, &file_info, filenameInZip, sizeof(filenameInZip), NULL, 0, NULL, 0);
    if (UNZ_OK != err) {
        std::string reason = "Error getting current file info in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    
    err = unzOpenCurrentFilePassword(m_unzFile, password.c_str());
    if (UNZ_OK != err) {
        std::string reason = "Error opening current file in \'" + m_zipFileName + "\'";
        throw CZipException(reason);
    }
    
    return CZipReadStream(m_unzFile, std::string(filenameInZip));
}

void CZipFile::Close() {
    if (ZipFileModeUnzip == m_zipFileMode) {
        int err = unzClose(m_unzFile);
        if (UNZ_OK != err) {
            std::string reason = "Error closing \'" + m_zipFileName + "\'";
            throw CZipException(reason);
        }
    } else if (ZipFileModeCreate == m_zipFileMode) {
        int err = zipClose(m_zipFile, NULL);
        if (ZIP_OK != err) {
            std::string reason = "Error closing \'" + m_zipFileName + "\'";
            throw CZipException(reason);
        }
    } else if (ZipFileModeAppend == m_zipFileMode) {
        int err = zipClose(m_zipFile, NULL);
        if (ZIP_OK != err) {
            std::string reason = "Error closing \'" + m_zipFileName + "\'";
            throw CZipException(reason);
        }
    } else {
        std::stringstream strStream;
        strStream << "Unknown mode \'" << m_zipFileMode << "\'" ;
        std::string reason = strStream.str();
        throw CZipException(reason);
    }
}