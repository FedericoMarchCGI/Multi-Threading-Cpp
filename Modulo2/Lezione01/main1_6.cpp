// Questo Programma tratterà dell'imprevedibilità dei thread

#include <iostream>
#include <thread>

int main()
{
    int counter = 0;

    // vogliamo incrementare questo contatore usando i threads.

    // fuzione anonima
    auto count_one_million = [&counter] () {
        for (int i = 0; i < 1000000; i++)
        {
            counter++;
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

    Ogni volta che eseguiamo il programma non otteniamo 1000000
    ma otteniamo altri valori ed i valori che otteniamo
    sono tutti diversi tra loro ad ogni esecuzione del programma.

    Counter: 1275698
    -----------------
    Counter: 1064037
    -----------------
    Counter: 1317767

    Questo accade perchè due thread stanno accedendo
    contemporanemanete allo stesso registro di memoria e lo stanno
    modificando contemporanemanete. 

    Pertanto, per fixare questo problema dobbiamo coordinare ithread. 

    

*/