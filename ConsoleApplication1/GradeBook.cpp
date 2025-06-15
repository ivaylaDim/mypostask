#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <direct.h> //debug
#include <map>
#include <set>
#include <algorithm>

// I am aware the requirements specify only using standard libraries
// ..but I tried and failed to connect the database to my project
// and because i most recently worked with json files for a course project, 
// I am most comfortable using that
// All the data and structuring in the json files come from the db
// which does exist and will be included as an sql query ...

using json = nlohmann::json;
using namespace std;

//file path constants
const string STUDENT_FILE = "data/Student.json";
const string GRADE_FILE = "data/Grade.json";
const string SUBJECT_FILE = "data/Subject.json";
const string TEACHER_FILE = "data/Teacher.json";
const string STUDENT_GRADE_FILE = "data/StudentGrade.json";
const string CLASS_FILE = "data/Class.json";

//defining classes that match files
class Student {
public:
    int id;
    string firstName;
    string lastName;
    tm date;
    int classNumber;
    int numberInClass;
};
class Teacher {
public:
    int teacherID;
    string firstName;
    string lastName;
    tm dateHired;
    bool isActive;
};
class Grade {
public:
    int id;
    string name;
    int value;
};
class Subject {
public:
    int subjectID;
    string subjectName;
    int teacherID;
    int classYear;
};
class StudentGrade {
public:
    int studentID;
    int gradeID;
    int subjectID;
    int teacherID;
    tm gradeDate;
};
class Class {
public:
    int classID;
    int classYear;
    int headTeacher;
};

//declaring functions (some work without being declared above main but im not risking it)

//main menu actions
void showStudentMenu();
void showGradeMenu();
void showSubjectMenu();
void showSummariesMenu();
//student actions
vector<Student> getAllStudents();
int readStudents();
int addStudent(const Student& student);
int editStudent(int studentId, const Student& updatedStudent);
int deleteStudent(int studentId);
tm parseDate(const string& dateStr);
string formatDate(const tm& date);
//helper functions for grade actions
string getGradeName(int gradeId);
string getSubjectName(int subjectId);
int getTeacherIdFromSubject(int subjectId);
pair<string, string> getTeacherInfo(int teacherId);
//grade actions
vector<StudentGrade> getAllStudentGrades();
int readStudentGrades();
int addStudentGrade(const StudentGrade& studentGrade);
int editStudentGrade(int studentId, int gradeId, int subjectId, const StudentGrade& updatedStudentGrade);
int editStudentGradePrompt();
int deleteStudentGrade(int studentId, int gradeId, int subjectId);
//subject actions
vector<Subject> getAllSubjects();
int readSubjects();
int addSubject(const Subject& subject);
int editSubject(int subjectId, const Subject& updatedSubject);
int deleteSubject(int subjectId);
//summaries actions
//helper functions
int getGradeValue(int gradeId);
double calculateAVG(int studentId);
double calculateAVGSubject(int studentId, int subjectId);
bool hasBirthady(const tm& birthDate);

int showAverageBySubject();
int showAverageOverall();
int showTopStudents();
int showMakeupTakers();
int showWeakStudents();
int showBirthdays();

int main() {
    cout << "Current working directory: " << _getcwd(NULL, 0) << endl; //debug

    while (true) {
        cout << "######## Welcome to project Grade book! Select an option to view more. ########\n" << endl;
        cout << " 1.View student actions.\n 2.View grade actions.\n 3.View subject actions.\n 4.Summaries &other.\n 5.Exit\n " << endl;
        int choice = 0;

        // insurance for incorrect data types
        try
        {
            // prevent infinite loop if not a number
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            switch (choice)
            {
            case 1:
                showStudentMenu();
                continue;
            case 2:
                showGradeMenu();
                continue;
            case 3:
                showSubjectMenu();
                continue;
            case 4:
                showSummariesMenu();
                continue;
            case 5:
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }
            break;
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
            continue;
        }
    }

    return 0;
}


//main menu functions defined
void showStudentMenu() {
    while (true)
    {
        cout << "######## STUDENT MENU ########\n 1.View all students.\n 2.Add new student.\n 3.Edit student by ID.\n 4.Delete student by ID.\n 5.Main menu.\n"
            << endl;
        int ch;
        try
        {
            if (!(cin >> ch))
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            switch (ch)
            {
            case 1:
                readStudents();
                continue;
            case 2:
            {
                Student newStudent;
                cout << "Enter student ID: ";
                cin >> newStudent.id;
                cin.ignore();

                cout << "Enter first name: ";
                getline(cin, newStudent.firstName);

                cout << "Enter last name: ";
                getline(cin, newStudent.lastName);

                string dateStr;
                cout << "Enter birth date (MM/DD/YYYY): ";
                getline(cin, dateStr);
                newStudent.date = parseDate(dateStr);

                cout << "Enter class number: ";
                cin >> newStudent.classNumber;

                cout << "Enter number in class: ";
                cin >> newStudent.numberInClass;

                addStudent(newStudent);
                continue;
            }
            case 3:
            {
                cout << "\n###### Available Students ######" << endl;
                readStudents();
                int id;
                cout << "Enter student ID to edit (Available student above): ";
                cin >> id;
                cin.ignore();

                // find existing student to show current values
                vector<Student> students = getAllStudents();
                auto it = find_if(students.begin(), students.end(),
                    [id](const Student& s) { return s.id == id; });

                if (it == students.end()) {
                    cout << "Student with ID " << id << " not found." << endl;
                    continue;
                }

                Student updatedStudent = *it;

                cout << "Enter new first name (current: " << updatedStudent.firstName << "): ";
                string input;
                getline(cin, input);
                if (!input.empty()) updatedStudent.firstName = input;

                cout << "Enter new last name (current: " << updatedStudent.lastName << "): ";
                getline(cin, input);
                if (!input.empty()) updatedStudent.lastName = input;

                cout << "Enter new birth date (MM/DD/YYYY) (current: " << formatDate(updatedStudent.date) << "): ";
                getline(cin, input);
                if (!input.empty()) updatedStudent.date = parseDate(input);

                cout << "Enter new class number (current: " << updatedStudent.classNumber << "): ";
                getline(cin, input);
                if (!input.empty()) updatedStudent.classNumber = stoi(input);

                cout << "Enter new number in class (current: " << updatedStudent.numberInClass << "): ";
                getline(cin, input);
                if (!input.empty()) updatedStudent.numberInClass = stoi(input);

                editStudent(id, updatedStudent);
                continue;
            }
            case 4:
            {
                cout << "\n###### Available Students ######" << endl;
                readStudents();
                int id;
                cout << "Enter student ID to delete (Available students above): ";
                cin >> id;

                char confirm;
                cout << "Are you sure you want to delete student with ID " << id << "? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    deleteStudent(id);
                }
                else {
                    cout << "Deletion cancelled." << endl;
                }
                continue;
            }
            case 5:
                cout << "Returning..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }
            break;
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
            continue;
        }
    }
}
void showGradeMenu() {
    while (true)
    {
        cout << "######## GRADE MENU ########\n 1.View all grades.\n 2.Add new grade.\n 3.Edit grades by student, subject, grade value.\n 4.Delete grade by ID.\n 5.Main menu.\n"
            << endl;
        int ch;
        try
        {
            if (!(cin >> ch))
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            switch (ch)
            {
            case 1:
                readStudentGrades();
                continue;
            case 2:
            {
                StudentGrade newStudentGrade;
                cout << "\n###### Available Students ######" << endl;
                readStudents();
                cout << "Enter student ID (Available students above): ";
                cin >> newStudentGrade.studentID;

                cout << "Enter grade ID (0-4 where 0 is 2 and 4 is 6): ";
                cin >> newStudentGrade.gradeID;

                cout << "\n###### Available Subjects ######" << endl;
                readSubjects();
                cout << "Enter subject ID: ";
                cin >> newStudentGrade.subjectID;
                cin.ignore();
                string dateStr;
                cout << "Enter grade date (MM/DD/YYYY): ";
                getline(cin, dateStr);
                newStudentGrade.gradeDate = parseDate(dateStr);

                addStudentGrade(newStudentGrade);
                continue;
            }
            case 3:
                editStudentGradePrompt();
                continue;
            case 4:
            {
                int studentId, gradeId, subjectId;
                cout << "\n###### Available Grades ######" << endl;
                readStudentGrades();
                cout << "Enter student ID (From the table above): ";
                cin >> studentId;

                cout << "Enter grade ID (0-4 where 0 is 2 and 4 is 6): ";
                cin >> gradeId;

                cout << "Enter subject ID (From the table above): ";
                cin >> subjectId;

                char confirm;
                cout << "Are you sure you want to delete this grade? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    deleteStudentGrade(studentId, gradeId, subjectId);
                }
                else {
                    cout << "Deletion cancelled." << endl;
                }
            }
            continue;
            case 5:
                cout << "Returning..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }
            break;
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
            continue;
        }
    }
}
void showSubjectMenu() {
    while (true)
    {
        cout << "######## SUBJECT MENU ########\n 1.View all subjects.\n 2.Add new subject.\n 3.Edit subject by ID.\n 4.Delete subject by ID.\n 5.Main menu.\n"
            << endl;
        int ch;
        try
        {
            if (!(cin >> ch))
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            switch (ch)
            {
            case 1:
                readSubjects();
                continue;
            case 2:
            {
                Subject newSubject;
                cout << "Enter subject ID: ";
                cin >> newSubject.subjectID;
                cin.ignore();

                cout << "Enter subject name: ";
                getline(cin, newSubject.subjectName);

                cout << "Enter teacher ID: ";
                cin >> newSubject.teacherID;

                cout << "Enter class year: ";
                cin >> newSubject.classYear;

                addSubject(newSubject);
                continue;
            }
            case 3:
            {
                cout << "\n###### Available Subjects ######" << endl;
                readStudentGrades();
                int id;
                cout << "Enter subject ID to edit: ";
                cin >> id;
                cin.ignore();

                // find existing subject first to show current values
                vector<Subject> subjects = getAllSubjects();
                auto it = find_if(subjects.begin(), subjects.end(),
                    [id](const Subject& s) { return s.subjectID == id; });

                if (it == subjects.end()) {
                    cout << "Subject with ID " << id << " not found." << endl;
                    continue;
                }

                Subject updatedSubject = *it;

                cout << "Enter new subject name (current: " << updatedSubject.subjectName << "): ";
                string input;
                getline(cin, input);
                if (!input.empty()) updatedSubject.subjectName = input;

                cout << "Enter new teacher ID (current: " << updatedSubject.teacherID << "): ";
                getline(cin, input);
                if (!input.empty()) updatedSubject.teacherID = stoi(input);

                cout << "Enter new class year (current: " << updatedSubject.classYear << "): ";
                getline(cin, input);
                if (!input.empty()) updatedSubject.classYear = stoi(input);

                editSubject(id, updatedSubject);
                continue;
            }
            case 4:
            {
                cout << "\n###### Available Subjects ######" << endl;
                readStudentGrades();
                int id;
                cout << "Enter subject ID to delete: ";
                cin >> id;

                char confirm;
                cout << "Are you sure you want to delete subject with ID " << id << "? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    deleteSubject(id);
                }
                else {
                    cout << "Deletion cancelled." << endl;
                }
                continue;
            }
            case 5:
                cout << "Returning..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }
            break;
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
            continue;
        }

    }
}
void showSummariesMenu() {
    while (true)
    {
        cout << "######## SUMMARIES MENU ########\n" << endl;
        cout << " 1.AVG grade by Student and Subject.\n 2.AVG Grade for Student in all subjects.\n 3.Top students.\n 4.Students for makeup exams.\n 5.Weak students.\n 6.Birthdays today\n 7.Return.\n" << endl;
        int ch;
        try
        {
            if (!(cin >> ch))
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a number." << endl;
                continue;
            }
            switch (ch)
            {
            case 1:
                showAverageBySubject();
                continue;
            case 2:
                showAverageOverall();
                continue;
            case 3:
                showTopStudents();
                continue;
            case 4:
                showMakeupTakers();
                continue;
            case 5:
                showWeakStudents();
                continue;
            case 6:
                showBirthdays();
                continue;
            case 7:
                cout << "Returning..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }
            break;
        }

        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
            continue;
        }
    }
}


//student functions defined

//date conversion into correct format
tm parseDate(const string& dateStr) {
    tm date = {};
    //initialisation. got some issues without this so im keeping it
    date.tm_year = 0;
    date.tm_mon = 0;
    date.tm_mday = 1;
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    date.tm_wday = 0;
    date.tm_yday = 0;
    date.tm_isdst = -1;

    istringstream ss(dateStr);
    ss >> get_time(&date, "%m/%d/%Y");
    if (ss.fail()) {
        cout << "Failed to parse date: " << dateStr << endl;
        // return a default date if parsing fails
        date.tm_year = 100;
        date.tm_mon = 0;
        date.tm_mday = 1;
    }
    return date;
}
string formatDate(const tm& date) {
    char buffer[20];
    // check date structure
    if (date.tm_year < 0 || date.tm_mon < 0 || date.tm_mon > 11 || date.tm_mday < 1 || date.tm_mday > 31) {
        return "01/01/1900"; // return default date for invalid tm structure
    }

    size_t result = strftime(buffer, sizeof(buffer), "%m/%d/%Y", &date);
    if (result == 0) {
        return "01/01/1900"; // return default date if strftime fails
    }
    return string(buffer);
}
//DATE CONVERTION ISSUES
//ADDING NEW STUDENT NOT WORKING
//NEVERMIND FIXED

vector<Student> getAllStudents() {
    vector<Student> students;
    ifstream input_file(STUDENT_FILE);
    if (!input_file.is_open()) {
        cerr << "Could not open file " << STUDENT_FILE << endl;
        return students;
    }

    try {
        json students_json;
        input_file >> students_json;
        input_file.close();

        for (const auto& item : students_json) {
            Student student;
            student.id = item["id"];
            student.firstName = item["firstName"];
            student.lastName = item["lastName"];
            student.date = parseDate(item["date"]);
            student.classNumber = item["classNumber"];
            student.numberInClass = item["numberInClass"];
            students.push_back(student);
        }
    }
    catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        input_file.close();
    }

    return students;
}
int readStudents() {
    vector<Student> students = getAllStudents();
    if (students.empty()) {
        cout << "No students found or error reading file." << endl;
        return 1;
    }

    cout << left << setw(5) << "ID"
        << setw(15) << "First Name"
        << setw(15) << "Last Name"
        << setw(15) << "Birthday"
        << setw(10) << "Class"
        << setw(10) << "Number" << endl;
    cout << string(70, '-') << endl;

    for (const auto& student : students) {
        cout << left << setw(5) << student.id
            << setw(15) << student.firstName
            << setw(15) << student.lastName
            << setw(15) << formatDate(student.date)
            << setw(10) << student.classNumber
            << setw(10) << student.numberInClass << endl;
    }

    return 0;
}

// adding new student to json file
int addStudent(const Student& student) {
    vector<Student> students = getAllStudents();

    // check if id exists
    for (const auto& s : students) {
        if (s.id == student.id) {
            cout << "Error: Student with ID " << student.id << " already exists." << endl;
            return 1;
        }
    }

    // add the new student to the vector
    students.push_back(student);

    // convert the vector to json
    json students_json = json::array();
    for (const auto& s : students) {
        json student_json;
        student_json["id"] = s.id;
        student_json["firstName"] = s.firstName;
        student_json["lastName"] = s.lastName;
        student_json["date"] = formatDate(s.date);
        student_json["classNumber"] = s.classNumber;
        student_json["numberInClass"] = s.numberInClass;
        students_json.push_back(student_json);
    }

    // write the josn back to file
    ofstream output_file(STUDENT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << students_json << endl;
    output_file.close();
    cout << "Student added successfully." << endl;
    return 0;
}

// edit student by id
int editStudent(int studentId, const Student& updatedStudent) {
    vector<Student> students = getAllStudents();
    bool found = false;

    for (auto& s : students) {
        if (s.id == studentId) {
            s.firstName = updatedStudent.firstName;
            s.lastName = updatedStudent.lastName;
            s.date = updatedStudent.date;
            s.classNumber = updatedStudent.classNumber;
            s.numberInClass = updatedStudent.numberInClass;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Student with ID " << studentId << " not found." << endl;
        return 1;
    }

    // convert the vector to json
    json students_json = json::array();
    for (const auto& s : students) {
        json student_json;
        student_json["id"] = s.id;
        student_json["firstName"] = s.firstName;
        student_json["lastName"] = s.lastName;
        student_json["date"] = formatDate(s.date);
        student_json["classNumber"] = s.classNumber;
        student_json["numberInClass"] = s.numberInClass;
        students_json.push_back(student_json);
    }

    // write the josn back to file
    ofstream output_file(STUDENT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << students_json << endl;
    output_file.close();
    cout << "Student updated successfully." << endl;
    return 0;
}

// delete student by id
int deleteStudent(int studentId) {
    vector<Student> students = getAllStudents();
    size_t originalSize = students.size();

    // remove student with that id
    students.erase(
        remove_if(students.begin(), students.end(),
            [studentId](const Student& s) { return s.id == studentId; }),
        students.end());

    if (students.size() == originalSize) {
        cout << "Error: Student with ID " << studentId << " not found." << endl;
        return 1;
    }

    // convert the vector to json
    json students_json = json::array();
    for (const auto& s : students) {
        json student_json;
        student_json["id"] = s.id;
        student_json["firstName"] = s.firstName;
        student_json["lastName"] = s.lastName;
        student_json["date"] = formatDate(s.date);
        student_json["classNumber"] = s.classNumber;
        student_json["numberInClass"] = s.numberInClass;
        students_json.push_back(student_json);
    }

    // write the josn back to file
    ofstream output_file(STUDENT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << students_json << endl;
    output_file.close();
    cout << "Student deleted successfully." << endl;
    return 0;
    //currently only removes student from the students file, doesnt look for any of their data in other files
}


//helper functions for grade actions

//get grade by id
string getGradeName(int gradeId) {
    ifstream input_file(GRADE_FILE);
    if (!input_file.is_open()) {
        return "Unknown";
    }

    try {
        json grades_json;
        input_file >> grades_json;
        input_file.close();

        for (const auto& item : grades_json) {
            if (item.contains("id") && item["id"] == gradeId) {
                return item.contains("name") ? item["name"].get<string>() : "Unknown";
            }
        }
    }
    catch (const exception& e) {
        input_file.close();
    }
    return "Unknown";
}

//get subject name by id
string getSubjectName(int subjectId) {
    ifstream input_file(SUBJECT_FILE);
    if (!input_file.is_open()) {
        return "Unknown Subject";
    }

    try {
        json subjects_json;
        input_file >> subjects_json;
        input_file.close();
        //FIXED DONT TOUCH
        for (const auto& item : subjects_json) {
            if (item.contains("id") && item["id"] == subjectId) {
                return item.contains("name") ? item["name"].get<string>() : "Unknown Subject";
            }
        }
    }
    catch (const exception& e) {
        input_file.close();
    }
    return "Unknown Subject";
}

// get teacher first and last names by id
pair<string, string> getTeacherInfo(int teacherId) {
    ifstream input_file(TEACHER_FILE);
    if (!input_file.is_open()) {
        return { "Unknown", "Teacher" };
    }

    try {
        json teachers_json;
        input_file >> teachers_json;
        input_file.close();

        for (const auto& item : teachers_json) {
            //FIXED ALSO 
            if (item.contains("id") && item["id"] == teacherId) {
                return {
                    item.contains("firstName") ? item["firstName"].get<string>() : "Unknown",
                    item.contains("lastName") ? item["lastName"].get<string>() : "Teacher"
                };
            }
        }
    }
    catch (const exception& e) {
        input_file.close();
    }
    return { "Unknown", "Teacher" };
}


//get student name by id
pair<string, string> getStudentInfo(int studentId) {
    ifstream input_file(STUDENT_FILE);
    if (!input_file.is_open()) {
        return { "Unknown", "Student" };
    }

    try {
        json students_json;
        input_file >> students_json;
        input_file.close();

        for (const auto& item : students_json) {
            if (item["id"] == studentId) {
                return { item["firstName"], item["lastName"] };
            }
        }
    }
    catch (const exception& e) {
        input_file.close();
    }
    return { "Unknown", "Student" };
}

//automatically tie teacher to subject
int getTeacherIdFromSubject(int subjectId) {
    ifstream input_file(SUBJECT_FILE);
    if (!input_file.is_open()) {
        cerr << "Could not open file " << SUBJECT_FILE << endl;
        return 0;
    }

    try {
        json subjects_json;
        input_file >> subjects_json;
        input_file.close();

        for (const auto& item : subjects_json) {
            if ((item.contains("subjectId") && item["subjectId"] == subjectId) ||
                (item.contains("id") && item["id"] == subjectId)) {
                return item.contains("teacherId") ? item["teacherId"].get<int>() : 0;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        input_file.close();
    }
    return 0;
}

//student grades actions
vector<StudentGrade> getAllStudentGrades() {
    vector<StudentGrade> studentGrades;

    ifstream input_file(STUDENT_GRADE_FILE);
    if (!input_file.is_open()) {
        cerr << "Could not open file " << STUDENT_GRADE_FILE << endl;
        return studentGrades;
    }

    try {
        json studentGrades_json;
        input_file >> studentGrades_json;
        input_file.close();

        for (const auto& item : studentGrades_json) {
            StudentGrade studentGrade;
            studentGrade.studentID = item.contains("studentId") ? item["studentId"].get<int>() : 0;
            studentGrade.gradeID = item.contains("gradeId") ? item["gradeId"].get<int>() : 0;
            studentGrade.subjectID = item.contains("subjectId") ? item["subjectId"].get<int>() : 0;
            studentGrade.teacherID = item.contains("teacherId") ? item["teacherId"].get<int>() : 0;
            studentGrade.gradeDate = item.contains("gradeDate") ? parseDate(item["gradeDate"]) : parseDate("01/01/2000");
            studentGrades.push_back(studentGrade);
        }
    }
    catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
    }

    return studentGrades;
}
int readStudentGrades() {
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    if (studentGrades.empty()) {
        cout << "No students grades found or error reading file." << endl;
        return 1;
    }

    cout << left << setw(10) << "ID"
        << setw(20) << "Student Name"
        << setw(17) << "Grade"
        << setw(5) << "ID"
        << setw(35) << "Subject"
        << setw(20) << "Teacher"
        << setw(12) << "Date" << endl;
    cout << string(119, '-') << endl;

    for (const auto& studentGrade : studentGrades) {
        // get student id and name
        auto studentInfo = getStudentInfo(studentGrade.studentID);
        string studentName = studentInfo.first + " " + studentInfo.second;

        // get grade 
        string gradeName = getGradeName(studentGrade.gradeID);

        // get subject
        string subjectName = getSubjectName(studentGrade.subjectID);

        // get teacher
        auto teacherInfo = getTeacherInfo(studentGrade.teacherID);
        string teacherName = teacherInfo.first + " " + teacherInfo.second;

        cout << left << setw(10) << studentGrade.studentID
            << setw(20) << studentName
            << setw(17) << gradeName
            << setw(5) << studentGrade.subjectID
            << setw(35) << subjectName
            << setw(20) << teacherName
            << setw(12) << formatDate(studentGrade.gradeDate) << endl;
    }

    return 0;
}

int addStudentGrade(const StudentGrade& studentGradeInput) {
    vector<StudentGrade> studentGrades = getAllStudentGrades();

    StudentGrade studentGrade = studentGradeInput;

    // automatically bind teacher to subject
    int teacherId = getTeacherIdFromSubject(studentGrade.subjectID);
    if (teacherId > 0) {
        studentGrade.teacherID = teacherId;
    }
    else {
        cout << "Warning: Could not find teacher for subject ID " << studentGrade.subjectID << endl;
    }

    // check if grade exists
    for (const auto& sg : studentGrades) {
        if (sg.studentID == studentGrade.studentID &&
            sg.gradeID == studentGrade.gradeID &&
            sg.subjectID == studentGrade.subjectID) {

            cout << "Error: Grade already exists for this student" << endl;
            return 1;
        }
    }

    // add new grade to the vector
    studentGrades.push_back(studentGrade);

    // convert the vector back to json and save to file
    json studentGrades_json = json::array();
    for (const auto& sg : studentGrades) {
        json studentGrade_json;
        studentGrade_json["studentId"] = sg.studentID;
        studentGrade_json["gradeId"] = sg.gradeID;
        studentGrade_json["subjectId"] = sg.subjectID;
        studentGrade_json["teacherId"] = sg.teacherID;
        studentGrade_json["gradeDate"] = formatDate(sg.gradeDate);
        studentGrades_json.push_back(studentGrade_json);
    }

    // write to file
    ofstream output_file(STUDENT_GRADE_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << studentGrades_json << endl;
    output_file.close();

    cout << "Student grade added successfully." << endl;

    return 0;
}

int editStudentGrade(int studentId, int gradeId, int subjectId, const StudentGrade& updatedStudentGrade) {
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    bool found = false;

    // automatically get teacher id
    int teacherId = getTeacherIdFromSubject(subjectId);
    if (teacherId == 0) {
        cout << "Warning: Could not find teacher for subject ID " << subjectId << endl;
        return 1;
    }

    for (auto& sg : studentGrades) {
        if (sg.studentID == studentId &&
            sg.gradeID == gradeId &&
            sg.subjectID == subjectId) {
            sg.gradeID = updatedStudentGrade.gradeID;
            sg.teacherID = teacherId;
            sg.gradeDate = updatedStudentGrade.gradeDate;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Student grade not found." << endl;
        return 1;
    }

    json studentGrades_json = json::array();
    for (const auto& sg : studentGrades) {
        json studentGrade_json;
        studentGrade_json["studentId"] = sg.studentID;
        studentGrade_json["gradeId"] = sg.gradeID;
        studentGrade_json["subjectId"] = sg.subjectID;
        studentGrade_json["teacherId"] = sg.teacherID;
        studentGrade_json["gradeDate"] = formatDate(sg.gradeDate);
        studentGrades_json.push_back(studentGrade_json);
    }

    ofstream output_file(STUDENT_GRADE_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << studentGrades_json << endl;
    output_file.close();

    auto teacherInfo = getTeacherInfo(teacherId);
    cout << "Student grade updated successfully." << endl;

    return 0;
}
int deleteStudentGrade(int studentId, int gradeId, int subjectId) {
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    size_t originalSize = studentGrades.size();

    // remove student grade with matching IDs
    studentGrades.erase(
        remove_if(studentGrades.begin(), studentGrades.end(),
            [studentId, gradeId, subjectId](const StudentGrade& sg) {
                return sg.studentID == studentId &&
                    sg.gradeID == gradeId &&
                    sg.subjectID == subjectId;
            }),
        studentGrades.end());

    if (studentGrades.size() == originalSize) {
        cout << "Error: Student grade not found." << endl;
        return 1;
    }

    // convert the vector to json
    json studentGrades_json = json::array();
    for (const auto& sg : studentGrades) {
        json studentGrade_json;
        studentGrade_json["studentId"] = sg.studentID;
        studentGrade_json["gradeId"] = sg.gradeID;
        studentGrade_json["subjectId"] = sg.subjectID;
        studentGrade_json["teacherId"] = sg.teacherID;
        studentGrade_json["gradeDate"] = formatDate(sg.gradeDate);
        studentGrades_json.push_back(studentGrade_json);
    }

    // write the json back to file
    ofstream output_file(STUDENT_GRADE_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << studentGrades_json << endl;
    output_file.close();
    cout << "Student grade deleted successfully." << endl;
    return 0;
}

int editStudentGradePrompt() {
    int studentId, gradeId, subjectId;

    readStudentGrades();

    cout << "Enter student ID: ";
    cin >> studentId;

    cout << "Enter grade ID: ";
    cin >> gradeId;

    cout << "Enter subject ID: ";
    cin >> subjectId;
    cin.ignore();

    //find grade
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    auto it = find_if(studentGrades.begin(), studentGrades.end(),
        [studentId, gradeId, subjectId](const StudentGrade& sg) {
            return sg.studentID == studentId && sg.gradeID == gradeId && sg.subjectID == subjectId;
        });

    if (it == studentGrades.end()) {
        cout << "Student grade not found." << endl;
        return 1;
    }

    StudentGrade updatedStudentGrade = *it;

    // get new values
    cout << "Enter new grade ID: ";
    cin >> updatedStudentGrade.gradeID;

    cout << "Enter new subject ID: ";
    cin >> updatedStudentGrade.subjectID;
    cin.ignore();

    cout << "Enter new date (MM/DD/YYYY): ";
    string dateStr;
    cin >> dateStr;
    updatedStudentGrade.gradeDate = parseDate(dateStr);

    return editStudentGrade(studentId, gradeId, subjectId, updatedStudentGrade);
}

//subject actions
vector<Subject> getAllSubjects() {
    vector<Subject> subjects;
    ifstream input_file(SUBJECT_FILE);
    if (!input_file.is_open()) {
        cerr << "Could not open file " << SUBJECT_FILE << endl;
        return subjects;
    }

    try {
        json subjects_json;
        input_file >> subjects_json;
        input_file.close();

        for (const auto& item : subjects_json) {
            Subject subject;
            subject.subjectID = item.contains("id") ? item["id"].get<int>() : 0;
            subject.subjectName = item.contains("name") ? item["name"].get<string>() : "";
            subject.teacherID = item.contains("teacherId") ? item["teacherId"].get<int>() : 0;
            subject.classYear = item.contains("grade") ? item["grade"].get<int>() : 0;
            subjects.push_back(subject);
        }
    }
    catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        input_file.close();
    }

    return subjects;
}
int readSubjects() {
    vector<Subject> subjects = getAllSubjects();
    if (subjects.empty()) {
        cout << "No subjects found or error reading file." << endl;
        return 1;
    }

    cout << left << setw(10) << "ID"
        << setw(35) << "Subject Name"
        << setw(20) << "Teacher"
        << setw(10) << "Grade" << endl;
    cout << string(75, '-') << endl;

    for (const auto& subject : subjects) {
        auto teacherInfo = getTeacherInfo(subject.teacherID);
        string teacherName = teacherInfo.first + " " + teacherInfo.second;

        cout << left << setw(10) << subject.subjectID
            << setw(35) << subject.subjectName
            << setw(20) << teacherName
            << setw(10) << subject.classYear << endl;
    }

    return 0;
}
int addSubject(const Subject& subject) {
    vector<Subject> subjects = getAllSubjects();

    // check if subject id already exists
    for (const auto& s : subjects) {
        if (s.subjectID == subject.subjectID) {
            cout << "Error: Subject with ID " << subject.subjectID << " already exists." << endl;
            return 1;
        }
    }

    // add the new subject to the vector
    subjects.push_back(subject);

    // convert the vector to json
    json subjects_json = json::array();
    for (const auto& s : subjects) {
        json subject_json;
        subject_json["id"] = s.subjectID;
        subject_json["name"] = s.subjectName;
        subject_json["teacherId"] = s.teacherID;
        subject_json["grade"] = s.classYear;
        subjects_json.push_back(subject_json);
    }

    // write the json back to file
    ofstream output_file(SUBJECT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << subjects_json << endl;
    output_file.close();
    cout << "Subject added successfully." << endl;
    return 0;
}
int editSubject(int subjectId, const Subject& updatedSubject) {
    vector<Subject> subjects = getAllSubjects();
    bool found = false;

    for (auto& s : subjects) {
        if (s.subjectID == subjectId) {
            s.subjectName = updatedSubject.subjectName;
            s.teacherID = updatedSubject.teacherID;
            s.classYear = updatedSubject.classYear;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Subject with ID " << subjectId << " not found." << endl;
        return 1;
    }

    // convert the vector to json
    json subjects_json = json::array();
    for (const auto& s : subjects) {
        json subject_json;
        subject_json["id"] = s.subjectID;
        subject_json["name"] = s.subjectName;
        subject_json["teacherId"] = s.teacherID;
        subject_json["grade"] = s.classYear;
        subjects_json.push_back(subject_json);
    }

    // write the json back to file
    ofstream output_file(SUBJECT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << subjects_json << endl;
    output_file.close();
    cout << "Subject updated successfully." << endl;
    return 0;
}
int deleteSubject(int subjectId) {
    vector<Subject> subjects = getAllSubjects();
    size_t originalSize = subjects.size();

    // remove subject with that id
    subjects.erase(
        remove_if(subjects.begin(), subjects.end(),
            [subjectId](const Subject& s) { return s.subjectID == subjectId; }),
        subjects.end());

    if (subjects.size() == originalSize) {
        cout << "Error: Subject with ID " << subjectId << " not found." << endl;
        return 1;
    }

    // convert the vector to json
    json subjects_json = json::array();
    for (const auto& s : subjects) {
        json subject_json;
        subject_json["id"] = s.subjectID;
        subject_json["name"] = s.subjectName;
        subject_json["teacherId"] = s.teacherID;
        subject_json["grade"] = s.classYear;
        subjects_json.push_back(subject_json);
    }

    // write the json back to file
    ofstream output_file(SUBJECT_FILE);
    if (!output_file.is_open()) {
        cerr << "Could not open file for writing." << endl;
        return 1;
    }

    output_file << setw(2) << subjects_json << endl;
    output_file.close();
    cout << "Subject deleted successfully." << endl;
    return 0;
}

//summaries here..
//helper functions

//same as getName, but for value so we can calculate
int getGradeValue(int gradeId) {
    ifstream input_file(GRADE_FILE);
    if (!input_file.is_open()) {
        return 0;
    }
    try {
        json grades_json;
        input_file >> grades_json;
        input_file.close();

        for (const auto& item : grades_json) {
            if (item.contains("id") && item["id"] == gradeId) {
                return item.contains("value") ? item["value"].get<int>() : 0;
            }
        }
    }
    catch (const exception& e) {
        input_file.close();
    }
    return 0;
}
//average grade in all subjects
double calculateAVG(int studentId) {
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    int sum = 0;
    int count = 0;

    for (const auto& grade : studentGrades) {
        if (grade.studentID == studentId) {
            int gradeValue = getGradeValue(grade.gradeID);
            sum += gradeValue;
            count++;
        }
    }
    double averageGrade = 0.0;
    if (count > 0) {
        averageGrade = static_cast<double>(sum) / count; //convert int to double and divide
    }

    return averageGrade;
}

//average in one subject
//same as above, but added clause for subject
double calculateAVGSubject(int studentId, int subjectId) {
    vector<StudentGrade> studentGrades = getAllStudentGrades();
    int sum = 0;
    int count = 0;

    for (const auto& grade : studentGrades) {
        if (grade.studentID == studentId && grade.subjectID == subjectId) {
            int gradeValue = getGradeValue(grade.gradeID);
            sum += gradeValue;
            count++;
        }
    }
    double averageGrade = 0.0;
    if (count > 0) {
        averageGrade = static_cast<double>(sum) / count; //convert int to double and divide
    }

    return averageGrade;
}


//birthday
bool hasBirthady(const tm& birthDate) {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    return (ltm.tm_mon == birthDate.tm_mon &&
        ltm.tm_mday == birthDate.tm_mday);
}

//average by subject
int showAverageBySubject() {
    int studentId, subjectId;
    cout << "\n###### Available Students ######" << endl;
    readStudents();
    cout << "\nEnter student ID (Available students above): ";
    cin >> studentId;
    cout << "\n###### Available Subjects ######" << endl;
    readSubjects();
    cout << "Enter subject ID (Available subjects above): ";
    cin >> subjectId;

    auto studentInfo = getStudentInfo(studentId);
    string studentName = studentInfo.first + " " + studentInfo.second;
    string subjectName = getSubjectName(subjectId);
    double average = calculateAVGSubject(studentId, subjectId);

    cout << "\n###### Average Grade ######" << endl;
    if (average > 0.0) {
        cout << "Student: " << studentName << " (ID: " << studentId << ")" << endl;
        cout << "Subject: " << subjectName << " (ID: " << subjectId << ")" << endl;
        cout << "Average Grade: " << fixed << setprecision(2) << average << endl; // two digits after floating point
    }
    else {
        cout << "No grades found for student " << studentName
            << " in subject " << subjectName << endl;
    }
    return 0;
}
//average in all subjects
int showAverageOverall() {
    int studentId;
    cout << "\n###### Available Students ######" << endl;
    readStudents();
    cout << "\nEnter student ID (Available students above):";
    cin >> studentId;

    auto studentInfo = getStudentInfo(studentId);
    string studentName = studentInfo.first + " " + studentInfo.second;
    double average = calculateAVG(studentId);

    cout << "\n###### Average Grade ######" << endl;
    if (average > 0.0) {
        cout << "Student: " << studentName << " (ID: " << studentId << ")" << endl;
        cout << "Average Grade: " << fixed << setprecision(2) << average << endl;
    }
    else {
        cout << "No grades found for student " << studentName << endl;
    }
    return 0;
}

//students with an average grade above 5.50
int showTopStudents() {
    vector<Student> students = getAllStudents();
    cout << "\n#### Top Students ####" << endl;

    vector<Student> topStudents;
    map<int, double> averages;

    //calculates average for all students, may lead to performance issues
    for (const auto& student : students) {
        double avg = calculateAVG(student.id); 
        if (avg >= 5.50) {
            topStudents.push_back(student);
            averages[student.id] = avg;
        }
    }

    //display results
    cout << left << setw(10) << "ID"
        << setw(25) << "Student Name"
        << setw(10) << "Average" << endl;
    cout << string(45, '-') << endl;

    for (const auto& student : topStudents) {
        cout << left << setw(10) << student.id
            << setw(25) << (student.firstName + " " + student.lastName)
            << setw(10) << fixed << setprecision(2) << averages[student.id] << endl;
    }

    if (topStudents.empty()) {
        cout << "No students found with average grade >= 5.50" << endl;
    }

    return 0;
}


//helper functions
map<int, int> loadAllGradeValues() {
    map<int, int> gradeValues;
    ifstream gradeFile(GRADE_FILE);
    if (!gradeFile.is_open()) {
        return gradeValues;
    }

    try {
        json gradesJson;
        gradeFile >> gradesJson;
        gradeFile.close();

        for (const auto& grade : gradesJson) {
            if (grade.contains("id") && grade.contains("value")) {
                gradeValues[grade["id"]] = grade["value"];
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error loading grade values: " << e.what() << endl;
    }

    return gradeValues;
}

int showMakeupTakers() {
    const int gradeToFail = 3; 

    cout << "\n##### Students Needing Makeup Exams #####" << endl;

    vector<Student> students = getAllStudents();
    vector<StudentGrade> allGrades = getAllStudentGrades();
    map<int, int> gradeValues = loadAllGradeValues();

    //lookup for students by ID (to be fair i dont really understand maps)
    map<int, string> studentNames;
    for (const auto& student : students) {
        studentNames[student.id] = student.firstName + " " + student.lastName;
    }

    map<int, string> subjectNames;
    ifstream subjectFile(SUBJECT_FILE);
    if (subjectFile.is_open()) {
        json subjectsJson;
        subjectFile >> subjectsJson;
        subjectFile.close();

        for (const auto& subject : subjectsJson) {
            if (subject.contains("id") && subject.contains("name")) {
                subjectNames[subject["id"]] = subject["name"];
            }
        }
    }

    map<int, set<int>> failedSubjectsByStudent;

    for (const auto& grade : allGrades) {
        auto valueIt = gradeValues.find(grade.gradeID);
        if (valueIt != gradeValues.end() && valueIt->second < gradeToFail) {
            failedSubjectsByStudent[grade.studentID].insert(grade.subjectID);
        }
    }

    if (failedSubjectsByStudent.empty()) {
        cout << "No students need makeup exams." << endl;
        return 0;
    }

    //display results
    cout << left << setw(10) << "ID"
        << setw(25) << "Student Name"
        << "Failed Subjects" << endl;
    cout << string(70, '-') << endl;
    for (const auto& pair : failedSubjectsByStudent) {
        int studentId = pair.first;
        const set<int>& failedSubjects = pair.second;

        // get name
        string studentName = studentNames.count(studentId) ?
            studentNames[studentId] : "Unknown Student";

        cout << left << setw(10) << studentId
            << setw(25) << studentName;

        // failed subjects
        for (const auto& subjectId : failedSubjects) {
            string subjectName = subjectNames.count(subjectId) ?
                subjectNames[subjectId] : "Unknown Subject";
            cout << subjectName << ", ";
        }
        cout << endl;
    }

    return 0;
}
int showWeakStudents() {
    vector<Student> students = getAllStudents();
    cout << "\n#### Weak Students ####" << endl;
    
    //vector to add the weak students
    vector<Student> weakStudents;
    map<int, double> averages;

    for (const auto& student : students) {
        double avg = calculateAVG(student.id); 
        if (avg > 0 && avg < 3) {  // dont print students with no grades
            weakStudents.push_back(student);
            averages[student.id] = avg;
        }
    }

    //display results
    cout << left << setw(10) << "ID"
        << setw(25) << "Student Name"
        << setw(10) << "Average" << endl;
    cout << string(45, '-') << endl;

    for (const auto& student : weakStudents) {
        cout << left << setw(10) << student.id
            << setw(25) << (student.firstName + " " + student.lastName)
            << setw(10) << fixed << setprecision(2) << averages[student.id] << endl; //two digits after floating point 
    }

    if (weakStudents.empty()) {
        cout << "No students with average grade below 3.00" << endl;
    }

    return 0;
}


int showBirthdays() {
    vector<Student> students = getAllStudents();
    cout << "\n#### Birthdays today ####" << endl;

    bool bday = false;
    for (const auto& student : students) {
        if (hasBirthady(student.date)) {
            cout << student.firstName << " " << student.lastName
                << " (" << formatDate(student.date) << ")" << endl;
            bday = true;
        }
    }

    if (!bday) {
        cout << "No student birthdays today!" << endl;
    }

    return 0;
}