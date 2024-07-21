#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

struct Course {
    string code;
    string name;
};

vector<Course> getCourses(const string& filename) {
    vector<Course> courses;
    ifstream file(filename);
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

            // Remove any leading/trailing whitespaces
            course.code.erase(course.code.begin(), find_if(course.code.begin(), course.code.end(), [](unsigned char ch) { return !isspace(ch); }));
            course.code.erase(find_if(course.code.rbegin(), course.code.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), course.code.end());
            course.name.erase(course.name.begin(), find_if(course.name.begin(), course.name.end(), [](unsigned char ch) { return !isspace(ch); }));
            course.name.erase(find_if(course.name.rbegin(), course.name.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), course.name.end());

            courses.push_back(course);
        }
    }

    file.close();

    // Debug print to verify courses
    cout << "Courses read from file:" << endl;
    for (const auto& course : courses) {
        cout << course.code << ": " << course.name << endl;
    }

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
                // Remove any leading/trailing whitespaces
                subject.erase(subject.begin(), find_if(subject.begin(), subject.end(), [](unsigned char ch) { return !isspace(ch); }));
                subject.erase(find_if(subject.rbegin(), subject.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), subject.end());
                subjects.push_back(subject);
            }
            break;
        }
    }

    file.close();

    // Debug print to verify student subjects
    cout << "Subjects taken by student:" << endl;
    for (const auto& subject : subjects) {
        cout << subject << endl;
    }

    return subjects;
}

void matchSubjects(const vector<string>& studentSubjects, const vector<Course>& courses) {
    unordered_map<string, string> courseMap;
    for (const auto& course : courses) {
        courseMap[course.code] = course.name;
    }

    cout << "Matched Courses:" << endl;
    for (const auto& subject : studentSubjects) {
        if (courseMap.find(subject) != courseMap.end()) {
            cout << subject << ": " << courseMap[subject] << endl;
        } else {
            cout << subject << ": Not found in course list" << endl;
        }
    }
}

int main() {
    string studentFile = "StudentRecords/test.txt";
    string coursesFile = "Curicculum/4th BSCS.txt";

    vector<string> studentSubjects = getStudentSubjects(studentFile);
    vector<Course> courses = getCourses(coursesFile);

    matchSubjects(studentSubjects, courses);

    return 0;
}
