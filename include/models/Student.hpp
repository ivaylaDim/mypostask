#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class Student {
public:
    int id;
    string firstName;
    string lastName;
    tm date;
    int classNumber;
    int numberInClass;
};

// Student-related functions
vector<Student> getAllStudents();
int readStudents();
int addStudent(const Student& student);
int editStudent(int studentId, const Student& updatedStudent);
int deleteStudent(int studentId);
pair<string, string> getStudentInfo(int studentId); 