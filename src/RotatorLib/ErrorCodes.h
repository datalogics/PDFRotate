/* file: ErrorCodes.h */
#ifndef ERROR_CODES_H
#define ERROR_CODES_H

/*
 * Return codes used by the CLU PDF Rotate tool's Rotator_Main function.
 * Rotator_Main should be the only function using these other than unit tests
 * that involve Rotator_Main.
 */

enum ReturnCode {
  /*
   * @ErrorMessage: The program successfully ran
   */
  ERR_NOERROR = 0,

  /*
   * @ErrorMessage: An unknown fatal error has occurred
   */
  ERR_UNKNOWN_FATAL = 101,

  /*
   * @ErrorMessage: An internal error has occurred (library init, etc.)
   */
  ERR_INTERNAL_ERROR = 102,

  /*
   * @ErrorMessage: The given PDF could not be found
   */
  ERR_MISSING_INPUT_FILE = 103,

  /*
   * @ErrorMessage: A given input file was corrupted, password protected, or
   * not a PDF.
   */
  ERR_UNABLE_TO_OPEN_INPUT_FILE = 104,

  /*
   * @ErrorMessage: The given page number is out of range for the input PDF.
   */
  ERR_INVALID_PAGE_NUMBER = 105,

  /*
   * @ErrorMessage: The output PDF could not be created.
   */
  ERR_UNABLE_TO_SAVE_OUTPUT_FILE = 106,

  /*
   * @ErrorMessage: The rotation value is not a multiple of 90 degrees.
   */
  ERR_INVALID_ROTATION = 107,

  /*
   * @ErrorMessage: Required command-line arguments were missing or malformed.
   */
  ERR_INVALID_ARGUMENTS = 108
};

#endif /* ERROR_CODES_H */
