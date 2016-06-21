#macro my_sphere(xPos, yPos, zPos) sphere { <xPos, yPos, zPos>, 0.2 texture { pigment {color <0, 0, 1>} } finish {phong 0.9} }
#end


#declare I = 0;
#while (I <= 6)
  #declare K = 0;
  #while (K <= 6)
    #declare J = 0;
    #while (J <= (6 - I - K) / 2)
      my_sphere(I, 3 * J, K)
    #declare J = J + 1;
    #end
  #declare K = K + 1;
  #end
#declare I = I + 1;
#end

my_sphere(0, 0, 0)

cylinder {
 <0, 20, 0>
 <0, -20, 0>
 0.1
 texture {pigment {color <1, 0, 0>}}
 finish {ambient 0.5}
}

cylinder {
 <20, 0, 0>
 <-20, 0, 0>
 0.1
 texture {pigment {color <1, 0, 0>}}
 finish {ambient 0.5}
}

cylinder {
 <0, 0, 20>
 <0, 0, -20>
 0.1
 texture {pigment {color <1, 0, 0>}}
 finish {ambient 0.5}
}

camera {
   orthographic
   location <3, 6, -10>
   look_at <0, 4, 0>
}

light_source { <-1, -1, -1> color <1, 1, 1>}

sky_sphere { pigment {color <1, 1, 1>} }

