#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//CONSTANTS
const int MAX_DOCTORS = 50;
const int MAX_NURSES = 50;
const int MAX_QUEUE = 100;
const int MAX_STACK = 100;

//STRUCTURES
struct Doctor
{
    int id;
    string name;
};

struct Nurse
{
    int id;
    string name;
};

struct Patient
{
    int id;
    string name;
    bool critical;
    Patient* next;
};

struct WaitingPatient
{
    int id;
    string name;
    bool critical;
};

struct ServedPatient
{
    int id;
    string name;
};

//GLOBALS
Doctor doctors[MAX_DOCTORS];
Nurse nurses[MAX_NURSES];

int doctorCount = 0;
int nurseCount = 0;

Patient* head = NULL;

// Queue
WaitingPatient queueArr[MAX_QUEUE];
int queueSize = 0;

// Stack
ServedPatient stackArr[MAX_STACK];
int top = -1;

// Roster
string shifts[3] = {"Morning", "Evening", "Night"};
int currentDoctor = -1;
int currentNurse = -1;
int currentShift = 0;

//ADD DOCTOR
void addDoctor()
{
    if (doctorCount >= MAX_DOCTORS)
    {
        cout << "Doctor limit reached!\n";
        return;
    }

    cout << "Enter Doctor ID: ";
    cin >> doctors[doctorCount].id;

    cout << "Enter Name: ";
    cin >> doctors[doctorCount].name;

    doctorCount++;
}

//ADD NURSE
void addNurse()
{
    if (nurseCount >= MAX_NURSES)
    {
        cout << "Nurse limit reached!\n";
        return;
    }

    cout << "Enter Nurse ID: ";
    cin >> nurses[nurseCount].id;

    cout << "Enter Name: ";
    cin >> nurses[nurseCount].name;

    nurseCount++;
}

//ADD PATIENT (LINKED LIST)
void addPatient()
{
    Patient* p = new Patient();

    cout << "Enter Patient ID: ";
    cin >> p->id;

    cout << "Enter Name: ";
    cin >> p->name;

    cout << "Critical? (1/0): ";
    cin >> p->critical;

    p->next = head;
    head = p;

    cout << "Patient added successfully!\n";
}

//PRIORITY QUEUE
void enqueuePatient()
{
    if (queueSize >= MAX_QUEUE)
    {
        cout << "Queue is full!\n";
        return;
    }

    WaitingPatient p;

    cout << "Enter ID: ";
    cin >> p.id;

    cout << "Enter Name: ";
    cin >> p.name;

    cout << "Critical? (1/0): ";
    cin >> p.critical;

    int i = queueSize - 1;

    while (i >= 0 && queueArr[i].critical < p.critical)
    {
        queueArr[i + 1] = queueArr[i];
        i--;
    }

    queueArr[i + 1] = p;
    queueSize++;

    cout << "Added to queue!\n";
}

//ROSTER
void nextRoster()
{
    if (doctorCount == 0 || nurseCount == 0)
    {
        cout << "Add doctors & nurses first!\n";
        return;
    }

    currentDoctor = (currentDoctor + 1) % doctorCount;
    currentNurse = (currentNurse + 1) % nurseCount;

    cout << "\nShift: " << shifts[currentShift] << endl;
    cout << "Doctor: " << doctors[currentDoctor].name << endl;
    cout << "Nurse: " << nurses[currentNurse].name << endl;

    currentShift = (currentShift + 1) % 3;
}

//SERVE PATIENT
void servePatient()
{
    if (queueSize == 0)
    {
        cout << "No patients in queue!\n";
        return;
    }

    if (currentDoctor == -1)
    {
        cout << "Assign roster first!\n";
        return;
    }

    if (top >= MAX_STACK - 1)
    {
        cout << "Stack overflow!\n";
        return;
    }

    WaitingPatient p = queueArr[0];

    for (int i = 0; i < queueSize - 1; i++)
        queueArr[i] = queueArr[i + 1];

    queueSize--;

    top++;
    stackArr[top].id = p.id;
    stackArr[top].name = p.name;

    cout << "\nPatient Served: " << p.name << endl;
    cout << "Doctor: " << doctors[currentDoctor].name << endl;
}

//DISPLAY
void display()
{
    cout << "DOCTORS/n";
    for (int i = 0; i < doctorCount; i++)
        cout << doctors[i].id << " " << doctors[i].name << endl;

    cout << "NURSES\n";
    for (int i = 0; i < nurseCount; i++)
        cout << nurses[i].id << " " << nurses[i].name << endl;

    cout << "\nPATIENTS (Linked List)\n";
    Patient* temp = head;
    while (temp)
    {
        cout << temp->id << " " << temp->name << " "
             << (temp->critical ? "Critical" : "Normal") << endl;
        temp = temp->next;
    }

    cout << "QUEUE\n";
    for (int i = 0; i < queueSize; i++)
        cout << queueArr[i].name << endl;

    cout << "SERVED (STACK)\n";
    for (int i = top; i >= 0; i--)
        cout << stackArr[i].name << endl;
}

//SAVE DATA
void saveData()
{
    ofstream out("hospital.txt");

    out << doctorCount << endl;
    for (int i = 0; i < doctorCount; i++)
        out << doctors[i].id << " " << doctors[i].name << endl;

    out << nurseCount << endl;
    for (int i = 0; i < nurseCount; i++)
        out << nurses[i].id << " " << nurses[i].name << endl;

    Patient* temp = head;
    while (temp)
    {
        out << temp->id << " " << temp->name << " " << temp->critical << endl;
        temp = temp->next;
    }

    out.close();
    cout << "Data Saved!\n";
}

//MEMORY CLEANUP
void freeMemory()
{
    Patient* temp;
    while (head)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

//MENU
void menu()
{
    cout << "\n===== MENU =====\n";
    cout << "1. Add Doctor\n";
    cout << "2. Add Nurse\n";
    cout << "3. Add Patient\n";
    cout << "4. Add to Queue\n";
    cout << "5. Next Shift\n";
    cout << "6. Serve Patient\n";
    cout << "7. Display\n";
    cout << "8. Save\n";
    cout << "9. Exit\n";
}

//MAIN
int main()
{
    int ch;

    do
    {
        menu();
        cin >> ch;

        switch (ch)
        {
        case 1: addDoctor(); break;
        case 2: addNurse(); break;
        case 3: addPatient(); break;
        case 4: enqueuePatient(); break;
        case 5: nextRoster(); break;
        case 6: servePatient(); break;
        case 7: display(); break;
        case 8: saveData(); break;
        }

    } while (ch != 9);

    freeMemory(); // cleanup linked list

    return 0;
}