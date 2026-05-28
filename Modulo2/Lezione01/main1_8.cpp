// vogliamo avere due thread che siano coordinati tra loro.
// ovvero avere un thread2 il cui comportamento è influenzato
// ed è dipendente dal thread 1. 
// per far questo useremo la libreria <condition_variable> e 
// introdurremo una seconda funzione lambda.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int main()
{
    int counter = 0;
    std::mutex counterMutex;
    // definiamo una contiion_variable che consenta di
    // abilitare la comunicazione tra queste due funzioni lambda.
    std::condition_variable condVar;

    // vogliamo incrementare questo contatore usando i threads.

    // fuzione anonima 1 (lambda)
    auto count_one_million = [&counter, &counterMutex, &condVar] () {
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

            condVar.notify_all();
            // quando il thread che sta lavorando su questa funzione
            // ha finito il suo accesso privilegiato
            // manda una notifica agli altri thread che erano rimasti
            // in attesa per l'accesso alla risorsa. 
            // questi thread in attesa si svegliano.
        }
    };


    // funzione anonima 2 (lambda)
    auto count_down_from_one_million = [&counter, &counterMutex, &condVar] () {
        for (int i = 0; i < 1000000; i ++)
        {
            // usiamo il RAII per gestire 
            //automaticamente il lock() e unlock() del registro

            std::unique_lock<std::mutex> lock(counterMutex);

            condVar.wait(lock, [&counter]() {return counter > 0;});
            //condVar.wait();
            // mette il thread corrente che gestisce questa funzione in attesa su
            //condVar, ovvero il thread si "addormenta" e smette
            //temporaneamente di eseguire il codice, senza consumare CPU inultimente,
            // aspettando un segnale sulla variabile di condizione condVar.

            // condVar.wait(lock, ...)
            // dice al thread che se la condizione non è vera, fermati qui e 
            // aspetta che qualcuno ti avvisi.
            // ovvero il thread che gestisce questa funzione si ferma
            // fino a che il valore di counter non è > 0.

            // la funzione lambda [&counter]() {return counter > 0;}
            // crea la condizione di riavvio del thread, usando un riferimento a counter.

            // STEP della wait:
            // 1. Il thread controlla la condizione;
            // 2. se la condizione è falsa, il thread rilascia temporanemanete il mutex;
            // 3. e il thread si mette a dormire.
            // 4. quando altri thread fanno notify_all(), il thread dormiente si risveglia
            // 5. riprende il mutex
            // 6. ricontrolla la condizione
            // 7. se la condizione è falsa, si rimette a dormire.
            // 8. se la condizione è vera il thread prende il controllo


            // aggiorna contatore
            counter--;
        }
    };


    // creiamo due thread che gestiscono la funzione
    //count_one_million

    std::thread t1(count_one_million);
    std::thread t2(count_down_from_one_million);

    // il main aspetta che i thread abbiano finito
    t1.join();
    t2.join();

    // stampiamo il valore finale di counter
    std::cout << "Counter: " << counter << std::endl;

    return 0;
}

/*

    Come funziona questa sincronizzazione.

    Immagina due thread.

    Thread A

    aspetta che succeda qualcosa.

    Thread B

    produce/modifica dati e poi avvisa.

    Esempio semplice.

    Thread A
    std::unique_lock<std::mutex> lock(counterMutex);

    condVar.wait(lock, [&counter]() {
        return counter > 0;
    });

    std::cout << "counter pronto\n";

    Questo thread dice:

    “Finché counter NON è > 0, mi fermo.”
    Thread B
    counter++;

    condVar.notify_all();

    Questo thread:

    modifica counter
    manda un segnale:
    “Ehi, qualcosa è cambiato!”

    A questo punto il thread A si risveglia, ricontrolla:

    counter > 0

    e se è vero continua.


    COMMENTO AI RISULTATI:

    Counter = 0

    t2 funziona così:

    1. guarda counter;
    2. se counter == 0, si addormenta;
    3. quando t1 fa counter++, chiama notify_all();
    4. t2 si sveglia;
    5. vede che counter > 0;
    6. fa counter--.

    Quindi t2 consuma ogni incremento prodotto da t1.
    Pertanto l'output è 0.
*/