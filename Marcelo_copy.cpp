#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
namespace fs = std::filesystem;

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
             << "Year Level: " << yearLevel << " Year\n"
             << "Course: " << course << "\n"
             << "Subjects Taken: " << subjects << "\n"
             << "Enrollment Status: " << status << "\n";

        file.close();
        cout << "Student information added successfully!" << endl;
    }

    void search_student_info(const string& folderName, const string& srCode) {
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

    bool edit_student_info(const string& folderName) {
        string srCode, new_srcode, new_name, new_yearlevel, new_course;
        cout<<"Enter student Sr-code:";
        getline(cin, srCode);
        cout<<"Enter new Sr-code:";
        getline(cin, new_srcode);
        cout<<"Enter new Name: ";
        getline(cin, new_name);
        cout<<"Enter new Year Level: ";
        getline(cin, new_yearlevel);
        cout<<"Enter new Course: " ;
        getline(cin, new_course);
        
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
                tempFile << "SR Code: " << new_srcode << endl;
                foundSRCode = true;
            } else if (line.find("Name: ") == 0) {
                tempFile << "Name: " << new_name << endl;
                foundName = true;
            } else if (line.find("Year Level: ") == 0) {
                tempFile << "Year Level: " << new_yearlevel << " Year" << endl;
                foundYearLevel = true;
            } else if (line.find("Course: ") == 0) {
                tempFile << "Course: " << new_course << endl;
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
            string newFilePath = folderName + "/" + new_srcode + ".txt";
            if (rename(tempFilePath.c_str(), newFilePath.c_str()) != 0) {
                cerr << "Error renaming temporary file!" << endl;
                return false;
            }
            cout << "Student information updated successfully!" << endl;
        } else {
            cerr << "One or more fields not found in the file!" << endl;
            remove(tempFilePath.c_str()); 
        }
        return foundSRCode || foundName || foundYearLevel || foundCourse;
    }

    void view_students_by_course(const string& folderName) {
        string course;
        cout<< "Enter the course to search for: ";
        getline(cin, course);
        cin.ignore();

        for (const auto& entry : fs::directory_iterator(folderName)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filePath = entry.path().string();
                ifstream inFile(filePath);

                if (!inFile) {
                    cerr << "Error opening file: " << filePath << endl;
                    continue;
                }

                string line;
                bool courseMatch = false;
                string srCode;

                while (getline(inFile, line)) {
                    if (line.find("SR Code: ") == 0) {
                        srCode = line.substr(9); 
                    }
                    if (line.find("Course: ") == 0 && line.substr(8) == course) {
                        courseMatch = true;
                    }
                }

                inFile.close();

                if (courseMatch) {
                    cout << "SR Code: " << srCode << endl;
                }
            }
        }
    }

    void view_students_by_yearlevel(const string& folderName) {
        string yearlevel;
        cout<< "Enter the course to search for: ";
        cin>> yearlevel;
        cin.ignore();

        for (const auto& entry : fs::directory_iterator(folderName)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                string filePath = entry.path().string();
                ifstream inFile(filePath);

                if (!inFile) {
                    cerr << "Error opening file: " << filePath << endl;
                    continue;
                }

                string line;
                bool yearLevelMatch = false;
                string srCode;

                while (getline(inFile, line)) {
                    if (line.find("SR Code: ") == 0) {
                        srCode = line.substr(9);
                    }
                    if (line.find("Year Level: ") == 0 && line.substr(12, yearlevel.size()) == yearlevel) {
                        yearLevelMatch = true;
                    }
                }
                inFile.close();

                if (yearLevelMatch) {
                    cout << "SR Code: " << srCode << endl;
                }
            }
        }
    }
}

int login(const string& folderName, const string& username, const string& password) {
    // Admin credentials
    const string adminUsername = "00-00001";
    const string adminPassword = "admin_pass";

    if (username == adminUsername && password == adminPassword) {
        cout << "Admin login successful!" << endl;
        return 1; // Admin login successful
    }

    // Student login
    string filePath = folderName + "/" + username + ".txt";
    ifstream file(filePath);

    string line;
    string storedPassword;
    while (getline(file, line)) {
        if (line.find("Password: ") == 0) {
            storedPassword = line.substr(10);
            break;
        }
    }

    file.close();

    if (password == storedPassword) {
        cout << "Student login successful!" << endl;
        return 2; // Student login successful
    } else {
        cerr << "Error: Incorrect password!" << endl;
        return 3; // Incorrect password
    }
}

namespace Menu_Function {
    void Admin_Menu(const string& foldername){
        int admin_menu_action;
        string srcode, course, yearlevel, new_srcode, new_name, new_yearlevel, new_course;
        bool admin_menu_loop = false;
        
        do{
            cout<<"Admin Menu"<<endl;
            cout<<"[1]Add Student"<<endl;
            cout<<"[2]Update Student Info"<<endl;
            cout<<"[3]View Student by Course"<<endl;
            cout<<"[4]View Student by Year Level"<<endl;
            cout<<"[5]Search Student Info"<<endl;
            cout<<"[6]Exit"<<endl;
            cout<<"\nEnter your action: ";
            cin >> admin_menu_action;
            cin.ignore();
            
            switch(admin_menu_action){
                case 1:
                cout<<"============================="<<endl;
                Admin_Menu_Function::add_student(foldername);
                cout<<"============================="<<endl;
                break;
                
                case 2:
                cout<<"============================="<<endl;
                Admin_Menu_Function::edit_student_info(foldername);
                cout<<"============================="<<endl;
                break;
                
                case 3:
                cout<<"============================="<<endl;
                Admin_Menu_Function::view_students_by_course(foldername);
                cout<<"============================="<<endl;
                break;
                
                case 4:
                cout<<"=============================\n";
                Admin_Menu_Function::view_students_by_yearlevel(foldername);
                cout<<"=============================\n";
                break;
                
                case 5:
                cout<<"=============================\n";
                cout<< "Enter Student Sr-Code: ";
                cin >> srcode;
                Admin_Menu_Function::search_student_info(foldername, srcode);
                cout<<"=============================\n";
                break;
                
                case 6:
                cout<<"=============================\n";
                cout<<"exit\n";
                cout<<"=============================\n";
                admin_menu_loop = true;
                break;
                
                default:
                cout<<"Wrong Input. Please Try Again.\n";
                break;
            }
        }while(admin_menu_loop == false);
        }

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
        cout<<"=========================="<<endl;
        cout << "Student Details:\n";
        cout << "SR Code: " << srCode << "\n";
        cout << "Name   : " << name << "\n";
        cout << "Year   : " << yearLevel << "\n";
        cout << "Course : " << course << "\n";
        cout << "Status : " << status << "\n";
        cout<<"=========================="<<endl;

        int student_menu_action;
        bool student_menu_loop = false;
        do{
            cout<<"Student Menu\n";
            cout<<"[1]View Curriculum\n";
            cout<<"[2]View Subject taken\n";
            cout<<"[3]Enrollment\n";
            cout<<"[4]Exit\n";
            cout<<"\nEnter your action: ";
            cin >> student_menu_action;
            
            switch(student_menu_action){
                case 1:
                cout<<"=============================\n";
                cout<<"View Curriculum\n";
                cout<<"Would need a new file handling to only show a certain curriculum for a students year level\n";
                cout<<"=============================\n";
                break;
                
                case 2:
                cout<<"=============================\n";
                if (status == "Not Enrolled"){
                    cout<<"This statement is given if the student havent been enrolled\n";
                }else{
                    cout<<"Subject taken:\n";
                    cout<<"Subject will be listed down below\n";
                }
                cout<<"=============================\n";
                break;
                
                case 3:
                cout<<"=============================\n";
                if(status == "Enrolled"){
                    cout<<"You are already enrolled\n";
                }else{
                    cout<<"Subject to be taken will be listed below\n";
                    cout<<"Secondary menu will be given. COnfirmation question everytime they add a subject.\n";
                    cout<<"Secondary menu will be given. A remove a subject option will be given\n";
                    status = "Enrolled";
                }
                cout<<"=============================\n";
                break;
                
                case 4:
                cout<<"=============================\n";
                cout<<"Exit\n";
                cout<<"=============================\n";
                student_menu_loop = true;
                break;
                
                default:
                cout<<"Wrong Input. Please Try Again.\n";
                break;
            }

        }while(student_menu_loop == false);
    }
}

int main() {
    string folderName = "StudentRecords";
    string username, password;
    cout << "Enter SR-Code: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    int login_result = login(folderName, username, password);
    if (login_result == 3) {
        cout << "Login failed. Exiting program." << endl;
        return 0;
    }

    else if (login_result == 2){
        Menu_Function::student_menu(folderName, username);
    }

    else if (login_result== 1){
        Menu_Function::Admin_Menu(folderName);
    }

    return 0;
}
