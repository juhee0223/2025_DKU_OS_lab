#include <pthread.h>
#include <iostream>
#ifndef SKIPLIST_H
#define SKIPLIST_H

/// @brief Key, Value, Update Count 구조체
struct KVC {
    int key;
    int value;
    int upd_cnt;
};

/// @brief Skip List 노드 구조체
struct Node {
    // 키
    int key;
    // 밸류
    int value;
    // 업데이트 횟수
    int upd_cnt;
    // 최대 레벨
    int level;
    // 다음 노드들의 포인터 배열 (레벨 별로 다음 노드 가르킴)
    Node** forward;
};

/// @brief SkipList 부모 클래스.
class DefaultSkipList {
    protected:
        // 최대 레벨
        int max_level_;
        // 확률 계수 (0.5)
        float prob_;
        // 헤드 노드
        Node* header_;
        
    public:
        DefaultSkipList(int max_level = 16, float prob = 0.5);
        virtual ~DefaultSkipList();
        
        /**
         * @brief 삽입 함수 : 인자로 받은 key와 value를 저장한다
         * 이미 key가 존재하는 경우, 기존 value에 새로운 value를 더하고 update count를 1 증가시킨다.
         * 
         * @param key 
         * @param value 
         */
        virtual void insert(int key, int value) = 0;

        /**
         * @brief 탐색 함수 : 인자로 받은 key의 value를 반환한다.
         * SkipList에 key가 존재하지 않을 경우, 0을 반환한다.
         * 
         * @param key 
         * @return int 
         */
        virtual int lookup(int key) = 0;

        /**
         * @brief 삭제 함수 : 인자로 받은 key가 저장된 노드를 삭제한다.
         * 
         * @param key 
         */
        virtual void remove(int key) = 0;

        /**
         * @brief 모든 노드를 순회하여, 배열에 저장한다.
         * check_answer 함수에서 사용되는 함수
         * @param arr 
         */
        void traversal(KVC* arr) {
            Node* current = header_->forward[0];
            int index = 0;
            while (current != nullptr) {
                arr[index].key = current->key;
                arr[index].value = current->value;
                arr[index].upd_cnt = current->upd_cnt;
                index++;
                current = current->forward[0];
            }
        };
    protected:
        /**
         * @brief 랜덤으로 레벨을 생성하는 함수
         * 
         * @return int 
         */
        virtual int random_level();
};
#endif // SKIPLIST_H
