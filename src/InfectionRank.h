#ifndef PATIENTMANAGER_INFECTIONRANK_H
#define PATIENTMANAGER_INFECTIONRANK_H

#include <list>
#include <iostream>

// Class for representing all patients with the same infection count.
// Contains a list with the ID numbers of the relevant patients.

class InfectionRank {
    unsigned int rank;
    std::list<unsigned int > patientList;

public:
    explicit InfectionRank(unsigned int infectionRank);

    std::list<unsigned int>::iterator addPatient(unsigned int patientID);

    unsigned int getRank() const;

    bool isEmpty() const;

    void removePatient(std::list<unsigned int>::iterator patient);

    void getTopK(int k, int * count , unsigned int* idArray); // Used to get the K top spreaders.

};

#endif
