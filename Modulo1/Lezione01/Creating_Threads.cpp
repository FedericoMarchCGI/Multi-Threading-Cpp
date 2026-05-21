#include <iostream>
#include <thread>

void hello()
{
    std::cout << "HELLO!" << std::endl;
}

int main()
{
    std::thread t1(hello); // crea un thread che esegue la funzione hello()

    t1.join(); // prima di terminare il main, aspetta che il thread t1 esegua la funzione hello() e termini.

    return 0; // termina il main
}