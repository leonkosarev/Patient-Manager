#include "PatientManager.h"

using namespace std;

// Helper function. Adds a new patient to the system. 
void PatientManager::addNewPatient(const string& name, unsigned int age, Time registrationTime) {
    int newID = patients.size();
    if (rankList.begin()->getRank()!=0){        
        rankList.push_front(InfectionRank(0)); // Creates zero InfectionRank if necessary.
    }
    auto zeroRank = rankList.begin();
    auto newRankNode = zeroRank->addPatient(newID);
    patients.emplace_back(newID, name,  age, registrationTime,  zeroRank, newRankNode);

}

// Helper function. Moves an existing patient up a rank.
void PatientManager::movePatient(unsigned int patientID){
    Patient& currentPatient = patients[patientID];
    auto currentRank = currentPatient.getRank();
    auto nextRank = next(currentRank);
    if (nextRank==rankList.end() ||
        (nextRank!=rankList.end() && nextRank->getRank()!=currentRank->getRank()+1)){
            nextRank = rankList.insert(nextRank, InfectionRank(currentRank->getRank()+1));
    }
    currentPatient.setRank(nextRank);
    if (currentRank->isEmpty()){
        rankList.erase(currentRank);
    }
}

// Helper function for printing a connected component in a directed graph.
void recursivePrintList(vector<list<unsigned int>>& spreadGraph, vector<Patient>& patients, unsigned int patientID, ostream& os){
    for(unsigned int & v : spreadGraph[patientID]){
        os << patients[v];
        recursivePrintList(spreadGraph, patients, v, os);
    }
}

PatientManager::PatientManager(const string& zeroPatientName, unsigned int zeroPatientAge, Time registrationTime)
: patients(), rankList(), spreadGraph(), riskGroupByTime(){
    rankList.push_front(InfectionRank(0));
    addNewPatient(zeroPatientName, zeroPatientAge, registrationTime);
    spreadGraph.emplace_back();
}

void PatientManager::print(std::ostream &os) const {
    for (auto const &v : patients) {
        v.print(os);
    }
}

PMResult PatientManager::printSuperSpreaders(unsigned int k, ostream& os) {
    if(k<=0 || patients.size()<k){
        return INVALID_INPUT;
    }
    int count = 0;
    unsigned int* idArray;
    try{
        idArray = new unsigned int[k]; // Store the ID numbers in an array in order to print the full info of the patients later.
    }catch (bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
    os << "The top " << k << " spreaders are:" << endl;
    for (auto v = rankList.rbegin(); v!= rankList.rend(); ++v) {
        v->getTopK(k, &count, idArray);
        if (count>= (int)k){
            break;
        }
    }
    for(int i=0 ; i < (int)k; ++i){
        os << patients[idArray[i]];
    }
    delete idArray;
    return SUCCESS;
}

PMResult PatientManager::addPatient(unsigned int spreaderID, const string& name,  unsigned int age, Time registrationTime) {
    if (registrationTime < patients[patients.size()-1].getRegTime()){
        return INVALID_REGISTRATION_TIME;
    }
    if(spreaderID>=patients.size()){
        return PATIENT_NOT_FOUND;
    }
    try{
        movePatient(spreaderID);
        addNewPatient(name, age, registrationTime);
        spreadGraph.emplace_back();
        spreadGraph[spreaderID].push_back(patients.size()-1);
    }catch (bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

PMResult PatientManager::printAllInfected(unsigned int patientID, ostream& os) {
    if(patients.size()<=patientID){
        return PATIENT_NOT_FOUND;
    }
    os << "The people who were infected by patient " << patientID << " are:" << endl;
    recursivePrintList(spreadGraph, patients,  patientID, os);
    return SUCCESS;
}

PMResult PatientManager::addToRiskGroup(unsigned int patientID) {
    if (patientID>= patients.size()){
        return PATIENT_NOT_FOUND;
    }
    Time regTime = patients[patientID].getRegTime();
    try{
        riskGroupByTime.insert(regTime);
    }catch (bad_alloc& ba){
        return ALLOCATION_ERROR;
    }
    patients[patientID].putInRiskGroup();
    return SUCCESS;
}

PMResult PatientManager::riskGroupInTimeRange(Time start, Time end , ostream& os) {
    os << "The number of risk group patients registered from: " << start << " to " << end << " is "
                    << riskGroupByTime.countInRange(start, end) << endl;
    return SUCCESS;
}

