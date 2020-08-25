#include <cassert>
#include <cstdio>
#include <cstring>
#include "Interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/* The command's strings */

typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADDPATIENT_CMD = 1,
    ADDTORISKGROUP_CMD = 2,
    RISKGROUPINTIMERANGE_CMD = 3,
    PRINTPATIENTS_CMD = 4,
    PRINTSUPERSPREADERS_CMD = 5,
    PRINTALLINFECTEDBY_CMD = 6,
    QUIT_CMD = 7
} commandType;


static const int numActions = 8;

static const char *commandStr[] = {
        "Init",
        "AddPatient",
        "AddToRiskGroup",
        "RiskGroupInRange",
        "PrintPatients",
        "PrintSuperSpreaders",
        "PrintAllInfectedBy",
        "Quit"};

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        case INVALID_REGISTRATION_TIME:
            return "INVALID_REGISTRATION_TIME";
        case (PATIENT_NOT_FOUND):
            return "PATIENT_NOT_FOUND";
        default:
            return "";
    }
}

#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum{
    error_free, error
}errorType;

static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/*
 * Main program
 */

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            printf("Please try again.\n");
    };
    return 0;
}

/*
 * Command checker
 */

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/*
 * Functions by command
 */

static errorType OnInit(void** PM, const char* const command);
static errorType OnAddPatient(void* PM, const char* const command);
static errorType OnAddToRiskGroup(void* PM, const char* const command);
static errorType OnRiskGroupInRange(void* PM, const char* const command);
static errorType OnPrintPatients(void* PM, const char* const command);
static errorType OnPrintSuperSpreaders(void* PM, const char* const command);
static errorType OnPrintAllInfectedBy(void* PM, const char* const command);
static errorType OnQuit(void** PM, const char* const command);

/*
 * Command parser
 */

static errorType parser(const char* const command) {
    static void *PM = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&PM, command_args);
            break;
        case (ADDPATIENT_CMD):
            rtn_val = OnAddPatient(PM, command_args);
            break;
        case (ADDTORISKGROUP_CMD):
            rtn_val = OnAddToRiskGroup(PM, command_args);
            break;
        case (RISKGROUPINTIMERANGE_CMD):
            rtn_val = OnRiskGroupInRange(PM, command_args);
            break;
        case (PRINTPATIENTS_CMD):
            rtn_val = OnPrintPatients(PM, command_args);
            break;
        case (PRINTSUPERSPREADERS_CMD):
            rtn_val = OnPrintSuperSpreaders(PM, command_args);
            break;
        case (PRINTALLINFECTEDBY_CMD):
            rtn_val = OnPrintAllInfectedBy(PM, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&PM, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

static errorType OnInit(void** PM, const char* const command) {
    if (isInit) {
        printf("init was already called.\n");
        return (error_free);
    };
    isInit = true;

    unsigned int zeroPatientAge;
    char tmp[MAX_BUFFER_SIZE];
    Time regTime;
    ValidateRead(sscanf(command, "%s %d %ld", tmp, &zeroPatientAge, &regTime), 3, "%s failed.\n", commandStr[INIT_CMD]);
    std::string zeroPatientName = tmp;

    *PM = Init(zeroPatientName,zeroPatientAge,regTime);

    if (*PM == NULL) {
        printf("init failed.\n");
        return error;
    };

    printf("init done.\n");
    return error_free;
}

static errorType OnAddPatient(void* PM, const char* const command){
    unsigned int age, sprederID;
    char tmp[MAX_BUFFER_SIZE];
    Time regTime;
    ValidateRead(sscanf(command, "%d %s %d %ld",   &sprederID, tmp, &age, &regTime), 4, "%s failed.\n", commandStr[ADDPATIENT_CMD]);
    std::string name = tmp;

    PMResult res = AddPatient(PM, sprederID, name, age, regTime);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDPATIENT_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDPATIENT_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnAddToRiskGroup(void* PM, const char* const command){
    unsigned int patientID;
    ValidateRead(sscanf(command, "%d",&patientID), 1, "%s failed.\n", commandStr[ADDTORISKGROUP_CMD]);

    PMResult res = AddToRiskGroup(PM, patientID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDTORISKGROUP_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDTORISKGROUP_CMD], ReturnValToStr(res));
    return error_free;
}


static errorType OnRiskGroupInRange(void* PM, const char* const command){
    Time start, end;
    ValidateRead(sscanf(command, "%ld %ld",&start, &end), 2, "%s failed.\n", commandStr[RISKGROUPINTIMERANGE_CMD]);

    PMResult res = RiskGroupInTimeRange(PM , start, end);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[RISKGROUPINTIMERANGE_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[RISKGROUPINTIMERANGE_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnPrintPatients(void* PM, const char* const command){
    ValidateRead(0, 0, "%s failed.\n", commandStr[PRINTPATIENTS_CMD]);

    PMResult res = PrintPatients(PM);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[PRINTPATIENTS_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[PRINTPATIENTS_CMD], ReturnValToStr(res));
    return error_free;

}

static errorType OnPrintSuperSpreaders(void* PM, const char* const command){
    int k;
    ValidateRead(sscanf(command, "%d",&k), 1, "%s failed.\n", commandStr[PRINTSUPERSPREADERS_CMD]);

    PMResult res = PrintSuperSpreaders(PM, k);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[PRINTSUPERSPREADERS_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[PRINTSUPERSPREADERS_CMD], ReturnValToStr(res));
    return error_free;

}

static errorType OnPrintAllInfectedBy(void* PM, const char* const command){
    int patientID;
    ValidateRead(sscanf(command, "%d",&patientID), 1, "%s failed.\n", commandStr[PRINTALLINFECTEDBY_CMD]);

    PMResult res = PrintAllInfectedBy(PM, patientID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[PRINTALLINFECTEDBY_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[PRINTALLINFECTEDBY_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnQuit(void** PM, const char* const command){
    Quit(PM);
    if (*PM != NULL) {
        printf("quit failed.\n");
        return error;
    };

    isInit = false;
    printf("quit done.\n");
    return error_free;
}

#ifdef __cplusplus
}
#endif
