# Patient-Manager

Leon Kosarev 

Version: 26.08.20 

Languages used: **C++**, **C**

## General Description: 

In light of recent events, I’ve decided to build a program that will help manage information about the spread of a virus. The program will make use of the C++ standard template library (STL), data structures and other tools, in order to implement the needed functionality  under optimal complexity. 

### Information Storage: 

The program will store patient information by assigning each patient with a unique ID number according to chronological order. In other words, the N-th patient will receive the ID number N. In addition, the program will keep track of the number of people infected by each patient (infection count). 

### Current Functionality: 

Init(string zeroPatientName, int zeroPatientAge, Time registrationTime) - Initializes a patient database containing information about the first known patient. - **O(1)** 

AddPatient(int spreaderID, string name, int age, Time registrationTime) - Adds a new patient to the system (infected by patient with ID number - spreaderID). -  **O(1)** (amortized) 

PrintSuperSpreaders(int K) - Prints the ID numbers of K patients with the highest infection count. - **O(K)** 

PrintInfectedBy(int patientID) - Prints the ID numbers of all K patients, directly of indirectly infected by patient with a given ID. - **O(K)** 

AddToRiskGroup(int patientID) - Indicate that an existing patient is in a risk group. - **O(logn)** 

RiskGroupInTimeRange(Time start, Time end) - Prints the number of risk group patients, registered within a certain time range. - **O(logn)** 

Quit - Frees the data and quits the application. - **O(1)** (amortized)

### Data Structures: 

Directed graph - Implemented using adjacency lists. An edge (x,y) will exist in the graph if and only if patient y was infected by patient x. 

Rank List – A list, where each node ("RankNode") represents a group of patients with the same infection count and contains a list ("PatientList") of ID numbers of those patients. 

Dynamic array – Contains the information of all patients in the system. 

SumTree – A balanced BST,  where each node contains an integer, indicating the quantity of the key representing the node.

## Implementation

### Patient Manager:

- Patients - Dynamic array < Patient >
- RankList - List < InfectionRank >
- SpreadGraph - List < List < int > >
- RiskGroupByTime - SumTree < Time > - BST sorted by Time

### Patient:

- ID < int >
- Name < String >
- Age < int >
- Rank < InfectionRank& > - Points to the "RankNode"
- SpotByRank - Node < int >& - Points to the relevant node in "PatientList"
- RegistrationTime < Time >

### InfectionRank:

- Rank < int >
- PatientList - List < int >

### SumTree < T >:

- Root < SumTreeNode < T > >
- Size < int >

SumTree is a balanced BST. It's insertion and deletion algorithms are similar to the AVL tree. A key may appear more than once. Each node represents a key and contains an integer value representing the quantity of the key in the structure. If the value reaches 0, the node is removed. In addition, each node contains the sum of all values in it's subtree. The additional information allows us to find the total quantity of keys in any range in **O(logn)**.

### SumTreeNode < T >:

- Height < int >
- Left, Right, Parent < SumTreeNode < T >& >
- Key < T& >
- Value < int >
- SubTreeSum < int >

## Supported Commands

The functions desribed above can be called using the following instruction formats:

- Init -Patient name- -Patient age- -Registration time-

- AddPatient -Spreader ID- -Patient Name- -Patient Age- -Registration Time-

- AddToRiskGroup -Patient ID-

- RiskGroupInRange -Start time- -End time-

- PrintSuperSpreaders -Number of patients to print-

- PrintAllInfectedBy -Patient ID-

- PrintPatients

- Quit
 
