#include <iostream>
#include <thread>
#include <chrono>
// usiamo questa libreria per vedere l'orarioin cui ogni thread termina la sua esecuzione

int main()
{
    // funct è una funzione anonima, chiamata anche lamba function.
    // in pratica sto creando una funzione direttamente dentro al main
    // auto -> fa si che C++ capisca automaticamente il tipo della variabile.
    // quindi, scrivendo auto func, il compilatore capisce che func è una funzione lambda.
    // [] -> signfica che la funzione lambda non usa variabili esterne
    // () -> sono i parametri passati in input alla funzione. 
    //Poichè le parentesi () sono vuote, la funzione non riceve parametri.
    auto func = []()
    {
        int i = 0;
        i++;

        std::cout << i << "\n" << std::endl;
    };   

    auto t1 = std::thread(func); // crea un thread t1 che esegue la funzione func
    auto t2 = std::thread(func);

    // facciamo in modo che il main aspetti che i thread abbiano completato i loro task
    //prima di terminare l'esecuzione del programma principale
    t1.join();
    t2.join();

    return 0;
}