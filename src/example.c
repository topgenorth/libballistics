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

#include <stdio.h>
#include "ballistics.h"

int main(int argc, char *argv[]) {
  int s, k = 0;
  ballistics_ctx_t context;   /* Our ballistics context */
  double bc=0.495;            /* Ballistic coefficient of the projectile */
  double v=2600;              /* Muzzle velocity (fps) */
  double w=175;               /* Bullet weight (gr) */
  double sh=1.5;              /* Sight height (in) */
  double angle=0;             /* LOS Angle (degrees) */
  double zero=300;            /* Zero range (yards) */
  double windspeed=10;        /* Wind speed (MPH) */
  double windangle=90;        /* Wind angle (degrees) */
  double zeroangle=0;         /* Zero angle of the bore */

  /* Custom atmosphere */
  double altitude = 0.0,
         pressure = 29.92,
         temperature = 59.0,
         humidity = .78; 

  /* If you wish to adjust for atmospheric conditions, correct the BC */

  printf("Original BC: %0.3f. ", bc);
  bc = libballistics_applyAtmosphere(bc, altitude, pressure, 
      temperature, humidity);
  printf("BC Corrected for atmosphere: %0.3f\n", bc);
  
  /* Calculate the zero angle */
  zeroangle = libballistics_computeZeroAngle(G1, bc, v, sh, zero, 0);
  
  /* Prints a simple trajectory chart */
  printf(" Range   Path  Path   Windage  Windage  Velocity  Energy   Time\n");
  printf("(yards)  (in)  (moa)   (in)     (moa)    (fps)    (ft-lb)  (sec)\n");
  printf("----------------------------------------------------------------\n");
 
  /* Create a new ballistics context */
  context = libballistics_create();

  /* Multiple BCs can be added for different velocity ranges. You'll
   * need at least one to compute a trajectory. Use zero for both
   * min/max FPS if you're adding just one. 
   */
  libballistics_addBallisticCoefficient(context, bc, 0.0, 0.0);

  /* Compute the trajectory */
  k = libballistics_computeTrajectory(context, G1, v, sh, angle, 
      zeroangle, windspeed, windangle, 1200);

  for (s=25; s<=1200; s+=25) {
      printf(" %4.0f  %+6.1f %+6.1f  %+6.1f  %+6.1f      %04.0f     %04.0f"
             "    %01.2f\n",
      libballistics_getRange(context, s),
      libballistics_getPathY(context, s),
      libballistics_getElevation(context, s),
      libballistics_getPathX(context, s),
      libballistics_getWindage(context, s),
      libballistics_getVelocity(context, s),
      libballistics_computeEnergy(libballistics_getVelocity(context, s), w),
      libballistics_getTime(context, s));;
  }
      
  libballistics_finish(context);
  return 0;
}

