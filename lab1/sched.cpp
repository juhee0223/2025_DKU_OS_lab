/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32221902
*	    Student name : 박주희
*	    Date : 2025.04.22.
*	    Contents : Lab1 RR, MLFQ, Stride, Lottery 클래스 구현하기
*/

#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <random>
#include <unordered_map>
#include "sched.h"

class RR : public Scheduler
{
private:
    int time_slice_;                 // 타임 퀀텀
    int left_slice_;                 // 남은 타임 퀀텀
    std::queue<Job> waiting_queue;   // 대기 큐

public:
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
        // [1] 현재 작업이 없고, 작업 큐에 작업이 있다면 첫 작업을 할당
        if (current_job_.name == 0 && !job_queue_.empty()) {
            current_job_ = job_queue_.front();  // job_queue에서 작업 꺼냄
            job_queue_.pop();                   // 큐에서 제거
        }

        // [2] 도착한 작업이 있으면 waiting_queue로 이동
        if (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
            waiting_queue.push(job_queue_.front()); // 레디 큐에 추가
            job_queue_.pop();                       // 작업 큐에서 제거
        }

        // [3] 현재 작업이 완료된 경우
        if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;  // 완료 시간 기록
            end_jobs_.push_back(current_job_);              // 완료 작업 목록에 저장
            left_slice_ = time_slice_;                      // 타임 퀀텀 초기화

            if (waiting_queue.empty()) return -1;           // 작업이 더 이상 없으면 종료

            current_job_ = waiting_queue.front();           // 다음 작업 할당
            waiting_queue.pop();
            current_time_ += switch_time_;                  // 컨텍스트 스위치 시간 반영
        }

        // [4] 타임 퀀텀이 소진된 경우 (작업 미완료 상태에서 선점)
        if (left_slice_ == 0) {
            if (!waiting_queue.empty()) {
                waiting_queue.push(current_job_);           // 현재 작업을 다시 레디 큐로 보냄
                current_job_ = waiting_queue.front();       // 새 작업 할당
                waiting_queue.pop();
                current_time_ += switch_time_;              // 컨텍스트 스위치 시간 반영
            }
            left_slice_ = time_slice_;                      // 타임 퀀텀 초기화
        }

        // [5] 처음 실행되는 작업이면 first_run_time 기록
        if (current_job_.service_time == current_job_.remain_time) {
            current_job_.first_run_time = current_time_;
        }

        // [6] 1초 실행
        left_slice_--;                 // 남은 타임 퀀텀 감소
        current_time_++;              // 전체 시간 증가
        current_job_.remain_time--;   // 작업 남은 시간 감소

        // [7] 현재 실행 중인 작업의 이름 반환
        return current_job_.name;
    }
};


class FeedBack : public Scheduler {
private:
    std::queue<Job> queue[4];           // 4개의 큐 (0~3)
    int quantum[4] = {1, 1, 1, 1};  // 각 큐의 타임 슬라이스
    int left_slice_ = 0;            // 남은 타임 슬라이스
    int current_queue;           // 현재 작업의 큐 인덱스
    
public:
    FeedBack(std::queue<Job> jobs, double switch_overhead, bool is_2i)
        : Scheduler(jobs, switch_overhead) {
        if (is_2i) {    // 방식에 맞게 타임퀀텀 설정
            name = "FeedBack_2i";
            quantum[0] = 1;
            quantum[1] = 2;
            quantum[2] = 4;
            quantum[3] = 8;
        } else {    //
            name = "FeedBack_1";
        }
    }

    void assignNewJob(int queue_index) {
        current_job_ = queue[queue_index].front();  // 해당 큐에서 작업 가져오기
        queue[queue_index].pop();                  // 큐에서 작업 제거
        current_queue = queue_index;               // 현재 큐 인덱스 업데이트
        left_slice_ = quantum[queue_index];        // 남은 타임 슬라이스 초기화
    }

    int run() override {
        bool areAllQueuesEmpty = true;              // 모든 큐가 비어있는지 확인
        int current_highest_queue = 0;              // 가장 높은 우선순위이면서 비어있지 않은 큐 인덱스

        // [1] 현재 시간에 도착한 작업들을 queue[0]에 추가
        while (!job_queue_.empty() && job_queue_.front().arrival_time <= current_time_) {
            queue[0].push(job_queue_.front());  // 큐0에 추가
            job_queue_.pop();                 // job_queue에서 제거
        }

        // [2] 가장 높은 우선순위의 비어있지 않은 큐를 탐색
        for (int i = 0; i < 4; ++i) {
            if (!queue[i].empty()) {
                current_highest_queue = i;
                areAllQueuesEmpty = false;
                break;
            }
        }

        // [3] 첫 번째 작업 할당 (현재시각 0에서 시작할 때만 실행)
        if (!queue[0].empty() && current_time_ == 0) {
            assignNewJob(0); 
        
        }

        // [4] 현재 작업이 완료된 경우
        else if (current_job_.remain_time == 0) {
            current_job_.completion_time = current_time_;      // 완료 시간 기록
            end_jobs_.push_back(current_job_);                  // 완료 리스트에 저장

            // 모든 큐와 job_queue가 비었다면 스케줄러 종료
            if (areAllQueuesEmpty && job_queue_.empty()) {
                return -1;
            }

            // 다음 작업 할당
            current_job_ = queue[current_highest_queue].front();
            queue[current_highest_queue].pop();
            left_slice_ = quantum[current_highest_queue];   // 남은 타임 슬라이스 초기화
            current_queue = current_highest_queue;
            current_time_ += switch_time_;                      // 컨텍스트 스위치 시간 적용
        }

        // [5] 타임 슬라이스가 소진된 경우 (작업 미완료 상태에서 큐 이동 또는 유지)
        else if (left_slice_ == 0) {
            if (!areAllQueuesEmpty) {
                // 현재 할당 가능한 작업의 우선순위가 지금 작업보다 높거나 같은 경우
                if (current_highest_queue <= current_queue + 1) {
                    // 현재 작업을 다음 큐 또는 같은 큐로 이동
                    if (current_queue == 3) {
                        queue[current_queue].push(current_job_);
                    } else {
                        queue[current_queue + 1].push(current_job_);
                    }
                    // 우선순위가 높은 작업으로 교체
                    assignNewJob(current_highest_queue);  // 함수 호출로 대체
                    current_time_ += switch_time_;  // 컨텍스트 스위치 오버헤드 시간 적용
                }
                // 우선순위 높은 작업이 없으면 현재 작업을 계속 진행하되 슬라이스 재설정
                else {
                    current_queue++;
                    left_slice_ = quantum[current_queue];
                }
            }
            // 큐가 비어있는 경우 슬라이스만 초기화
            else {
                left_slice_ = quantum[current_queue];
            }
        }

        // [6] 현재 작업이 처음 실행되는 경우 first_run_time 기록
        if (current_job_.service_time == current_job_.remain_time) {
            current_job_.first_run_time = current_time_;
        }

        // [7] 작업 실행: 시간 및 상태 업데이트
        current_time_++;
        current_job_.remain_time--;
        left_slice_--;

        // [8] 현재 실행 중인 작업 이름 반환
        return current_job_.name;
    }
};

class Stride : public Scheduler {
private:
    std::unordered_map<int, int> pass_map;       // 각 작업의 현재 pass 값을 저장하는 맵
    std::unordered_map<int, int> stride_map;     // 각 작업의 stride 값을 저장하는 맵
    const int BIG_NUMBER = 10000;                // stride 계산 시 사용할 큰 정수 값
    int previousJobName = -1;                    // 직전에 실행한 작업의 이름을 저장 (context switch 판단용)

public:
    // 생성자: 초기화 시 모든 작업에 대해 stride와 pass 값을 설정
    Stride(std::list<Job> jobs, double switchOverhead) : Scheduler(jobs, switchOverhead) {
        name = "Stride";

        // job_list_의 각 작업에 대해 stride와 pass 초기화
        for (auto& job : job_list_) {
            stride_map[job.name] = BIG_NUMBER / job.tickets; // stride는 ticket 수에 반비례
            pass_map[job.name] = 0;                          // 초기 pass는 0으로 설정
        }
    }

    int run() override {
    // [1] 모든 작업이 완료되었는지 확인
    if (std::all_of(job_list_.begin(), job_list_.end(), [](const Job& job) {
            return job.remain_time <= 0;
        })) {
        return -1; // 모든 작업 완료 시 종료
    }

    // [2] 아직 완료되지 않은 작업 중 pass 값이 가장 작은 작업 선택
    auto selectedJobIt = std::min_element(job_list_.begin(), job_list_.end(), [&](const Job& a, const Job& b) {
        if (a.remain_time <= 0) return false; // 완료된 작업은 제외
        if (b.remain_time <= 0) return true;
        return pass_map[a.name] < pass_map[b.name]; // pass 값이 작은 작업 선택
    });

    // [3] 유효한 작업이 선택된 경우 실행
    if (selectedJobIt != job_list_.end()) {
        Job& selectedJob = *selectedJobIt; // 선택된 작업 참조

        // [4] 직전 작업과 다른 경우 context switch 시간 반영
        if (previousJobName != -1 && previousJobName != selectedJob.name) {
            current_time_ += switch_time_;
        }

        // [5] 처음 실행되는 작업이면 first_run_time 설정
        if (selectedJob.service_time == selectedJob.remain_time) {
            selectedJob.first_run_time = current_time_;
        }

        // [6] 작업 실행 (1초)
        current_time_++;                      // 현재 시간 증가
        selectedJob.remain_time--;           // 작업의 남은 시간 감소
        pass_map[selectedJob.name] += stride_map[selectedJob.name]; // pass 값 갱신

        // [7] 작업 완료 시 completion_time 기록 및 결과 리스트에 추가
        if (selectedJob.remain_time == 0) {
            selectedJob.completion_time = current_time_;
            end_jobs_.push_back(selectedJob);
        }

        // [8] 이전 작업 이름 갱신 및 현재 작업 이름 반환
        previousJobName = selectedJob.name;
        return selectedJob.name;
    }

    // [9] 유효한 작업을 찾지 못했을 경우 종료
    return -1;
    }
};

class Lottery : public Scheduler {
private:
    int counter = 0;                    // 티켓 누적 카운터
    int total_tickets = 0;              // 현재 남은 전체 티켓 수
    int winner = 0;                     // 당첨 티켓 번호
    std::mt19937 gen;                   // 난수 생성기
    int previousJob = -1;               // 직전에 실행한 작업 이름 (context switch 판별용)

public:
    // 생성자: 작업 리스트 기반으로 스케줄러 초기화
    Lottery(std::list<Job> jobs, double switchOverhead)
        : Scheduler(jobs, switchOverhead), previousJob(-1) {
        name = "Lottery";

        // 고정된 시드값으로 난수 생성기 초기화
        uint seed = 10; // seed 값 수정 금지
        gen = std::mt19937(seed);

        // 전체 티켓 수 초기 계산
        total_tickets = 0;
        for (const auto& job : job_list_) { // 모든 작업에 대해
            total_tickets += job.tickets;   // 티켓 수 누적
        }
    }

    // 지정된 범위 내에서 난수 하나 생성
    int getRandomNumber(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    int run() override {
    // [1] 남은 작업이 있는지 확인하고, 총 티켓 수 계산
    bool allJobsDone = true; // 모든 작업이 완료되었는지 여부 초기화
    total_tickets = 0;  // 현재 기준의 총 티켓 수 초기화

    for (const auto& job : job_list_) {
        if (job.remain_time > 0) {
            allJobsDone = false;           // 완료되지 않은 작업이 있음을 표시
            total_tickets += job.tickets; // 티켓 수 누적
        }
    }

    // [2] 모든 작업이 완료되었으면 종료
    if (allJobsDone) {
        return -1;
    }

    // [3] 무작위 당첨 티켓 번호 추첨
    winner = getRandomNumber(0, total_tickets - 1);

    // [4] 당첨된 작업을 찾기 위해 job_list_ 순회
    int accumulated_tickets = 0; // 누적 티켓 수를 추적
    auto selectedJobIt = std::find_if(job_list_.begin(), job_list_.end(), [&](Job& job) {
        if (job.remain_time > 0) {
            accumulated_tickets += job.tickets;
            return accumulated_tickets > winner; // 당첨 티켓 번호를 초과하면 선택
        }
        return false;
    });

    // [5] 유효한 작업이 선택된 경우 실행
    if (selectedJobIt != job_list_.end()) {
        Job& selectedJob = *selectedJobIt; // 선택된 작업 참조

        // [6] 이전 작업과 다르면 context switch 시간 추가
        if (previousJob != -1 && previousJob != selectedJob.name) {
            current_time_ += switch_time_;
        }

        // [7] 처음 실행되는 작업이면 first_run_time 기록
        if (selectedJob.service_time == selectedJob.remain_time) {
            selectedJob.first_run_time = current_time_;
        }

        // [8] 작업 실행 (1초)
        current_time_++;              // 현재 시간 증가
        selectedJob.remain_time--;    // 작업의 남은 시간 감소

        // [9] 작업 완료 시 정보 기록 및 종료 리스트에 추가
        if (selectedJob.remain_time == 0) {
            selectedJob.completion_time = current_time_;
            end_jobs_.push_back(selectedJob);
        }

        // [10] 이전 작업 이름 갱신 및 현재 작업 이름 반환
        previousJob = selectedJob.name;
        return selectedJob.name;
    }

    // [11] 유효한 작업을 찾지 못했을 경우 종료
    return -1;
    }
};