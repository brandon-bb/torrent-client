#include <gtest/gtest.h>
#include <torrent-client/lib/errors.hpp>

//template only takes enum - check for other types
//error returns generic
//EXIT_FAILURE

TEST (error_test, template_results)
{
  EXPECT_EQ ("Must be enum", error_template ("ll"));
}

int main (int argc, char **argv)
{
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS ();
}