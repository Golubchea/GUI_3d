//R is 3x3 array
#include <matrix/matrix.h>
#include <stdbool.h>
#include <math.h>
#include <matrix/Euler_angles.h>
#include <vec3.h>

MAT_API void euler_to_mat3_double(double M[3][3], vec3 e )
{


    double cosX=cos(e.x);
    double cosY=cos(e.y);
    double cosZ=cos(e.z);

    double sinX=sin(e.x);
    double sinY=sin(e.y);
    double sinZ=sin(e.z);


    double RX[3][3];
    matrix_identity(RX,3);
    RX[1][1]=cosX;
    RX[1][2]=-sinX;
    RX[2][1]=sinX;
    RX[2][2]=cosX;


    double RY[3][3];
    matrix_identity(RY,3);
    RY[0][0]=cosY;
    RY[0][2]=sinY;
    RY[2][0]=-sinY;
    RY[2][2]=cosY;

    double RZ[3][3];
    matrix_identity(RZ,3);
    RZ[0][0]=cosZ;
    RZ[0][1]=-sinZ;
    RZ[1][0]=sinZ;
    RZ[1][1]=cosZ;

    double temp_res [3][3];
    matrix_mul_matrix(RZ,RY,temp_res,3,3,3,3);

    matrix_mul_matrix( temp_res,RX,M,3,3,3,3);

}


MAT_API void eulO_to_mat3(float M[3][3], vec3 e )
{
    //const RotOrderInfo *R = get_rotation_order_info(order);
    short i = 0, j = 2, k = 1;
    double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;



    ti = e.x;
    tj = e.y;
    th = e.z;


    ci = cos(ti);
    cj = cos(tj);
    ch = cos(th);
    si = sin(ti);
    sj = sin(tj);
    sh = sin(th);

    cc = ci * ch;
    cs = ci * sh;
    sc = si * ch;
    ss = si * sh;

    M[i][i] = (float)(cj * ch);
    M[j][i] = (float)(sj * sc - cs);
    M[k][i] = (float)(sj * cc + ss);
    M[i][j] = (float)(cj * sh);
    M[j][j] = (float)(sj * ss + cc);
    M[k][j] = (float)(sj * cs - sc);
    M[i][k] = (float)(-sj);
    M[j][k] = (float)(cj * si);
    M[k][k] = (float)(cj * ci);
}




void twoaxisrot(double r11, double r12, double r21, double r31, double r32, double * res )
{
  res[0] = atan2( r11, r12 );
  res[1] = acos ( r21 );
  res[2] = atan2( r31, r32 );
}

void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double* res )
{
  res[0] = atan2( r31, r32 );
  res[1] = asin ( r21 );
  res[2] = atan2( r11, r12 );
}





void quaternion2Euler(const quat q, double * res, enum RotSeq rotSeq)
{
    switch(rotSeq){
    case zyx:
      threeaxisrot( 2*(q.x*q.y + q.w*q.z),
                     q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                    -2*(q.x*q.z - q.w*q.y),
                     2*(q.y*q.z + q.w*q.x),
                     q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                     res);
      break;

    case zyz:
      twoaxisrot( 2*(q.y*q.z - q.w*q.x),
                   2*(q.x*q.z + q.w*q.y),
                   q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                   2*(q.y*q.z + q.w*q.x),
                  -2*(q.x*q.z - q.w*q.y),
                  res);
      break;

    case zxy:
      threeaxisrot( -2*(q.x*q.y - q.w*q.z),
                      q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                      2*(q.y*q.z + q.w*q.x),
                     -2*(q.x*q.z - q.w*q.y),
                      q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                      res);
      break;

    case zxz:
      twoaxisrot( 2*(q.x*q.z + q.w*q.y),
                  -2*(q.y*q.z - q.w*q.x),
                   q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                   2*(q.x*q.z - q.w*q.y),
                   2*(q.y*q.z + q.w*q.x),
                   res);
      break;

    case yxz:
      threeaxisrot( 2*(q.x*q.z + q.w*q.y),
                     q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                    -2*(q.y*q.z - q.w*q.x),
                     2*(q.x*q.y + q.w*q.z),
                     q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                     res);
      break;

    case yxy:
      twoaxisrot( 2*(q.x*q.y - q.w*q.z),
                   2*(q.y*q.z + q.w*q.x),
                   q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                   2*(q.x*q.y + q.w*q.z),
                  -2*(q.y*q.z - q.w*q.x),
                  res);
      break;

    case yzx:
      threeaxisrot( -2*(q.x*q.z - q.w*q.y),
                      q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                      2*(q.x*q.y + q.w*q.z),
                     -2*(q.y*q.z - q.w*q.x),
                      q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                      res);
      break;

    case yzy:
      twoaxisrot( 2*(q.y*q.z + q.w*q.x),
                  -2*(q.x*q.y - q.w*q.z),
                   q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                   2*(q.y*q.z - q.w*q.x),
                   2*(q.x*q.y + q.w*q.z),
                   res);
      break;

    case xyz:
      threeaxisrot( -2*(q.y*q.z - q.w*q.x),
                    q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                    2*(q.x*q.z + q.w*q.y),
                   -2*(q.x*q.y - q.w*q.z),
                    q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                    res);
      break;

    case xyx:
      twoaxisrot( 2*(q.x*q.y + q.w*q.z),
                  -2*(q.x*q.z - q.w*q.y),
                   q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                   2*(q.x*q.y - q.w*q.z),
                   2*(q.x*q.z + q.w*q.y),
                   res);
      break;

    case xzy:
      threeaxisrot( 2*(q.y*q.z + q.w*q.x),
                     q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                    -2*(q.x*q.y - q.w*q.z),
                     2*(q.x*q.z + q.w*q.y),
                     q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                     res);
      break;

    case xzx:
      twoaxisrot( 2*(q.x*q.z - q.w*q.y),
                   2*(q.x*q.y + q.w*q.z),
                   q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                   2*(q.x*q.z + q.w*q.y),
                  -2*(q.x*q.y - q.w*q.z),
                  res);
      break;
    default:
     printf( "Unknown rotation sequence \n\n");
      break;
   }
}


MAT_API void matrix_mul3x3_vec3_float(vec3 *resultvector, const float *matrix, vec3 * pvector)
{
    resultvector->x=matrix[0]*pvector->x+matrix[1]*pvector->y+matrix[2]*pvector->z;
    resultvector->y=matrix[3]*pvector->x+matrix[4]*pvector->y+matrix[5]*pvector->z;
    resultvector->z=matrix[6]*pvector->x+matrix[7]*pvector->y+matrix[8]*pvector->z;
}


MAT_API void matrix_mul3x3_vec3(vec3 *resultvector, const double *matrix, vec3 * pvector)
{
    resultvector->x=matrix[0]*pvector->x+matrix[1]*pvector->y+matrix[2]*pvector->z;
    resultvector->y=matrix[3]*pvector->x+matrix[4]*pvector->y+matrix[5]*pvector->z;
    resultvector->z=matrix[6]*pvector->x+matrix[7]*pvector->y+matrix[8]*pvector->z;
}

MAT_API void matrix_mul3x3_vec3_in( const double *matrix, vec3   * p )
{
    vec3 pv=vec3_copy(p);
    vec3 *pvector=&pv;
    p->x=matrix[0]*pvector->x+matrix[1]*pvector->y+matrix[2]*pvector->z;
    p->y=matrix[3]*pvector->x+matrix[4]*pvector->y+matrix[5]*pvector->z;
    p->z=matrix[6]*pvector->x+matrix[7]*pvector->y+matrix[8]*pvector->z;
}

MAT_API void Rodrigues(vec3  r , float *Matrix)
{
    //https://ru.wikipedia.org/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D1%83%D0%BB%D0%B0_%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82%D0%B0_%D0%A0%D0%BE%D0%B4%D1%80%D0%B8%D0%B3%D0%B0


    float norm =sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
    r.x=r.x/norm;
    r.y=r.y/norm;
    r.z=r.z/norm;

    float sin_theta=sinf(norm);
    float one_minus_theta=1-cosf(norm);


    float I[9];
    matrix_identity_float(I,3);

    float K[]={0,   -r.z, r.y,
               r.z,  0 , -r.x,
               -r.y, r.x, 0};


    float K_trans[]={0,   -r.z, r.y,
                     r.z,  0 , -r.x,
                     -r.y, r.x, 0};

    float KK_t[9];
    matrix_zero_float(KK_t,9);
    matrix_mul_matrix_float(K,K_trans,KK_t,3,3,3,3);

    matrix_mul_scalar_float(KK_t ,one_minus_theta,3,3);


    matrix_mul_scalar_float(K ,sin_theta,3,3);
    float temp_I_plusKsin_theta[9];
    matrix_zero_float(temp_I_plusKsin_theta,9);
    matrix_add_matrix_float(I,K, temp_I_plusKsin_theta,3,3);
    //main add
    matrix_add_matrix_float( temp_I_plusKsin_theta,KK_t, Matrix,3,3);
}
MAT_API void getEulerYPR(float * yaw,float*pitch, float*roll,float *mat ,unsigned int solution_number)
{
    struct Euler
    {
        float yaw;
        float pitch;
        float roll;
    };

    struct Euler euler_out;
    struct Euler euler_out2; //second solution
    //get the pointer to the raw data

    // Check that pitch is not at a singularity

    //0   1  2    3
    //4   5  6    7
    //8   9 10   11
    //12 13 14   15


    if ( abs(mat[6]) >= 1)
    {
        euler_out.yaw = 0;
        euler_out2.yaw = 0;

        // From difference of angles formula
        if (mat[6] < 0)  //gimbal locked down
        {
            float  delta = atan2f(mat[1],mat[2]);
            euler_out.pitch = M_PI /  2.0f;
            euler_out2.pitch = M_PI / 2.0f;
            euler_out.roll = delta;
            euler_out2.roll = delta;
        }
        else // gimbal locked up
        {
            float   delta = atan2f(-mat[1],-mat[2]);
            euler_out.pitch = M_PI /  2.0f;
            euler_out2.pitch = M_PI / 2.0f;
            euler_out.roll = delta;
            euler_out2.roll = delta;
        }
    }
    else
    {

        //0 1 2
        //3 4 5
        //6 7 8
        euler_out.pitch = - asinf( mat[6]  );
        euler_out2.pitch = M_PI - euler_out.pitch;

        euler_out.roll = atan2f( mat[7]/cos(euler_out.pitch),
                mat[8]/cos(euler_out.pitch));
        euler_out2.roll = atan2f(mat[7]/cos(euler_out2.pitch),
                mat[8]/cos(euler_out2.pitch));

        euler_out.yaw = atan2f(mat[3]/cosf(euler_out.pitch),
                mat[0]/cos(euler_out.pitch));
        euler_out2.yaw = atan2(mat[3]/cos(euler_out2.pitch),
                mat[0]/cos(euler_out2.pitch));
    }

    if (solution_number == 1)
    {
        *yaw = euler_out.yaw;
        *pitch = euler_out.pitch;
        *roll = euler_out.roll;
    }
    else
    {
        *yaw = euler_out2.yaw;
        *pitch = euler_out2.pitch;
        *roll = euler_out2.roll;
    }
}











bool isRotationMatrix(float *R)
{
    float Rt[3*3];
    matrix_transpose2(R,Rt,3,3);

    float shouldBeIdentity[3*3];
    matrix_mul_matrix(Rt,R,shouldBeIdentity,3,3,3,3);

    float I[3*3]={1,0,0,   0,1,0,   0,0,1};


    float res[3*3];
    // R^t*R-E=0
    matrix_substract_matrix(shouldBeIdentity,I,res,3,3);
    //TODO : make norm
    //norm(I, shouldBeIdentity) < 1e-6;

    if(res[0]<1e-6)
    {
        return  1;
    }
    else
    {
        return 0;
    }

}




MAT_API void rotationMatrixToEulerAngles_float(float *Yaw2,float *Pitch2,float *Roll2,float * R  )// (float*)R[4][4] or R[4*4]
{

    // assert(isRotationMatrix(R));
    float Yaw,Pitch,Roll;
    if (R[0*4+0] == 1.0f)
    {
        Yaw = atan2f(R[0*4+2], R[2*4+3] );
        Pitch = 0;
        Roll = 0;

    }else if (R[0*4+0] == -1.0f)
    {
        Yaw = atan2f(R[0*4 +2], R[2*4+3]);
        Pitch = 0;
        Roll = 0;
    }else
    {

        Yaw = atan2f(-R[2*4+0],R[0*4+0]);
        Pitch = asinf(R [1*4+0]);
        Roll = atan2f(-R[1*4+2],R[1*4+1]);
    }


    *Yaw2=Yaw/(float)M_PI *180.0f;
    *Pitch2=Pitch/(float)M_PI *180.0f;
    *Roll2=Roll/(float)M_PI *180.0f;
}
