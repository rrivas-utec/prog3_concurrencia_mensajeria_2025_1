#include <iostream>
#include "queue.h"
#include <vector>
#include <thread>

void ejercicio_1() {
    std::mutex mtx;
    std::vector v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    UTEC::Queue<int> q1(v.begin(), v.end());
    UTEC::Queue<int> q3({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});


    std::cout << q1.front() << std::endl;
    std::cout << q1.back() << std::endl;

    std::cout << std::string(20, '-') << std::endl;
    // while (!q2.empty()) {
    //     std::cout << q2.front() << std::endl;
    //     q2.pop();
    // }
    std::cout << std::string(20, '-') << std::endl;
    if (q1.empty()) { std::cout << "Queue is empty" << std::endl; }
    std::cout << q1.size() << std::endl;
}

void ejercicio_2() {
    UTEC::Queue<int> q1;

    int nt = 1000;
    std::vector<std::jthread> vt_pushes;
    std::vector<std::jthread> vt_popes;
    vt_pushes.reserve(nt);
    vt_popes.reserve(nt);
    for (int i = 0; i < nt; i++) {
        vt_pushes.emplace_back([&q1]{ q1.push(1); });
        vt_popes.emplace_back([&q1]{ q1.pop(); });
    }
    for (int i = 0; i < nt; i++) {
        vt_pushes[i].join();
        vt_popes[i].join();
    }
    std::cout << q1.size() << std::endl;

}

int main() {
    // ejercicio_1();
    ejercicio_2();
 }
