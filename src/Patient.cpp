#include "Patient.h"

Patient::Patient(unsigned int patientID, std::string name, unsigned int patientAge, Time registrationTime,
                 std::list<InfectionRank>::iterator patientRank, std::list<unsigned int>::iterator rankNode)
                 :id(patientID), name(name), age(patientAge), riskGroup(false), rank(patientRank), registrationTime(registrationTime), spotByRank(rankNode) {}


void Patient::setRank(std::list<InfectionRank>::iterator patientRank){
    auto newSpot = patientRank->addPatient(id);
    rank->removePatient(spotByRank);
    spotByRank = newSpot;
    rank = patientRank;
}

void Patient::print(std::ostream &os) const  {
    os << "ID: " << id << ", Name: " << name <<  ", Age: " << age << ", Infected: " << rank->getRank() <<
                 ", Registration time: " << registrationTime <<std::endl;
}

std::list<InfectionRank>::iterator Patient::getRank() {
    return rank;
}

void Patient::putInRiskGroup() {
    riskGroup = true;
}

Time Patient::getRegTime() {
    return registrationTime;
}


