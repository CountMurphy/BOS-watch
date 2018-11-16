include<knurledFinishLib.scad>
include<polyScrewThread.scad>

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
            cylinder(r=11.80/2,h=6.62);
        };
    };


    //laser
    translate([9.87,53.35,(6.42-1.6-(5.9/2))]){
        color([1,0,0]){
            rotate([90,90,0]) {
                cylinder(r=(5.9/2),h=10);
            };
        };
    };

    //usb
    rotate([90,0,90]){
        translate([45.21,4,75.24]){
            minkowski() {
                    linear_extrude(5.56)
                        polygon([[-7.25/2,0], [7.25/2,0],
                                [7.25/2,-1], [5.25/2,-2.3],
                                [-5.25/2,-2.3], [-7.25/2,-1]]);
                    cylinder(r=0.5, h=1);
            }    
      }
     }
    //screen
    translate([25.8,14.33,1.6]){
        color([1,0,0]){
            cube([30.90,8.43,5.25]);
        };
    };
    
    //toggles
    translate([11,17.32,1.6]){
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
    
    translate([11,11.96,1.6]){
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
    translate([11,6.35,1.6]){
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
    translate([11,0.99,1.6]){
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
    translate([64.23,48.92,1.6,]){
        union(){
            color([0,0,1]){
            cube([10.12,3.80,(6.86-1.6)]);
            translate([3.80,3.8,2.5])
                cube([1.90,4.82,2.03]);
        }}
    }
    //bat
    translate([44.38,44.01,1.6]){
        color([0,1,0]){
            cube([6.25,7.93,(7.32-1.6)]);
        }
    }
    //rtc?
    translate([57.58,38.57,1.6]){
        // cylinder(h=13.75-1.6,r=2.94/2);
    }
    //SWD pins
    translate([14.5,44.33,1.6]){
        cube([2.35,7.78,7.13]);
    }
    //ir
    translate([23,53.10-5.44,1.6]){
        cube([4.5,2.43,5.13]);
    }
}

module boardBaseOnly(){
        union(){
        linear_extrude(5.6){  //got to exagerate to make hole in bottom
            translate([-1.5,-1.5,0]){
                square(size=[82.74,55.62]);
                translate([19.90,0]){
                    // polygon(points=[[0,0],[43.22,0],[33.72,7.89],[10.20,7.89]]);
                }
            }
        };
            //laser
    translate([8.15,53.35+40,(6.42-1.6-(5.9/2))]){
        color([1,0,0]){
            rotate([90,90,0]) {
                cylinder(r=(7.4/2),h=50);
            };
        };
    };
    //power switch
    translate([64.23,48.92,1.6,]){
        union(){
            color([0,0,1]){
            cube([10.12,3.80,(6.86-1.6)]);
            translate([3.80,3.8,2.5])
                cube([1.90,4.82,2.03]);
        }}
    };
    
        //usb
    rotate([90,0,90]){
        translate([45.21,4,75.24]){
            minkowski() {
                    linear_extrude(15.56)
                        polygon([[-7.25/2,0], [7.25/2,0],
                                [7.25/2,-1], [5.25/2,-2.3],
                                [-5.25/2,-2.3], [-7.25/2,-1]]);
                    cylinder(r=0.5, h=1);
            }    
      }
     }
    };
}    

module boardBaseTop(){
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
        translate([63.05-5.5,19,1.6]){
            color([1,0,1]){
            cube([20.63,25.63,5]);}
        };
    };
    
    //LEDS
    translate([71,11,1]){
        //led group hole
        color([0,1,0]){
            cylinder(r=9,h=10);
        }
    }
    translate([71.39,16.80,1.6]){
        color([1,1,1]){
            rotate([0,0,90]){
                cylinder(h=12,r=3,$fn=3);
            }
        }
    };
    translate([71.39,5.26,1.6]){
        color([1,1,1]){
            rotate([0,0,30]){
                cylinder(h=12,r=3,$fn=3);
            }
        }
    };
     translate([65.80,11.3,1.6]){
        color([1,1,1]){
            rotate([0,0,180]){
                cylinder(h=12,r=3,$fn=3);
            }
        }
    };
    translate([76.74,11.3,1.6]){
        color([1,1,1]){
            cylinder(h=12,r=3,$fn=3);
        }
    };

    //buzzer
    translate([20.76,38.15,1.6]){
        color([0,1,1]){
            cylinder(r=14/2,h=6.62);
        };
    };


     //laser
    translate([9.15,53.35+40,(6.42-1.6-(5.9/2))]){
        color([1,0,0]){
            rotate([90,90,0]) {
                cylinder(r=(7.4/2),h=60);
            };
        };
    };

        //usb
    rotate([90,0,90]){
        translate([45.21,4,75.24]){
            minkowski() {
                    #linear_extrude(15.56)
                        polygon([[-10.25/2,0], [10.25/2,0],
                                [10.25/2,-1], [8.25/2,-5.3],
                                [-8.25/2,-5.3], [-10.25/2,-1]]);
                    cylinder(r=.5, h=1);
            }    
      }
     }
    //screen
    translate([24.8,14.33,1.6]){
        color([1,0,0]){
            cube([31.90,8.43,10.25]);
        };
    };
    
    //toggles
    translate([11,.5,1.6]){
        cube([10,22,7.54]);
    }
    
    
    //compass
    translate([1.5,23.5,2.5]){
        cube([13.5,32,2.5]);
    }
    //power switch
    translate([62,48.92,1.6,]){
        union(){
            color([0,0,1]){
            #cube([11.62,15.80,(6.86-1.6)]);
            translate([3.80,3.8,2.5])
                cube([1.90,4.82,2.03]);
        }}
    }
    //bat
    translate([42.84-5,44.01,1.6]){
        color([0,1,0]){
            cube([16,8.93,(7.32-1.6)]);
        }
    }
    //rtc?
    translate([27.58,26.57,3]){
        #cube([47.8,18.63,5]);
    }
    //SWD pins
    translate([12.5,44.33,1.6]){
        cube([4.5,9.78,7.73]);
    }
    translate([14.5,41.33,3]){
        #cube([2.5,5,2]);
    }
    //ir
    translate([22.5,53.10-5.44,1.6]){
        cube([5.5,15.43,8.13]);
    }
}

module pins(){
            color([0,1,0]){
            //Create Pins
            union(){
                translate([25,10,-2]){
                    rotate([0,90,0]){
                        translate([4,0,0]){
                            //pin
                            cylinder(r=4.82/2,h=30.39);
                        }
                    }
                    translate([-1.5,0,-7]){
                        rotate([0,0,45]){
                            cylinder(9,2,7,$fn=4);
                        }
                    }
                    translate([32,0,-7]){
                        rotate([0,0,45]){
                            cylinder(9,2,7,$fn=4);
                        }
                    }
                }
            }
        }
}

module bottom(){
    
    difference(){
    union(){
        //battery walls
        translate([58.5+2.5/2,16.5-2.5/2,-4.2-.5]){
            rotate([0,0,90]){
                // cube([24.5,36.4,5.5]);
                #cube([28,39.9,7.2]);
            }
        };
        union(){
            color([1,0,1]){
            difference(){
                linear_extrude(7){
                    difference(){
                        hull(){
                            circle(r=3);
    
                            translate([0,53.12,0]){
                                circle(r=3);
                            }
                            translate([80.24,53.12,0]){
                                circle(r=3);
                            }
                            translate([80.24,0,0]){
                                circle(r=3);
                            }
                        };
                    translate([19.90,-3,0]){
                        polygon(points=[[0,0],[43.22,0],[33.72,7.89],[10.20,7.89]]);
                    };
                    }
                };
                translate([0,0,4]){
                    boardBaseOnly();
                };
            };
            };
            pins();
            translate([0,40,0]){
                pins();
            };
        }
    }//bat union
        {
            //wire trench
            translate([58.5,27,1]){
                cube([9,5,4]);
            }
            translate([63.5,30,1]){
                cube([4,17,4]);
            }
            translate([63.5-24,47-4,1]){
                cube([24,4,4]);
            }
            translate([63.5-27.75,47-4,1]){
                cube([4,11.05,4]);
            }
            //battery hole
            translate([58.5,16.5,-2.5-.5]){
                rotate([0,0,90]){
                    cube([25.5,37.4,7.5]);
                    // cube([24.5+1.7,36.4+1.7,5.5+1.7]);
                }
            }
        }
        
    }
}

//

module bottomForHollow(){
    union(){
        color([1,0,1]){
            linear_extrude(9){
                difference(){
                    hull(){
                        translate([-.5,0,0]){
                            circle(r=3);
                        }
    
                        translate([-0.5,53.62,0]){
                            circle(r=3);
                        }
                        translate([80.84,53.62,0]){
                            circle(r=3);
                        }
                        translate([80.84,0,0]){
                            circle(r=3);
                        }
                    };
                translate([19.90,-3,0]){
                    polygon(points=[[0,0],[43.22,0],[33.72,7.89],[10.20,7.89]]);
                };
                }
            };
        };
        pins();
        translate([0,35,0]){
            pins();
        };
    }
}


module bottomWithClasp(){
    union(){
        bottom();
        difference(){
            translate([-13,28.5,0]){
                union(){
                    color([0,1,0]){
                        cylinder(r=5.5,h=4,$fn=100);
                        translate([0,-5.5,0])
                        cube([10,11,4]);
                    }
                }
            }
            {
                translate([-13.5,28.5,17])
                rotate([0,180,0])
                threadWithMoreSpace();
            }
        }   
    }
}

module top(){
    union(){
    difference(){
    difference(){
   color([1,1,0]){
       union(){
       difference(){
       hull(){
          //Bezel thing
            translate([-5,0,4.5]){
              difference(){ 
               rotate([0,90,0]){
                   cylinder(r=60,h=50,$fn=100);
               };
               translate([-50,-80,-83]){
                   cube([125,225,140]);
               }
           };
      
            }
            //hull Pegs
            translate([-28,-40.5,50]){
                cylinder(r=2,h=7.52);
            };
            translate([-28,-32+.7+53.12,50]){
                cylinder(r=2,h=7.52);
            };
            translate([-22+80.24,-40-.7,50]){
                cylinder(r=2,h=7.52);
            };
            translate([-22+80.24,-31+53.12,50]){
                cylinder(r=2,h=7.52);
            };
        }

        linear_extrude(100){
        translate([-5,-40,-10]){
                    polygon(points=[[-10,-10],[53.22,-9],[33.72,7.89],[10.20,7.89]]);
                };
    
        }
   }
   //add led circle here
   translate([46,-25,59])
   {
       cylinder(r=10,h=7,$fn=100);
   }
   }//union
         //laser thing
        translate([-15.82,22,57]){
            difference(){
                rotate([90,0,0]){
                    cylinder(r=5,h=15,$fn=100);
                };
                translate([-10,-16,-13]){
                    cube([17,18,15]);
                }
            };
        };
    }

    translate([-25,-36,53.95]){
        boardBaseTop();
    }
    }//hole diff
    
    translate([-25,-36,48])
    {
        bottomForHollow();
    }
    }//hollow diff
    translate([-9.25,-33.5,-8.7]){
        difference(){
            difference(){
                translate([-1,52.5,70]){
                    rotate([90,0,0])
                        cylinder(r=3.5,h=11,$fn=100);
                }
                translate([-1,51.5,70]){
                    rotate([90,0,0])
                        cylinder(r=3,h=9.5,$fn=100);
                }
            }
            translate([-3,41,65])
                cube([6,10,4]);
        }
    }
    }//uu

}
//
//

module topWithClasp(){
        difference(){
            union(){
                top();
                translate([-38.5,-7.5,50]){
                    difference(){
                        union(){
                            cylinder(r=8,h=7,$fn=100);
                            translate([0,-7.5,0])
                            cube([10,15,7]);
                        }
                        translate([0,0,17])
                        rotate([0,180,0])
                        threadWithMoreSpace();
                    }

                }   
            }
            { 
                translate([-38.5,-7.5,50]){
                    cylinder(r=6,h=4,$fn=100);
                    translate([0,-6,0])
                    cube([11,12,4]);
                }
            }
        };
}

module logoDiff(){
 //add logo
    difference(){
        translate([9,-5,62]){
            logo();
        };
        top();
    }
}


module knob(){
    t_od=8;    // Thread outer diameter
    t_st=2.5;   // Step/traveling per turn
    t_lf=55;    // Step angle degrees
    t_ln=18;    // Length of the threade section
    t_rs=PI/2;  // Resolution
    t_se=1;     // Thread ends style
    t_gp=0;     // Gap between nut and bolt threads


    difference()
    {
        union()
        {
             k_cyl_hg=10;   // Knurled cylinder height
    k_cyl_od=15.5;   // Knurled cylinder outer* diameter

    knurl_wd=3;      // Knurl polyhedron width
    knurl_hg=3;      // Knurl polyhedron height
    knurl_dp=1.5;    // Knurl polyhedron depth

    e_smooth=1;      // Cylinder ends smoothed height
    s_smooth=0;      // [ 0% - 100% ] Knurled surface smoothing amount

    knurled_cyl(k_cyl_hg, k_cyl_od, 
                knurl_wd, knurl_hg, knurl_dp, 
                e_smooth, s_smooth);

            translate([0,0,b_hg])
              screw_thread(t_od+t_gp, t_st, t_lf, t_ln, t_rs, t_se);
        }

    }
}

module threadWithMoreSpace(){
    t_od=8.5;    // Thread outer diameter
    t_st=2.5;   // Step/traveling per turn
    t_lf=55;    // Step angle degrees
    t_ln=18;    // Length of the threade section
    t_rs=PI/2;  // Resolution
    t_se=1;     // Thread ends style
    t_gp=0;     // Gap between nut and bolt threads


    difference()
    {
        union()
        {
             k_cyl_hg=10;   // Knurled cylinder height
    k_cyl_od=15.5;   // Knurled cylinder outer* diameter

    knurl_wd=3;      // Knurl polyhedron width
    knurl_hg=3;      // Knurl polyhedron height
    knurl_dp=1.5;    // Knurl polyhedron depth

    e_smooth=1;      // Cylinder ends smoothed height
    s_smooth=0;      // [ 0% - 100% ] Knurled surface smoothing amount

    knurled_cyl(k_cyl_hg, k_cyl_od, 
                knurl_wd, knurl_hg, knurl_dp, 
                e_smooth, s_smooth);

            translate([0,0,b_hg])
              screw_thread(t_od+t_gp, t_st, t_lf, t_ln, t_rs, t_se);
        }

    }
}

module logo() {
    linear_extrude(10) {
        scale(.15)
        import("outfile.dxf");
    }
}

module topWithClaspAndLogo(){
    difference(){
        topWithClasp();
        translate([0,0,-.5]){
            logoDiff();
        }
    }
}

module directionalLED(){
    //LEDS
    translate([71,11,1]){
        //led group hole
        color([0,1,0]){
            cylinder(r=8.5,h=1);
        }
    }
    translate([71.39,16.80,1.6]){
        color([1,1,1]){
            rotate([0,0,90]){
                cylinder(h=2.5,r=2.5,$fn=3);
            }
        }
    };
    translate([71.39,5.26,1.6]){
        color([1,1,1]){
            rotate([0,0,30]){
                cylinder(h=2.5,r=2.5,$fn=3);
            }
        }
    };
     translate([65.80,11.3,1.6]){
        color([1,1,1]){
            rotate([0,0,180]){
                cylinder(h=2.5,r=2.5,$fn=3);
            }
        }
    };
    translate([76.74,11.3,1.6]){
        color([1,1,1]){
            cylinder(h=2.5,r=2.5,$fn=3);
        }
    };


}

// translate([-13.5,28.5,17])
// rotate([0,180,0])
// knob();
bottomWithClasp();
translate([0,0,4]){
//    board();
};

//  translate([25,36,-50]){topWithClaspAndLogo();};
//  directionalLED();
//  translate([25,36,-40]){top();};
// boardBaseTop();


