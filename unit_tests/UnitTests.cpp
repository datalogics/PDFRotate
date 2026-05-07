#include <catch2/catch_test_macros.hpp>

#include "clu_pdf_rotate_utility.h"
#include "rotatorlib.h"

TEST_CASE("NormalizeRotation maps positive multiples of 90 into [0, 360)",
          "[normalize]") {
  REQUIRE(NormalizeRotation(0) == 0);
  REQUIRE(NormalizeRotation(90) == 90);
  REQUIRE(NormalizeRotation(180) == 180);
  REQUIRE(NormalizeRotation(270) == 270);
  REQUIRE(NormalizeRotation(360) == 0);
  REQUIRE(NormalizeRotation(450) == 90);
  REQUIRE(NormalizeRotation(720) == 0);
}

TEST_CASE("NormalizeRotation handles negative inputs", "[normalize]") {
  REQUIRE(NormalizeRotation(-90) == 270);
  REQUIRE(NormalizeRotation(-180) == 180);
  REQUIRE(NormalizeRotation(-270) == 90);
  REQUIRE(NormalizeRotation(-360) == 0);
  REQUIRE(NormalizeRotation(-450) == 270);
}

TEST_CASE("NormalizeRotation rejects non-multiples of 90", "[normalize]") {
  REQUIRE_THROWS_AS(NormalizeRotation(45), CLUError);
  REQUIRE_THROWS_AS(NormalizeRotation(1), CLUError);
  REQUIRE_THROWS_AS(NormalizeRotation(-1), CLUError);
  REQUIRE_THROWS_AS(NormalizeRotation(91), CLUError);
}

TEST_CASE("DeriveOutputPath inserts -rotated before the extension",
          "[derive_path]") {
  REQUIRE(DeriveOutputPath("foo.pdf") == "foo-rotated.pdf");
  REQUIRE(DeriveOutputPath("/tmp/foo.pdf") == "/tmp/foo-rotated.pdf");
  REQUIRE(DeriveOutputPath("/tmp/sub/foo.PDF") ==
          "/tmp/sub/foo-rotated.PDF");
}

TEST_CASE("DeriveOutputPath handles missing extension", "[derive_path]") {
  REQUIRE(DeriveOutputPath("foo") == "foo-rotated");
  REQUIRE(DeriveOutputPath("/tmp/foo") == "/tmp/foo-rotated");
}
