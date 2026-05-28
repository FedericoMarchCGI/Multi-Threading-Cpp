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

// ThreadGuard
class ThreadGuard {
private:
    std::thread& thread_;
    
public:
    explicit ThreadGuard(std::thread& t) : thread_(t) {}
    
    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    // Prevent copying
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};


// dato un analyzerID e un vector di dati, calcola il richio 
void riskAnalyzer(int analyzerId, const std::vector<double>& data, 
                  double& result) {
    try {
        if (data.empty()) {
            throw std::runtime_error("No data to analyze");
        }
        
        double sum = 0.0;
        // per ogni valore nel vector data (simile alle liste di Python)
        for (double value : data) {
            sum += value * value;  // Risk calculation simulation
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        
        result = sum / data.size();
        std::cout << "Analyzer " << analyzerId << " calculated risk: " 
                  << result << std::endl;
                  
    } catch (const std::exception& e) {
        std::cerr << "Analyzer " << analyzerId << " error: " 
                  << e.what() << std::endl;
        result = -1.0;
    }
}

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
    // questi thread vanno a riempire un vector con 100 elementi
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

    TASK 2:

    Using the code below, create a system that:

    - A -> Divides the collected data into segments for parallel risk analysis
    Esempio con 12 dati:

    sharedData:
    [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120]

    Li puoi dividere in 4 segmenti:

    Segmento 1: [10, 20, 30]
    Segmento 2: [40, 50, 60]
    Segmento 3: [70, 80, 90]
    Segmento 4: [100, 110, 120]

    Poi crei 4 thread:

    Thread 1 analizza Segmento 1
    Thread 2 analizza Segmento 2
    Thread 3 analizza Segmento 3
    Thread 4 analizza Segmento 4

    - B -> Uses ThreadGuard to ensure all threads complete properly

    - C -> Handles exceptions that might occur during processing

    Test with both valid data and empty data segments to observe exception handling.


*/