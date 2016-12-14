/* ****************
 *
 * Date: 16/08/2016
 * Vers:   1.0
 *
 * Description:  
 *             this file is a fake of dc motor. this need to generate hole in the base
 *					
 *          ATTENTION: this module is 4mm more large compared to original
 *
 * Note:   - first release
 *    		 - Added 4mm for create more space to place a motor and wire.      
 *         
 * Originally by Matteo Saccaro, 2016.
 * Attribution-Share Alike 3.0
 *
 * **************** */

spess = 18.8+4;  //4mm for more space
len = 64.2;
altezz = 22.5;

// assi
asse_len = 9.5;
asse_spess = 3.6;
asse_altezz = 1.9; // da verificare

module dc_axial(){
difference(){
	cube([spess,len,altezz],center = true);

}

translate([0,0.3,-8.75]) rotate([0,90,0]) cylinder(r=1.5,h=spess+25,$fn=100,center=true);
translate([0,0.3,8.75]) rotate([0,90,0]) cylinder(r=1.5,h=spess+25,$fn=100,center=true);

translate([(altezz/2),20.9,0]) cube([asse_len,asse_spess,asse_altezz],center = true);
translate([-(altezz/2),20.9,0]) cube([asse_len,asse_spess,asse_altezz],center = true);

}

//dc_axial();



