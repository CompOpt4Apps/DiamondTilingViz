/* CellField.cpp
   
   September 2013: modified Andy's grid visualization CellFieldPicture
   class so that can have an array of cell fields.  This class is now
   just a single CellField and not a full picture.
   
*/
/******************************************************************************
 * GRIDGEN: Grid Generating Compiler
 * By: Andy Stone (aistone@gmail.com)
 * (C) Copyright 2011 Colorado State University
 *****************************************************************************/
#ifndef CELLFIELD_HPP_
#define CELLFIELD_HPP_

#include <string>
#include "svgprinter.hpp"
using namespace std;

/**
 * Outputs an array of circles (cells) at a particular x and y offset.
 * All x and y positions are relative to the x and y start positions.
 * Those start positions are pixel start positions.  The indices x and y
 * must also start at 0 and be less than with width or height respectively.
 * Each circle may filled with a specified color and be labeled with text.  
 * A box may optionally be placed behind a region of cells.
 *
 * A list of valid colors for stroke and fill properties is available
 * at: <http://www.december.com/html/spec/colorsvg.html>
 */
class CellField {
  public:
    CellField(int x_start, int y_start, int width, int height);

    // File I/O for just this CellField.  SVG file headers and footers
    // must be handled by another file.
    void printToSVG(SVGPrinter& svg_printer);

    // Modifiers
    void setLabel1(int x, int y, string label);
    void setLabel2(int x, int y, string label);
    void setStroke(int x, int y, string stroke);
    void setFill(int x, int y, string fill);
    void setBox(int x1, int y1, int x2, int y2);

    // Accessors
    string getLabel1(int x, int y);
    string getLabel2(int x, int y);
    string getStroke(int x, int y);
    string getFill(int x, int y);

    // The following constants control the spacing between cells:
    static int sSpacing;
    static int sRadius;


  private:
    int mXStart, mYStart;
    int mWidth; int mHeight;
    string **mLabel1;
    string **mLabel2;
    string **mStroke;
    string **mFill;

    // A box may be placed behind some set of the cells
    int mBoxX1, mBoxY1, mBoxX2, mBoxY2;

    static const int NO_BOX = -1;
};

#endif
