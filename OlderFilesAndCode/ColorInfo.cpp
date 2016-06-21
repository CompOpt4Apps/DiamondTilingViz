/*!
 * \file ColorInfo.cpp
 *
 * \brief Implements ColorInfo.
 *
 * \date Started: 12/19/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */

#include "ColorInfo.hpp"

// Since I don't want to install boost.
// http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


Color::Color(int pR, int pG, int pB, std::string hc) 
    : r(pR), g(pG), b(pB), hexcode(hc) { }
Color::Color() : r(0), g(0), b(0) {}
Color::Color(const Color& other) 
    : r(other.r), g(other.g), b(other.b), hexcode(other.hexcode) {}

ColorInfo::ColorInfo(std::string filename) {
    std::cout << "In constructor" << std::endl;
    
    std::ifstream infile;
    infile.open(filename);
    std::string line;
    if (infile.is_open()) {
        std::vector<std::string> tokens;
        
        // Grab all the tokens from the file.
        std::copy(std::istream_iterator<std::string>(infile),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

        // Assuming color name, hex code, and rgb, then can unpack rgb.
        for (int i=0; i<tokens.size(); i+=3) {
            // getting rgb and hexcode
            std::vector<std::string> rgbstring = split(tokens[i+2], ',');
            
            colorMap[tokens[i]] = Color(std::stoi(rgbstring[0]),
                                        std::stoi(rgbstring[1]),
                                        std::stoi(rgbstring[2]),
                                        tokens[i+1]);                            
        }

    } else {
        std::cout << "File not found: " << filename << std::endl;
        exit(0);
    }

}

// The below 3 getters will return black if the given color is not found.

int ColorInfo::getR( std::string colorname ) {
    if (colorMap.find( colorname ) != colorMap.end() ) {
        return colorMap[colorname].r;
    } else {
        return 0;
    }
}

int ColorInfo::getG( std::string colorname ) {
    if (colorMap.find( colorname ) != colorMap.end() ) {
        return colorMap[colorname].g;
    } else {
        return 0;
    }
}

int ColorInfo::getB( std::string colorname ) {
    if (colorMap.find( colorname ) != colorMap.end() ) {
        return colorMap[colorname].b;
    } else {
        return 0;
    }
}

// FIXME
std::string ColorInfo::getHexCode( std::string colorname ) {
    if (colorMap.find( colorname ) != colorMap.end() ) {
        return colorMap[colorname].hexcode;
    } else {
        return "#000000";
    }
}
