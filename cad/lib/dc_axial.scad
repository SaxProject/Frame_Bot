/* ****************
 *
 * Date: 16/08/2016
 * Vers:   1.0
 *
 * Description:  
 *             this file is a sample of dc motor.
 *					
 *
 * Note:   - first release
 *
 *         
 * Originally by Matteo Saccaro, 2016.
 * Dual licenced under Creative Commons Attribution-Share Alike 3.0 and LGPL2 or later
 *
 * **************** */


spess = 18.8;
len = 64.2;
altezz = 22.5;

// assi
asse_len = 9.5;
asse_spess = 3.6;
asse_altezz = 1.9; // da verificare

module dc_axial(){
difference(){
	cube([spess,len,altezz],center = true);
	translate([0,0.3,-8.75]) rotate([0,90,0]) cylinder(r=1.5,h=spess+25,$fn=100,center=true);
	translate([0,0.3,8.75]) rotate([0,90,0]) cylinder(r=1.5,h=spess+25,$fn=100,center=true);

}

translate([(altezz/2),20.9,0]) cube([asse_len,asse_spess,asse_altezz],center = true);

translate([-(altezz/2),20.9,0]) cube([asse_len,asse_spess,asse_altezz],center = true);

}

//dc_axial();



