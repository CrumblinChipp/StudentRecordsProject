#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(x) _mkdir(x)
#else
#include <unistd.h>
#define MKDIR(x) mkdir(x, 0777)
#endif

namespace fs = std::filesystem;
using namespace std;

void student_menu(const string& folderName, string& srCode) {
    string filePath = folderName + "/" + srCode + ".txt";
    ifstream inFile(filePath);

    if (!inFile) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    string name, yearLevel, course, status;

    while (getline(inFile, line)) {
        if (line.find("SR Code: ") == 0) {
            srCode = line.substr(9);
        } else if (line.find("Name: ") == 0) {
            name = line.substr(6);
        } else if (line.find("Year Level: ") == 0) {
            yearLevel = line.substr(12);
        } else if (line.find("Course: ") == 0) {
            course = line.substr(8);
        } else if (line.find("Enrollment Status: ") == 0) {
            status = line.substr(19);
        }
    }

    inFile.close();

    cout << "==========================" << endl;
    cout << "Student Details:" << endl;
    cout << "SR Code: " << srCode << endl;
    cout << "Name   : " << name << endl;
    cout << "Year   : " << yearLevel << endl;
    cout << "Course : " << course << endl;
    cout << "Status : " << status << endl;
    cout << "==========================" << endl;

    int student_menu_action;
    bool student_menu_loop = false;
    do {
        cout << "Student Menu" << endl;
        cout << "[1] View Curriculum" << endl;
        cout << "[2] View Subjects Taken" << endl;
        cout << "[3] Enrollment" << endl;
        cout << "[4] Exit" << endl;
        cout << "Enter your action: ";
        cin >> student_menu_action;

        switch (student_menu_action) {
            case 1:
                cout << "=============================" << endl;
                cout << "View Curriculum" << endl;
                cout << "=============================" << endl;
                break;

            case 2:
                cout << "=============================" << endl;
                if (status == "Not Enrolled") {
                    cout << "No subjects taken yet." << endl;
                } else {
                    cout << "Subjects Taken:" << endl;
                    // Implement listing subjects taken logic here
                }
                cout << "=============================" << endl;
                break;

            case 3:
                cout << "=============================" << endl;
                if (status == "Enrolled") {
                    cout << "Already enrolled." << endl;
                } else {
                    cout << "Subjects to be taken will be listed below." << endl;
                    cout << "Confirmation question every time they add a subject." << endl;
                    cout << "Option to remove a subject will be provided." << endl;
                    status = "Enrolled";
                }
                cout << "=============================" << endl;
                break;

            case 4:
                cout << "=============================" << endl;
                cout << "Exit" << endl;
                cout << "=============================" << endl;
                student_menu_loop = true;
                break;

            default:
                cout << "Wrong Input. Please Try Again." << endl;
                break;
        }

    } while (!student_menu_loop);
}

int main() {
    string foldername = "StudentRecords";
    string srcode = "21-08330";
    student_menu(foldername, srcode);
    return 0;
}
