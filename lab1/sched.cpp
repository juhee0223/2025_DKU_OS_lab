/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32221902
*	    Student name : 박주희
*	    Date : 2025.04.22
*	    Contents :
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>
#include <unordered_map>
#include "sched.h"

const int INT_MAX = 2147483647; 

// =======================================================
// 1) Round Robin 스케줄러
//    - 고정된 time quantum으로 선점하며 순환
// =======================================================
class RR : public Scheduler
{
private:
    int time_slice_;                 // 타임 퀀텀
    int left_slice_;                 // 현재 작업에 남은 퀀텀
    std::queue<Job> waiting_queue;   // 대기 큐

public:
    RR(std::queue<Job> jobs, double switch_overhead, int time_slice)
        : Scheduler(jobs, switch_overhead)
    {
        name = "RR_" + std::to_string(time_slice);
        /*
         * 위 생성자 선언 및 이름 초기화 코드 수정하지 말것.
         * 나머지는 자유롭게 수정 및 작성 가능 (아래 코드 수정 및 삭제 가능)
         */
        time_slice_ = time_slice;
        left_slice_ = time_slice;
    }

    int run() override
    {
        // [1] 처음 실행 시: current_job_이 비어 있고 job_queue에 작업이 있다면 하나 할당
        if (current_job_.name == 0 && !job_queue_.empty()) {
            current_job_ = job_queue_.front();
            job_queue_.pop();
        }

        // [2] 도착한 작업을 waiting_queue로 이동 (하나씩만 이동)
        if (!job_queue_.empty() &&
            job_queue_.front().arrival_time <= current_time_) {
            waiting_queue.push(job_queue_.front());
            job_queue_.pop();
        }

        // [3] 작업 완료 시: 완료 시간 기록, end_jobs_에 저장
        if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;
            end_jobs_.push_back(current_job_);
            left_slice_ = time_slice_;  // 다음 작업 대비 초기화

            // 대기 큐가 비어 있으면 더 이상 실행할 작업 없음
            if (waiting_queue.empty()) return -1;

            // 다음 작업 할당 및 context switch 반영
            current_job_ = waiting_queue.front();
            waiting_queue.pop();
            current_time_ += switch_time_;
        }

        // [4] time quantum 소진 시: 현재 작업 선점 → 대기열로, 다음 작업 할당
        if (left_slice_ == 0) {
            if (!waiting_queue.empty()) {
                waiting_queue.push(current_job_);
                current_job_ = waiting_queue.front();
                waiting_queue.pop();
                current_time_ += switch_time_;
            }
            left_slice_ = time_slice_;
        }

        // [5] 첫 실행인 경우, first_run_time 기록
        if (current_job_.service_time == current_job_.remain_time) {
            current_job_.first_run_time = current_time_;
        }

        // [6] 시간 1초 흐름 반영
        left_slice_--;
        current_time_++;
        current_job_.remain_time--;

        // [7] 현재 실행 중인 작업의 이름 반환
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
