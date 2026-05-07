#include <clocale>
#include <iostream>
#include <string>

#include "CommonUtils.h"
#include "InitializeLibrary.h"
#include "clu_pdf_rotate_utility.h"
#include "rotatorlib.h"

// Windows.h does nasty things we have to undo
#ifdef min
#undef min
#endif // min

void RotatePage(const RotateRec &rec) {
  if (!is_file_exist(rec.input_document.c_str())) {
    throw CLUError("Input Document Could Not Be Found",
                   ERR_MISSING_INPUT_FILE);
  }

  // Validate rotation up front so we fail before opening the PDF.
  int delta = NormalizeRotation(rec.rotation_degrees);

  try {
    APDFLDoc probe(rec.input_document.c_str());
    (void)probe;
  } catch (APDFLException::ASExceptionInfo) {
    throw CLUError("Unable to open input file",
                   ERR_UNABLE_TO_OPEN_INPUT_FILE);
  }

  APDFLDoc inDoc(rec.input_document.c_str());
  PDDoc pdDoc = inDoc.getPDDoc();
  ASInt32 numPages = PDDocGetNumPages(pdDoc);

  if (rec.page_number < 1 || rec.page_number > numPages) {
    throw CLUError("Page number is out of range for input document",
                   ERR_INVALID_PAGE_NUMBER);
  }

  PDPage page = PDDocAcquirePage(pdDoc, rec.page_number - 1);
  ASInt32 currentRotation = 0;
  ASInt32 newRotation = 0;
  try {
    currentRotation = PDPageGetRotate(page);
    newRotation = (currentRotation + delta) % 360;
    if (newRotation < 0) {
      newRotation += 360;
    }
    PDPageSetRotate(page, newRotation);
  } catch (...) {
    PDPageRelease(page);
    throw;
  }
  PDPageRelease(page);

  ASPathName outPathName = APDFLDoc::makePath(rec.output_document.c_str());
  try {
    PDDocSave(pdDoc, PDSaveFull | PDSaveCollectGarbage, outPathName, NULL,
              NULL, NULL);
  } catch (APDFLException::ASExceptionInfo) {
    ASFileSysReleasePath(NULL, outPathName);
    throw CLUError("Unable to save output file",
                   ERR_UNABLE_TO_SAVE_OUTPUT_FILE);
  }
  ASFileSysReleasePath(NULL, outPathName);

  std::cout << "Rotated page " << rec.page_number << " of "
            << rec.input_document << " from " << currentRotation
            << " to " << newRotation << " degrees -> "
            << rec.output_document << std::endl;
}

ReturnCode Rotator_Main(int argc, char *argv[]) {
  std::setlocale(LC_CTYPE, "");

#ifdef ROTATE_VERSION
  std::cout << "PDFRotate v" ROTATE_VERSION << std::endl;
#endif

  try {
    APDFLib lib(NULL);
    RotateRec rec;
    ProcessCmdArgs(argc, argv, rec);
    RotatePage(rec);
  } catch (CLUError &e) {
    if (e.os_code != ERR_NOERROR) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
    return e.os_code;
  } catch (APDFLException::ASExceptionInfo) {
    return ERR_INTERNAL_ERROR;
  } catch (...) {
    return ERR_UNKNOWN_FATAL;
  }
  return ERR_NOERROR;
}
