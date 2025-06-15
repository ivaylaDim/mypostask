#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

tm parseDate(const string& dateStr);
string formatDate(const tm& date);
bool hasBirthady(const tm& birthDate); 