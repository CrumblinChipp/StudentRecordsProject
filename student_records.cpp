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

namespace Admin_Menu_Function {

    void add_student(const string& folderName) {
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

    void search_display_student_info(const string& folderName, const string& srCode) {
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

    bool edit_student_info(const string& folderName, const string& srCode, const string& newSRCode, const string& newName, const string& newYearLevel, const string& newCourse) {
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
        bool foundSRCode = false, foundName = false, foundYearLevel = false, foundCourse = false;

        while (getline(inFile, line)) {
            if (line.find("SR Code: ") == 0) {
                tempFile << "SR Code: " << newSRCode << endl;
                foundSRCode = true;
            } else if (line.find("Name: ") == 0) {
                tempFile << "Name: " << newName << endl;
                foundName = true;
            } else if (line.find("Year Level: ") == 0) {
                tempFile << "Year Level: " << newYearLevel << " Year" << endl;
                foundYearLevel = true;
            } else if (line.find("Course: ") == 0) {
                tempFile << "Course: " << newCourse << endl;
                foundCourse = true;
            } else {
                tempFile << line << endl;
            }
        }

        inFile.close();
        tempFile.close();

        // Delete original file and rename temporary file
        if (foundSRCode || foundName || foundYearLevel || foundCourse) {
            if (remove(filePath.c_str()) != 0) {
                cerr << "Error deleting original file!" << endl;
                return false;
            }
            string newFilePath = folderName + "/" + newSRCode + ".txt";
            if (rename(tempFilePath.c_str(), newFilePath.c_str()) != 0) {
                cerr << "Error renaming temporary file!" << endl;
                return false;
            }
            cout << "Student information updated successfully!" << endl;
        } else {
            cerr << "One or more fields not found in the file!" << endl;
            remove(tempFilePath.c_str()); // Remove temporary file if no changes made
        }

        return foundSRCode || foundName || foundYearLevel || foundCourse;
    }
}

int main() {
    string folderName = "StudentRecords";
    string srCode, newSRCode, newName, newYearLevel, newCourse;
    
    int admin_menu_action;
    bool admin_menu_loop = false;
    do{
        cout<<"Admin Menu\n";
        cout<<"[1]Add Student\n";
        cout<<"[2]Update Student Info\n";
        cout<<"[3]View Student by Course\n";
        cout<<"[4]View Student by Year Level\n";
        cout<<"[5]Search Student Info\n";
        cout<<"[6]Exit\n";
        cout<<"\nEnter your action: ";
        cin >> admin_menu_action;
        
        switch(admin_menu_action){
            case 1:
            cout<<"=============================\n";
            cout<<"add student\n";
            cout<<"=============================\n";
            break;
            
            case 2:
            cout<<"=============================\n";
            cout<<"Update student information\n";
            cout<<"=============================\n";
            break;
            
            case 3:
            cout<<"=============================\n";
            cout<<"View Student by Course\n";
            cout<<"=============================\n";
            break;
            
            case 4:
            cout<<"=============================\n";
            cout<<"View Student by Year Level\n";
            cout<<"=============================\n";
            break;
            
            case 5:
            cout<<"=============================\n";
            cout<<"search student\n";
            cout<<"=============================\n";
            break;
            
            case 6:
            cout<<"=============================\n";
            cout<<"exit\n";
            cout<<"=============================\n";
            admin_menu_loop = false;
            break;
            
            default:
            cout<<"Wrong Input. Please Try Again.\n";
        }
        

    }while(admin_menu_loop == true);
    return 0;
}
