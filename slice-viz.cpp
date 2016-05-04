/*!
 * \file slice-viz.cpp
 *
 * \brief Driver for visualizing 2D slices of 3D diamond
 *        or pipelined tiles.
 *
 * \date Started: 9/21/13
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2016, University of Arizona.
 * Copyright (c) 2013, Colorado State University <br>
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
#include <map>

#include "intops.h"

//==============================================
// Global parameters with their default values.
int T = 4;
int Tstart = 1;
int Tend = 4;
int N = 10;
int tau = 15;
int grid_spacing = -1;
int cell_spacing = 60;
int cell_radius = 20;
bool label = false;
bool debug = false;
int color_incr = 1;
bool one_tile = false;
int one_tile_c1 = 1;
int one_tile_c2 = 1;
int one_tile_c3 = -1;
std::map<int, std::map<int, std::map<int, int> > > colors; // NEW COLOR

typedef enum {
    pipelined_4x4x4,
    diamonds,
    diamond_prizms_6x6,
    diamond_prizms_8x8,
    diamond_prizms_12x12,
    diamond_prizms_6x6_noping
} tiling_type;
tiling_type tilingChoice = diamonds;
char tilingStr[MAXPOSSVALSTRING];
#define num_TPairs 6
static EnumStringPair TPairs[] = {{pipelined_4x4x4,"pipelined_4x4x4"},
                        {diamonds,"diamonds"},
                        {diamond_prizms_6x6,"diamond_prizms_6x6"},
                        {diamond_prizms_8x8,"diamond_prizms_8x8"},
                        {diamond_prizms_12x12,"diamond_prizms_12x12"},
                        {diamond_prizms_6x6_noping,"diamond_prizms_6x6_noping"}
                };

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
// example: pipelined-3x3x3-T4N10-c60r20
std::string create_file_name() {
    std::stringstream ss;
    ss << tilingStr;
    ss << "-T" << T << "N" << N;
    ss << "-a" << tau;
    ss << "-s" << Tstart << "e" << Tend;
    ss << "-p" << grid_spacing 
       << "c" << cell_spacing << "r" << cell_radius << "l" << label;
    ss << "i" << color_incr;
    ss << "-o" << one_tile;
    if (one_tile) { 
       ss << "." << one_tile_c1 << "." << one_tile_c2 << "." << one_tile_c3;
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

    CmdParams_describeEnumParam(cmdparams, "tiling", 't', 1,
            "specify the tiling to use",
            TPairs, num_TPairs, diamonds);

    CmdParams_describeNumParam(cmdparams,"numTimeSteps", 'T', 1,
            "number of time steps",
            1, 30, 4);

    CmdParams_describeNumParam(cmdparams,"tau", 'a', 1,
            "tile size for diamond tiles (tau)",
            3, 30, 15);

    CmdParams_describeNumParam(cmdparams,"Tstart", 's', 1,
            "start visualization at Tstart",
            1, 30, 1);
            
    CmdParams_describeNumParam(cmdparams,"Tend", 'e', 1,
            "end visualization at Tend, will default to T",
            1, 30, -1);
            
    CmdParams_describeNumParam(cmdparams,"spatialDim", 'N', 1,
            "2D data will be NxN", 
            1, 50, 10);

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

    CmdParams_describeNumParam(cmdparams,"one_tile_c1", '1', 1,
            "c1 coord for one tile being shown", 
            -10, 20, 1);

    CmdParams_describeNumParam(cmdparams,"one_tile_c2", '2', 1,
            "c2 coord for one tile being shown", 
            -10, 20, 1);

    CmdParams_describeNumParam(cmdparams,"one_tile_c1", '3', 1,
            "c2 coord for one tile being shown", 
            -10, 20, -1);

}   

// converts the tile coordinates to a string
std::string tileCoordToString(int c1, int c2, int c3) {
    std::stringstream ss;

    switch (tilingChoice) {
    
        // Diamond prizms only have 2 dimensions of tiling.
        case diamond_prizms_6x6:
        case diamond_prizms_8x8:
        case diamond_prizms_12x12:
            ss << c1 << "," << c2;
            break;
        default:
            ss << c1 << "," << c2 << "," << c3;
            break;
    }
    
    return ss.str();
}

// converts the spatial coords to a string
std::string spatialCoordToString(int i, int j) {

    std::stringstream ss;

    ss << i << "," << j;

    return ss.str();
}

// Array of colors.
std::string svgColors[] = 
//{"bisque","red","aqua","yellow","blue","green","fuchsia","lime","silver","coral","lavender","pink","powderblue","plum","palegreen"};
{"red","lavender","yellow","green","lime","aqua","purple","coral","pink","powderblue","plum","palegreen","fuchsia","teal","navy"};
//{"maroon","red","olive","yellow","green","lime","teal","aqua","navy","blue","purple","fuchsia","black","grey","silver","white"};
//{"yellow","green","aqua","navy","red","teal","fuchsia","lime","maroon","silver","olive","blue","black","purple","gray","white"};
int num_colors = 15;

// converts the tile coordinates to a string
// otherColor(list) - return color from svgColors NOT in given list
// Dictionary of strings -> colors "(coordinates)" -> "color"

std::string tileCoordToColor(int c1, int c2, int c3) {
    colors[c1][c2][c3] = colors[c1-1][c2-1][c3-1]+1;
    if (colors[c1][c2][c3] >= num_colors) {
        colors[c1][c2][c3] = 0;
    }
    // check adjacent coordinates
    while (colors[c1][c2][c3]==colors[c1-1][c2][c3] ||
        colors[c1][c2][c3]==colors[c1][c2-1][c3] ||
        colors[c1][c2][c3]==colors[c1][c2][c3-1] ||
        colors[c1][c2][c3]==colors[c1-1][c2-1][c3+1] ||
        colors[c1][c2][c3]==colors[c1-1][c2-1][c3] ||
        colors[c1][c2][c3]==colors[c1-1][c2][c3-1] ||
        colors[c1][c2][c3]==colors[c1-2][c2-1][c3] ||
        colors[c1][c2][c3]==colors[c1-2][c2-1][c3-1] ||
        colors[c1][c2][c3]==colors[c1+1][c2-1][c3-1] ||
        colors[c1][c2][c3]==colors[c1+2][c2+1][c3+1] ||
        colors[c1][c2][c3]==colors[c1-2][c2][c3-1] ||
        colors[c1][c2][c3]==colors[c1-2][c2+1][c3] ||
        colors[c1][c2][c3]==colors[c1][c2-1][c3+1] ||
        colors[c1][c2][c3]==colors[c1][c2-1][c3-1] ||
        colors[c1][c2][c3]==colors[c1+1][c2+1][c3]) {
        (colors[c1][c2][c3])++;
    }


    // We want to change the tile color if the tile coordinate
    // has changed.
    /*

    // Check that tiles aren't 
    switch (tilingChoice) {

        // Diamond prizms only have 2 dimensions of tiling.
        case diamond_prizms_6x6:
        case diamond_prizms_8x8:
        case diamond_prizms_12x12:
            if (c1!=last_c1 || c2!=last_c2) {
                last_c1 = c1;
                last_c2 = c2;
                last_c3 = c3;
                count += color_incr;
            }
            break;
        default:
            if (c1!=last_c1 || c2!=last_c2 || c3!=last_c3) {
                last_c1 = c1;
                last_c2 = c2;
                last_c3 = c3;
                count += color_incr;
            }
            break;
    }
    */
    
    return svgColors[ colors[c1][c2][c3] ];
}

// Definitions and declarations needed for diamonds-tij-skew.is
#include "eassert.h"
#include "intops.h"
//#define N 10
//#define T T
#define do_edge_pong(i,j) // nothing
#define do_edge_ping(i,j) // nothing
#define do_init_pong(i,j) // nothing
#define startclock() // nothing

int c1, c2, c3, c4, c5, c6, c7, c8;


// The calc_ping and calc_pong macros are capturing the slices,
// c1, c2, and c3 variables.
#define calc_ping(t,i,j) { \
    if (label) slices.setLabel1(t,i,j,tileCoordToString(c1,c2,c3)); \
    if (debug) { \
      cout << "c1,c2,c3 = " << c1 << ", " << c2 << ", " << c3 << "    "; \
      cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    } \
    if (!one_tile || (c1==one_tile_c1 && c2==one_tile_c2 && c3==one_tile_c3)) {\
      slices.setFill(t,i,j,tileCoordToColor(c1,c2,c3)); } }
    
#define calc_pong(t,i,j) { \
    if (label) slices.setLabel1(t,i,j,tileCoordToString(c1,c2,c3)); \
    if (debug) { \
      cout << "c1,c2,c3 = " << c1 << ", " << c2 << ", " << c3 << "    "; \
      cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    } \
    if (!one_tile || (c1==one_tile_c1 && c2==one_tile_c2 && c3==one_tile_c3)) {\
      slices.setFill(t,i,j,tileCoordToColor(c1,c2,c3)); } }

// Used for debugging problem with diamond prizms.
#define calc(t,i,j) { \
    if (label) slices.setLabel1(t,i,j,tileCoordToString(c1,c2,c2)); \
    if (debug) { \
      cout << "c1,c2 = " << c1 << ", " << c2 << "    "; \
      cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    } \
    if (!one_tile || (c1==one_tile_c1 && c2==one_tile_c2)) {\
      slices.setFill(t,i,j,tileCoordToColor(c1,c2,c2)); } }

 // if (label) slices.setLabel(t,i,j,tileCoordToString(kt,k1,k2)); \
// Taking the ping and pong out of diamonds.
#define calc_diamond(kt,k1,k2,t,i,j) { \
    if (label && (gridspacingChoice!=halfradius || Tend == t)) { \
        slices.setLabel1(t,i,j,tileCoordToString(kt,k1,k2)); \
        slices.setLabel2(t,i,j,spatialCoordToString(i,j)); \
    } \
    if (debug) { \
      cout << "kt,k1,k2 = " << kt << ", " << k1 << ", " << k2 << "    "; \
      cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
    } \
    if (!one_tile || (kt==one_tile_c1 && k1==one_tile_c2 && k2==one_tile_c3)) {\
      slices.setFill(t,i,j,tileCoordToColor(kt,k1,k2)); } }
    

int main(int argc, char ** argv) {
    // Do command-line parsing.
    CmdParams *cmdparams = CmdParams_ctor(1);
    initParams(cmdparams);
    CmdParams_parseParams(cmdparams,argc,argv);
    tilingChoice = (tiling_type)CmdParams_getValue(cmdparams,'t');
    strncpy(tilingStr, CmdParams_getString(cmdparams,'t'), MAXPOSSVALSTRING);
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
    one_tile_c1 = CmdParams_getValue(cmdparams,'1');
    one_tile_c2 = CmdParams_getValue(cmdparams,'2');
    one_tile_c3 = CmdParams_getValue(cmdparams,'3');

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
    // FIXME: This is N+3 because of the spatial dimension starting at 1, not 0,
    // and CellFieldArray is now starting at N+2, so this has to start at N+3... 
    SVGPrinter svg(file, cell_spacing*(N+3) + ((Tend-Tstart+1)-1)*grid_spacing,
                         (N+3)*cell_spacing);
    svg.printHeader();
    
    // Declare the array of iteration spaces.
    CellField::sSpacing = cell_spacing;
    CellField::sRadius = cell_radius;
    // FIXME: the N+1 is so we can start our spatial dimensions at 1.
    // The CellFieldArray handles the fact that T starts at 1, but not
    // that N starts at 1.
    CellFieldArray slices(T,N+2,N+2,grid_spacing,Tstart,Tend);

    // Have the particular tiling type mark iterations
    // in each tile.
    switch (tilingChoice) {
    
        case pipelined_4x4x4:
            #include "pipelined-4x4x4.is"
            break;
        case diamonds:
            {
            int kt, k1, k2, t, i, j;
            int Li=1;
            int Lj=1;
            int Ui=N;
            int Uj=N;
            // Copied from ICS 2014 paper.
            // Loop over tile wavefronts.
            for (kt=ceild(3,tau)-3; kt<=floord(3*T,tau); kt++) {
              // The next two loops iterate within a tile wavefront.
              int k1_lb = ceild(3*Lj+2+(kt-2)*tau,tau*3);
              int k1_ub = floord(3*Uj+(kt+2)*tau,tau*3);
              //Loops over tile coordinates within a parallel tile wavefront.
              for (k1 = k1_lb; k1 <= k1_ub; k1++) {
                int k2_lb = floord((2*kt-2)*tau-3*Ui+2,tau*3)-k1;
                int k2_ub = floord((2+2*kt)*tau-3*Li-2,tau*3)-k1;
                for (k2 = k2_lb; k2 <= k2_ub; k2++) {
                  // Loop over time within a tile.
                  for (t = max(1, floord(kt*tau-1, 3)); 
                       t < min(T+1, tau + floord(kt*tau, 3)); t++) {
                    // Loops over the spatial dimensions within each tile.
                    for (i = max(Li,max((kt-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2));
                         i <= min(Ui,min((1+kt-k1-k2)*tau-t-1, 
                                         2*t-(k1+k2)*tau)); i++) {
                      for (j = max(Lj,max(tau*k1-t, t-i-(1+k2)*tau+1));
                           j <= min(Uj,min((1+k1)*tau-t-1, t-i-k2*tau)); j++) {
                        calc_diamond(kt,k1,k2,t,i,j);
                      }
                    }
                  }
                }
              }
            }
            }
            break;
        case diamond_prizms_6x6:
            #include "diamond-prizms-skew-6x6.is"
            break;
        case diamond_prizms_8x8:
            #include "diamond-prizms-skew-8x8.is"
            break;
        case diamond_prizms_12x12:
            #include "diamond-prizms-skew-12x12.is"
            break;
        case diamond_prizms_6x6_noping:
            #include "diamond-prizms-skew-noping-6x6.is"
            break;
        default:
            std::cerr << "ERROR: slice-viz: unknown tiling type" << std::endl;
    }  

    // Print the array of iteration slices out to the file.
    slices.printToSVG(svg,Tstart,Tend);
    std::cout << "Generating file " << filename << std::endl;
    
    // End of the file.
    svg.printFooter();

    return 0;
}
