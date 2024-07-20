#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>

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
