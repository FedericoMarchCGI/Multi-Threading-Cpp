// esempio di utilizzo di MUTEX per coordinare i thread tra loro
#include <iostream>
#include <thread>
#include <mutex>

int main()
{
    int counter = 0;
    std::mutex counterMutex;

    // vogliamo incrementare questo contatore usando i threads.

    // fuzione anonima
    auto count_one_million = [&counter, &counterMutex] () {
        for (int i = 0; i < 1000000; i++)
        {
            counterMutex.lock();
            // il registro ora è bloccato.
            // quindi solo un thread alla volta può accedervi
            
            // il contatore deve essere modificato solo durante
            // questo accesso esclusivo alla memoria.
            counter++;

            // una volta terminato, libero la memoria bloccata
            // quindi altri thread possono accedere.
            counterMutex.unlock();
        }
    };

    // creiamo due thread che gestiscono la funzione
    //count_one_million

    std::thread t1(count_one_million);
    std::thread t2(count_one_million);

    // il main aspetta che i thread abbiano finito
    t1.join();
    t2.join();

    // stampiamo il valore finale di counter
    std::cout << "Counter: " << counter << std::endl;

    return 0;
}

/*
    COMMENTO AI RISULTATI

    Quello che succede è che ora Counter = 2000000.
    Questo risultato è lo stesso per ogni esecuzione del codice. 

    Ciò avviene in quanto l'accesso a counter++ da parte
    dei thread avviene in maniera controllata. 

    Ottengo Counter = 2000000 perchè ho due thread che stanno 
    contando fino a 1 milione ma modificando la stessa variabile.
*/   