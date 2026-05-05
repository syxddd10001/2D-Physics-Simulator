#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include <Object.hpp>
#include <Engine.hpp>

bool startsWith( const string& s, const string& prefix );

void writeQuotedLine( std::ofstream& os, const string& s );

std::string trim( const string& s );

vector<string> splitCSV( const string& line );

std::string cleanLine (std::string line );

bool readCleanLine( std::istream& is, std::string& out );

void parseEngineName( std::istream& is, Engine_Data& ed );

void parseObjects( std::istream& is, Engine_Data& ed );

void parseWindowSettings( std::istream& is, Engine_Data& ed );

std::string ensure_trailing_semicolon( std::string s );

std::vector<std::string> readSection(std::istream& is, const std::string& sectionName );

void parseEngineNameFromLines(const std::vector<std::string>& lines, Engine_Data& ed);

void parseObjectsFromLines(const std::vector<std::string>& lines, Engine_Data& ed);

void parseWindowSettingsFromLine(const std::string& line, WINDOW_SETTINGS& ws);
