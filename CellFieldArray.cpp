#include "CellFieldArray.hpp"

#include <fstream>
#include <assert.h>
using namespace std;

CellFieldArray::CellFieldArray(int num_fields, int width, int height, 
                               int slice_spacing, int Tstart, int Tend) {
    // create an array for all fields
    mNumFields = num_fields;
    mArray = new CellField*[mNumFields];

    // Shift the y start for each new cell field slice_spacing away from last.
    // But only do the shifting for fields in [Tstart,Tend]
    int count = 0;
    for(int i = 0; i < mNumFields; i++) {
        mArray[i] = new CellField(0, count*slice_spacing, width, height);
        int t = i+1;
        if (Tstart<=t && t<=Tend) {
            count++;
        }
    }
}

void CellFieldArray::printToSVG(SVGPrinter& svg_printer, int Tstart, int Tend) {
    assert(Tstart>=1 && Tstart<=mNumFields && Tend>=1 && Tend<=mNumFields);
    // -1 because mArray is indexed starting at 0 and t indices start at 1
    for(int i = (Tstart-1); i < Tend; i++) {
        mArray[i]->printToSVG( svg_printer );
    }
}

void CellFieldArray::setLabel1(int t, int x, int y, string label) {
   // printf("t: %d\nmnumfields: %d", t, mNumFields);
    assert(t>=1 && t<=mNumFields);
    mArray[t-1]->setLabel1(x,y,label);
}

void CellFieldArray::setLabel2(int t, int x, int y, string label) {
   // printf("t: %d\nmnumfields: %d", t, mNumFields);
    assert(t>=1 && t<=mNumFields);
    mArray[t-1]->setLabel2(x,y,label);
}
void CellFieldArray::setStroke(int t, int x, int y, string stroke) {
    mArray[t-1]->setStroke(x,y,stroke);
}

void CellFieldArray::setFill(int t, int x, int y, string fill) {
    mArray[t-1]->setFill(x,y,fill);
}

void CellFieldArray::setBox(int t, int x1, int y1, int x2, int y2) {
    mArray[t-1]->setBox(x1,y1,x2,y2);
}

string CellFieldArray::getLabel1(int t, int x, int y) {
    return mArray[t-1]->getLabel1(x,y);
}

string CellFieldArray::getStroke(int t, int x, int y) {
    return mArray[t-1]->getStroke(x,y);
}

string CellFieldArray::getFill(int t, int x, int y) {
    return mArray[t-1]->getFill(x,y);
}

