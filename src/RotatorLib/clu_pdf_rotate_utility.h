#ifndef CLU_PDF_ROTATE_UTILITY_H
#define CLU_PDF_ROTATE_UTILITY_H

#include <string>

#include "APDFLDoc.h"
#include "ASExtraCalls.h"
#include "CosCalls.h"
#include "DLExtrasCalls.h"
#include "DLExtrasExpT.h"
#include "InitializeLibrary.h"
#include "PDCalls.h"
#include "PDMetadataCalls.h"
#include "PERCalls.h"
#include "PEWCalls.h"
#include "PagePDECntCalls.h"

#ifndef WIN_ENV
#include <unistd.h>
#define PATH_SEP "/"
#else
#define strcasecmp _stricmp
#define PATH_SEP "\\"
#endif

#include "rotatorlib.h"

bool is_file_exist(const char *fileName);

// Normalize an arbitrary integer rotation (positive or negative) to one of
// 0, 90, 180, 270.  Throws CLUError(ERR_INVALID_ROTATION) if not a multiple
// of 90.
int NormalizeRotation(int degrees);

// Build the auto-named output path for a given input path:
// "/path/to/foo.pdf" -> "/path/to/foo-rotated.pdf"
// "/path/to/foo"     -> "/path/to/foo-rotated"
std::string DeriveOutputPath(const std::string &input_path);

// Parse the command-line arguments into a RotateRec.  If --output is omitted
// the output path is derived from the input path.
void ProcessCmdArgs(int argc, char *argv[], RotateRec &rec);

#endif /* CLU_PDF_ROTATE_UTILITY_H */
