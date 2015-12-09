//
//  ZipTypeDefine.h
//
//  Created by DebugLife on 9/16/15.
//  Copyright (c) 2015 Kevin_Perkins. All rights reserved.
//

#ifndef _ZipTypeDefine_h
#define _ZipTypeDefine_h

typedef enum {
    ZipFileModeUnzip,
    ZipFileModeCreate,
    ZipFileModeAppend
} ZipFileMode;

typedef enum {
    ZipCompressionLevelDefault= -1,
    ZipCompressionLevelNone= 0,
    ZipCompressionLevelFastest= 1,
    ZipCompressionLevelBest= 9
} ZipCompressionLevel;

#endif
