/* file: rotatorlib.h */
#ifndef ROTATORLIB_H
#define ROTATORLIB_H

#include "ErrorCodes.h"
#include <exception>
#include <string>

struct RotateRec {
  std::string input_document;
  std::string output_document;
  int page_number;     // 1-based, as supplied on the command line
  int rotation_degrees; // additive; must be a multiple of 90
};

extern ReturnCode Rotator_Main(int argc, char *argv[]);
extern void RotatePage(const RotateRec &rec);

class CLUError : public std::invalid_argument {
public:
  explicit CLUError(const std::string &what_arg, ReturnCode code) :
    std::invalid_argument(what_arg), os_code(code) {}
  explicit CLUError(const char *what_arg, ReturnCode code) :
    std::invalid_argument(what_arg), os_code(code) {}
  ReturnCode os_code;
};

#endif /* ROTATORLIB_H */
