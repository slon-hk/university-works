#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

class CircularDeque
{       
private:
    vector<int> arr;
    size_t cap;
    size_t head;
    size_t count;
    bool force;

    size_t back_index() const {
        return (head + count - 1) % cap;
    }

    size_t free_back_index() const {
        return (head + count) % cap;
    }

    void ensure_full() const{
        if(this->count == this->cap && this->force == false){
            throw overflow_error("Index out of range!");
        }
    }

    void ensure_not_empty() const{
        if(this->count == 0){
            throw underflow_error("Deque is empty");
        }
    }

    size_t next_index(size_t x) const { return (x + 1) % cap; }
    size_t prev_index(size_t x) const { return (x + cap - 1) % cap; }

public:
    explicit CircularDeque(size_t N, bool push_force = false){
        if(N == 0) throw invalid_argument("N must be N>0");
        this->cap = N;
        this->head = 0;
        this->count = 0;
        this->force = push_force;
        this->arr.resize(N);
    }

    void push_front(int x){
        ensure_full();
        size_t idx = prev_index(head);
        arr[idx] = x;
        head = idx;
        if (count < cap) count++;
    }

    void push_back(int x){
        ensure_full();
        if(cap==count){
            arr[this->head] = x;
            this->head = next_index(this->head);
        } else {
            arr[free_back_index()] = x;
            count++;
        }
    }

    int pop_front(){
        ensure_not_empty();
        int x = arr[head];
        head = next_index(head);
        count--;
        return x;
    }

    int pop_back(){
        ensure_not_empty();
        size_t idx = back_index();
        int x = arr[idx];
        count--;
        return x;
    }

    int& front(){
        ensure_not_empty();
        return arr[head];
    }
    int& back(){
        ensure_not_empty();
        return arr[back_index()];
    }

    int size() const{
        return count;
    }

    bool empty() const{
        return count == 0;
    }

    bool full() const{
        return count==cap;
    }

    void resize(size_t new_cap){
       if (new_cap == 0) throw invalid_argument("new_cap must be > 0");

        size_t new_count = (count < new_cap) ? count : new_cap;
        vector<int> new_arr(new_cap);
        for (size_t i = 0; i < new_count; i++) {
            new_arr[i] = arr[(head + i) % cap];
        }

        arr = new_arr;
        cap = new_cap;
        head = 0;
        count = new_count;
    }
};