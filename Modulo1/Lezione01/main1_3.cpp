#include <iostream>
#include <thread>

void thread1()
{
    std::cout << "THREAD 1" << std::endl;
}

void thread2()
{
    std::cout << "THREAD 2" << std::endl;
}

int main()
{
    // crea e avvia i thread
    std::thread t1(thread1);
    std::thread t2(thread2);

    // il main apsetta che i thread abbiano eseguito i loro compiti prima di continuare
    t1.join();
    t2.join();

    return 0;
}