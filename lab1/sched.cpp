/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32221902
*	    Student name : 박주희
*	    Date : 2025.04.22
*	    Contents :
*/

/*
요구사항: 스케줄링 알고리즘에 따라 프로세스를 정확한 순서대로 스케줄링하고, 모든 작업정보를 정확히 기록하도록 구현.
RR, FeedBack, Lottery, Stride
스케줄러는 생성자를 통해 실행할 작업들이 저장된 job_queue/job_list와 context switch time을 전달받는다.
스케줄러는 run()함수가 호출될 때마다, 다음 1초간 실행할 작업 명을 반환한다.
스케줄러는 job구조체의 정보가 변경될 때마다, 이를 모두 update한다.
스케줄러는 완료된 job을 "end-jobs"에 순서대로 저장(push-back)한다"

입력(워크로드)
프로세스 개수 (n):

구현해야하는 내용:
RR, FeedBack, Lottery, Stride 클래스를 구현한다
각 클래스의 위쪽 두가지 함수의 선언은 수정할수없다
생성자는 부모 생성자를 호출하고 name을 초기화해야한다-기존 내용을 수정하지 말것
각 클래스의 멤버변수와 함수는 자유롭게 추가 가능
라이브러리는 c++ STL만 사용가능
sched.cpp 외의 다른 파일은 제출하지 않음
RR과 FeedBack은 time quantum이 다르더라도 동일한 class로 작성한다 
(+ 피드백은 두개의 버전으로 돌아가게 되어있는데 클래스 하나로 두개 다 돌아가게 만들어야한다)
생성자를 통해 time quantum을 전달받는다
FeedBack의 큐 개수는 4개이며, boosting 정책은 없다

line by line으로 코드 설명하는 주석 달것
*/
#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>
#include <unordered_map>
#include "sched.h"

class RR : public Scheduler{    // Round Robin 스케줄러
    private:
        int time_slice_;    // time quantum
        int left_slice_;    // 남은 ?? 
        std::queue<Job> waiting_queue;  // 대기 큐

    public:
        //TEST_P에서 생성자 호출하면 아래 생성자가 호출된다. 
        //자식생성자가 부모생성자인 Scheduler의 생성자를 호출한다.
        //결과적으로 job_queue_와 switch_time_, time_slice가 초기화된다.
        //time slice는 1인 버전과, 4인 버전이 있다.  
        
        RR(std::queue<Job> jobs, double switch_overhead, int time_slice) : Scheduler(jobs, switch_overhead) {
            name = "RR_"+std::to_string(time_slice);
            /*
            * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
            * 나머지는 자유롭게 수정 및 작성 가능 (아래 코드 수정 및 삭제 가능)
            */
            time_slice_ = time_slice; 
            left_slice_ = time_slice;
        }

        int run() override {
            return current_job_.name;
        }
                
};

class FeedBack : public Scheduler {
private:
    std::queue<Job> queue[4]; // 각 요소가 하나의 큐인 배열 선언
    int quantum[4] = {1, 1, 1, 1};
    int left_slice_;
    int current_queue;

public:
    FeedBack(std::queue<Job> jobs, double switch_overhead, bool is_2i) : Scheduler(jobs, switch_overhead) {
        if (is_2i) {
            name = "FeedBack_2i";
        } else {
            name = "FeedBack_1";
        }
        /*
        * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
        * 나머지는 자유롭게 수정 및 작성 가능
        */
        // Queue별 time quantum 설정
        if (name == "FeedBack_2i") {
            quantum[0] = 1;
            quantum[1] = 2;
            quantum[2] = 4;
            quantum[3] = 8;
        }
    }

    int run() override {
        return current_job_.name;
    }
};

class Lottery : public Scheduler{
    private:
        int counter = 0;
        int total_tickets = 0;
        int winner = 0;
        std::mt19937 gen;  // 난수 생성기
        
    public:
        Lottery(std::list<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "Lottery";
            // 난수 생성기 초기화
            uint seed = 10; // seed 값 수정 금지
            gen = std::mt19937(seed);
            total_tickets = 0;
            for (const auto& job : job_list_) {
                total_tickets += job.tickets;
            }
        }

        int getRandomNumber(int min, int max) {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        }

        int run() override {
            return current_job_.name;
        }
};


class Stride : public Scheduler{
    private:
        // 각 작업의 현재 pass 값과 stride 값을 관리하는 맵
        std::unordered_map<int, int> pass_map;  
        std::unordered_map<int, int> stride_map;  
        const int BIG_NUMBER = 10000; // stride 계산을 위한 상수 (보통 큰 수를 사용)

    public:
        Stride(std::list<Job> jobs, double switch_overhead) : Scheduler(jobs, switch_overhead) {
            name = "Stride";
                    // job_list_에 있는 각 작업에 대해 stride와 초기 pass 값(0)을 설정
            for (auto &job : job_list_) {
                // stride = BIG_NUMBER / tickets (tickets는 0이 아님을 가정)
                stride_map[job.name] = BIG_NUMBER / job.tickets;
                pass_map[job.name] = 0;
            }
        }

        int run() override {
            return current_job_.name;
        }
};
