#include <iostream>
#include <thread>
#include <chrono>

class ThreadGuard {

public:

    // costruttore
    ThreadGuard(std::thread& thread) : t(thread) {};

    // evitiamo che una ThreadGuard possa essere copiata
    // Questo perchè una ThreadGuard controlla un thread
    // e se potessimo copiarla, si avrebbero due guardie 
    // che controllano lo stesso thread e questo è pericoloso.
    // un tread deve avere solo un responsabile della sua gestione.

    // elimina il costruttore di copia. 
    // significa che non posso fare 
    //ThreadGuard guard1(t);
    //ThreadGuard guard2 = guard1; // ERRORE -> corrisponde a ThreadGuard guard2(guard1); ovvero chiama il copy constructor.
    // Copy Constructor: className (const ClassName &obj)
    // ovvero costruisco un oggetto ThreadGuard a partire da un altro esistente,
    // ovvero l'oggetto che costruisco è una copia di un oggetto esistente.
    ThreadGuard(ThreadGuard&) = delete;

    // elimina l'operatore di assegnazione.
    // significa che non posso fare
    //ThreadGuard guard1(t1);
    //ThreadGuard guard2(t2); -> deve gestire thread diverso da t1
    //guard2 = guard1; -> ERRORE
    // assegnazione: se guard2 esiste gia, provi ad assegnarli un altro valore, ovvero guard1
    ThreadGuard& operator = (ThreadGuard&) = delete;

    // distruttore
    ~ThreadGuard()
    {
        if(t.joinable())
        {
            t.join();
        }
    }

private:
    std::thread& t;

};

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

    // creo un blocco di scope, ovvero una zona del codice
    // in cui le viarabili definite esistono solo li dentro

    {
        // avvio e creo il thread t1
        auto t1 = std::thread(updateTime, &t1_start, &t1_end); 
        // avvio e creao il thread t2
        auto t2 = std::thread(updateTime, &t2_start, &t2_end);

        // crea due guardie, guard1 e guard2 che protegono rispettivamente
        // guard1 -> t1
        // guard2 -> t2
        ThreadGuard guard1{t1}, guard2{t2}; // più rpeciso
        //TheadGuard guard1(t1), guard2(t2); // è analogo, ottengo lo stesso risultato
    }

    std::cout << "Thread 1: " << t1_start << " - " << t1_end << std::endl;
    std::cout << "Thread 2: " << t2_start << " - " << t2_end << std::endl;

    return 0;
}