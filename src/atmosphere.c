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

/* Atmospheric calculations */

double libballistics_calcFR (
  double temperature, 
  double barometricPressure, 
  double relativeHumidity) 
{
    double VPw = 4e-6 * pow(temperature,3) - 0.0004 
        * pow(temperature,2) + 0.0234 * temperature - 0.2517;
    double FRH = 0.995 * 
          (barometricPressure 
        / (barometricPressure - 0.3783 * relativeHumidity * VPw) );
    return FRH;
}

double libballistics_calcFP (double barometricPressure) {
    double FP = (barometricPressure - 29.92) / 29.92;
    return FP;
}

double libballistics_calcFT (double temperature, double altitude) {
    double Tstd = -0.0036 * altitude + 59.0;
    double FT = (temperature-Tstd) / (459.6 + Tstd);
    return FT;
}

double libballistics_calcFA (double altitude) {
    double FA = -4e-15 * pow(altitude, 3)
                +4e-10 * pow(altitude, 2)
                -3e-5  * altitude + 1;
    return 1.0 / FA;
}

double libballistics_applyAtmosphere(
    double bC,
    double altitude,
    double barometricPressure,
    double temperature,
    double relativeHumidity)
{
    double FA = libballistics_calcFA(altitude);
    double FT = libballistics_calcFT(temperature, altitude);
    double FR = libballistics_calcFR(temperature, barometricPressure, 
        relativeHumidity);
    double FP = libballistics_calcFP(barometricPressure);

    double cbC = (FA * (1 + FT - FP) * FR );
    return bC * cbC;
}
