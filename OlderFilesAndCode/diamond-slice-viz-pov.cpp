/*!
 * \file diamond-slice-viz-pov.cpp
 *
 * \brief Driver for visualizing 3D diamond slabs in povray.
 *
 * Goal is to eventually incorporate back into the diamond-slice-viz.cpp
 * code, but first going to try it out here.
 *
 * \date Started: 12/19/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */
#include "CmdParams.h"
#include "ColorInfo.hpp"

#include "Printer.hpp"
#include "PrinterSVG.hpp"
#include "PrinterPOV.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


//==============================================
// Global parameters with their default values.
int tau = 15;  // tile size is TAUxTAUxTAU
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
    
    return ss.str();
}

void initParams(CmdParams * cmdparams)
/*--------------------------------------------------------------*//*!
  Uses a CmdParams object to describe all of the command line
  parameters.

  \author  Michelle Strout 9/21/13
*//*--------------------------------------------------------------*/
{

/*    CmdParams_describeNumParam(cmdparams,"tau", 't', 1,
            "size of the tile, tau x tau x tau, must be multiple of 3 and >=15",
            15, 129, 15); // arbitrary upper bound at this point
*/
    CmdParams_describeNumParam(cmdparams,"tau", 't', 1,
            "size of the tile, tau x tau x tau",
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
            20, 75, 20);

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
// MMS, 2/16/15, making things really simple
// one color per wave of tiles
std::string tileCoordToColor(int c0, int c1, int c2) {
    if (c0==-2) return "red";
    else if (c0==-1) return "yellow";
    else if (c0==0) return "green";
    else return "white";
}
/*
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
*/

// Definitions and declarations needed for diamonds-tij-skew.is
#include "eassert.h"
#include "intops.h"

int c0, c1, c2, c3, c4, c5, c6, c7, c8;

// The computation macro prints the svg for each iteration point.
// Capturing the c0, c1, and c2 variables in the generated code, which
// should be the tile coordinates.
#define computation(c0,c1,c2,t,i,j) { \
    std::string colorstr = tileCoordToColor(c0,c1,c2); \
    if (debug) { \
      std::cout << "c0,c1,c2 = " << c0 << ", " << c1 << ", " << c2 << "    "; \
      std::cout << "t,i,j = " << t << ", " << i << ", " << j << std::endl; \
      std::cout << "color = " << colorstr << std::endl; \
    } \
    pov.printCircle(t,i,j, colorTable.getR(colorstr), \
                           colorTable.getG(colorstr), \
                           colorTable.getB(colorstr) ); \
    std::cout << "{ pos: " << t << " " << i << " " << j << "; color: "; \
    std::cout << colorTable.getHexCode(colorstr) << "}" << std::endl; \
  }
    //if (label) slices.setLabel(t,i,j,tileCoordToString(c0,c1,c2)); \
    if (!one_tile || (c0==one_tile_c0 && c1==one_tile_c1 && c2==one_tile_c2)) {\
      slices.setFill(t,i,j,tileCoordToColor(c0,c1,c2)); } }
//Can use for SVG: rgb(205,133,63)
    
int main(int argc, char ** argv) {

    // Do command-line parsing.
    CmdParams *cmdparams = CmdParams_ctor(1);
    initParams(cmdparams);
    CmdParams_parseParams(cmdparams,argc,argv);
    tau = CmdParams_getValue(cmdparams,'t');
/*    if (tau<15 || (tau%3)!=0) { 
        cerr << "Error: tau must be 15 or greater and a multiple of 3" << endl;
        exit(-1); 
    }
*/
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

    //========================================
    ColorInfo colorTable("svg-colors-rgb.txt");


    //========================================
    // Open the svg file and print the header.
    /*
    std::string filename = create_file_name() + ".svg";
    ofstream svgfile(filename.c_str());
        
    // Specify file and height and width.
    SVGPrinter svg( svgfile, 
                    cell_spacing*(N+1)+((Tend-Tstart+1)-1)*grid_spacing,
                    (N+1)*cell_spacing,
                    cell_radius,
                    grid_spacing);
    svg.printHeader();
    */
    //========================================
    // Open the povray file and print the header.
    std::string povfilename = create_file_name() + ".pov";
    ofstream povfile(povfilename.c_str());
    PrinterPOV pov(povfile);
    std::cout << "Generating file " << povfilename << std::endl;
    pov.printHeader();
    
    //========================================
    int k1, k2, t, i, j;
    int Li=0, Ui=N, Lj=0, Uj=N;
    int tau_times_3 = 3*tau;

    // loops over bottom left, middle, top right
    for (int thyme = -2; thyme<=0; thyme+=1){
    
        // MMS, 2/16/15, copied in code from
        // Jacobi2D-DiamondByHandParam-OMP.test.c
        // The next two loops iterate within a tile wavefront.
        int k1_lb = floord(3*Lj+2+(thyme-2)*tau,tau_times_3);
        int k1_ub = floord(3*Uj+(thyme+2)*tau-2,tau_times_3);

        // These bounds have been unskewed by adding in k1.
        // That also turns them into a bounding box.
        int k2_lb = floord((2*thyme-2)*tau-3*Ui+2,tau_times_3);
        int k2_ub = floord((2+2*thyme)*tau-2-3*Li,tau_times_3);
    
        for (k1=k1_lb; k1<=k1_ub; k1++) {
          for (int x=k2_lb; x<=k2_ub; x++) {
            k2 = x-k1; // skew back
            // Don't have to check bounds based on k1 because the skew
            // was the only dependence of k2 bounds on k1.

            // Loop over time within a tile.
            for (t=max(1,floord(thyme*tau,3)); 
                 t<= min(T,floord((3+thyme)*tau-3,3)); t++) {
              // Loops over spatial dimensions within tile.
              for (i=max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2));
                   i<=min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)); i++) {
                for (j=max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1));
                     j<=min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)); j++) {
                  computation( thyme,k1,k2, t, i, j);
                } // for j
              } // for i
            } // for t
          } // for k2
        } // for k1
    } // for thyme  

    // End of the file.
    //svg.printFooter();
    pov.printFooter();

    return 0;
}

// MMS, 2/16/15, commented below out on plane
/*    // loops over bottom left, middle, top right
    for (int c0 = -2; c0<=0; c0+=1){
      // loops horizontally?
      for (int c1 = 0; c1 <= (Uj+tau-3)/(tau-3) ; c1 += 1){
         // loops vertically?, but without skew
        for (int x = (-Ui-tau+2)/(tau-3); x<=0 ; x += 1){
          int c2 = x-c1; //skew
          // loops for time steps within a slab (slices within slabs)
          for (int c3 = 1; c3<=subset_s; c3 += 1){
      
            for (int c4 = max(max(max(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2), -Uj - tau * c2 + c3 - (tau-2)), tau * c0 - tau * c1 - tau * c2 - c3), Li); c4 <= min(min(min(tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1), -tau * c1 - tau * c2 + 2 * c3), -Lj - tau * c2 + c3), Ui - 1); c4 += 1){
        
              for (int c5 = max(max(tau * c1 - c3, Lj), -tau * c2 + c3 - c4 - (tau-1)); c5 <= min(min(Uj - 1, -tau * c2 + c3 - c4), tau * c1 - c3 + (tau-1)); c5 += 1){
                computation(c3, c4, c5);
              }
            }
          }
        }
      }
    }
*/        
