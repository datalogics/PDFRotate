#pragma once
#include "pdfrotate.h"

#define VER_FILE_DESCRIPTION_STR    "Datalogics PDF Rotate"
#define VER_FILE_VERSION            DLCOMP_MAJOR_VER, DLCOMP_MINOR_VER, DLCOMP_REL_VER

#define VER_PRODUCTNAME_STR         MyAppName
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     MyAppVersion
#define VER_ORIGINAL_FILENAME_STR   MyAppExeName
#define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR           MyCopyright

#ifdef _DEBUG
#define VER_VER_DEBUG             VS_FF_DEBUG
#else
#define VER_VER_DEBUG             0
#endif

#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               VER_VER_DEBUG
#define VER_FILETYPE                VFT_APP
