#include "sched.cpp"
#include "test_util.h"
#include "gtest/gtest.h"

// 각 TEST_P는 SchedulerTest::SetUp() 이후 실행됨
// 각 TEST_P 실행 이후, SchedulerTest::TearDow()이 실행됨
TEST_P(SchedulerTest, FCFS) {
  // 부모 Scheduler 클래스 포인터로 자식 FCFS 클래스를 가리킴
  sched_ = new FCFS(job_queue_, switch_time_);
}

TEST_P(SchedulerTest, RR_1) {
  sched_ = new RR(job_queue_, switch_time_, /*time slice*/ 1);
  //이렇게 하면, RR클래스의 생성자가 호출된다. 
}

TEST_P(SchedulerTest, RR_4) {
  sched_ = new RR(job_queue_, switch_time_, /*time slice*/ 4);
}

TEST_P(SchedulerTest, FeedBack_1) {
  sched_ = new FeedBack(job_queue_, switch_time_, /*is_2i*/ false);
}

TEST_P(SchedulerTest, FeedBack_2i) {
  sched_ = new FeedBack(job_queue_, switch_time_, /*is_2i*/ true);
}

TEST_P(SchedulerTest, Lottery) {
  sched_ = new Lottery(job_list_, switch_time_);
}

TEST_P(SchedulerTest, Stride) {
  sched_ = new Stride(job_list_, switch_time_);
}

// 각 8가지 TEST_P(스케줄링 기법)는 각각 4가지 경우(workload 2가지 x switch_time 2가지)를 TEST 함 
INSTANTIATE_TEST_CASE_P(Default, SchedulerTest,
  ::testing::Values(
    // std::make_tuple("워크로드 파일", "context_switch 시간"),
    std::make_tuple("A", 0.01)
    // std::make_tuple("A", 0.1),
    
    // std::make_tuple("B", 0.05),
    // std::make_tuple("B", 0.2)
  )
);

int main() {
    ::testing::InitGoogleTest(); //테스트 환경 초기화

    //내부적으로 SchedulerTest::SetUp()이 실행되고,
    //각각의 TEST_P가 실행된다. -> INSTANTIATE_TEST_CASE_P에서 정의한 모든 경우에 대해 실행된다.
    //마지막으로 SchedulerTest::TearDown()이 실행된다.
    return RUN_ALL_TESTS(); 
    
}
