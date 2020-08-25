#ifndef PATIENTMANAGER_INTERFACE_H
#define PATIENTMANAGER_INTERFACE_H

#ifndef __cplusplus
extern "C" {
#endif

#include "string"

typedef enum{
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3,
    INVALID_REGISTRATION_TIME = -4,
    PATIENT_NOT_FOUND = -5
} PMResult;

typedef long Time; // Use simple representation until a more sophisticated time class is implemented.
    
// Wrapper functions for user interface:    

void *Init(const std::string& zeroPatientName , unsigned int zeroPatientAge, Time regTime);

PMResult AddPatient(void* PM, unsigned int spreaderID,  std::string name , unsigned int age, Time regTime);

PMResult AddToRiskGroup(void* PM, unsigned int id);

PMResult RiskGroupInTimeRange(void* PM, Time start, Time end);

PMResult PrintPatients(void* PM);

PMResult PrintSuperSpreaders(void* PM, unsigned int k);

PMResult PrintAllInfectedBy(void* PM, unsigned int patientID);

void Quit(void** DS);

#endif

