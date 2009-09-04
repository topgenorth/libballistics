/*
 GNU EXTERNAL BALLISTICS LIBRARY

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License
 as published by the Free Software Foundation; version 2
 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "ballistics.h"

/* Zero Calculations */

double libballistics_computeZeroAngle(
    int dragFunction, 
    double bC, 
    double velocity, 
    double sightHeight, 
    double zeroRange, 
    double yIntercept)
{
    double t=0, x=0;
    double dt=1 / velocity;
    double y =-sightHeight/12;
    double da; /* Change in bore angle to iterate on the correct zero angle */

    /* State variables */
    double v=0, vx=0, vy=0;    /* Velocity */
    double vx1=0, vy1=0;       /* Last velocity, for computing avg. velocity */
    double dv=0, dvx=0, dvy=0; /* Acceleration */
    double Gx=0, Gy=0;         /* Gravitational acceleration */
    double angle=0;            /* Angle of the bore */

    /* Start with a coarse angular change to quickly solve
     *  large launch angle problems */
    da=libballistics_deg2rad(14);


    /* Start at 0 degrees of elevation and increase the elevation by 14 degrees
     * until we are above the correct elevation. Then reduce the angular change
     * by half, and begin reducing the angle. Once we are again below the 
     * correct angle, reduce the angular change by half again, and go back
     * up.  This allows for a fast successive approximation of the correct 
     * elevation, usually within less than 20 iterations. */

    for (angle = 0; ;angle = angle + da) {
        vy=velocity*sin(angle);
        vx=velocity*cos(angle);
        Gx=LIBBALLISTICS_GRAVITY*sin(angle);
        Gy=LIBBALLISTICS_GRAVITY*cos(angle);

        for (t=0,x=0,y=-sightHeight/12;x<=zeroRange*3;t=t+dt) {
            vy1=vy;
            vx1=vx;
            v=pow((pow(vx,2)+pow(vy,2)),0.5);
            dt=1/v;
            
            dv = libballistics_computeRetardation(dragFunction, bC, v);
            dvy = -dv*vy/v*dt;
            dvx = -dv*vx/v*dt;

            vx=vx+dvx;
            vy=vy+dvy;
            vy=vy+dt*Gy;
            vx=vx+dt*Gx;
            
            x=x+dt*(vx+vx1)/2;
            y=y+dt*(vy+vy1)/2;

            if (vy<0 && y<yIntercept)
                break;

            if (vy>3*vx) 
                break;
        }
    
        if (y>yIntercept && da>0) 
            da=-da/2;

        if (y<yIntercept && da<0) 
            da=-da/2;

        if (fabs(da) < libballistics_moa2rad(0.01))
            break; /* Accuracy is sufficient; stop approximating */
        if (angle > libballistics_deg2rad(45))
            break; /* Exceeded 45 degree launch angle, so the projectile just
                    * won't get there */
    }

    return libballistics_rad2deg(angle);
}

