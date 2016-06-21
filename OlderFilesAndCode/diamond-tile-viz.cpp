/*!
 * \file diamond-tile-viz.cpp
 *
 * \brief Driver for diamond tile visualization using svg output.
 *
 * \date Started: 8/28/13
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013, Colorado State University <br>
 * All rights reserved. <br>
 */
#include "CellField.hpp"
#include "CellFieldArray.hpp"
#include "svgprinter.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

// converts the tile coordinates to a string
std::string tileCoordToString(int c1, int c2, int c3) {
    std::stringstream ss;
    ss << c1 << "," << c2 << "," << c3;
    
    return ss.str();
}

// Array of colors.
std::string svgColors[] = 
{"red","yellow","green","lime","teal","aqua","blue","fuchsia","grey","silver","white"};
//{"maroon","red","olive","yellow","green","lime","teal","aqua","navy","blue","purple","fuchsia","black","grey","silver","white"};
//{"yellow","green","aqua","navy","red","teal","fuchsia","lime","maroon","silver","olive","blue","black","purple","gray","white"};
int num_colors = 11;

// converts the tile coordinates to a string
std::string tileCoordToColor(int c1, int c2, int c3) {
    //static int count = -1;
    //count++;
    int tile_id = (c1+4)*25+(4+c2)*5+c3;
    return svgColors[ tile_id % num_colors ];
}

// Definitions and declarations needed for diamonds-tij-skew.is
#include "eassert.h"
#include "intops.h"
#define N 10
#define T 4
#define do_edge_pong(i,j) // nothing
#define do_edge_ping(i,j) // nothing
#define do_init_pong(i,j) // nothing
#define startclock() // nothing

int c1, c2, c3, c4, c5, c6, c7, c8;

#define calc_ping(t,i,j) {test2.setLabel(t,i,j,tileCoordToString(c1,c2,c3)); \
    cout << "c1,c2,c3 = " << c1 << ", " << c2 << ", " << c3 << "    "; \
    cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    test2.setFill(t,i,j,tileCoordToColor(c1,c2,c3)); }
    
#define calc_pong(t,i,j) {test2.setLabel(t,i,j,tileCoordToString(c1,c2,c3)); \
    cout << "c1,c2,c3 = " << c1 << ", " << c2 << ", " << c3 << "    "; \
    cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    test2.setFill(t,i,j,tileCoordToColor(c1,c2,c3)); }



int main() {

    // Now create header and footer ourselves
    // and a shifted CellField.
  {
    CellField::sSpacing = 60;
    CellField::sRadius = 20;
    ofstream file("diamonds-tij-skew-count.svg");
    SVGPrinter svg(file);
    svg.printHeader();
    CellFieldArray test2(4,10,10);

    //#include "diamonds-tij-skew.is"
    //#include "pipelined.is"
    //#include "diamonds-tij-skew-6x6x6.is"
    #include "diamonds-tij-skew-3x6x6.is"

    test2.printToSVG(svg);
    svg.printFooter();
  }

  return 0;
}
