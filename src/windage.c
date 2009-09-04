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

/* Windage Calculations */

double libballistics_computeWindage(
    double windVelocity, 
    double projectileVelocity, 
    double range, 
    double time)
{
    double Vw = windVelocity*17.60; /* Convert to in/sec */
        return (Vw*(time-range/projectileVelocity));
}

double libballistics_headWind (double windSpeed, double windAngle) {
    double Wangle = libballistics_deg2rad(windAngle);
    return (cos(Wangle)*windSpeed);
}

double libballistics_crossWind (double windSpeed, double windAngle) {
    double Wangle = libballistics_deg2rad(windAngle);
    return (sin(Wangle)*windSpeed);
}


