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

/* Retrieval functions */

double libballistics_getRange (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->range;
    } 
    return 0;
}

double libballistics_getPathY (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->pathY;
    }
    return 0;
}

double libballistics_getPathX (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->pathX;
    }
    return 0;
}

double libballistics_getElevation (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->elevation;
    }
    return 0;
}


double libballistics_getWindage (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->windage;
    }
    return 0;
}


double libballistics_getTime (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->time;
    }
    return 0;
}

double libballistics_getVelocity (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->velocity;
    }
    return 0;
}


double libballistics_getVelocityY (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->velocityY;
    }
    return 0;
}


double libballistics_getVelocityX (ballistics_ctx_t context, int range) {
    if (range <= context->maxValidRange) {
        trajectory_path_t traj = context->trajectory + range;
        return traj->velocityX;
    }
    return 0;
}

double libballistics_computeEnergy (double velocity, double bulletWeight) {
    return bulletWeight * (velocity * velocity) / ( 2 *-LIBBALLISTICS_GRAVITY * 7000);
}


int libballistics_getMinPBR(ballistics_ctx_t context, int zeroRange, double vitalZoneRadius) {
	int minpbr = zeroRange;
	int i;
	
	for(i=zeroRange; i>0; i--) {
		if (fabs(libballistics_getPathY(context, i)) <= vitalZoneRadius)
			minpbr = i;
		else
			break;
	}
	return minpbr;
}


int libballistics_getMaxPBR(ballistics_ctx_t context, int zeroRange, double vitalZoneRadius) {
	int maxpbr = zeroRange;
	int i;
		
	for(i=zeroRange; i<context->maxValidRange; i++) {
		if (fabs(libballistics_getPathY(context, i)) <= vitalZoneRadius) {
			maxpbr = i;
		} else {
			break;
		}
	}
	return maxpbr;
}
