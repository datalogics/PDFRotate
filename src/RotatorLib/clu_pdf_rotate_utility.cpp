#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "clu_pdf_rotate_utility.h"

bool is_file_exist(const char *fileName) {
  std::ifstream infile(fileName);
  return infile.good();
}

int NormalizeRotation(int degrees) {
  if (degrees % 90 != 0) {
    throw CLUError("Rotation must be a multiple of 90 degrees",
                   ERR_INVALID_ROTATION);
  }
  // Use Euclidean modulo so that negative inputs map into [0, 360).
  int normalized = degrees % 360;
  if (normalized < 0) {
    normalized += 360;
  }
  return normalized;
}

std::string DeriveOutputPath(const std::string &input_path) {
  boost::filesystem::path p(input_path);
  boost::filesystem::path stem = p.stem();
  boost::filesystem::path ext = p.extension();
  boost::filesystem::path parent = p.parent_path();
  std::string new_name = stem.string() + "-rotated" + ext.string();
  if (parent.empty()) {
    return new_name;
  }
  return (parent / new_name).string();
}

void ProcessCmdArgs(int argc, char *argv[], RotateRec &rec) {
  namespace po = boost::program_options;

  std::string input_path;
  std::string output_path;
  int page_number = 0;
  int rotation_degrees = 0;
  bool show_help = false;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",
     po::bool_switch(&show_help),
     "Show this help message")
    ("input,i",
     po::value<std::string>(&input_path)->required(),
     "Path to the input PDF file")
    ("page,p",
     po::value<int>(&page_number)->required(),
     "1-based page number to rotate")
    ("rotation,r",
     po::value<int>(&rotation_degrees)->required(),
     "Rotation amount in degrees (multiple of 90; may be negative)")
    ("output,o",
     po::value<std::string>(&output_path),
     "Output PDF path (defaults to <input>-rotated.pdf next to the input)");

  po::positional_options_description positional;
  positional.add("input", 1).add("page", 1).add("rotation", 1);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv)
                  .options(desc)
                  .positional(positional)
                  .style(po::command_line_style::default_style &
                         ~po::command_line_style::allow_guessing &
                         ~po::command_line_style::allow_sticky)
                  .run(),
              vm);

    if (vm.count("help") && vm["help"].as<bool>()) {
      std::cout << "Usage: pdfrotate <input.pdf> <page> <rotation> [-o <output.pdf>]\n\n"
                << desc << std::endl;
      throw CLUError("Help requested", ERR_NOERROR);
    }

    po::notify(vm);
  } catch (const CLUError &) {
    throw;
  } catch (const po::error &e) {
    std::cerr << "Argument error: " << e.what() << "\n\n"
              << "Usage: pdfrotate <input.pdf> <page> <rotation> [-o <output.pdf>]\n\n"
              << desc << std::endl;
    throw CLUError(e.what(), ERR_INVALID_ARGUMENTS);
  }

  if (page_number < 1) {
    throw CLUError("Page number must be 1 or greater",
                   ERR_INVALID_PAGE_NUMBER);
  }

  rec.input_document = input_path;
  rec.page_number = page_number;
  rec.rotation_degrees = rotation_degrees;
  rec.output_document = output_path.empty() ? DeriveOutputPath(input_path)
                                            : output_path;
}
