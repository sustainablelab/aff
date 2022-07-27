#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "aff.h"

int main()
{
    { // A quick lib test
        AffPoint A = {.x=1, .y=3};
        AffPoint B = {.x=8, .y=1};
        /* AffSeg AB = {.A=A, .B=B}; */
        /* printf("AffSeg (oriented side) AB: { [%0.1f,%0.1f], [%0.1f,%0.1f]}", AB.A.x, AB.A.y, AB.B.x, AB.B.y); */
        AffPoint C = {.x=5, .y=9};
        AffOrT ABC = {.A=A, .B=B, .C=C};
        { printf("AffOrT (oriented triangle) ABC:\n"
                "A: [%0.1f,%0.1f]\n"
                "B: [%0.1f,%0.1f]\n"
                "C: [%0.1f,%0.1f]\n",
                ABC.A.x, ABC.A.y,
                ABC.B.x, ABC.B.y,
                ABC.C.x, ABC.C.y);
        }
        { puts("Demonstrate the triangle cross product theorem:");
            printf("Oriented Triangle ABC has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT(ABC));
            AffOrT BCA = {.A=B, .B=C, .C=A};
            printf("Oriented Triangle BCA has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT(BCA));
            AffOrT CAB = {.A=C, .B=A, .C=B};
            printf("Oriented Triangle CAB has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT(CAB));
        }
        { puts("Demonstrate alternate formula for signed area also works:");
            printf("Oriented Triangle ABC has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT_method2(ABC));
            AffOrT BCA = {.A=B, .B=C, .C=A};
            printf("Oriented Triangle BCA has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT_method2(BCA));
            AffOrT CAB = {.A=C, .B=A, .C=B};
            printf("Oriented Triangle CAB has signed area: ");
            printf("%0.2f\n", aff_sarea_OrT_method2(CAB));
        }
    }
    puts("Now explore questions about signed area.");
    // Given an AffPoint A and an oriented polygon P,
    // 1. Sum the signed areas of all oriented TRIANGLES ABC
    // where BC is each oriented side in P.
    //
    // Does this sum seem to depend on whether A is inside or
    // outside P? Nope. Signed area is always -8.
    //
    // Not a surprise: I made a rectangle. And the direction I
    // chose made area negative.
    //
    // This was an anticipation of the lecture here about the
    // Meister formula:
    //
    // https://www.youtube.com/watch?v=f81crFAj5UE&list=PLzdiPTrEWyz4rKFN541wFKvKPSg5Ea6XB&index=9
    //
    // The punchline is that the sum of signed areas of all
    // oriented sides of the polygon is also the signed area of
    // the polygon, and this ends up being the "smaller" formula.
    // The proof is that taking the sum of signed areas of
    // triangles method, then writing out each of those terms as
    // the sum of the signed areas of its oriented sides, there
    // are 12 terms, but eight of them cancel because they are
    // the same sides, just opposite orientations. These are the
    // eight sides where the point A is one of the endpoints. In
    // other words, point A drops out of the picture -- the
    // signed area is agnostic to the choice of A. The resulting
    // formula is simply the sum of signed areas of the oriented
    // sides of the polygon.
    //
    // 2. Sum the signed areas of all oriented SIDES AB where B
    // is each point in P.
    //
    // This is a different idea from 1. This is NOT the area of
    // the polygon because it does not include any of the
    // "canceling" oriented sides and it does not include the
    // oriented sides of the polygon itself! I honestly don't
    // know what to expect from this sum, but I know it is NOT
    // going to be the same as 1.
    //
    // Is this sum the same as the sum from question 1? No.
    //
    // Does this sum seem to depend on whether A is inside or
    // outside P? No... I don't know what it depends on.
    // TODO: make this into an interactive simulation where I can
    // move the mouse around to set the point and I can watch the
    // signed area update -- maybe draw the triangles too
    //
    // 3. Insert A into P. <-- THIS IS THE MOST PROMISING IDEA
    // SO FAR
    //
    // The basic idea sounds obvious -- think of P like an
    // ameoba. Introduce a point A. Let A define a new vertex on
    // the ameoba. If A was inside the ameoba area, the area is
    // now smaller. If A was outsides, the area is now bigger.
    //
    // But it makes a huge difference where A is inserted.
    // Remember that the ordering of points determines how the
    // sides of the polygon are drawn.
    //
    // The location that is easiest to code is to put A as the
    // last point (not the last point in the array that is a copy
    // of the first point, but the point just before that final
    // copied value). So that is how I've done it in my tests.
    //
    // Now the polygon has an additional two oriented sides.
    // Compute the signed area of old P and new P.
    //
    // I suspect if A is inside P, then s(old P) > s(new P)
    // Similarly, if A is outside P, then s(new P) > s(old P)
    // And if A happens to lie on a side of old P, then s(new P) == s(old P)
    //
    // That is just formally stating my ameoba picture.
    //
    // From my tests, the above seems true! But only if inserting
    // A does not induce a twist.
    //
    // Inserting A at an arbitrary location in the order of
    // points of P can result in a twisted polygon.
    //
    // For example, if A is outside the original polygon, then
    // depending on where it is placed in the order of the
    // points, the new polygon will either be not twisted, in
    // which case it has a bigger signed area, or it will be
    // twisted, in which case the signed area is reduced (though
    // I don't know if a reduction in signed area is guaranteed).
    //
    // A simple example: induce a twist in the rectangle. Twist
    // the simple rectangle into an hourglass. (Twist by
    // reordering the points in P.)
    //
    // There are two ways to twist: imagine grabbing the long
    // side and twisting it, then the other way, imagine grabbing
    // the short side and twisting it. It results in two
    // different hourglass shapes.
    //
    // The resulting two triangles are the same size and oriented
    // in opposite directions. So the two oriented triangle
    // signed areas should cancel.
    //
    // And in fact, calculating this by just the sum of signed
    // areas of oriented sides and expressing the twist by
    // ordering the polygon points to induce the two twists I
    // described, both signed areas are zero!
    //
    // That is an exciting result.
    //
    // But it also puts a kink in my idea for determining
    // inside/outside.
    //
    // Twists cause flips in the polarity of the signed area.
    // Twists also remove area that used to be inside the shape.
    //
    // Then I cannot insist that s(old P) > s(new P) means A
    // is inside. I have examples of A outside inducing a twist
    // and resulting in s(old P) > s(new P).
    //
    // So no twists. How do I guarantee I inserted A at a
    // location that does not induce a twist?
    //
    // 3b. One idea is to find the side closest to A and insert A
    // between the two points that define that side. That should
    // guarantee that A does not cause a twist.
    //
    // Note this is NOT the same as finding the two points
    // closest to A. Imagine A is just above a side where one
    // endpoint happens to be very far away. A is not close to
    // both endpoints (because that's who I set this situation
    // up) and yet it is closest to this side.
    //
    // So this solution requires having a way to find the
    // distance between A and every side to decide where to
    // insert A.
    //
    // For that matter, what if I just find the two points A is
    // closest to. Is that enough to tell me if A is inside or
    // outside? No, just imagine mirroring the polygon about the
    // side that A is closest to. That operation does not alter
    // the side in question, but if A was inside it is now
    // outside and vice versa.
    //  
    // Can signed area rescue me once again? Find the signed area
    // of every oriented triangle that uses A as one of the
    // points. This is exactly what I did earlier in idea 1. But
    // this time don't sum them! Now I just want to know which
    // area has the smallest magnitude. I propose that the area
    // with the smallest magnitude is the side that A is closest
    // to.

    // Make a polygon
    // Define points walking clockwise (yields positive signed area)
    int NPOINTS = 5;
    AffPoint *poly = malloc(sizeof(AffPoint)*NPOINTS);
    poly[0] = (AffPoint){ 0, 0};
    poly[1] = (AffPoint){ 2, 0};
    poly[2] = (AffPoint){ 2, 4};
    poly[3] = (AffPoint){ 0, 4};
    poly[4] = poly[0];
    puts("\nPolygon:\n");
    for(int i=0; i<NPOINTS; i++)
    {
        printf("poly[%d]: [%5.1f,%5.1f]\n",i, poly[i].x,poly[i].y);
    }
    puts("");
    printf("Signed area \"aff_sarea_poly()\": %4.1f\n\n", aff_sarea_poly(poly, NPOINTS));
    // Make some points inside and outside
    AffPoint A = { 1.0, 1.0};                                   // inside
    AffPoint B = { 1.5, 2.1};                                   // inside
    AffPoint C = { 3.0, 1.0};                                   // outside
    AffPoint D = { 1.0, 5.0};                                   // outside
    AffPoint E = {-1.0, 2.0};                                   // outside
    AffPoint F = {-2.0,-2.0};                                   // outside
    AffPoint G = { 2.0, 2.0};                                   // inside
    AffPoint I = { 1.9, 3.9};                                   // inside
    AffPoint J = { 1.9, 0.1};                                   // inside
    AffPoint K = { 1.0,-1.0};                                   // outside
    AffPoint L = { 0.0, 1.0};                                   // on a side
    #define H 11
    AffPoint p[H] = {A, B, C, D, E, F, G, I, J, K, L};
    bool p_inside[H] = {true, true, false, false, false, false, true, true, true, false, true};
    const char *p_label[H] = {"A", "B", "C", "D", "E", "F", "G", "I", "J", "K", "L"};
    float p_s[H];
    if(0)
    { // Idea 1: always get the same signed area
        for(int h=0; h<H; h++)
        {
            printf("Point %s is at [%4.1f,%4.1f] (%s)\n", p_label[h], p[h].x, p[h].y, p_inside[h]?"inside":"outside");
            printf("\tCalc signed area of all oriented triangles %sxy\n", p_label[h]);
            puts("\t(xy is each oriented side in polygon P)");
            float s_total = 0;
            for(int i=0; i<NPOINTS-1; i++)
            {
                AffOrT Axy = {.A=p[h], .B=poly[i], .C=poly[i+1]};
                float s = aff_sarea_OrT(Axy);
                float s2 = aff_sarea_OrT_method2(Axy);
                assert( s==s2 );
                printf("\tsA%d%d: %4.1f\n",i,i+1,s);
                s_total += s;
            }
            printf("Total signed area for %s: %4.1f\n\n", p_label[h], s_total);
        }
    }
    if(0)
    { // Idea 2
        for(int h=0; h<H; h++)
        {
            printf("\nPoint %s: [%4.1f,%4.1f]\n", p_label[h], p[h].x, p[h].y);
            printf("\tCalc signed area of all oriented sides %sP\n", p_label[h]);
            puts("\t(P is each point in polygon P)");
            float s_total = 0;
            AffVec u,v;
            for(int i=0; i<NPOINTS-1; i++)
            {
                u = p[h]; v = poly[i];
                float s = 0.5*(u.x*v.y - v.x*u.y);
                printf("\tsigned area of oriented side %s%d: %4.1f\n",
                        p_label[h], i, s);
                s_total += s;
            }
            printf("\tTotal signed area of oriented sides with (%s)%s: %4.1f\n",
                    p_inside[h]?"inside":"outside", p_label[h], s_total);
            p_s[h] = s_total;
        }
        puts("Point | [ x , y ] | In or Out | Total Signed Area");
        puts("----- | --------- | --------- | -----------------");
        for(int h=0; h<H; h++)
        {
            printf("%5s | %4.1f,%4.1f | %9s | %4.1f\n",
                    p_label[h], p[h].x, p[h].y,
                    p_inside[h]?"inside":"outside", p_s[h]);
        }
    }
    { // Idea 3
        puts("[ x , y ] | s(old P) | s(new P) | old >=new?| In or Out");
        puts("--------- | -------- | -------- | --------- | ---------");
        for(int h=0; h<H; h++)
        {
            AffPoint *poly_aug = malloc(sizeof(AffPoint)*(NPOINTS+1));
            poly_aug[0] = poly[0];
            poly_aug[1] = poly[1];
            poly_aug[2] = poly[2];
            poly_aug[3] = poly[3];
            poly_aug[4] = p[h];
            poly_aug[5] = poly[0];
            float old_s = aff_sarea_poly(poly, NPOINTS);
            float new_s = aff_sarea_poly(poly_aug, NPOINTS+1);
            printf("%4.1f,%4.1f | %8.1f | %8.1f | %9s | %9s\n",
                    p[h].x, p[h].y, old_s, new_s,
                    (old_s >= new_s) ? "true":"false",
                    p_inside[h]?"inside":"outside");
            free(poly_aug);
        }
    }
    { // Twists
        puts("Area of hourglass (reorder rectangle points to induce a twist)");
        AffPoint *poly_twist = malloc(sizeof(AffPoint)*NPOINTS);
        // Long side twist by swapping the order of index 1 and 2
        poly_twist[0] = (AffPoint){ 0, 0};
        poly_twist[1] = (AffPoint){ 2, 4};
        poly_twist[2] = (AffPoint){ 2, 0};
        poly_twist[3] = (AffPoint){ 0, 4};
        poly_twist[4] = poly[0];
        printf("Long side twist: %4.1f\n",aff_sarea_poly(poly_twist, NPOINTS));
        // Short side twist by swapping the order of index 1 and 3, then new index 1 and 2
        poly_twist[0] = (AffPoint){ 0, 0};
        poly_twist[1] = (AffPoint){ 2, 4};
        poly_twist[2] = (AffPoint){ 0, 4};
        poly_twist[3] = (AffPoint){ 2, 0};
        poly_twist[4] = poly[0];
        printf("Short side twist: %4.1f\n",aff_sarea_poly(poly_twist, NPOINTS));
        free(poly_twist);
    }
    free(poly);
    return 0;
}
