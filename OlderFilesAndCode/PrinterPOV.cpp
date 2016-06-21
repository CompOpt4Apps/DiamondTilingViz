#include "PrinterPOV.hpp"

PrinterPOV::PrinterPOV(ostream &out) :
    mOut(out)
{
}

void PrinterPOV::printHeader() {
    mOut  << "#macro my_sphere(xPos, yPos, zPos)\n"
    "    sphere { <xPos,yPos,zPos>, 0.2 texture { pigment {color <0, 0, 1>} }\n" "finish {phong 0.9} }\n"
    "#end\n"
    "\n"
    "camera {\n"
    "   orthographic\n"
    "//   location <3, 6, -10>\n"
    "   location <3, 6, -10>\n"
    "   look_at <0, 4, 0>\n"
    "}\n"
    "\n"
    "light_source { <-1, -1, -1> color <1, 1, 1>}\n"
    "\n"
    "sky_sphere { pigment {color <1, 1, 1>} }\n"
    "\n"
    "cylinder {\n"
    " <0, 20, 0>\n"
    " <0, -20, 0>\n"
    " 0.05\n"
    " texture {pigment {color <0.66, 0.625, 0.66>}}\n"
    " finish {ambient 0.5}\n"
    "}\n"
    "\n"
    "cylinder {\n"
    " <20, 0, 0>\n"
    " <-20, 0, 0>\n"
    " 0.05\n"
    " texture {pigment {color <1, 0, 0>}}\n"
    " finish {ambient 0.5}\n"
    "}\n"
    "\n"
    "cylinder {\n"
    " <0, 0, 20>\n"
    " <0, 0, -20>\n"
    " 0.05\n"
    " texture {pigment {color <1, 0, 0>}}\n"
    " finish {ambient 0.5}\n"
    "}\n";
    
}

void PrinterPOV::printFooter() {
   mOut << "// End of the povray file" << endl;
}

void PrinterPOV::printCircle(int t, int x, int y, 
                             int red, int green, int blue) {
    
    double red_percent = ((double)red)/(255.0);
    double green_percent = ((double)green)/(255.0);
    double blue_percent = ((double)blue)/(255.0);

    //  sphere { <xPos, yPos, zPos>, 0.2 texture 
    //  { pigment {color <0, 0, 1>} } finish {phong 0.9} }
    mOut << " sphere { <" << x << ", " << t << ", " << y << ">, "
            "0.2 texture { pigment {"
            "color <" << red_percent << ", " 
                      << green_percent << ", " 
                      << blue_percent << ">} } finish {phong 0.9} }" 
                      << std::endl;
                      
}

