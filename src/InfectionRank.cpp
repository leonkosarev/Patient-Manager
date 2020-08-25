#include "InfectionRank.h"

InfectionRank::InfectionRank(unsigned int infectionRank)
        : rank(infectionRank), patientList() {}

std::list<unsigned int>::iterator InfectionRank::addPatient(unsigned int patientID) {
    patientList.push_front(patientID);
    return patientList.begin();
}

unsigned int InfectionRank::getRank() const {
    return rank;
}

bool InfectionRank::isEmpty() const  {
    return patientList.empty();
}

void InfectionRank::removePatient(const std::list<unsigned int>::iterator patient) {
    patientList.erase(patient);
}

void InfectionRank::getTopK(int k, int *count, unsigned int* idArray) {
    for(unsigned int & v : patientList) {
        if (*count>=k){
            break;
        }
        idArray[*count] = v;
        ++(*count);
    }
}
