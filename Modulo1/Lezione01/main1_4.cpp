#include <iostream>
#include <thread>
#include <ctime> // per generare numeri random
#include <chrono>



int saldo = 100;

int random()
{
    srand(time(0)); // inizializza il seed, ovvero inizializza il generatore casuale prendendo l'orario corrente
    // questo evita di ottenere sempre gli stessi numeri

    // genera un numero casuale tra 1 e 100
    return (int) rand() % 100 + 1;
}

void stop()
{
    int time = random();

    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}
void somma()
{
    stop();
    saldo += 100;
    std::cout << "SOMMA: " << saldo << std::endl;
}

void sottrai()
{
    stop();
    saldo -=50;
    std::cout << "SOTTRAI: " << saldo << std::endl;

}

void nada()
{
    stop();
    std::cout << "NADA: " << std::endl;
}




int main()
{
    // creaiamo e avviamo i thread
    std::thread t1(somma);
    auto t3 = std::thread(sottrai);
    auto t2= std::thread(nada);
    

    // il main deve aspettare che i thread abbiano finito i loro task prima di continuare
    t1.join();
    t2.join();
    t3.join();

    return 0;

}

/*
    Output:
    NADA: SOMMA: SOTTRAI: 50
    50 (il print della somma viene fatto dopo aver sottratto)
    ----------------------------
    SOMMA: SOTTRAI: 200200 (il print del sottrai viene fatto dopo aver sommato)
    NADA: 
    ----------------------------
    SOTTRAI: SOMMA: 5050 
    NADA:

    I risulati mostrano come i thread vengano eseguti in modo casuale e come
    modificano il valore di saldo. 
    


*/