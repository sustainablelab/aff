#ifndef __AFF_H__
#define __AFF_H__

/* *************Affine geometry***************
 * Series: WildLinAlg: A geometric course in Linear Algebra
 *
 * - WildLinAlg10: Equations of lines and planes in 3D
 *
 * Series: WildTrig: Intro to Rational Trigonometry
 *
 * - WildTrig12: Cartesian coordinates and geometry
 * - WildTrig42: An algebraic framework for rational trigonometry (I)
 *
 * Series: Wild Egg Maths Algebraic Calculus One: 
 *
 * - Points and Lines in the Affine Plane
 * - Projective Coordinates for Points and Lines
 *
 * *******************************/
/* *************Types***************
 * Object   :   Notation            :   Interpretation
 * ------   :   --------            :   --------------
 * AffPoint :   { x1, y1 }          :
 *          :                       :
 * AffLine  :   { a, b, c }         :   ax + by = c
 *          :                       :   - "infinite" length
 *          :                       :   - fixed origin (has notion of colinearity)
 *          :                       :   - no direction
 *          :                       :
 * AffVec   :   {   α  ,   β   }    :   vector vaB = (αx, βy)
 *          :   { x2-x1, y2-y1 }    :   - finite length
 *          :                       :   - no fixed origin (no notion of colinearity)
 *          :                       :   - has direction
 *          :                       :
 * AffSeg   :   {A{x1,y1}, B{x2,y2}}:   directed line segment sAB is a line from A to B
 *          :                       :   - finite length
 *          :                       :   - fixed position (A and B are colinear with sAB)
 *          :                       :   - has direction:
 *          :                       :       - direction from A to B is { x2-x1, y2-y1 }
 *          :                       :       - adding this α,β to x1,y1 yields x2,y2
 *          :                       :
 *
 * *******************************/
/* *************Definitions***************
 * AffPoint A lies on AffLine l         <=> a*x1 + b*y1 = c
 * AffLines l1 and l2 are parallel      <=> a1*b2 - a2*b1 = 0
 * AffLines l1 and l2 are perpendicular <=> a1*a2 + b1*b2 = 0
 * JoinOfPoints 1 (AffVec)  : AffPoints A and B form directed line segment sAB
 * JoinOfPoints 2 (AffLine) : AffPoints A and B define line l
 * *******************************/
/* *************Recipes***************
 * 1. Find AffLine l given AffPoints A and B
 *
 * Inputs
 * ------
 * AffPoint A = {x1,y1}
 * AffPoint B = {x2,y2}
 *
 * Intermediate Values
 * -------------------
 * AffVec vAB = {x2-x1,y2-y1}
 *
 * Output
 * ------
 * AffLine l = {-β, α, c}
 * β = y2-y1
 * α = x2-x1
 * c = -β*x1 + α*y1
 *
 * Closed Form
 * -----------
 * Affline l = {-1*(y2-y1), (x2-x1), -1*(y2-y1)*x1 + (x2-x1)*y1}
 * *******************************/


#ifndef bool
#include <stdbool.h>
#endif

// Aliases to use math terminology with SDL API
#ifndef SDL
typedef struct
{
    float x,y;
} AffPoint;                                                     // point
#else
typedef SDL_FPoint AffPoint;
#endif
typedef AffPoint AffVec;                                        // vector

AffVec aff_vec_from_points(AffPoint A, AffPoint B)
{ // Return vector AB (the vector that goes from A to B)
    return (AffPoint){B.x-A.x, B.y-A.y};
}

typedef struct
{
    AffPoint A, B;
} AffSeg;
// Alias affine segments as oriented sides
typedef AffSeg AffOrS;                                          // oriented side

typedef struct
{
    AffPoint A, B, C;
} AffOrT;                                                       // oriented triangle

// Keep oriented triangles or just use polygons for everything?
// One reason to keep triangles is I can avoid malloc and size is
// known (always three points)
//
// If I keep oriented triangles, keep one of the following two
// functions
//
// 1. Define signed area as cross product (u)x(v)
float aff_sarea_OrT(AffOrT ABC)
{ // Signed area of oriented triangle ABC
    // (u)x(v) = (AB)x(AC)
    // Construct vec AB and AC
    AffVec u = aff_vec_from_points(ABC.A, ABC.B);
    AffVec v = aff_vec_from_points(ABC.A, ABC.C);
    float s = 0.5*(u.x*v.y - v.x*u.y);                          // 1/2(ad-bc)
    return s;
}
// 2. Define signed area as sum of signed areas s(AB) + s(BC) + s(CA)
float aff_sarea_OrT_method2(AffOrT ABC)
{ // Signed area of oriented triangle ABC
    // Construct oriented sides
    AffSeg AB = {.A=ABC.A, .B=ABC.B};
    AffSeg BC = {.A=ABC.B, .B=ABC.C};
    AffSeg CA = {.A=ABC.C, .B=ABC.A};
    // No vectors to construct... O is [0,0] so vectors look like points
    // Vector OA is just point A, OB is just B, and OC is just C
    AffVec u,v;                                                 // Take two vectors at a time
    u = AB.A; v = AB.B; float sAB = 0.5*(u.x*v.y - v.x*u.y);    // 1/2*(OA)x(OB)
    u = BC.A; v = BC.B; float sBC = 0.5*(u.x*v.y - v.x*u.y);    // 1/2*(OB)x(OC)
    u = CA.A; v = CA.B; float sCA = 0.5*(u.x*v.y - v.x*u.y);    // 1/2*(OC)x(OA)
    return (sAB+sBC+sCA);
}

float aff_sarea_poly(AffPoint *poly, int n)
{ // Signed area of polygon with n points
    /* *************DOC***************
     * Uses definition of signed area of a polygon as the sum of
     * the signed areas of each oriented side.
     *
     * The order of the points (clockwise or counter clockwise)
     * affects the sign of the signed area.
     *
     * clockwise            : signed area is positive
     * counter-clockwise    : signed area is negative
     * *******************************/
    float s = 0;                                                // Total signed area
    AffVec u,v;
    for(int i=0; i<(n-1); i++)
    {
        u = poly[i]; v = poly[i+1];
        s += 0.5*(u.x*v.y - v.x*u.y);
    }
    return s;
}

/* *************DOC***************
 * STUFF BELOW HERE IS NOT USED
 * *******************************/
typedef struct
{
    float a,b,c;
} AffLine;                                                      // line (infinite extent)

AffLine aff_join_of_points(AffPoint A, AffPoint B)
{ // Return join of points A and B
    float alpha = B.x-A.x; float beta = B.y-A.y;
    float c = -1*beta*A.x + alpha*A.y;
    AffLine l = {-1*beta, alpha, c};
    return l;
}

/* bool do_once = true; */
/* int cnt_down = 20; */
AffPoint aff_meet_of_lines(AffLine l1, AffLine l2)
{ // Return meet of lines l1 and l2
    float a1 = l1.a; float b1 = l1.b; float c1 = l1.c;
    float a2 = l2.a; float b2 = l2.b; float c2 = l2.c;
    float det = 1/(a1*b2 - a2*b1);
    float x = det*(b2*c1 - b1*c2);
    float y = det*(a1*c2 - a2*c1);
    /* if(do_once) */
    /* { */
    /*     printf("det: %f, x: %f, y: %f\n", det, x, y); */
    /*     cnt_down--; if(cnt_down==0) do_once = false; */
    /* } */
    AffPoint M = {x,y};
    return M;
}

bool aff_point_on_seg(AffPoint M, AffSeg seg)
{
    // First establish that lines AB and AM are parallel
    AffLine line_AB = aff_join_of_points(seg.A, seg.B);
    AffLine line_AM = aff_join_of_points(seg.A, M);
    float a1 = line_AB.a; float b1 = line_AB.b;
    float a2 = line_AM.a; float b2 = line_AM.b;
    if(  (a1*b2 - a2*b1)!=0  ) { return false; }
    // So they are parallel -- now check the scaling factor lambda for the two vectors
    AffVec vec_AB = aff_vec_from_points(seg.A, seg.B);
    AffVec vec_AM = aff_vec_from_points(seg.A, M);
    if(  (vec_AB.x == 0) && (vec_AB.y == 0)  ) // vec.x and vec.y cannot both be zero!
    { // Both vector components are zero -- it is meaningless to ask if point is on seg
        return false;
    }
    // Find lambda (the scaling between the two vectors)
    float lambda;
    if(  vec_AB.x != 0  )   { lambda = vec_AM.x / vec_AB.x; }   // Use vec.x if non-zero
    else                    { lambda = vec_AM.y / vec_AB.y; }   // Use vec.y if vec.x is 0
    if(  lambda < 0  ) {lambda = -1*lambda;}                    // Make sure lambda is positive
    if(  lambda > 1  ) { return false;}                         // lambda > 1 -- point is off line seg
    else { return true; }                                       // lambda <=1 -- point is on line seg
}

#endif // __AFF_H__

