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
#include <stdio.h>

/* Context Functions */

ballistics_ctx_t libballistics_create(void) {
    ballistics_ctx_t context = calloc(1, sizeof(struct ballistics_ctx));
    return context;
}

void libballistics_finish(ballistics_ctx_t context) {
    ballistic_coefficient_t cur, ptr;
	
	if (context == NULL)
		return;
    free(context->trajectory);
    ptr = context->bCs;
    while(ptr) {
        cur = ptr;
        ptr = cur->next;
        free(cur);
    }
    free(context);
}

int libballistics_addBallisticCoefficient(ballistics_ctx_t context, double bC, double minFPS, double maxFPS) {
    ballistic_coefficient_t coefficient = calloc(1,
        sizeof(struct ballistic_coefficient));
    ballistic_coefficient_t ptr;
    if (coefficient == NULL)
        return -1;
    coefficient->bC = bC;
    coefficient->minFPS = minFPS;
    coefficient->maxFPS = maxFPS;
    coefficient->next = NULL;
    
    if (context->bCs == NULL) {
        context->bCs = coefficient;
        return 0;
    }
    
    ptr = context->bCs;
    while(ptr->next)
        ptr = ptr->next;
    ptr->next = coefficient;
    return 0;
}    

double libballistics_getBallisticCoefficient(
    ballistics_ctx_t context, 
    double velocity) 
{
    ballistic_coefficient_t cur;
    double best = 0.0;
    int lowestVelocity = -1;
    cur = context->bCs;
    while(cur) {
        if (cur->maxFPS == 0 && cur->minFPS == 0)
            return cur->bC;
        if (   (velocity >= cur->minFPS || cur->minFPS == 0) 
            && (velocity <= cur->maxFPS || cur->maxFPS == 0)  )
        {
            if (lowestVelocity == -1 || cur->minFPS < lowestVelocity) {
                lowestVelocity = cur->minFPS;
                best = cur->bC;
            }
        }
        cur = cur->next;
    }
    return best;
}

double libballistics_getBallisticCoefficientForLowestVelocity(
    ballistics_ctx_t context)
{
    ballistic_coefficient_t cur;
    double bestBC = 0.0;
    int lowestVelocity = -1;
    cur = context->bCs;
    while(cur) {
        if (lowestVelocity == -1 || cur->minFPS < lowestVelocity) {
            lowestVelocity = cur->minFPS;
            bestBC = cur->bC;
        }
        cur = cur->next;
    }
    return bestBC;
}
							 
/* Trajectory Calculations */

int libballistics_computeTrajectory (
    ballistics_ctx_t context, 
    int dragFunction, 
    double velocity,
    double sightHeight, 
    double losAngle, 
    double zeroAngle, 
    double windVelocity, 
    double windAngle,
    unsigned long maxRange)
{
    double t = 0, v = 0, vx = 0, vx1 = 0, vy = 0, vy1 = 0;
    double dv = 0, dvx = 0, dvy = 0, x = 0, y = 0;
    double dt = 0.5 / velocity;
    double bC, lastBc = 0.0;
    trajectory_path_t traj;
    int n = 0;
    
    double headwind = libballistics_headWind (windVelocity, windAngle);
    double crosswind = libballistics_crossWind (windVelocity, windAngle);
    
    /* Calculate X/Y gravity */
    double Gy = LIBBALLISTICS_GRAVITY 
        * cos(libballistics_deg2rad((losAngle + zeroAngle)));

    double Gx = LIBBALLISTICS_GRAVITY 
        * sin(libballistics_deg2rad((losAngle + zeroAngle)));

    maxRange++;
    context->maxRange = maxRange;
    context->trajectory = calloc(1, sizeof(struct trajectory_path) 
        * (maxRange + 1));

    vx = velocity * cos(libballistics_deg2rad(zeroAngle));
    vy = velocity * sin(libballistics_deg2rad(zeroAngle));
    y = -sightHeight/12;

    for (t = 0; ;t = t + dt) {
        vx1 = vx, vy1 = vy;
        v = pow(pow(vx,2) + pow(vy,2), 0.5);
        dt = 0.5 / v;

        /* Variable BCs may be used by adding multiple BCs with differing 
         * min/max velocieis. The correct BC will be selected at each
         * distance calculated. */

        bC = libballistics_getBallisticCoefficient(context, v);
        if (bC == 0.0) {
            bC = lastBc;
            if (bC == 0.0)
                bC = libballistics_getBallisticCoefficientForLowestVelocity(
                    context);
            if (bC == 0.0)
                break;
        } else {
            lastBc = bC;
        }

        /* Compute acceleration using the drag function retardation */
        dv = libballistics_computeRetardation(dragFunction, bC, v + headwind);
        dvx = -(vx/v) * dv;
        dvy = -(vy/v) * dv;

        /* Compute velocity, including resolved gravity vectors */
        vx = vx + dt * dvx + dt * Gx;
        vy = vy + dt * dvy + dt * Gy;
        
        traj = context->trajectory + n;
        if (x/3 >= n ) {
            traj->range = x / 3;
            traj->pathY = y * 12;
            traj->pathX = libballistics_computeWindage(crosswind, 
                velocity, x, t + dt);
            traj->elevation = libballistics_rad2moa(atan(y/x));
            traj->windage = libballistics_computeWindage(crosswind, 
                velocity, x, t + dt) * 95.5 / (x / 3);
            traj->time = t + dt;
            traj->velocity = v;
            traj->velocityX = vx;
            traj->velocityY = vy;
            n++;    
        }    
        
        /* Compute position based on average velocity */
        x = x + dt * (vx+vx1) / 2;
        y = y + dt * (vy+vy1) / 2;
        
        if (fabs(vy) > fabs(3*vx))
            break;
        if (n >= maxRange)
            break;
        if (v <= 0.0 || x <= 0.0)
            break;
        context->maxValidRange = (int) x/3;
    }

    traj = context->trajectory + maxRange;
    traj->range = n;
    return n;
}
