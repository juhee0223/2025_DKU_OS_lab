/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure : Skip List)
*	    Student id : 
*	    Student name : 
*	    Date : 
*/

#include "skiplist_impl.h"
#include <cstdlib>
#include <ctime>

// DefaultSkipList 생성자
DefaultSkipList::DefaultSkipList(int max_level, float prob) {
    this->max_level_ = max_level;
    this->prob_ = prob;
    
    header_ = new Node();
    header_->key = -1;  
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;
    
    header_->forward = new Node*[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++) {
        header_->forward[i] = nullptr;
    }
    
    srand(time(nullptr));
}

// DefaultSkipList 소멸자
DefaultSkipList::~DefaultSkipList() {
    Node* current = header_;
    Node* temp;
    
    while(current) {
        temp = current->forward[0];
        delete[] current->forward;
        delete current;
        current = temp;
    }
}

// 랜덤 레벨 생성 함수
int DefaultSkipList::random_level() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < prob_ && 
            level < max_level_) {
        level++;
    }
    return level;
}

// SkipList 생성자
SkipList::SkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {}

// SkipList 소멸자
SkipList::~SkipList() {}

void SkipList::insert(int key, int value) {
    // TODO
}

int SkipList::lookup(int key) {
    // TODO
}

void SkipList::remove(int key) {
    // TODO
}

// CoarseSkipList 생성자
CoarseSkipList::CoarseSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {
    pthread_mutex_init(&mutex_lock, nullptr);
}

// CoarseSkipList 소멸자
CoarseSkipList::~CoarseSkipList() {
    pthread_mutex_destroy(&mutex_lock);
}

void CoarseSkipList::insert(int key, int value) {
    // TODO
}

int CoarseSkipList::lookup(int key) {
    // TODO
}

void CoarseSkipList::remove(int key) {
    // TODO
}

// FineSkipList 생성자
FineSkipList::FineSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {
    delete header_;
    header_ = new FineNode();
    header_->key = -1;
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;
    
    header_->forward = new Node*[max_level_ + 1];
    for (int level = 0; level <= max_level_; level++) {
        header_->forward[level] = nullptr;
    }
}

// FineSkipList 소멸자
FineSkipList::~FineSkipList() {
    FineNode* current = (FineNode*)header_;
    FineNode* temp;
    
    while(current) {
        temp = (FineNode*)current->forward[0];
        pthread_mutex_destroy(&((FineNode*)current)->lock);
        delete[] current->forward;
        delete current;
        current = temp;
    }
    
    header_ = nullptr;
}

void FineSkipList::insert(int key, int value) {
    // TODO
}

int FineSkipList::lookup(int key) {
    // TODO
}

void FineSkipList::remove(int key) {
    // TODO
}
