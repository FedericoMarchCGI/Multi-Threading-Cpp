#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>

std::mutex dataMutex;
std::vector<double> sharedData;
std::atomic<int> processedCount{0};

// il processore processorID (ad esempio 1), deve elaborare itemCount elementi (ad esempio 5)
void dataProcessor(int processorId, int itemCount) {
    // prepara l'ambiente per generare un numero casuale tra 1 e 100 in virgola mobile (double)
    std::random_device rd; // genera un seme casuale, ovvero il punto di partenza dei numeri casuali.
    std::mt19937 gen(rd()); // crea un generatore di numeri pseudo-casuali. Il generatore gen sta usando come seme il valore prodotto da rd(),
    // dove rd() produce un valore casuale iniziale. 
    std::uniform_real_distribution<> dis(1.0, 100.0); // definisce una distribuzione uniforme di numeri reali tra 10 e 100.0
    // uniform significa che ogni valore dell'intervallo ha la stessa probabilità di uscire. 
    
    for (int i = 0; i < itemCount; ++i) {
        // per ogni iterazione genera un numero casuale double tra 1 e 100
        double value = dis(gen);
        
        // Thread-safe data insertion
        {
            // mutex
            std::lock_guard<std::mutex> lock(dataMutex);
            sharedData.push_back(value); // push_back aggiunge un elemento alla fine di un vector (array dinamico)
            //std::cout << "Value: " << value << std::endl;
        }
        
        processedCount.fetch_add(1); // fetch_add è una funzione delle variabili atomiche che consente di incrementare la variabile atomica. Il comportamento è simile a counter++
        
        // Simulate processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "Processor " << processorId << " completed processing " 
              << itemCount << " items\n";
}


 

int main() {

    // Task 1 - A 
    //dataProcessor(1,5);

    // Task 1-C

    // creiamo i thread e avviamoli
    std::thread t1(dataProcessor,1, 20);
    std::thread t2(dataProcessor,2, 20);
    std::thread t3(dataProcessor,3, 20);
    std::thread t4(dataProcessor,4, 20);
    std::thread t5(dataProcessor,5, 20);

    // il manin deve aspettare che i thread finisciano prima di continuare
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();


    return 0;
}


/*

    TASK 1:

    Run the code below and observe the output. Think about:

    -A -> How does std::lock_guard ensure thread safety when accessing sharedData?
    assicura un accesso sicuro a datashared in quanto utilizza un mutex, ovvero l'accesso alla risorsa sharedData è consentito solo per un thread alla volta

    - B-> Why is std::atomic used for processedCount instead of a regular integer?
    poichè per incrementare un valore, usare un integer regolare come counter++ , prevede la lettura del valore attuale di counter, poi aumetare counter di 1 
    e poi salvare il valore ottenuto dentro counter. Quindi, stiamo facendo tre azioni intermedie diverse per arrivare al risultato finale.
    Questo può causare che i thread accedano alla risorsa counter durante le tre azioni intermedie e quindi la modifica di counter 
    può non essere effettuata correttamente.
    usare una variabile atomica per il counter ci consente di avere un incremento atomico, ovvero che tutte quest tre azioni di lettura, incremento e scrittura del nuovo valore
    vengono considerate come una azione e quindi svolte tutte inseme e non possono essere interrotte nelle varie fasi intermedie, come invece accadrebbe per un regular integer.
    quindi usare la variabile atomica garantisce un'integrità del dato processedCount.

    - C -> Modify the code to create 5 threads, each processing 20 data items. Test your changes and observe:

    - D -> How the total processed count matches the expected value (100) (verificare che il numerod egli elementi elaborati sia 100, come previsto se ogni thread elabora 20 dati)
        Processor Processor 3 completed processing 20 items
        Processor 5 completed processing 20 items
        Processor 1 completed processing 20 items
        4 completed processing 20 items
        Processor 2 completed processing 20 items

        20 + 20 + 20 + 20 + 20 = 100, quindi corretto

    - E -> Whether the output messages from different threads appear in order

    si l'ordine dei messaggi di output è casuale, questo dipende da come vengono eseguiti i thread,
    in quanto l'oridne di esecuzione è gestito dallo scheduler. 



*/