#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <unordered_map>
#include <cstdlib>
#include <conio.h>
#include <algorithm>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(x) _mkdir(x)
#else
#include <unistd.h>
#define MKDIR(x) mkdir(x, 0777)
#endif

namespace fs = std::filesystem;
using namespace std;

struct Course {
    string code;
    string name;
};

void clear_screen(){
    cout<<"\n====================================="<<endl;
    cout << "Press any key to continue..\n\n\n";
    _getch();
    system("cls");
}
namespace Admin_Menu_Function {
    void add_student(const string& folderName) {
        string srCode, name, pass, yearLevel, course, status;
        
        cout << "Enter SR Code(or type 'exit' to return): ";
        cin >> srCode;
        cin.ignore();
        if (srCode == "exit"){
            system("cls");
            return;
        }
        cout << "Enter password: ";
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
             << "Subjects Taken: \n"
             << "Enrollment Status: " << status << "\n";

        file.close();
        cout << "Student information added successfully!" << endl;
        clear_screen();
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
        clear_screen();
    }

    void edit_student_info(const string& folderName) {
        string srCode, new_srcode, new_name, new_yearlevel, new_course;
        cout << "Enter student SR Code(or type 'exit' to return): ";
        getline(cin, srCode);
        if (srCode == "exit"){
            system("cls");            
            return;

        }
        cout << "Enter new SR Code: ";
        getline(cin, new_srcode);
        cout << "Enter new Name: ";
        getline(cin, new_name);
        cout << "Enter new Year Level: ";
        getline(cin, new_yearlevel);
        cout << "Enter new Course: ";
        getline(cin, new_course);
        
        string filePath = folderName + "/" + srCode + ".txt";
        ifstream inFile(filePath);

        if (!inFile) {
            cerr << "Error opening file for reading or file does not exist!" << endl;
            return;
        }

        string tempFilePath = folderName + "/temp.txt";
        ofstream tempFile(tempFilePath);
        if (!tempFile) {
            cerr << "Error creating temporary file!" << endl;
            inFile.close();
            return;
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

        if (foundSRCode || foundName || foundYearLevel || foundCourse) {
            if (remove(filePath.c_str()) != 0) {
                cerr << "Error deleting original file!" << endl;
                return;
            }
            string newFilePath = folderName + "/" + new_srcode + ".txt";
            if (rename(tempFilePath.c_str(), newFilePath.c_str()) != 0) {
                cerr << "Error renaming temporary file!" << endl;
                return;
            }
            cout << "Student information updated successfully!" << endl;
        } else {
            cerr << "One or more fields not found in the file!" << endl;
            remove(tempFilePath.c_str()); 
        }
        clear_screen();
        return;
    }

    void view_students_by_course(const string& folderName) {
        string course;
        cout << "Enter the course to search for: ";
        getline(cin, course);

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

        clear_screen();
    }

    void view_students_by_yearlevel(const string& folderName) {
        string yearlevel;
        cout << "Enter the year level to search for: ";
        getline(cin, yearlevel);

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

        clear_screen();
    }

}

namespace Student_Menu_Function { 
    string curicullum_file(const string& course, const string& yearlevel) {
        string course_code, year_code;
        if (course == "Computer Science"){
            course_code = "BSCS";
        }
        else if (course == "Information Technology"){
            course_code = "BSIT";
        }

        if (yearlevel == "1 Year"){
            year_code ="1st ";
        }
        else if (yearlevel == "2 Year"){
            year_code ="2nd ";
        }
        else if (yearlevel == "3 Year"){
            year_code ="3rd ";
        }
        else if (yearlevel == "4 Year"){
            year_code ="4th ";
        }

        string course_year = year_code + course_code;
        return course_year;
    }

    void view_curicculum(const string& folderName, const string& course_year){
        string filePath = folderName + "/" + course_year + ".txt";
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

    void Enrollment(const string& filename, const vector<string>& newSubjects) {
        
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error opening file for reading." << endl;
            return;
        }

        string content;
        string line;
        while (getline(inputFile, line)) {
            content += line + "\n";
        }
        inputFile.close();

        
        string subjectsStr;
        for (const string& subject : newSubjects) {
            subjectsStr += subject + " ";
        }
        if (!subjectsStr.empty()) {
            subjectsStr.pop_back();
        }

        size_t pos = content.find("Subjects Taken:");
        if (pos != string::npos) {
            size_t endPos = content.find('\n', pos);
            if (endPos == string::npos) {
                endPos = content.length();
            }
            content.replace(pos + 15, endPos - pos - 15, " " + subjectsStr); 
        } else {
            cerr << "Line 'Subjects Taken:' not found in file." << endl;
        }

        pos = content.find("Enrollment Status:");
        if (pos != string::npos) {
            size_t endPos = content.find('\n', pos);
            if (endPos == string::npos) {
                endPos = content.length();
            }
            content.replace(pos + 18, endPos - pos - 18, " Enrolled"); 
        } else {
            cerr << "Line 'Enrollment Status:' not found in file." << endl;
        }

        ofstream outputFile(filename);
        if (!outputFile) {
            cerr << "Error opening file for writing." << endl;
            return;
        }

        outputFile << content;
        outputFile.close();
        cout<<"============================\n"<<endl;
        cout<<"Enrollment Successful\n"<<endl;
        cout<<"============================\n"<<endl;
    }

    vector<Course> getCourses(const string& filename, const string& course_path) {
        vector<Course> courses;
        string fielPath = filename + "/" + course_path + ".txt";
        ifstream file(fielPath);
        if (!file.is_open()) {
            cerr << "Could not open the file: " << filename << endl;
            return courses;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line.find("Year Level") != string::npos || line.find("Course") != string::npos) {
                continue;
            }

            size_t pos = line.find(':');
            if (pos != string::npos) {
                Course course;
                course.code = line.substr(0, pos);
                course.name = line.substr(pos + 1);

                course.code.erase(course.code.begin(), find_if(course.code.begin(), course.code.end(), [](unsigned char ch) { return !isspace(ch); }));
                course.code.erase(find_if(course.code.rbegin(), course.code.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), course.code.end());
                course.name.erase(course.name.begin(), find_if(course.name.begin(), course.name.end(), [](unsigned char ch) { return !isspace(ch); }));
                course.name.erase(find_if(course.name.rbegin(), course.name.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), course.name.end());

                courses.push_back(course);
            }
        }

        file.close();
        return courses;
    }

    vector<string> getStudentSubjects(const string& filename) {
        vector<string> subjects;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open the file: " << filename << endl;
            return subjects;
        }

        string line;
        while (getline(file, line)) {
            if (line.find("Subjects Taken:") != string::npos) {
                stringstream ss(line.substr(line.find(':') + 1));
                string subject;
                while (ss >> subject) {
                    subject.erase(subject.begin(), find_if(subject.begin(), subject.end(), [](unsigned char ch) { return !isspace(ch); }));
                    subject.erase(find_if(subject.rbegin(), subject.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), subject.end());
                    subjects.push_back(subject);
                }
                break;
            }
        }

        file.close();
        return subjects;
    }

    void matchSubjects(const vector<string>& studentSubjects, const vector<Course>& courses) {
        unordered_map<string, string> courseMap;
        for (const auto& course : courses) {
            courseMap[course.code] = course.name;
        }

        for (const auto& subject : studentSubjects) {
            if (courseMap.find(subject) != courseMap.end()) {
                cout << subject << ": " << courseMap[subject] << endl;
            } else {
                cout << subject << ": Not found in course list" << endl;
            }
        }
    }
}

int login(const string& folderName, const string& username, const string& password) {
    const string adminUsername = "00-00001";
    const string adminPassword = "admin_pass";

    if (username == adminUsername && password == adminPassword) {
        cout<<"\n====================================="<<endl;        
        cout << "Admin login successful!" << endl;
        clear_screen();
        return 1;
    }

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
        cout<<"\n====================================="<<endl;        
        cout << "Student login successful!" << endl;
        clear_screen();
        return 2;
    } else {
        cerr << "Error: Incorrect password!" << endl;
        return 3;
    }
}

namespace Menu_Function {
    void Admin_Menu(const string& foldername) {
        int admin_menu_action;
        string srcode;
        bool admin_menu_loop = false;

        do {
            cout << "Admin Menu" << endl;
            cout << "[1] Add Student" << endl;
            cout << "[2] Update Student Info" << endl;
            cout << "[3] View Student by Course" << endl;
            cout << "[4] View Student by Year Level" << endl;
            cout << "[5] Search Student Info" << endl;
            cout << "[6] Exit" << endl;
            cout << "Enter your action: ";
            cin >> admin_menu_action;
            cin.ignore();

            switch (admin_menu_action) {
                case 1:
                    cout << "=============================" << endl;
                    Admin_Menu_Function::add_student(foldername);
                    cout << "=============================" << endl;
                    break;

                case 2:
                    cout << "=============================" << endl;
                    Admin_Menu_Function::edit_student_info(foldername);
                    cout << "=============================" << endl;
                    break;

                case 3:
                    cout << "=============================" << endl;
                    Admin_Menu_Function::view_students_by_course(foldername);
                    cout << "=============================" << endl;
                    break;

                case 4:
                    cout << "=============================" << endl;
                    Admin_Menu_Function::view_students_by_yearlevel(foldername);
                    cout << "=============================" << endl;
                    break;

                case 5:
                    cout << "=============================" << endl;
                    cout << "Enter Student SR-Code: ";
                    cin >> srcode;
                    Admin_Menu_Function::search_student_info(foldername, srcode);
                    cout << "=============================" << endl;
                    break;

                case 6:
                    cout << "=============================" << endl;
                    cout << "Exit" << endl;
                    cout << "=============================" << endl;
                    admin_menu_loop = true;
                    break;

                default:
                    cout << "Wrong Input. Please Try Again." << endl;
                    break;
            }
        } while (!admin_menu_loop);
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
            string folder = "Curicculum";
            string coursePath = Student_Menu_Function::curicullum_file( course, yearLevel);

            switch (student_menu_action) {
                case 1:
                    cout << "=============================" << endl;
                    Student_Menu_Function::view_curicculum(folder,coursePath);
                    clear_screen();
                    cout << "=============================" << endl;
                    break;

                case 2:
                    cout << "=============================" << endl;
                    if (status == "Not Enrolled") {
                        cout << "No subjects taken yet." << endl;
                        clear_screen();
                    } else {
                        vector<string> studentSubjects = Student_Menu_Function::getStudentSubjects(filePath);
                        vector<Course> courses = Student_Menu_Function::getCourses(folder, coursePath);
                        cout << "Subjects Taken:" << endl;
                        Student_Menu_Function::matchSubjects(studentSubjects, courses);
                        clear_screen();
                    }
                    cout << "=============================" << endl;
                    break;

                case 3:
                    cout << "=============================" << endl;
                    if (status == "Enrolled") {
                        cout << "Already enrolled." << endl;
                        clear_screen();
                    } else {

                        Student_Menu_Function::view_curicculum(folder, coursePath);
                        vector<string> subjectsTaken;
                        cout<<"=========================================================================="<<endl;

                        while (true) {
                            string input;
                            cout << "Enter COURSE CODE to take separated by commas (or 'exit' to stop): ";
                            getline(cin, input);

                            if (input == "exit") {
                                break;
                            }

                            stringstream ss(input);
                            string subject;
                            while (getline(ss, subject, ',')) {
                                subject.erase(subject.find_last_not_of(" \n\r\t") + 1);
                                subjectsTaken.push_back(subject);
                            }
                        }

                        Student_Menu_Function::Enrollment(filePath, subjectsTaken);
                    }
                    clear_screen();
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
}

int main() {
    string folderName = "StudentRecords";
    string username, password;

    cout<<"==================================\n\n";
    cout<<"\tEnrollment System\n\n";
    cout<<"==================================\n\n";

    cout << "Enter SR-Code: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    int login_result = login(folderName, username, password);
    if (login_result == 3) {
        cout << "Login failed. Exiting program." << endl;
        return 0;
    } else if (login_result == 2) {
        Menu_Function::student_menu(folderName, username);
    } else if (login_result == 1) {
        Menu_Function::Admin_Menu(folderName);
    }

    return 0;
}

