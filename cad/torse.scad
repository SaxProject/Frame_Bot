/*
 * Date: 16/08/2016
 * Vers:   1.1.0
 *
 * Description:  
 *							this file is the vertical frame of robot. 
 *							In this support you mount servo motors for arms, 
 *              head of robot and elettronic board.
 *
 *
 * Note:   - first release
 *         - insert servo motors 9g dimension. 
 *           But i'm mounting a 5g. In this case use only one screew. 
 *         - work in progress. the places of hole for mount electronic, is not definitive.
 *
 *  resource:
 * 					https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/DXF_Extrusion
 * 					https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Using_the_2D_Subsystem
 * 					https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/2D_to_3D_Extrusion
 *
 *
 * Originally by Matteo Saccaro, 2016.
 * Dual licenced under Creative Commons Attribution-Share Alike 3.0 and LGPL2 or later
 * 
 * */


include <lib/9g_servo.scad>

altezza_torso = 110;  //130;
r_fianco = 25;
h_torso = 20;  // 13

module torse(){
module my_torse(){
	difference (){
		square([70,altezza_torso],center=true);
		translate([-38,10,0]) circle(r=r_fianco,$fn=11);
		translate([38,10,0]) circle(r=r_fianco,$fn=11);
	}
}


module my_torse_internal(){
	difference (){
		square([65,altezza_torso-5],center=true);
		translate([-38,0,0]) circle(r=r_fianco,$fn=11);
		translate([38,0,0]) circle(r=r_fianco,$fn=11);
	}
}

difference (){
	linear_extrude(height = h_torso,center = true, convexity = 10) my_torse();
	translate([0,0,5]) linear_extrude(height =h_torso,center = true, convexity = 10) my_torse_internal();
	translate([-40.5,(altezza_torso/2)-25,-5]) cube([81,30,15]);

translate([-35,-35,1.4]) rotate([90,0,-90]) 9g_motor();
translate([35,-35,1.4]) rotate([-90,0,-90]) 9g_motor();

translate([-35,-49,1.8])rotate([90,0,-90]) cylinder(r=1.5,h=15,$fn=100,center=true);
translate([35,-49,1.8])rotate([90,0,-90]) cylinder(r=1.5,h=15,$fn=100,center=true);

translate([-35,-21,1.8])rotate([90,0,-90]) cylinder(r=1.5,h=45,$fn=100,center=true);
translate([35,-21,1.8])rotate([90,0,-90]) cylinder(r=1.5,h=45,$fn=100,center=true);

translate([0,((altezza_torso/2)-7.5),-5]) cylinder(r=1.5,h=10,$fn=100,center=true);
translate([(70/4),((altezza_torso/2)-7.5),-5]) cylinder(r=1.5,h=10,$fn=100,center=true);
translate([-(70/4),((altezza_torso/2)-7.5),-5]) cylinder(r=1.5,h=10,$fn=100,center=true);

translate([0,-(altezza_torso/2),0]) rotate([90,0,0]) cylinder(r=1.5,h=10,$fn=100,center=true);    
}


translate([0,-((altezza_torso/2)-(2.5+20)),-(h_torso/2)+7]) difference(){
    cylinder(r=3.5,h=5,$fn=100,center=true);
    cylinder(r=1.5,h=5,$fn=100,center=true);
}
translate([0,-((altezza_torso/2)-(2.5+20+60)),-(h_torso/2)+7])  difference(){
    cylinder(r=3.5,h=5,$fn=100,center=true);
    cylinder(r=1.5,h=5,$fn=100,center=true);
}



}


torse();

