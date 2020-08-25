#include "Interface.h"
#include <iostream>
#include "PatientManager.h"

using namespace std;

void* Init(const std::string& zeroPatientName , unsigned int zeroPatientAge, Time regTime){
    auto* PM = new PatientManager(zeroPatientName, zeroPatientAge, regTime);
    return (void*)PM;
}

PMResult AddPatient(void* PM, unsigned int spreaderID,  std::string name , unsigned int age, Time regTime){
    return ((PatientManager*)PM)->addPatient(spreaderID, name, age, regTime);
}

PMResult AddToRiskGroup(void* PM, unsigned int id){
    return ((PatientManager*)PM)->addToRiskGroup(id);
}

PMResult RiskGroupInTimeRange(void* PM, Time start, Time end){
    return ((PatientManager*)PM)->riskGroupInTimeRange(start, end, cout);
}

PMResult PrintPatients(void* PM){
    ((PatientManager*)PM)->print(cout);
    return SUCCESS;
}

PMResult PrintSuperSpreaders(void* PM, unsigned int k){
    return ((PatientManager*)PM)->printSuperSpreaders(k, cout);
}

PMResult PrintAllInfectedBy(void* PM, unsigned int patientID){
    return ((PatientManager*)PM)->printAllInfected(patientID, cout);
}

void Quit(void** PM){
    delete (PatientManager*)(*PM);
    *PM = NULL;
}
