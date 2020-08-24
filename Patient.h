#ifndef PATIENTMANAGER_PATIENT_H
#define PATIENTMANAGER_PATIENT_H

#include <list>
#include "InfectionRank.h"
#include <iostream>
#include "Interface.h"

class Patient {
    unsigned int id;
    std::string name;
    unsigned int age;
    bool riskGroup;
    std::list<InfectionRank>::iterator rank;
    Time registrationTime;
    std::list<unsigned int>::iterator spotByRank;

public:
    Patient(unsigned int patientID, std::string name, unsigned  int patientAge, Time registrationTime,
            std::list<InfectionRank>::iterator patientRank, std::list<unsigned int>::iterator rankNode);


    void setRank(std::list<InfectionRank>::iterator patientRank);

    void print(std::ostream& os) const;

    std::list<InfectionRank>::iterator getRank();

    void putInRiskGroup();

    Time getRegTime();

};

inline std::ostream& operator<<(std::ostream& os, Patient& patient) {
    patient.print(os);
    return os;
}

#endif
