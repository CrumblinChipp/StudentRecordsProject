#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(x) _mkdir(x)
#else
#include <unistd.h>
#define MKDIR(x) mkdir(x, 0777)
#endif

namespace fs = std::filesystem;
using namespace std;

// Function to join subjects with a comma
string joinSubjects(const vector<string>& subjects) {
    stringstream ss;
    for (size_t i = 0; i < subjects.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << subjects[i];
    }
    return ss.str();
}

void modifySubjects(const string& filename, const vector<string>& newSubjects) {
    // Read the file content
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
    
    // Modify the line containing "Subjects Taken:"
    string subjectsStr = joinSubjects(newSubjects);
    size_t pos = content.find("Subjects Taken:");
    if (pos != string::npos) {
        size_t endPos = content.find('\n', pos);
        if (endPos == string::npos) {
            endPos = content.length();
        }
        content.erase(pos + 16, endPos - (pos + 16)); // Erase existing subjects
        content.insert(pos + 16, subjectsStr); // Add new subjects
    } else {
        cerr << "Line 'Subjects Taken:' not found in file." << endl;
        return;
    }

    // Write the modified content back to the file
    ofstream outputFile(filename);
    if (!outputFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    
    outputFile << content;
    outputFile.close();
}

int main() {
    string filename = "StudentRecords/test.txt"; // Replace with your file name

    vector<string> subjectsTaken;

    // Continuously take input and add it to the vector
    while (true) {
        string input;
        cout << "Enter subjects taken separated by commas (or 'exit' to stop): ";
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        stringstream ss(input);
        string subject;
        while (getline(ss, subject, ',')) {
            subject.erase(subject.find_last_not_of(" \n\r\t") + 1); // Remove any trailing whitespace
            subjectsTaken.push_back(subject);
        }
    }

    modifySubjects(filename, subjectsTaken);
    cout << "File updated successfully." << endl;

    return 0;
}
