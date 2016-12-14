/*
 * Date: 16/08/2016
 * Vers:   1.1.0
 *
 * Description:  
 *             this file generate the dc wheel. 
 *
 * Note:   - first release
 *          
 *         
 * Originally by Matteo Saccaro, 2016.
 * Attribution-Share Alike 3.0
 *
 */ 




module wheel(){
difference(){
 cylinder(r=5,h=10,$fn=100);
 translate([0,0,5]) cube([5.5,4.85,10],center=true);
}
difference(){
  cylinder(r=15,h=5,$fn=100,center=true);
	difference(){
	  cylinder(r=25,h=1.5,$fn=100,center=true);
	  cylinder(r=12.5,h=1.5,$fn=100,center=true);
	
	}
	
}

}



wheel();
