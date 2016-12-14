/*
 * Date: 16/08/2016
 * Vers:   1.2.1
 *
 * Description:  
 *							this file is the base of robot. 
 *							In this support you mount dc motor, 
 *							vertical frame and front wheel axis.
 *
 *
 * Note:   - first release
 *         - insert motor's fake modified. This fake 
 *           is more large 4 mm. Now the slot is more large to insert the motor.
 *         - inserted 2 mm to adjust the fake center
 *          
 *         
 * Originally by Matteo Saccaro, 2016.
 * Dual licenced under Creative Commons Attribution-Share Alike 3.0 and LGPL2 or later
 *
 ***************************************** */

include <lib/dc_axial_fake.scad>

spessore = 4;
base_base = 70;
base_profonda = 80;  //73;
base_altezza = 25;

/// coordinate per ruote
altezz_foro_ruote = -((base_altezza/2)-11); 

y_ruote_ant = (base_profonda/2)-13;



module base_frame(){
difference(){
  cube([(base_base+spessore),(base_profonda+spessore),(base_altezza+spessore)],center=true);
  translate([0,0,spessore-1]) cube([base_base,base_profonda,(base_altezza+spessore)],center=true);

// foro ruote frontale   n.b. 11.5
translate([0,y_ruote_ant,altezz_foro_ruote]) rotate([0,90,0]) cylinder(r=3, h=80,$fn=100,center=true);

// foro ruote posteriore
translate([0,-(base_profonda/2)+26,altezz_foro_ruote]) rotate([0,90,0]) cylinder(r=6, h=80,$fn=100,center=true);

// fori frontali
translate([0,(base_profonda/2),0]) rotate([90,90,0]) cylinder(r=1.5, h=6,$fn=100,center=true);

translate([(base_base/4),(base_profonda/2),0]) rotate([90,90,0]) cylinder(r=1.5, h=6,$fn=100,center=true);

translate([-(base_base/4),(base_profonda/2),0]) rotate([90,90,0]) cylinder(r=1.5, h=6,$fn=100,center=true);

// fori per supporto motori parte sinistra
translate([-(base_base/2)+15,-(base_profonda/2)+5,-25/2]) cylinder(r=1.5, h=6,$fn=100,center=true);

translate([-(base_base/2)+15,-(base_profonda/2)+5+(15.99*2),-25/2]) cylinder(r=1.5, h=6,$fn=100,center=true);


// fori per supporto motori parte sinistra
translate([(base_base/2)-15,-(base_profonda/2)+5,-25/2]) cylinder(r=1.5, h=6,$fn=100,center=true);

translate([(base_base/2)-15,-(base_profonda/2)+5+(15.99*2),-25/2]) cylinder(r=1.5, h=6,$fn=100,center=true);

// attenzione i fake sono stati allargati per consentire di inserire più comodomaente i motori e far passare i cavi
translate([-23.6,-(base_profonda/2)+5,-0.2]) dc_axial();  
translate([23.6,-(base_profonda/2)+5,-0.2]) dc_axial();

}
}

base_frame();



