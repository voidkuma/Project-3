// Created Wednesday Feb. 5th 2025
// Rei Hernandez
//
#include <vector>
#include <iostream>
#include <sstream>
#include "DSVReader.h"

// The private structure that holds the data members: Source, Delimeter and LeftOvers
struct CDSVReader::SImplementation { 
    std::shared_ptr<CDataSource> Source; //Pointer to the data source where the data gets read.
    char Delimiter; //Stands for , \t, and more...Used to seperate values
    std::string Leftovers; //Leftovers = text we didn't process yet
    SImplementation(std::shared_ptr<CDataSource> src, char delimiter){
        Source = src; //Initalizing src as Source
        Delimiter = delimiter; //Initalizing Delimiter as delimiter 
    }
};
//Assignment:
// Constructor for DSV reader that takes in two parameters,
// src specifies the data source
// delimiter specifies the delimiting character
CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter); //Initalizing pointer to manage memory
}

// Assignment:
// Destructor for DSV reader
CDSVReader::~CDSVReader(){
}

// Returns true if all rows have been read from the DSV
// So it's true if...
// Source as no more data
// There's no more leftover characters to process
bool CDSVReader::End() const{
    return DImplementation->Source->End() && DImplementation->Leftovers.empty(); //Returns bool status of End()
}
// Assignment: 
// Returns true if the row is successfully read, 
// one string will be put in the row per column
bool CDSVReader::ReadRow(std::vector<std::string> &row){
    row.clear();  // Starts off with a clean row
    std::string line; // Temp string to hold current line being worked on 

   while (true) { //Continues until there isn't anything to process
    
    // If there's no more leftovers,
    // we make more by processing more data from the source
    if (DImplementation->Leftovers.empty()) { 
        std::vector<char> buffer(1024); // Reading 1024 characters at a time
        if (!DImplementation->Source->Read(buffer, buffer.size())) {
            if (DImplementation->Leftovers.empty()) { // If no data is read...
                return false; // no more data
            }
        }
        //Appending new data to leftovers
        DImplementation->Leftovers.append(buffer.begin(), buffer.end()); 
    }

    // Here we continue to process the data (sus) but now we making a line
    // Look for a newline character in our Leftovers
    size_t newlinePos = DImplementation->Leftovers.find('\n');

    if (newlinePos != std::string::npos) { // If a newline is found, we extract it
        line = DImplementation->Leftovers.substr(0, newlinePos); 
        DImplementation->Leftovers.erase(0, newlinePos + 1);
        break; // We now have a full line
    } else { // We gotta read more data
        line += DImplementation->Leftovers; //Add all of leftovers into line
        DImplementation->Leftovers.clear(); //Clears leftovers variable
    }
}

    // Initalizing stringstream so we can line parse 
    std::stringstream ss(line);
    std::string cell; // Will hold values from the current row we in, (the individual columns)
    bool insideQuotes = false;

    // Loops through each character in the current line we in
    for (size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];

        if (ch == '"') { //Checks for embedded quotes
            if (insideQuotes && (i + 1 < line.size()) && line[i + 1] == '"') {
                cell += '"';  // Store a single quote
                ++i; // Skip the second quote
            } else {
                insideQuotes = !insideQuotes;  
            }
        } else if (ch == DImplementation->Delimiter && !insideQuotes) {
            row.push_back(cell);
            cell.clear();
        } else {
            cell += ch;
        }
    }
    row.push_back(cell);

    return true;
}