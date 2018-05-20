module board(){
    union(){
        linear_extrude(1.6){  
            difference(){
                square(size=[81.24,53.12]);
                translate([19.90,0]){
                    polygon(points=[[0,0],[43.22,0],[33.72,7.89],[10.20,7.89]]);
                }
            }
        };
    
    //GPS
        translate([63.05,23,1.6]){
            color([1,0,1]){
            cube([15.13,15.13,5]);}
        };
    };
    
    //LEDS
    translate([71.39,16.80,1.6]){
        color([1,1,1]){
            cylinder(h=8.1,r=2.5);
        }
    };
    translate([71.39,5.26,1.6]){
        color([1,1,1]){
            cylinder(h=8.1,r=2.5);
        }
    };
     translate([65.80,11.3,1.6]){
        color([1,1,1]){
            cylinder(h=8.1,r=2.5);
        }
    };
    translate([76.74,11.3,1.6]){
        color([1,1,1]){
            cylinder(h=8.1,r=2.5);
        }
    };

    //buzzer
    translate([22.21,38.15,1.6]){
        color([0,1,1]){
            cylinder(r=4.35,h=6.62);
        };
    };


    //laser
    translate([8.37,53.35,(6.42-1.6-(5.9/2))]){
        color([1,0,0]){
            rotate([90,90,0]) {
                cylinder(r=(5.9/2),h=10);
            };
        };
    };

    //usb
    
    //screen
    translate([25.8,10.33,1.6]){
        color([1,0,0]){
            cube([30.90,8.43,5.25]);
        };
    };
    
    //toggles
    translate([12.55,17.32,1.6]){
        color([0,0,1]){
            union(){
                cube([9.5,4.95,7.52]);
                translate([(9.5/2),(4.95/2),7.52]){
                    cylinder(r=4.03/2,h=4.03);
                };
                translate([9.5/2,4.95/2,7.52+4.03]){
                    cylinder(h=9.95,r=2.7/2);
                }
            };
        };
    };
    
    translate([12.55,11.96,1.6]){
        color([0,0,1]){
            union(){
                cube([9.5,4.95,7.52]);
                translate([(9.5/2),(4.95/2),7.52]){
                    cylinder(r=4.03/2,h=4.03);
                };
                translate([9.5/2,4.95/2,7.52+4.03]){
                    cylinder(h=9.95,r=2.7/2);
                }
            };
        };
    };
    translate([12.55,6.35,1.6]){
        color([0,0,1]){
            union(){
                cube([9.5,4.95,7.52]);
                translate([(9.5/2),(4.95/2),7.52]){
                    cylinder(r=4.03/2,h=4.03);
                };
                translate([9.5/2,4.95/2,7.52+4.03]){
                    cylinder(h=9.95,r=2.7/2);
                }
            };
        };
    };
    translate([12.55,0.99,1.6]){
        color([0,0,1]){
            union(){
                cube([9.5,4.95,7.52]);
                translate([(9.5/2),(4.95/2),7.52]){
                    cylinder(r=4.03/2,h=4.03);
                };
                translate([9.5/2,4.95/2,7.52+4.03]){
                    cylinder(h=9.95,r=2.7/2);
                }
            };
        };
    };
    //power switch
    
    //bat
    
    //rtc?
}



board();