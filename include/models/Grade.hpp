#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class Grade {
public:
    int id;
    string name;
    int value;
};

class StudentGrade {
public:
    int studentID;
    int gradeID;
    int subjectID;
    int teacherID;
    tm gradeDate;
};

// Grade-related functions
vector<StudentGrade> getAllStudentGrades();
int readStudentGrades();
int addStudentGrade(const StudentGrade& studentGrade);
int editStudentGrade(int studentId, int gradeId, int subjectId, const StudentGrade& updatedStudentGrade);
int deleteStudentGrade(int studentId, int gradeId, int subjectId);
string getGradeName(int gradeId);
int getGradeValue(int gradeId); 