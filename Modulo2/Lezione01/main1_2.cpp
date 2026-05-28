#include <iostream>
#include <thread>
#include <chrono>
// usiamo questa libreria per vedere l'orarioin cui ogni thread termina la sua esecuzione

long long getCurrentEpochTime()
{
    // otteniamo l'istante time (now()) a partire dalla Unix Epoch (1 Gennaio 1970 ore 00:00) (time_since_epoch())
    auto time = std::chrono::system_clock::now().time_since_epoch();
    //converte time in millisecondi, dove count() estrae il valore numerico
    return std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}

void updateTime(long long* start, long long* end)
{
    *start = getCurrentEpochTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    *end = getCurrentEpochTime();
}


int main()
{
    long long t1_start, t1_end, t2_start, t2_end;

    // avvio e creo il thread t1
    auto t1 = std::thread(updateTime, &t1_start, &t1_end); 
    // avvio e creao il thread t2
    auto t2 = std::thread(updateTime, &t2_start, &t2_end);

    // l'ordine di creazione dei thread è garantito, ma non è garantito l'ordine di esecuzione.

    // facciamo in modo che il main aspetti che i thread abbiano completato i loro task
    //prima di terminare l'esecuzione del programma principale
    t1.join(); // aspetta che il thread t1 abbia completato i suoi task prima di continuare il main
    t2.join(); // aspetta che il thread t2 abbia completato i suoi task prima di continuare il main

    std::cout << "Thread 1: " << t1_start << " - " << t1_end << std::endl;
    std::cout << "Thread 2: " << t2_start << " - " << t2_end << std::endl;

    // il main fa questo:
    //crea t1 e crea t2
    //aspetta la fine di t1
    //aspetta la fine di t2
    //mostra i risultati
    //termina

    return 0;
}

//Output
/*

Thread 1: 1779360042368 - 1779360042475
Thread 2: 1779360042369 - 1779360042475
-----------------------------------------
Thread 1: 1779360682623 - 1779360682726
Thread 2: 1779360682623 - 1779360682726
-----------------------------------------
Thread 1: 1779360694207 - 1779360694317
Thread 2: 1779360694208 - 1779360694317
-----------------------------------------

Commento ai risultati:
I risultati mostrano che i due thread stanno lavorando quasi in parallelo.
Quindi, nei casi 1 e 3 t2 parte 1 millisecondo dopo t1. 

Mentre nel caso 2, i due thread sono avviati nello stesso momento. 
Poicchè ogni thread dura più o meno 100 millisecondi, finisocno nello stesso momento.



*/