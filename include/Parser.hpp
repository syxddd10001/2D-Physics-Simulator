#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include <Object.hpp>
#include <Engine.hpp>

bool startsWith(const string& s, const string& prefix);

void writeQuotedLine(std::ofstream& os, const string& s);

static inline string trim(const string& s);

vector<string> splitCSV(const string& line);

std::string cleanLine(std::string line);

bool readCleanLine(std::istream& is, std::string& out);

void parseEngineName(std::istream& is, Engine_Data& ed);

void parseObjects(std::istream& is, Engine_Data& ed);

void parseWindowSettings(std::istream& is, Engine_Data& ed);