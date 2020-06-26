#pragma once

#include <math.h>
#include <vec3.h>
#include <vec2.h>


static vec4 compute_unproject_koefficients_x(float *p,float *m)
{
vec4 a=vec4_create(0,0,0,0);
a.vals[3]=m[12]*p[0] + m[13]*p[4] + m[14]*p[8] + m[15]*p[12];
a.vals[0]=(m[0]*p[0] + m[1]*p[4] + m[2]*p[8] + m[3]*p[12]);
a.vals[1]=(m[4]*p[0] + m[5]*p[4] + m[6]*p[8] + m[7]*p[12]);
a.vals[2]=(m[10]*p[8] + m[11]*p[12] + m[8]*p[0] + m[9]*p[4]);
return a;
}


static vec4 compute_unproject_koefficients_y(float *p,float *m)
{
    vec4 a=vec4_create(0,0,0,0);
 a.vals[3]=m[12]*p[1] + m[13]*p[5] + m[14]*p[9] + m[15]*p[13] ;
 a.vals[0]=m[0]*p[1] + m[1]*p[5] + m[2]*p[9] + m[3]*p[13] ;
 a.vals[1]=m[4]*p[1] + m[5]*p[5] + m[6]*p[9] + m[7]*p[13] ;
 a.vals[2]=m[10]*p[9] + m[11]*p[13] + m[8]*p[1] + m[9]*p[5];


    return a;
}

static vec2 project3dto2dFast2(vec3 obj, vec2 window_size,float *modelview,vec4 *kx,vec4 *ky)
{

    float x=kx->vals[0]*obj.x+kx->vals[1]*obj.y+kx->vals[2]*obj.z+kx->vals[3]; // w is always 1
    float y=ky->vals[0]*obj.x+ky->vals[1]*obj.y+ky->vals[2]*obj.z+ky->vals[3];

   float denom=1.0/(-1.0*(modelview[2]*obj.x+modelview[6]*obj.y+modelview[10]*obj.z+modelview[14]));


      x*=denom;
      y*=denom;

      vec2 v=vec2_create(0,0);
    v.x=(x*0.5+0.5)* window_size.x;
    v.y=(-y*0.5+0.5)* window_size.y;
    return v;
}

static void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
{
    result[0]=matrix1[0]*matrix2[0]+
            matrix1[4]*matrix2[1]+
            matrix1[8]*matrix2[2]+
            matrix1[12]*matrix2[3];
    result[4]=matrix1[0]*matrix2[4]+
            matrix1[4]*matrix2[5]+
            matrix1[8]*matrix2[6]+
            matrix1[12]*matrix2[7];
    result[8]=matrix1[0]*matrix2[8]+
            matrix1[4]*matrix2[9]+
            matrix1[8]*matrix2[10]+
            matrix1[12]*matrix2[11];
    result[12]=matrix1[0]*matrix2[12]+
            matrix1[4]*matrix2[13]+
            matrix1[8]*matrix2[14]+
            matrix1[12]*matrix2[15];
    result[1]=matrix1[1]*matrix2[0]+
            matrix1[5]*matrix2[1]+
            matrix1[9]*matrix2[2]+
            matrix1[13]*matrix2[3];
    result[5]=matrix1[1]*matrix2[4]+
            matrix1[5]*matrix2[5]+
            matrix1[9]*matrix2[6]+
            matrix1[13]*matrix2[7];
    result[9]=matrix1[1]*matrix2[8]+
            matrix1[5]*matrix2[9]+
            matrix1[9]*matrix2[10]+
            matrix1[13]*matrix2[11];
    result[13]=matrix1[1]*matrix2[12]+
            matrix1[5]*matrix2[13]+
            matrix1[9]*matrix2[14]+
            matrix1[13]*matrix2[15];
    result[2]=matrix1[2]*matrix2[0]+
            matrix1[6]*matrix2[1]+
            matrix1[10]*matrix2[2]+
            matrix1[14]*matrix2[3];
    result[6]=matrix1[2]*matrix2[4]+
            matrix1[6]*matrix2[5]+
            matrix1[10]*matrix2[6]+
            matrix1[14]*matrix2[7];
    result[10]=matrix1[2]*matrix2[8]+
            matrix1[6]*matrix2[9]+
            matrix1[10]*matrix2[10]+
            matrix1[14]*matrix2[11];
    result[14]=matrix1[2]*matrix2[12]+
            matrix1[6]*matrix2[13]+
            matrix1[10]*matrix2[14]+
            matrix1[14]*matrix2[15];
    result[3]=matrix1[3]*matrix2[0]+
            matrix1[7]*matrix2[1]+
            matrix1[11]*matrix2[2]+
            matrix1[15]*matrix2[3];
    result[7]=matrix1[3]*matrix2[4]+
            matrix1[7]*matrix2[5]+
            matrix1[11]*matrix2[6]+
            matrix1[15]*matrix2[7];
    result[11]=matrix1[3]*matrix2[8]+
            matrix1[7]*matrix2[9]+
            matrix1[11]*matrix2[10]+
            matrix1[15]*matrix2[11];
    result[15]=matrix1[3]*matrix2[12]+
            matrix1[7]*matrix2[13]+
            matrix1[11]*matrix2[14]+
            matrix1[15]*matrix2[15];
}

static void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
{
    resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
    resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
    resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
    resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

// This code comes directly from GLU except that it is for float
static  int glhInvertMatrixf2(float *m, float *out)
{
    float wtmp[4][8];
    float m0, m1, m2, m3, s;
    float *r0, *r1, *r2, *r3;
    r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
    r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
            r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
            r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
            r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
            r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
            r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
            r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
            r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
            r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
            r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
            r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
            r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
    /* choose pivot - or die */
    if (fabsf(r3[0]) > fabsf(r2[0]))
        SWAP_ROWS_FLOAT(r3, r2);
    if (fabsf(r2[0]) > fabsf(r1[0]))
        SWAP_ROWS_FLOAT(r2, r1);
    if (fabsf(r1[0]) > fabsf(r0[0]))
        SWAP_ROWS_FLOAT(r1, r0);
    if (0.0 == r0[0])
        return 0;
    /* eliminate first variable */
    m1 = r1[0] / r0[0];
    m2 = r2[0] / r0[0];
    m3 = r3[0] / r0[0];
    s = r0[1];
    r1[1] -= m1 * s;
    r2[1] -= m2 * s;
    r3[1] -= m3 * s;
    s = r0[2];
    r1[2] -= m1 * s;
    r2[2] -= m2 * s;
    r3[2] -= m3 * s;
    s = r0[3];
    r1[3] -= m1 * s;
    r2[3] -= m2 * s;
    r3[3] -= m3 * s;
    s = r0[4];
    if (s != 0.0) {
        r1[4] -= m1 * s;
        r2[4] -= m2 * s;
        r3[4] -= m3 * s;
    }
    s = r0[5];
    if (s != 0.0) {
        r1[5] -= m1 * s;
        r2[5] -= m2 * s;
        r3[5] -= m3 * s;
    }
    s = r0[6];
    if (s != 0.0) {
        r1[6] -= m1 * s;
        r2[6] -= m2 * s;
        r3[6] -= m3 * s;
    }
    s = r0[7];
    if (s != 0.0) {
        r1[7] -= m1 * s;
        r2[7] -= m2 * s;
        r3[7] -= m3 * s;
    }
    /* choose pivot - or die */
    if (fabsf(r3[1]) > fabsf(r2[1]))
        SWAP_ROWS_FLOAT(r3, r2);
    if (fabsf(r2[1]) > fabsf(r1[1]))
        SWAP_ROWS_FLOAT(r2, r1);
    if (0.0 == r1[1])
        return 0;
    /* eliminate second variable */
    m2 = r2[1] / r1[1];
    m3 = r3[1] / r1[1];
    r2[2] -= m2 * r1[2];
    r3[2] -= m3 * r1[2];
    r2[3] -= m2 * r1[3];
    r3[3] -= m3 * r1[3];
    s = r1[4];
    if (0.0 != s) {
        r2[4] -= m2 * s;
        r3[4] -= m3 * s;
    }
    s = r1[5];
    if (0.0 != s) {
        r2[5] -= m2 * s;
        r3[5] -= m3 * s;
    }
    s = r1[6];
    if (0.0 != s) {
        r2[6] -= m2 * s;
        r3[6] -= m3 * s;
    }
    s = r1[7];
    if (0.0 != s) {
        r2[7] -= m2 * s;
        r3[7] -= m3 * s;
    }
    /* choose pivot - or die */
    if (fabsf(r3[2]) > fabsf(r2[2]))
        SWAP_ROWS_FLOAT(r3, r2);
    if (0.0 == r2[2])
        return 0;
    /* eliminate third variable */
    m3 = r3[2] / r2[2];
    r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
            r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
    /* last check */
    if (0.0 == r3[3])
        return 0;
    s = 1.0 / r3[3];     /* now back substitute row 3 */
    r3[4] *= s;
    r3[5] *= s;
    r3[6] *= s;
    r3[7] *= s;
    m2 = r2[3];          /* now back substitute row 2 */
    s = 1.0 / r2[2];
    r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
            r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
    m1 = r1[3];
    r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
            r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
    m0 = r0[3];
    r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
            r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
    m1 = r1[2];          /* now back substitute row 1 */
    s = 1.0 / r1[1];
    r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
            r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
    m0 = r0[2];
    r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
            r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
    m0 = r0[1];          /* now back substitute row 0 */
    s = 1.0 / r0[0];
    r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
            r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
    MAT(out, 0, 0) = r0[4];
    MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
    MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
    MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
    MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
    MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
    MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
    MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
    MAT(out, 3, 3) = r3[7];
    return 1;
}


static vec2 project3dto2d(vec3 obj, vec2 window_size,float *modelview, float *projection)
{
    // Transformation vectors
    float fTempo[8];
    // Modelview transform
    fTempo[0]=modelview[0]*obj.x+modelview[4]*obj.y+modelview[8]*obj.z+modelview[12]; // w is always 1
    fTempo[1]=modelview[1]*obj.x+modelview[5]*obj.y+modelview[9]*obj.z+modelview[13];
    fTempo[2]=modelview[2]*obj.x+modelview[6]*obj.y+modelview[10]*obj.z+modelview[14];
    fTempo[3]=modelview[3]*obj.x+modelview[7]*obj.y+modelview[11]*obj.z+modelview[15];
    // Projection transform, the final row of projection matrix is always [0 0 -1 0]
    // so we optimize for that.
    fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
    fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
    fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
    fTempo[7]=-fTempo[2];
    // The result normalizes between -1 and 1
     vec2 v=vec2_create(0,0);
    if(fTempo[7]==0.0) // The w value
        return v;
    fTempo[7]=1.0/fTempo[7];
    // Perspective division
    fTempo[4]*=fTempo[7];
    fTempo[5]*=fTempo[7];
    fTempo[6]*=fTempo[7];
    // Window coordinates
    // Map x, y to range 0-1
    //  windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
    //  windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];
    // This is only correct when glDepthRange(0.0, 1.0)
    // windowCoordinate[2]=(1.0+fTempo[6])*0.5;  // Between 0 and 1

     ;
    v.x=(fTempo[4]*0.5+0.5)* window_size.x;
    v.y=(-fTempo[5]*0.5+0.5)* window_size.y;
    return v;
}
//#include <glad.h>


static vec3 unProject2dto3d(vec2 mouse,vec2 window_size, float *modelview, float *projection)
{

    vec3 win_coords=vec3_create(mouse.x,window_size.y-mouse.y,.99);

    float viewport[4];
    viewport[0]=0;
    viewport[1]=0;
    viewport[2]=window_size.x ;
    viewport[3]=window_size.y;


    // Transformation matrices
    float m[16], A[16];
    float in[4], out[4];
    // Calculation for inverting a matrix, compute projection x modelview
    // and store in A[16]
    MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
    // Now compute the inverse of matrix A
    vec3 v=vec3_create(0,0,0);
    if(glhInvertMatrixf2(A, m)==0)
       return v;
    // Transformation of normalized coordinates between -1 and 1
    in[0]=( win_coords.x- viewport[0])/ viewport[2]*2.0-1.0;
    in[1]=( win_coords.y- viewport[1])/ viewport[3]*2.0-1.0;
    in[2]=2.0* win_coords.z-1.0;
    in[3]=1.0;
    // Objects coordinates
    MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
    if(out[3]==0.0)
        return v;
    out[3]=1.0/out[3];
     v =vec3_create(out[0]*out[3],   out[1]*out[3],   out[2]*out[3]);
   // v.norm_vector();
    return v;
}

