/*!
 * \file ColorInfo.hpp
 *
 * \brief Object for storing SVG color names with rgb values.
 *
 * Upon construction, reads in the specified file.  File needs to
 * be in the following format;
 *      	aliceblue	#f0f8ff	240,248,255
 *          ...
 * After that rgb values can be queried by color name.
 *
 * RGB values for colors, found here
 * http://webdesign.about.com/od/colorcharts/l/bl_namedcolors.htm
 *
 * \date Started: 12/19/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */
#ifndef COLORINFO_HPP_
#define COLORINFO_HPP_

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

class Color {
  public:
    Color(int pR, int pG, int pB, std::string hc);
    Color();
    Color(const Color&);
    std::string name;
    std::string hexcode;
    int r;
    int g;
    int b;
};

class ColorInfo {
  public:
    ColorInfo(std::string filename);

    int getR( std::string colorname );
    int getG( std::string colorname );
    int getB( std::string colorname );
    
    std::string getHexCode( std::string colorname );

  private:
    std::map<std::string,Color> colorMap;

};

#endif