/*
 * Date: 16/08/2016
 * Vers:   1.0.0
 *
 * Description:  
 *							this file is the arm (DX and SX) of robot. 
 *							When you call this module pass with argument 1 to generate a right arm.
 *              If you  pass with argument 0 to generate a left arm.
 *
 *
 * Note:   - work in progress.
 *         - TO DO: insert holes correct (diameter and position) to attach a servo.
 *
 *          
 *         
 * Originally by Matteo Saccaro, 2016.
 *
 ***************************************** */

avan_bracc = 40;
lbraccio = 40;
spesso = 5;

alt_br = spesso *2;

/// var per centratura fori 
distance_to_orig = -(lbraccio-8);
resize_y = 20;

/////
r_hole_extern = 1;
r_hole2_extern = 1.8;
r_hole3_extern = 1.75;

module arm_module(side=0){


difference(){    
 translate([-(lbraccio/2),0,0]) cube([lbraccio,spesso,alt_br],center=true);
 rotate([90,0,0]) translate([distance_to_orig,0,0]) cylinder(r=r_hole_extern,h=20,$fn=100,center=true);
 rotate([90,0,0]) translate([distance_to_orig+8,0,0]) cylinder(r=r_hole2_extern,h=20,$fn=100,center=true);
 rotate([90,0,0]) translate([distance_to_orig+16,0,0]) cylinder(r=r_hole_extern,h=20,$fn=100,center=true);    
 }
 
  translate([(avan_bracc/2)-10,0,10]) rotate([0,-35,0]) cube([avan_bracc,spesso,alt_br],center=true);
 



difference(){
translate([(avan_bracc/2)+8,0,23]) rotate([0,-35,0]) cylinder(r=2.5,h=alt_br-2,$fn=100,center=true);
translate([(avan_bracc/2)+8,0,23]) rotate([0,-35,0]) cylinder(r=1.5,h=alt_br+4,$fn=100,center=true);
}

difference(){
if (side == 0){
translate([-(lbraccio/2),(spesso/2),0]) rotate([90,0,0]) resize([0,resize_y,0]) linear_extrude(height = 5,center = true, convexity = 10) circle(20,$fn=5);

 translate([(avan_bracc/2)-10,spesso,10]) rotate([0,-35,0]) cube([avan_bracc+8,spesso,alt_br+5],center=true);
} else {
 translate([-(lbraccio/2),-(spesso/2),0]) rotate([90,0,0]) resize([0,resize_y,0]) linear_extrude(height = 5,center = true, convexity = 10) circle(20,$fn=5);
translate([(avan_bracc/2)-10,-spesso,10]) rotate([0,-35,0]) cube([avan_bracc+8,spesso,alt_br+5],center=true);
}

rotate([90,0,0]) translate([distance_to_orig,0,spesso-10]) cylinder(r=r_hole3_extern,h=30,$fn=100,center=true);
rotate([90,0,0]) translate([distance_to_orig+8,0,spesso-10]) cylinder(r=r_hole2_extern,h=30,$fn=100,center=true);
rotate([90,0,0]) translate([distance_to_orig+16,0,spesso-10]) cylinder(r=r_hole3_extern,h=30,$fn=100,center=true);
}
}



arm_module(0);

