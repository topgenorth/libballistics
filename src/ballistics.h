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

#ifndef __LIBBALLISTICS_H__
#define __LIBBALLISTICS_H__

#include <math.h>
#include <stdlib.h>

#define LIBBALLISTICS_GRAVITY			(-32.194)
#define LIBBALLISTICS_ABSOLUTE_ZERO		459.67

enum MilDotSpec {
	MilDotUSMC=0,
	MilDotArmy=1
};

enum BallisticDragFunction {
    G1=1,
    G2,
    G3,
    G4,
    G5,
    G6,
    G7,
    G8
};

/* Angular Conversion Functions */
double libballistics_deg2moa(double deg); /* Degrees to MOA */
double libballistics_deg2rad(double deg); /* Degrees to Radians */
double libballistics_moa2deg(double moa); /* MOA to Degrees */
double libballistics_moa2rad(double moa); /* MOA to Radians */
double libballistics_rad2deg(double rad); /* Radians to Degrees */
double libballistics_rad2moa(double rad); /* Radians to MOA */
double libballistics_moa2mil(double moa, int spec);

/* libballistics_computeRetardation: calculates retardation values based on 
 *     standard drag functions 
 * Arguments:
 *     dragFunction: G1, G2, G5, G6, G7, or G8
 *               bC: Ballistic coefficient for the projectile
 *         velocity: Velocity of the projectile
 *
 * Returns:
 *    Projectile drag retardation velocity in fps
 */
 
double libballistics_computeRetardation (int dragFunction, double bc, 
    double velocity);

/* libballistics_dragForModel: returns the drag coefficient for the given
 *     drag model's standard 'G' bullet at the given velocity and
 *     temperature. This can be used to convert between different drag
 *     models.
 *
 * Arguments:
 *     dragFunction: G1, G2, G5, G6, G7, or G8
 *         velocity: Velocity of the projectile
 *      temperature: Ambient temperature (used to calculate the speed
 *          of sound, to calculate mach value)
 * Returns:
 *     drag coefficient for standard 'G' bullet at velocity/temperature
 */
 
double libballistics_dragForModel(int dragFunction, double velocity,
	double temperature);

/* libballistics_applyAtmosphere: Applies atmospheric conditions to an 
 *     existing BC 
 * Arguments:
 *                     bC: Ballistic coefficient for the projectile
 *               altitude: Altitude above sea level (feet). 
 *                         Standard altitude is 0 feet above sea level.
 *     barometricPressure: Barometric pressure (Hg). 
 *                         Standard pressure is 29.92 Hg.
 *            temperature: Temperature (F). Standard temperature is 59F.
 *      relativeHumiditiy: Relative humidity (0.00 - 1.00) 
 *                         Standard humidity is 0.78
 * Returns:
 *     Corrected ballistic coefficient
 */

double libballistics_applyAtmosphere (double bC, double altitude, 
    double barometricPressure, double temperature, double relativeHumidity);

/* libballistics_computeWindage: Compute windage deflection for a given 
 *     crosswind
 * Arguments:
 *          windVelocity: Wind speed (MPH) 
 *    projectileVelocity: Velocity of the projectile 
 *                 range: Range at which to compute windage (feet)
 *                  time: Travel time to traverse the supplied range (seconds)
 * Returns:
 *    Windage correction (inches) required to achieve zero at given range
 */
 
double libballistics_computeWindage(double windVelocity, 
    double projectileVelocity, double range, double time);

/* libballistics_headWind: Resolve wind/angle to headwind components
 * libballistics_crossWind: Resolve wind/angle to crosswind components
 * Arguments:
 *    windVelocity: Wind velocity (MPH)
 *       windAngle: Angle of wind origin (degrees):
 *        0: Straight ahead
 *       90: Right to left
 *      180: From behind
 *      270: Left to right
 *      -90: Left to right
 * Returns:
 *     Wind velocity component (MPH)
 */

double libballistics_headWind (double windVelocity, double windAngle);
double libballistics_crossWind (double windVelocity, double windAngle);

/* libballistics_computeZeroAngle: Compute bore angle needed for zero 
 *     at given range 
 * Arguments:
 *     dragFunction: G1, G2, G3, G4, G4, G6, G7, or G8
 *               bC: Ballistic coefficient for the projectile
 *         velocity: Velocity of the projectile
 *      sightHeight: Distance between bore centerline and center of 
 *                   scope / sight (inches)
 *        zeroRange: Projectile intersect zero (yards)
 *       yIntercept: Height for projectile when crossing zeroRange (inches), 
 *                   usually 0
 */

double libballistics_computeZeroAngle(int DragFunction, double bC, 
    double velocity, double sightHeight, double zeroRange, double yIntercept);

/* ballistic_coefficient: A single ballistic coefficient property for a
 *     projectile. One or more ballistic coefficients may be specified within
 *     a single ballistics calculation, based on velocity. Before computing
 *     the trajectory, each coefficient structure must be added to the context
 *     using the libballistic_addBallisticCoefficient() function.
 * Elements:
 *       bC: The actual ballistic coefficient value
 *   minFPS: The minimum velocity (fps) for the BC to be valid
 *   maxFPS: The maximum velocity (fps) for the BC to be valid
 */

typedef struct ballistic_coefficient {
        double bC;
        int minFPS;
        int maxFPS;
        struct ballistic_coefficient *next;
} *ballistic_coefficient_t;


/* trajectory_path: A single point on a trajectory */

typedef struct trajectory_path {
    double range;       /* Range in yards */
    double pathY;       /* Bullet path (inches) */
    double pathX;       /* Windage (inches) */
    double elevation;   /* Bullet path (arcminutes) */
    double windage;     /* Windage (arcminutes) */
    double time;        /* Flight time (seconds) */
    double velocity;    /* Velocity (fps) */
    double velocityX;   /* X Velocity */
    double velocityY;   /* Y Velocity */
} *trajectory_path_t;

/* ballistics_ctx: Context for a ballistic computation
 * Elements:
 *     trajectory: Pointer to allocated memory to store the trajectory solution
 *       maxRange: Maximum range computed for this computation
 *  maxValidRange: Automatically set by libballistics_computeTrajectory to
 *                 specify the maximum valid range
 */

typedef struct ballistics_ctx {
	trajectory_path_t trajectory;
	unsigned long maxRange;
	unsigned long maxValidRange;
	ballistic_coefficient_t bCs;
} *ballistics_ctx_t;

/* libballistics_create: Creates a ballistic context
 * Returns:
 *     Pointer to a newly allocated ballistics context
 */

ballistics_ctx_t libballistics_create(void);

/* libballistics_finish: Destroys a ballistic context
 * Arguments:
 *     context: Pointer to the ballistic context to destroy
 * Returns:
 *     void
 */

void libballistics_finish(ballistics_ctx_t context);
 
/* libballistics_addBallisticCoefficient: Add a ballistic coefficient to a
 * ballistics context. Some projectiles (such as Sierra) are documented with 
 * stepped BC valuess by velocity. As trajectory is calculated, the correct BC 
 * for the given velocity will be applied.
 *
 * At least one BC must be added in order to perform a calculation. If you only
 * have one BC, use zero values for both min/max FPS.
 *
 * Arguments:
 *     context: pointer to the ballistic context to add the coefficient to
 *          bC: the actual BC
 *      minFPS: minimum fps for the BC to be valid
 *      maxFPS: maximum fps valid for this BC (use zero for no limit)
 *
 * Returns:
 *     0: operation succeessful
 */

int libballistics_addBallisticCoefficient(ballistics_ctx_t context, 
	double bC, double minFPS, double maxFPS);

/* libballistics_computeTrajectory: Generate a ballistics solution table 
 *     in 1 yard increments 
 * Arguments:
 *          context: Pointer to a ballistics context to store the solution
 *     dragFunction: G1, G2, G3, G4, G4, G6, G7, or G8
 *               bC: Ballistic coefficient for the projectile
 *         velocity: Velocity of the projectile
 *      sightHeight: Distance between bore centerline and center of 
 *                   scope / sight (inches)
 *         losAngle: Line-of-sight angle; angle at which the barrel is canted:
 *                   90: Straight up
 *                  -90: Straight down
 *        zeroAngle: Angle relative to the bore (degrees). 
 *                   Computed with libballistics_computeZeroAngle
 *     windVelocity: Wind velocity (MPH)
 *        windAngle: Angle of wind origin (degrees):
 *            0: Straight ahead
 *           90: Right to left
 *          180: From behind
 *          270: Left to right
 *          -90: Left to right
 *         maxRange: Maximum range to compute for
 * Returns:
 *     Integer specifying the maximum valid range (and number of entries) 
 *     within the trajectory table.
 */

int libballistics_computeTrajectory(ballistics_ctx_t context, int dragFunction,
    double velocity, double sightHeight, double losAngle, 
    double zeroAngle, double windVelocity, double windAngle, 
    unsigned long maxRange);

/* Data retrieval functions: Returns individual values for any valid range 
 *     specified.
 * Arguments:
 *    context: Solutions context
 *      range: Desired range to return data for (yards)  
 */

double libballistics_getRange (ballistics_ctx_t context, int range);
double libballistics_getPathY (ballistics_ctx_t context, int range);
double libballistics_getPathX (ballistics_ctx_t context, int range);
double libballistics_getElevation (ballistics_ctx_t context, int range);
double libballistics_getWindage (ballistics_ctx_t context, int range);
double libballistics_getTime (ballistics_ctx_t context, int range);       
double libballistics_getVelocity (ballistics_ctx_t context, int range);
double libballistics_getVx (ballistics_ctx_t context, int range);
double libballistics_getVy (ballistics_ctx_t context, int range);
int libballistics_getMinPBR(ballistics_ctx_t context, int zeroRange, double vitalZoneRadius);
int libballistics_getMaxPBR(ballistics_ctx_t context, int zeroRange, double vitalZoneRadius);

double libballistics_computeEnergy (double velocity, double bulletWeight);

#endif /* __LIBBALLISTICS_H_ */
