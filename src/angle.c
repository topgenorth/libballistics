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

/* Angular conversion functions */

double libballistics_deg2moa (double deg) {
    return deg * 60.0;
}

double libballistics_deg2rad (double deg) {
    return deg * M_PI / 180.0;
}

double libballistics_moa2deg (double moa) {
    return moa / 60.0;
}

double libballistics_moa2rad (double moa) {
    return moa / 60.0 * M_PI / 180.0;
}

double libballistics_rad2deg (double rad) {
    return rad * 180.0 / M_PI;
}

double libballistics_rad2moa (double rad) {
    return rad * 60 * 180 / M_PI;
}

double libballistics_moa2mil(double moa, int flags) {
	if (flags == MilDotArmy)
		return (moa / 3.375);
	else
		return (moa / 3.438);
}
