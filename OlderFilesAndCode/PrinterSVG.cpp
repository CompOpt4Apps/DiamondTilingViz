#include "PrinterSVG.hpp"

PrinterSVG::PrinterSVG(ostream &out, int height, int width, 
                       int radius, int spacing) :
    mOut(out), mHeight(height), mWidth(width), 
    mCellRadius(radius), mCellSpacing(spacing)
{
}

void PrinterSVG::printHeader() {
    mOut << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
    mOut << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
    mOut << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
    mOut << "" << endl;
    mOut << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";
    mOut << "height=\"" << mHeight << "px\" ";
    mOut << "width=\"" << mWidth << "px\">" << endl;
}

void PrinterSVG::printFooter() {
   mOut << "</svg>" << endl;
}

void PrinterSVG::printCircle(int t, int x, int y, int red, int green, int blue){
    mOut << "<circle cx=\"" << x << "\" cy=\"" << y 
         << "\" r=\"" << mCellRadius << "\" "
         << "stroke=black fill=\"rgb(" << red << ", " << green << ", " 
         << blue << ")\" />" << endl;
}

/*
void PrinterSVG::printCircle(int x, int y, int r, string stroke, string fill) {
    mOut << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << r << "\" "
         << "stroke=\"" << stroke << "\" fill=\"" << fill << "\" />" << endl;
}
*/

void PrinterSVG::printCenteredText(int t, int x, int y, string text) {
    mOut << "<text x=\"" << x << "\" y=\"" << y
         << "\" alignment-baseline=\"central\" "
         << "text-anchor=\"middle\">" << endl;
    mOut << text << endl;
    mOut << "</text>" << endl;
}

/*
void PrinterSVG::printRectangle(
    int x, int y, int w, int h, string stroke, string fill)
{
    mOut << "<rect "
         << "x=\"" << x << "\" "
         << "y=\"" << y << "\" "
         << "width=\"" << w << "\" "
         << "height=\"" << h << "\" "
         << "fill=\"" << fill << "\" "
         << "stroke=\"" << stroke << "\" />" << endl;
}
*/