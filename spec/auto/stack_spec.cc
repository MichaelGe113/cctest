#include "cctest/core/test_runner.h"
#include "cctest/auto/auto_test_method.h"
#include "cctest/listener/text/text_progress.h"
#include "gtest/gtest.h"
#include <stack>
#include <queue>

using namespace cctest;

namespace {

struct StackSpec : TestFixture {
  std::stack<int> v;

  void setUp() override {
    v.push(1);
    v.push(2);
  }

  void test1() {
    ASSERT_EQ(2, v.top());
  }

  AutoTestMethod m1{1, "apply pop 0 time", &StackSpec::test1};

  void test2() {
    v.pop();
    ASSERT_EQ(1, v.top());
  }

  AutoTestMethod m2{2, "apply pop 1 time", &StackSpec::test2};

  void test3() {
    v.pop();
    v.pop();
    ASSERT_TRUE(v.empty());
  }

  AutoTestMethod m3{3, "apply pop 2 times", &StackSpec::test3};
};

template<typename Fixture>
struct GenericAutoSpec: testing::Test {
protected:
  GenericAutoSpec() : progress(ss), runner(progress, factory()) {
  }

private:
  static TestFactory& factory() {
    return TestMethodRegistry<Fixture>::inst();
  }

protected:
  std::ostringstream ss;
  TextProgress progress;
  TestRunner runner;
};

using AutoStackSpec = GenericAutoSpec<StackSpec>;

TEST_F(AutoStackSpec, auto_register_test_cases) {
  runner.run();
  ASSERT_EQ("starting...\n***\nend.\n", ss.str());
}

struct QueueSpec : TestFixture {
  std::queue<int> q;

  void setUp() override {
    q.push(1);
    q.push(2);
  }

  void apply_pop_0_time() {
    ASSERT_EQ(1, q.front());
    ASSERT_EQ(2, q.back());
  }

  AutoTestMethod m1 { 1, "apply_pop_0_time", &QueueSpec::apply_pop_0_time };

  void apply_pop_1_time() {
    q.pop();
    ASSERT_EQ(2, q.front());
    ASSERT_EQ(2, q.back());
  }

  AutoTestMethod m2 { 2, "apply_pop_1_time", &QueueSpec::apply_pop_1_time };

  void apply_pop_2_times() {
    q.pop();
    q.pop();
    ASSERT_TRUE(q.empty());
  }

  AutoTestMethod m3 { 3, "apply_pop_2_times", &QueueSpec::apply_pop_2_times };
};

using AutoQueueSpec = GenericAutoSpec<QueueSpec>;

TEST_F(AutoQueueSpec, auto_register_test_cases) {
  runner.run();
  ASSERT_EQ("starting...\n***\nend.\n", ss.str());
}

} // namespace