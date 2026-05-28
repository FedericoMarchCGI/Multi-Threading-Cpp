// scriviamo due thread che modificano il valore di counter fino ad arrivare
// a counter == 1000000

#include <iostream>
#include <thread>
#include <mutex>

int main()
{
    int counter = 0;
    std::mutex counterMutex;

    // funzione lambda

    auto count_until_one_million = [&counter, &counterMutex]() {

        while(true)
        {
            // mutex che gestisce automaticamente il lock e unlock della risorsa condivisa per l'accesso di un solo thread alla volta.
            std::lock_guard<std::mutex> lock(counterMutex);
            // lock_guard è una classe che gestisce automaticamente un mutex. 
            // lock è il nome della variabile di tipo lock_guard<std::mutex>
            // dentro le parentesi del costruttore vado a mettere il mutex che voglio bloccare.
            
            // lockguard è più rigido di unique_lock, in quanto blocca subito il mutex e non è possibile controllare il mutex
            // durante la vita dell'oggetto lock
            
            //unique_lock è più flessibile perchè lascia controllare il mutex durante
            // la vita dell'oggetto lock.

            
            // interrompi il ciclo quando counter supera 1000000
            if (counter >= 1000000)
            {
                break;
            }

            counter++;
            
        }
    };


    // avviare i thread
    std::thread t1(count_until_one_million);
    std::thread t2(count_until_one_million);


    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;

    return 0;


}