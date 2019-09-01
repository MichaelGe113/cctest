#include "gtest/gtest.h"
#include "cctest/core/test_method.h"

using namespace cctest;

namespace {

std::string result;

struct WasRun : TestFixture {
  void testMethod() {
    result += "[runTest]";
  }

private:
  void setUp() override {
    result += "[setUp]";
  }

  void tearDown() override {
    result += "[tearDown]";
  }
};

struct TestMethodSpec : testing::Test {
protected:
  TestMethodSpec() {
    result = "";
  }

  void run(cctest::Test& test) {
    test.run();
  }
};

TEST_F(TestMethodSpec, full_lifecycle_for_test_case) {
  TestMethod<WasRun> test = &WasRun::testMethod;
  run(test);
  ASSERT_EQ("[setUp][runTest][tearDown]", result);
}

} // namespace
