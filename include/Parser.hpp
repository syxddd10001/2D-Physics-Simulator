#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include <utils.hpp>
#include <Object.hpp>
#include <Engine.hpp>

struct Engine_Block {
    std::string engine_name;
    std::vector<std::string> objects;
    std::string window_settings;
};

bool startsWith( const string& s, const string& prefix );

void writeQuotedLine( std::ofstream& os, const string& s );

std::string trim( const string& s );

std::string trimQuotes(const std::string& s);

vector<string> splitCSV( const string& line );

std::string cleanLine (std::string line );

bool readCleanLine( std::istream& is, std::string& out );

void parseEngineName( std::istream& is, Engine_Data& ed );

void parseObjects( std::istream& is, Engine_Data& ed );

void parseWindowSettings( std::istream& is, Engine_Data& ed );

std::string ensure_trailing_semicolon( std::string s );

void parseEngineName( const std::string& rawName, Engine_Data& ed );

void parseObjects(const std::vector<std::string>& lines, Engine_Data& ed);

void parseWindowSettings(const std::string& line, WINDOW_SETTINGS& ws);

std::vector<Engine_Block> parseEngines(std::istream& is);

std::vector<Engine_Data> parseAllEngineBlocks( std::vector<Engine_Block> blocks );

Engine_Data parseEngineBlock( Engine_Block block );

std::vector<Engine_Block> engineDataToBlockBulk( std::vector<Engine_Data> engine_data );

Engine_Block engineDataToBlock( Engine_Data engine_data );

