#ifndef PATIENTMANAGER_PATIENTMANAGER_H
#define PATIENTMANAGER_PATIENTMANAGER_H

#include <vector>
#include <list>
#include "Patient.h"
#include "InfectionRank.h"
#include "Interface.h"
#include "SumTree.h"
#include <iostream>

// Class for the main structure, with functionality as described in README file.

class PatientManager {
    std::vector<Patient> patients;
    std::list<InfectionRank> rankList;
    std::vector<std::list<unsigned int> > spreadGraph;
    SumTree<Time > riskGroupByTime;

    void movePatient(unsigned int patientID);

    void addNewPatient(const std::string& name, unsigned int age, Time registrationTime);

public:

    explicit PatientManager(const std::string& name, unsigned int zeroPatientAge, Time registrationTime);

    PMResult addPatient(unsigned int spreaderID, const std::string& name,  unsigned int age, Time registrationTime);

    void print(std::ostream& os) const;

    PMResult printSuperSpreaders(unsigned int k, std::ostream& os);

    PMResult printAllInfected(unsigned int patientID, std::ostream& os);

    PMResult addToRiskGroup(unsigned int patientID);

    PMResult riskGroupInTimeRange(Time start , Time end, std::ostream& os);

};

inline std::ostream& operator<<(std::ostream& os, PatientManager& pm) {
    pm.print(os);
    return os;
}


#endif
