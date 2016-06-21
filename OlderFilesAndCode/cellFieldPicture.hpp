/******************************************************************************
 * GRIDGEN: Grid Generating Compiler
 * By: Andy Stone (aistone@gmail.com)
 * (C) Copyright 2011 Colorado State University
 *****************************************************************************/
#ifndef CELLFIELDPICTURE_HPP_
#define CELLFIELDPICTURE_HPP_

#include <string>
using namespace std;

/**
 * Outputs an array of circles (cells).  Each circle may filled with a
 * specified color and be labeled with text.  A box may optionally be placed
 * behind a region of cells.
 *
 * A list of valid colors for stroke and fill properties is available
 * at: <http://www.december.com/html/spec/colorsvg.html>
 */
class CellFieldPicture {
  public:
    CellFieldPicture(int width, int height);

    // File I/O
    void saveToFile(string filename);

    // Modifiers
    void setLabel(int x, int y, string label);
    void setStroke(int x, int y, string stroke);
    void setFill(int x, int y, string fill);
    void setBox(int x1, int y1, int x2, int y2);

    // Accessors
    string getLabel(int x, int y);
    string getStroke(int x, int y);
    string getFill(int x, int y);


  private:
    int mWidth; int mHeight;
    string **mLabel;
    string **mStroke;
    string **mFill;

    // A box may be placed behind some set of the cells
    int mBoxX1, mBoxY1, mBoxX2, mBoxY2;

    // The following constants control the spacing between cells:
//    static const int mSpacing = 100;
//    static const int mRadius  = 45;
    static const int mSpacing = 50;
    static const int mRadius  = 22;

    static const int NO_BOX = -1;
};

#endif
