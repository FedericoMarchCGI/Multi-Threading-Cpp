#include <iostream>
#include <thread>
#include <atomic>

int main()
{

    std::atomic<int> counter(0);
    // la variabile atomic evita problemi di concorrenza tra i thread.
    // in questo modo, viene creato un intero speciale chiamato counter e inizializzato a 0,
    // che può essere modificato in sicurezza da più thread contemporaneamente.
    // un'operazione atomica è un operazione che avviene comlpetamente
    // oppure non avviene, senza che altri thread possano interferire a metà
    // (counter++ -> sono tre operazioni, ovvero lettura, aggiungi 1, scrivi nuovo valore)

    // funzione lambda
    auto count_one_million = [&counter] () {
        for (int i = 0; i < 1000000; i++)
        {
            counter ++;
        }
    };

    std::thread t1(count_one_million);
    std::thread t2(count_one_million);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;

    return 0;
}

/*

    Counter = 2000000

    perchè t1 incrementa il contatore di 1000000 volte.
    e anche t2 incrementa di 1000000 volte counter. 

    
*/