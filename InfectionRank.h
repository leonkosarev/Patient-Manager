#ifndef PATIENTMANAGER_INFECTIONRANK_H
#define PATIENTMANAGER_INFECTIONRANK_H

#include <list>
#include <iostream>

class InfectionRank {
    unsigned int rank;
    std::list<unsigned int > patientList;

public:
    explicit InfectionRank(unsigned int infectionRank);


    std::list<unsigned int>::iterator addPatient(unsigned int patientID);

    unsigned int getRank() const;

    bool isEmpty() const;

    void removePatient(std::list<unsigned int>::iterator patient);

    void getTopK(int k, int * count , unsigned int* idArray);

};

#endif
