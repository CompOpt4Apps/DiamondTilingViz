/* CellFieldArray.cpp
   
   September 2013: modified Andy's grid visualization CellFieldPicture
   class so that can have an array of cell fields.  This class is now
   an array of CellFields.  Does not print SVG header and footer. 
*/
/******************************************************************************
 * GRIDGEN: Grid Generating Compiler
 * By: Andy Stone (aistone@gmail.com)
 * (C) Copyright 2011 Colorado State University
 *****************************************************************************/
#ifndef CELLFIELDARRAY_HPP_
#define CELLFIELDARRAY_HPP_

#include <string>
#include "svgprinter.hpp"
#include "CellField.hpp"
using namespace std;

/**
 * Outputs an array of
 * fields of circles (cells) at a particular x and y offset.
 * All x and y positions are relative to the x and y start positions.
 * Each circle may filled with a specified color and be labeled with text.  
 * A box may optionally be placed behind a region of cells.
 *
 * A list of valid colors for stroke and fill properties is available
 * at: <http://www.december.com/html/spec/colorsvg.html>
 *
 * t indices start at 1, and x and y start at 0.
 */
class CellFieldArray {
  public:
    CellFieldArray(int num_fields, int width, int height, 
                   int slice_spacing, int Tstart, int Tend);

    // File I/O for just this CellFieldArray.  SVG file headers and footers
    // must be handled by another file.
    // Will print Tstart through Tend fields.
    void printToSVG(SVGPrinter& svg_printer, int Tstart, int Tend);

    // Modifiers
    void setLabel1(int t, int x, int y, string label);
    void setLabel2(int t, int x, int y, string label);
    void setStroke(int t, int x, int y, string stroke);
    void setFill(int t, int x, int y, string fill);
    void setBox(int t, int x1, int y1, int x2, int y2);

    // Accessors
    string getLabel1(int t, int x, int y);
    string getStroke(int t, int x, int y);
    string getFill(int t, int x, int y);

  private:
    // Implementing the CellFieldArray as an array of CellField.
    int mNumFields;
    CellField **mArray;

};

#endif
