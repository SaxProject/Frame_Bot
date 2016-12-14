/*
 * Date: 16/08/2016
 * Vers:   1.0.0
 *
 * Description:  
 *							This file is a simple show of the final result of robot. 
 *							Before use this file, you need change in some files and comment the module call.
 *              list of files to change is:
 *
 *							       frame_bot.scad
 *							       torse.scad
 *							       arm.scad
 *							       wheel_front.scad
 *							       wheel_dc_axial.scad
 *						
 *						Head is a simple version of MS Zaku created by me. 
 *            This head is a work in progress. For this simulation 
 *            i have put only a stl file. Source code is in separate folder.
 *
 *					
 * Note:   - first release
 *				
 *          
 *         
 * Originally by Matteo Saccaro, 2016.
 * Dual licenced under Creative Commons Attribution-Share Alike 3.0 and LGPL2 or later
 *
 ***************************************** */

include <lib/9g_servo.scad>
include <lib/dc_axial.scad>
include <frame_bot.scad>
include <torse.scad>
include <arm.scad>

include <wheel_front.scad>
include <wheel_dc_axial.scad>

rotate([0,0,180]) base_frame();
rotate([-90,0,0]) translate([0,-47.5,-30])torse();

translate([31,-28,82.5]) rotate([0,90,0]) 9g_motor();
translate([-31,-28,82.5]) rotate([-90,90,90]) 9g_motor();

translate([0,-25,0]) rotate([0,90,0]) cylinder(r=3, h=90,$fn=100,center=true);

translate([50,-28,52.5]) rotate([90,90,0]) arm_module(0);
translate([-50,-28,52.5]) rotate([90,90,0]) arm_module(1);


//translate([20,7,0]) rotate([0,0,180]) dc_axial();
translate([25,35,0]) rotate([0,0,180]) dc_axial();

translate([-25,35,0]) rotate([0,0,180]) dc_axial();

translate([-55,15,0]) rotate([0,90,0]) wheel();
translate([55,20,0]) rotate([0,-90,0]) wheel();

translate([-55,-25,0]) rotate([0,90,0]) wheel_front();
translate([55,-25,0]) rotate([0,-90,0]) wheel_front();

translate([-20,-15,100]) import("lib_stl/char_head.stl",center);