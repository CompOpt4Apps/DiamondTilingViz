/*!
 * \file diamond-slice-viz.cpp
 *
 * \brief Driver for visualizing 2D slices of 3D diamond tiles.
 *        
 *  Being used to develop code for traversing a slice of the diamond
 *  tiling space.
 *
 * \date Started: 7/16/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */
#include "CellField.hpp"
#include "CellFieldArray.hpp"
#include "svgprinter.hpp"
#include "CmdParams.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

//==============================================
// Global parameters with their default values.
int tau = 3;  // tile size is TAUxTAUxTAU
int T = 4;
int subset_s = 3;
int Tstart = 1;
int Tend = 4;
int N = 10;
int grid_spacing = -1;
int cell_spacing = 60;
int cell_radius = 20;
bool label = false;
bool debug = false;
int color_incr = 1;
bool one_tile = false;
int one_tile_c0 = 1;
int one_tile_c1 = 1;
int one_tile_c2 = -1;

typedef enum {
    normal,
    halfradius,
} gridspacing_type;
gridspacing_type gridspacingChoice = normal;
char gridspacingStr[MAXPOSSVALSTRING];
#define num_GPairs 2
static EnumStringPair GPairs[] = {{normal,"normal"},
                                  {halfradius,"halfradius"}
                                 };            

//==============================================

// Create the file name based on parameters.
// Needs global parameter variables and TAU, SIGMA, and GAMMA defines.
// example: diamond-slice-3x3x3-T4N10-c60r20
std::string create_file_name() {
    std::stringstream ss;
    ss << "diamond-slice-" << tau << "x" << tau << "x" << tau;
    ss << "-T" << T << "u" << subset_s << "N" << N;
    ss << "-s" << Tstart << "e" << Tend;
    ss << "-p" << grid_spacing 
       << "c" << cell_spacing << "r" << cell_radius << "l" << label;
    ss << "i" << color_incr;
    ss << "-o" << one_tile;
    if (one_tile) { 
       ss << "." << one_tile_c0 << "." << one_tile_c1 << "." << one_tile_c2;
    }
    ss << ".svg";
    
    return ss.str();
}

void initParams(CmdParams * cmdparams)
/*--------------------------------------------------------------*//*!
  Uses a CmdParams object to describe all of the command line
  parameters.

  \author  Michelle Strout 9/21/13
*//*--------------------------------------------------------------*/
{

    CmdParams_describeNumParam(cmdparams,"tau", 't', 1,
            "size of the tile, tau x tau x tau, must be multiple of 3",
            3, 129, 15); // arbitrary upper bound at this point

    CmdParams_describeNumParam(cmdparams,"numTimeSteps", 'T', 1,
            "number of time steps",
            1, 30, 4);
            
    CmdParams_describeNumParam(cmdparams,"subset_s", 'u', 1,
            "number of slices within a slab",
            1, 30, 4);

    CmdParams_describeNumParam(cmdparams,"Tstart", 's', 1,
            "start visualization at Tstart",
            1, 30, 1);
            
    CmdParams_describeNumParam(cmdparams,"Tend", 'e', 1,
            "end visualization at Tend, will default to T",
            1, 30, -1);
            
    CmdParams_describeNumParam(cmdparams,"spatialDim", 'N', 1,
            "2D data will be NxN", 
            1, 75, 10);

    CmdParams_describeEnumParam(cmdparams,"grid_spacing_approach", 'g', 1,
            "approach for spacing between top of slices", 
            GPairs, num_GPairs, normal);
            
    CmdParams_describeNumParam(cmdparams,"grid_spacing", 'p', 1,
            "precise spacing between top of slices, "
            "overrides grid spacing approach if set", 
            1, 1000, -1);
            
    CmdParams_describeNumParam(cmdparams,"cell_spacing", 'c', 1,
            "cell spacing between iteration circles", 
            1, 100, 60);
            
    CmdParams_describeNumParam(cmdparams,"cell_radius", 'r', 1,
            "radius for iteration circles", 
            1, 100, 20);
            
    CmdParams_describeNumParam(cmdparams,"debug", 'd', 1,
            "debug flag", 
            0, 1, 0);
            
    CmdParams_describeNumParam(cmdparams,"label", 'l', 1,
            "whether to put tile coordinates on iteration points or not", 
            0, 1, 0);

    CmdParams_describeNumParam(cmdparams,"color_incr", 'i', 1,
            "amount to increment color by when tile changes", 
            0, 10, 1);

    CmdParams_describeNumParam(cmdparams,"one_tile", 'o', 1,
            "whether to just color one tile", 
            0, 1, 0);

    CmdParams_describeNumParam(cmdparams,"one_tile_c0", '0', 1,
            "c0 coord for one tile being shown", 
            -10, 20, 1);

    CmdParams_describeNumParam(cmdparams,"one_tile_c1", '1', 1,
            "c1 coord for one tile being shown", 
            -10, 20, 1);

    CmdParams_describeNumParam(cmdparams,"one_tile_c2", '2', 1,
            "c2 coord for one tile being shown", 
            -10, 20, -1);

}   

// converts the tile coordinates to a string
std::string tileCoordToString(int c0, int c1, int c2) {

    std::stringstream ss;

    ss << c0 << "," << c1 << "," << c2;
    
    return ss.str();
}

// Array of colors.
std::string svgColors[] = 
//{"bisque","red","aqua","yellow","blue","green","fuchsia","lime","silver","coral","lavender","pink","powderblue","plum","palegreen"};
{"red","yellow","green","lime","aqua","blue","fuchsia","silver","bisque","coral","lavender","pink","powderblue","plum","palegreen","teal","navy"};
//{"maroon","red","olive","yellow","green","lime","teal","aqua","navy","blue","purple","fuchsia","black","grey","silver","white"};
//{"yellow","green","aqua","navy","red","teal","fuchsia","lime","maroon","silver","olive","blue","black","purple","gray","white"};
int num_colors = 17;

// converts the tile coordinates to a string
std::string tileCoordToColor(int c0, int c1, int c2) {
    // CRO -- trying something a little different here
    return svgColors[c0*(-1)];
    
    static int count = -1;
    static int last_c0 = -99;
    static int last_c1 = -99;
    static int last_c2 = -99;

    // We want to change the tile color if the tile coordinate
    // has changed.

    if (c0!=last_c0 || c1!=last_c1 || c2!=last_c2) {
        last_c0 = c0;
        last_c1 = c1;
        last_c2 = c2;
        count += color_incr;
    }
    
    return svgColors[ count % num_colors ];
}

// Definitions and declarations needed for diamonds-tij-skew.is
#include "eassert.h"
#include "intops.h"

int c0, c1, c2, c3, c4, c5, c6, c7, c8;

// The computation macro prints the svg for each iteration point.
// Capturing the c0, c1, and c2 variables in the generated code, which
// should be the tile coordinates.
#define computation(t,i,j) { \
    if (label) slices.setLabel1(t,i,j,tileCoordToString(c0,c1,c2)); \
    if (debug) { \
      cout << "c0,c1,c2 = " << c0 << ", " << c1 << ", " << c2 << "    "; \
      cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    } \
    if (!one_tile || (c0==one_tile_c0 && c1==one_tile_c1 && c2==one_tile_c2)) {\
      slices.setFill(t,i,j,tileCoordToColor(c0,c1,c2)); } }
    
int main(int argc, char ** argv) {
    // Do command-line parsing.
    CmdParams *cmdparams = CmdParams_ctor(1);
    initParams(cmdparams);
    CmdParams_parseParams(cmdparams,argc,argv);
    tau = CmdParams_getValue(cmdparams,'t');
    if (tau<15 || (tau%3)!=0) { 
        cerr << "Error: tau must be 15 or greater and a multiple of 3" << endl;
        exit(-1); 
    }
    T = CmdParams_getValue(cmdparams,'T');
    Tstart = CmdParams_getValue(cmdparams,'s');
    Tend = CmdParams_getValue(cmdparams,'e');
    if (Tend<0) { Tend = T; }  // if Tend not set, then default for Tend is T
    N = CmdParams_getValue(cmdparams,'N');
    grid_spacing = CmdParams_getValue(cmdparams,'p');
    gridspacingChoice = (gridspacing_type)CmdParams_getValue(cmdparams,'g');
    cell_spacing = CmdParams_getValue(cmdparams,'c');
    cell_radius = CmdParams_getValue(cmdparams,'r');
    debug = CmdParams_getValue(cmdparams,'d');
    label = CmdParams_getValue(cmdparams,'l');
    color_incr = CmdParams_getValue(cmdparams,'i');
    one_tile = CmdParams_getValue(cmdparams,'o');
    one_tile_c0 = CmdParams_getValue(cmdparams,'0');
    one_tile_c1 = CmdParams_getValue(cmdparams,'1');
    one_tile_c2 = CmdParams_getValue(cmdparams,'2');

    // Compute the spacing between slices.
    if (grid_spacing<0) {
        switch (gridspacingChoice) {
            case normal:
                grid_spacing = cell_spacing*(N+1);
                break;
            case halfradius:
                grid_spacing = 0.5*(double)cell_radius;
                break;
        }
    }

    // Open the svg file and print the header.
    std::string filename = create_file_name();
    ofstream file(filename.c_str());
        
    // Specify file and height and width.
    SVGPrinter svg(file, cell_spacing*(N+1) + ((Tend-Tstart+1)-1)*grid_spacing,
                         (N+1)*cell_spacing);
    svg.printHeader();
    
    // Declare the array of iteration spaces.
    CellField::sSpacing = cell_spacing;
    CellField::sRadius = cell_radius;
    CellFieldArray slices(T,N,N,grid_spacing,Tstart,Tend);

    int Li=0, Ui=N, Lj=0, Uj=N;
    
    // loops over bottom left, middle, top right
    for (int c0 = -2; c0<=0; c0+=1)
      // loops horizontally?
      for (int c1 = 0; c1 <= (Uj+tau-3)/(tau-3) ; c1 += 1)
         // loops vertically?, but without skew
        for (int x = (-Ui-tau+2)/(tau-3); x<=0 ; x += 1){
          int c2 = x-c1; //skew
          // loops for time steps within a slab (slices within slabs)
          for (int c3 = 1; c3<=subset_s; c3 += 1)
      
            for (int c4 = max(max(max(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2), -Uj - tau * c2 + c3 - (tau-2)), tau * c0 - tau * c1 - tau * c2 - c3), Li); c4 <= min(min(min(tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1), -tau * c1 - tau * c2 + 2 * c3), -Lj - tau * c2 + c3), Ui - 1); c4 += 1)
        
              for (int c5 = max(max(tau * c1 - c3, Lj), -tau * c2 + c3 - c4 - (tau-1)); c5 <= min(min(Uj - 1, -tau * c2 + c3 - c4), tau * c1 - c3 + (tau-1)); c5 += 1) {
                computation(c3, c4, c5);
              }
        }
              


    // Print the SVG string out to the file.
    slices.printToSVG(svg,Tstart,Tend);
    std::cout << "Generating file " << filename << std::endl;
    
    // End of the file.
    svg.printFooter();

    return 0;
}
