#include <iostream>
#include <vector>
#include <thread>

void myInit(std::vector<int>& vector, int start, int end) {
    for (int i = start; i < end; i++)
        vector.at(i) = rand() % 1001;
}

int main() {
    system("chcp 1251");
    srand(static_cast<unsigned int>(time(0)));

    std::vector<int> vect(100000000);

    time_t timer = clock(); // фиксируем время старта

    // Определяем количество аппаратных потоков, доступных системе
    unsigned int numThreads = std::thread::hardware_concurrency();

    int size = static_cast<int>(vect.size());
    int chunk = size / static_cast<int>(numThreads); // размер куска на один поток (целочисленное деление)
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (unsigned int t = 0; t < numThreads; t++) {
        int start = static_cast<int>(t) * chunk; // начало диапазона для текущего потока

        int end = (t == numThreads - 1) ? size : start + chunk;

        threads.emplace_back(myInit, std::ref(vect), start, end);
    }

    for (auto& th : threads)
        th.join();

    std::cout << "Time: "
        << (clock() - timer) / 1000.0 << " сек." << std::endl;

    return 0;
}