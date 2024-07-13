#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(x) _mkdir(x)
#else
#include <unistd.h>
#define MKDIR(x) mkdir(x, 0777)
#endif

using namespace std;

namespace StudentManagement {
    
    void createFolderIfNotExist(const string& folderName) {
        struct stat info;
        if (stat(folderName.c_str(), &info) != 0) {
            cout << "Folder does not exist. Creating folder: " << folderName << endl;
            MKDIR(folderName.c_str());
        } else if (info.st_mode & S_IFDIR) {
            cout << "Folder already exists." << endl;
        } else {
            cerr << "Error: " << folderName << " exists but is not a folder!" << endl;
        }
    }

    void addStudentInfo(const string& folderName) {
        string srCode, name, pass, yearLevel, course, subjects, status;
        
        cout << "Enter SR Code: ";
        cin >> srCode;
        cin.ignore();
        cout<<"Enter password: ";
        getline(cin, pass);
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Year Level: ";
        getline(cin, yearLevel);
        cout << "Enter Course: ";
        getline(cin, course);
        status = "Not Enrolled";

        string filePath = folderName + "/" + srCode + ".txt";
        ofstream file(filePath);
        if (!file) {
            cerr << "Error opening file for writing!" << endl;
            return;
        }

        file << "SR Code: " << srCode << "\n"
             << "Name: " << name << "\n"
             << "Password: " << pass << "\n"
             << "Year Level: " << yearLevel << "Year\n"
             << "Course: " << course << "\n"
             << "Subjects Taken: " << subjects << "\n"
             << "Enrollment Status: " << status << "\n";

        file.close();
        cout << "Student information added successfully!" << endl;
    }

    void displayStudentInfo(const string& folderName, const string& srCode) {
        string filePath = folderName + "/" + srCode + ".txt";
        ifstream file(filePath);

        if (!file) {
            cerr << "Error opening file for reading or file does not exist!" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }


    bool editStudentYearLevel(const string& folderName, const string& srCode, const string& newYearLevel) {
        string filePath = folderName + "/" + srCode + ".txt";
        ifstream inFile(filePath);

        if (!inFile) {
            cerr << "Error opening file for reading or file does not exist!" << endl;
            return false;
        }

        // Create a temporary file
        string tempFilePath = folderName + "/temp.txt";
        ofstream tempFile(tempFilePath);
        if (!tempFile) {
            cerr << "Error creating temporary file!" << endl;
            inFile.close();
            return false;
        }

        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.find("Year Level: ") == 0) {
                tempFile << "Year Level: " << newYearLevel <<"Year"<< endl;
                found = true;
            } else {
                tempFile << line << endl;
            }
        }

        inFile.close();
        tempFile.close();

        // Delete original file and rename temporary file
        if (found) {
            if (remove(filePath.c_str()) != 0) {
                cerr << "Error deleting original file!" << endl;
                return false;
            }
            if (rename(tempFilePath.c_str(), filePath.c_str()) != 0) {
                cerr << "Error renaming temporary file!" << endl;
                return false;
            }
            cout << "Year Level updated successfully!" << endl;
        } else {
            cerr << "Year Level not found in the file!" << endl;
            remove(tempFilePath.c_str()); // Remove temporary file if no changes made
        }

        return found;
    }
}

int main() {
    string folderName = "StudentRecords";
    StudentManagement::createFolderIfNotExist(folderName);
    string srCode, yearlevel;

    cout << "Enter SR Code of the student to edit: ";
    cin >> srCode;
    cout<< "yearlevel: ";
    cin>> yearlevel;
    StudentManagement::editStudentYearLevel(folderName, srCode, yearlevel);
    
    return 0;
}
