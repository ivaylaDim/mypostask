#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class Teacher {
public:
    int teacherID;
    string firstName;
    string lastName;
    tm dateHired;
    bool isActive;
};

class Subject {
public:
    int subjectID;
    string subjectName;
    int teacherID;
    int classYear;
};

class Class {
public:
    int classID;
    int classYear;
    int headTeacher;
};

// Subject-related functions
vector<Subject> getAllSubjects();
int readSubjects();
int addSubject(const Subject& subject);
int editSubject(int subjectId, const Subject& updatedSubject);
int deleteSubject(int subjectId);
string getSubjectName(int subjectId);
pair<string, string> getTeacherInfo(int teacherId); 