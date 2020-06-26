#include <vec3.h>
#include <matrix/matrix.h>
#include <stdbool.h>
#include <math.h>


MAT_API  void matrix_rotate4x4_around_axis_float(float * srcmat4x4 ,vec3 u,float  angle)
{
    float rads=( angle/180.f)*M_PI;
    float rotx [16];

    matrix_identity_float(rotx,4);

    rotx[0]= cosf(rads)+u.x*u.x*(1-cosf(rads));
    rotx[1]= u.y*u.x*(1-cosf(rads)) +u.z* sinf(rads)  ;
    rotx[2]= u.z*u.x*(1-cosf(rads)) -u.y* sinf(rads)  ;
    //3
    rotx[4]= u.x*u.y*(1-cosf(rads)) -u.z* sinf(rads)  ;
    rotx[5]=  cosf(rads)  +u.y*u.y*(1-cosf(rads))  ;
    rotx[6]= u.z*u.y*(1-cosf(rads)) +u.x* sinf(rads)  ;
//7
    rotx[8]= u.x*u.z*(1-cosf(rads)) +u.y* sinf(rads)  ;
    rotx[9]= u.y*u.z*(1-cosf(rads)) -u.x* sinf(rads) ;
    rotx[10]=  cosf(rads)  +u.z*u.z*(1-cosf(rads))  ;


//    rotx[0]= cosf(rads)+u.x*u.x*(1-cosf(rads));
//    rotx[4]= u.y*u.x*(1-cosf(rads)) +u.z* sinf(rads)  ;
//    rotx[8]= u.z*u.x*(1-cosf(rads)) -u.y* sinf(rads)  ;
//    //3
//    rotx[1]= u.x*u.y*(1-cosf(rads)) -u.z* sinf(rads)  ;
//    rotx[5]=  cosf(rads)  +u.y*u.y*(1-cosf(rads))  ;
//    rotx[9]= u.z*u.y*(1-cosf(rads)) +u.x* sinf(rads)  ;
////7
//    rotx[2]= u.x*u.z*(1-cosf(rads)) +u.y* sinf(rads)  ;
//    rotx[6]= u.y*u.z*(1-cosf(rads)) -u.x* sinf(rads) ;
//    rotx[10]=  cosf(rads)  +u.z*u.z*(1-cosf(rads))  ;

    float src_temp[16];
    matrix_copy_float( srcmat4x4,src_temp,4);

    // matrix_print_float(translate_mat,4,4,"floatmat");
    matrix_mul_matrix_float(rotx,src_temp,srcmat4x4,4,4,4,4);

}


MAT_API  void matrix_rotate4x4_z_float(float * srcmat4x4 ,float  angle)
{
    float rads=( angle/180.f)*M_PI;
    float rotx [16];

    matrix_identity_float(rotx,4);

    rotx[0]= cosf(rads);
    rotx[1]= -sinf(rads);
    rotx[4]= sinf(rads);
    rotx[5]=cosf(rads);

    float src_temp[16];
    matrix_copy_float( srcmat4x4,src_temp,4);

    // matrix_print_float(translate_mat,4,4,"floatmat");
    matrix_mul_matrix_float(rotx,src_temp,srcmat4x4,4,4,4,4);

}


MAT_API  void matrix_rotate4x4_y_float(float * srcmat4x4 ,float  angle)
{
    float rads=( angle/180.f)*M_PI;
    float rotx [16];

    matrix_identity_float(rotx,4);

    rotx[0]= cosf(rads);
    rotx[2]= sinf(rads);
    rotx[8]= -sinf(rads);
    rotx[10]= cosf(rads);

    float src_temp[16];
    matrix_copy_float( srcmat4x4,src_temp,4);

    // matrix_print_float(translate_mat,4,4,"floatmat");
    matrix_mul_matrix_float(rotx,src_temp,srcmat4x4,4,4,4,4);

}











MAT_API  void matrix_mul_scalar(double * src ,double scalar,unsigned int M,unsigned int N)
{
    unsigned i=0;


    for (i = 0; i <M*N; ++i)
    {
        src[i]=src[i]*scalar;
    }


}


MAT_API  void matrix_mul_scalar_float(float * src ,float scalar,unsigned int M,unsigned int N)
{
    unsigned i=0;


    for (i = 0; i <M*N; ++i)
    {
        src[i]=src[i]*scalar;
    }


}

MAT_API  void matrix_transpose2(double  *array,double  *out , int m, int n)
{

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int old_idx = i * n + j;
            int new_idx = j * m + i;
            out[new_idx] = array[old_idx];
        }
    }

}

MAT_API  void matrix_transpose(double  *array, int m, int n)
{
    double new_array[m * n];
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int old_idx = i * n + j;
            int new_idx = j * m + i;
            new_array[new_idx] = array[old_idx];
        }
    }
    for (int i = 0; i < m * n; i++)
    {
        array[i] = new_array[i];
    }
}



MAT_API  void matrix_transpose_float(float  *array, int m, int n)
{
    float new_array[m * n];
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int old_idx = i * n + j;
            int new_idx = j * m + i;
            new_array[new_idx] = array[old_idx];
        }
    }
    for (int i = 0; i < m * n; i++)
    {
        array[i] = new_array[i];
    }
}


//   A[4*0+0] ; A[4*0+1] ; A[4*0+2] ; A[4*0+3];
//   A[4*1+0] ; A[4*1+1] ; A[4*1+2] ; A[4*1+3];
//   A[4*2+0] ; A[4*2+1] ; A[4*2+2] ; A[4*2+3];

//    B[3*0+0]=0.5;     B[3*0+1]=0;     B[3*0+2]=-0.5;
//    B[3*1+0]=0.5;     B[3*1+1]=0;     B[3*1+2]=0.5;
//    B[3*2+0]=-0.5;    B[3*2+1]=0;     B[3*2+2]=-0.5;
//    B[3*3+0]=-0.5;    B[3*3+1]=0;     B[3*3+2]=0.5;


//    Arows,  Acols,  Brows,  Bcols
//mat 3        4    x  4        3      = 3x3
//Arows Bcols


//    res[ Bcols*0+0]=A[Acols*0+0]*B[Bcols*0+0]+A[Acols*0+1]*B[Bcols*1+0]+A[Acols*0+2]*B[Bcols*2+0]+A[Acols*0+3]*B[Bcols*3+0];
//    res[ Bcols*0+1]=A[Acols*0+0]*B[Bcols*0+1]+A[Acols*0+1]*B[Bcols*1+1]+A[Acols*0+2]*B[Bcols*2+1]+A[Acols*0+3]*B[Bcols*3+1];
//    res[ Bcols*0+2]=A[Acols*0+0]*B[Bcols*0+2]+A[Acols*0+1]*B[Bcols*1+2]+A[Acols*0+2]*B[Bcols*2+2]+A[Acols*0+3]*B[Bcols*3+2];

//    res[ Bcols*1+0]=A[Acols*1+0]*B[Bcols*0+0]+A[Acols*1+1]*B[Bcols*1+0]+A[Acols*1+2]*B[Bcols*2+0]+A[Acols*1+3]*B[Bcols*3+0];
//    res[ Bcols*1+1]=A[Acols*1+0]*B[Bcols*0+1]+A[Acols*1+1]*B[Bcols*1+1]+A[Acols*1+2]*B[Bcols*2+1]+A[Acols*1+3]*B[Bcols*3+1];
//    res[ Bcols*1+2]=A[Acols*1+0]*B[Bcols*0+2]+A[Acols*1+1]*B[Bcols*1+2]+A[Acols*1+2]*B[Bcols*2+2]+A[Acols*1+3]*B[Bcols*3+2];

//    res[ Bcols*2+0]=A[Acols*2+0]*B[Bcols*0+0]+A[Acols*2+1]*B[Bcols*1+0]+A[Acols*2+2]*B[Bcols*2+0]+A[Acols*2+3]*B[Bcols*3+0];
//    res[ Bcols*2+1]=A[Acols*2+0]*B[Bcols*0+1]+A[Acols*2+1]*B[Bcols*1+1]+A[Acols*2+2]*B[Bcols*2+1]+A[Acols*2+3]*B[Bcols*3+1];
//    res[ Bcols*2+2]=A[Acols*2+0]*B[Bcols*0+2]+A[Acols*2+1]*B[Bcols*1+2]+A[Acols*2+2]*B[Bcols*2+2]+A[Acols*2+3]*B[Bcols*3+2];



MAT_API  void matrix_mul_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols,unsigned int Brows,unsigned int Bcols)
{
    assert(Acols==Brows);

    unsigned i=0;
    unsigned j=0;
    unsigned k=0;
    for ( i = 0; i <  Arows; i++)
    {
        for ( j = 0; j < Bcols; j++)
        {
            double sum = 0.0;
            for (k = 0; k < Brows; k++)
            {
                sum = sum + A[i * Acols + k] * B[k * Bcols + j];
            }
            res[i * Bcols + j] = sum;
        }
    }
}







MAT_API  void matrix_add_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols)
{


    unsigned i=0;
    unsigned j=0;
    unsigned k=0;
    for ( i = 0; i <  Arows; i++)
    {
        for ( j = 0; j < Acols; j++)
        {

            res[i *Arows + j] = A[i *Arows + j]  +B[i *Arows + j];
        }
    }
}


MAT_API  void matrix_add_matrix_float(float * A ,float * B,float *res,unsigned int Arows,unsigned int Acols)
{


    unsigned i=0;
    unsigned j=0;
    unsigned k=0;
    for ( i = 0; i <  Arows; i++)
    {
        for ( j = 0; j < Acols; j++)
        {

            res[i *Arows + j] = A[i *Arows + j]  +B[i *Arows + j];
        }
    }
}

MAT_API  void matrix_substract_matrix(double * A ,double * B,double *res,unsigned int Arows,unsigned int Acols)
{


    unsigned i=0;
    unsigned j=0;
    //unsigned k=0;
    for ( i = 0; i <  Arows; i++)
    {
        for ( j = 0; j < Acols; j++)
        {

            res[i *Arows + j] = A[i *Arows + j]  -  B[i *Arows + j];
        }
    }
}


MAT_API  void matrix_mul_matrix_float(float * A ,float * B,float *res,unsigned int Arows,unsigned int Acols,unsigned int Brows,unsigned int Bcols)
{
    assert(Acols==Brows);

    unsigned i=0;
    unsigned j=0;
    unsigned k=0;
    for ( i = 0; i <  Arows; i++)
    {
        for ( j = 0; j < Bcols; j++)
        {
            float sum = 0.0;
            for (k = 0; k < Brows; k++)
            {
                sum = sum + A[i * Acols + k] * B[k * Bcols + j];
            }
            res[i * Bcols + j] = sum;
        }
    }
}

MAT_API void Atranspose_mul_A(double *src,double *res,unsigned  M,unsigned   N) //use malloc res[N+1]
{
    double *A_transposed=(double*)malloc((N )*(M)*sizeof (double));

    matrix_transpose2(src,A_transposed,M,N);
    matrix_mul_matrix(A_transposed,src,res,N,M,M,N);
    free(  A_transposed);
}

MAT_API void matrix_print(double* c,unsigned int M, unsigned int N, char* name)
{
    printf("%s matrix%ux%u :\n",name,M,N);
    for(unsigned int i = 0; i <M; i++)
    {
        for(unsigned int j = 0; j < N; j++)
        {
            printf("%lf ",c[i*N+j]);
        }
        printf("\n");
    }
}

MAT_API void matrix_print_float(float* c,unsigned int M, unsigned int N, char* name)
{
    printf("%s matrix%ux%u :\n",name,M,N);
    for(unsigned int i = 0; i <M; i++)
    {
        for(unsigned int j = 0; j < N; j++)
        {
            printf("%g ",c[i*N+j]);
        }
        printf("\n");
    }
}



MAT_API  void matrix4x4_vec4(float *A,vec4 *B,  vec4  *res)
{

    //                 a b c d
    //            M =  e f g h
    //                 i j k l
    //                 m n o p

    float Ax0=A[0]; float Ax1=A[1];  float Ax2=A[2]; float Ax3=A[3];
    float Ay0=A[4]; float Ay1=A[5];  float Ay2=A[6]; float Ay3=A[7];
    float Az0=A[8]; float Az1=A[9];  float Az2=A[10];float Az3=A[11];
    float Aw0=A[12]; float Aw1=A[13];  float Aw2=A[14];float Aw3=A[15];


//    float Ax0=A[0]; float Ax1=A[1];  float Ax2=A[2]; float Ax3=A[3];
//    float Ay0=A[4]; float Ay1=A[5];  float Ay2=A[6]; float Ay3=A[7];
//    float Az0=A[8]; float Az1=A[9];  float Az2=A[10];float Az3=A[11];
//    float Aw0=A[12]; float Aw1=A[13];  float Aw2=A[14];float Aw3=A[15];


    float Bx=(float)B->vals[0];
    float By=(float)B->vals[1];
    float Bz=(float)B->vals[2];
    float Bw=(float)B->vals[3];

//    res->vals[0]=  Ax0*Bx + Ax1*Bx + Ax2*Bx + Ax3*Bx;
//    res->vals[1]=  Ay0*By + Ay1*By + Ay2*By + Ay3*By;
//    res->vals[2]=  Az0*Bz + Az1*Bz + Az2*Bz + Az3*Bz;
//    res->vals[3]=  Aw0*Bw + Aw1*Bw + Aw2*Bw + Aw3*Bw;


    res->vals[0]=  Ax0*Bx + Ay0*By + Az0*Bz + Aw0*Bw;
    res->vals[1]=   Ax1*Bx + Ay1*By + Az1*Bz + Aw1*Bw;
    res->vals[2]=   Ax2*Bx + Ay2*By + Az2*Bz + Aw2*Bw;
    res->vals[3]=   Ax3*Bx + Ay3*By + Az3*Bz + Aw3*Bw;

}


MAT_API void matrix_Setscale( float *matrix, float x,  float y,  float z)
{
    matrix[0] = x;
    matrix[5] = y;
    matrix[10] = z;
}


MAT_API void matrix_load(double * mat, int M)
{
    FILE *f;
    f = fopen("mat.txt", "r");

    for (int i = 0; i < M; ++i)
    {
        fscanf(f, "%lf;", &mat[i]);
    }

}
MAT_API void matrix_zero(double * mat, int M)
{

    for (int i = 0; i < M; ++i)
    {
        mat[i]=0.0;
    }

}

MAT_API void matrix_copy(double * src,double * dst, int rang)
{

    for (int i = 0; i < rang*rang; ++i)
    {
        dst[i]=src[i];
    }

}


MAT_API void matrix_copy_float(const float * src,float * dst, int rang)
{

    for (int i = 0; i < rang*rang; ++i)
    {
        dst[i]=src[i];
    }

}




MAT_API void matrix_identity(double * mat, int M)
{
    matrix_zero(mat,M*M);

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            mat[i*M+i]=1.0 ;
        }

    }
}

MAT_API void matrix_zero_float(float * mat, int M)
{
    for (int i = 0; i < M; ++i)
    {
        mat[i]=0.0;
    }
}

MAT_API void matrix_identity_float(float * mat, int M)
{
    matrix_zero_float(mat,M*M);

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            mat[i*M+i]=1.0f;
        }

    }
}


MAT_API void matrix_inverse3x3(double* m,double * res)
{
    double det = m[0] * (m[1*3+ 1] * m[3*2+ 2] - m[3*2+ 1] * m[3*1+ 2]) -
            m[0+ 1] * (m[1*3+ 0] * m[2*3+ 2] - m[3*1+ 2] * m[2*3+ 0]) +
            m[0+ 2] * (m[1*3+ 0] * m[2*3+ 1] - m[1*3+ 1] * m[2*3+ 0]);

    double invdet = 1.0 / det;

    printf("det %f \n",det);
    res[0] = (m[1*3+ 1] * m[2*3+ 2] - m[2*3+ 1] * m[1*3+ 2]) * invdet;
    res[1] = (m[0*3+ 2] * m[2*3+ 1] - m[0*3+ 1] * m[2*3+ 2]) * invdet;
    res[2] = (m[0*3+ 1] * m[1*3+ 2] - m[0*3+ 2] * m[1*3+ 1]) * invdet;
    res[3] = (m[1*3+ 2] * m[2*3+ 0] - m[1*3+ 0] * m[2*3+ 2]) * invdet;
    res[4] = (m[0*3+ 0] * m[2*3+ 2] - m[0*3+ 2] * m[2*3+ 0]) * invdet;
    res[5] = (m[1*3+ 0] * m[0*3+ 2] - m[0*3+ 0] * m[1*3+ 2]) * invdet;
    res[6] = (m[1*3+ 0] * m[2*3+ 1] - m[2*3+ 0] * m[1*3+ 1]) * invdet;
    res[7] = (m[2*3+ 0] * m[0*3+ 1] - m[0*3+ 0] * m[2*3+ 1]) * invdet;
    res[8] = (m[0*3+ 0] * m[1*3+ 1] - m[1*3+ 0] * m[0*3+ 1]) * invdet;
}

MAT_API void matrix_inverse3x3_float(float* m,float * res)
{
    float det = m[0] * (m[1*3+ 1] * m[3*2+ 2] - m[3*2+ 1] * m[3*1+ 2]) -
            m[0+ 1] * (m[1*3+ 0] * m[2*3+ 2] - m[3*1+ 2] * m[2*3+ 0]) +
            m[0+ 2] * (m[1*3+ 0] * m[2*3+ 1] - m[1*3+ 1] * m[2*3+ 0]);

    float invdet = 1.0f / det;

    // printf("det %f \n",det);
    res[0] = (m[1*3+ 1] * m[2*3+ 2] - m[2*3+ 1] * m[1*3+ 2]) * invdet;
    res[1] = (m[0*3+ 2] * m[2*3+ 1] - m[0*3+ 1] * m[2*3+ 2]) * invdet;
    res[2] = (m[0*3+ 1] * m[1*3+ 2] - m[0*3+ 2] * m[1*3+ 1]) * invdet;
    res[3] = (m[1*3+ 2] * m[2*3+ 0] - m[1*3+ 0] * m[2*3+ 2]) * invdet;
    res[4] = (m[0*3+ 0] * m[2*3+ 2] - m[0*3+ 2] * m[2*3+ 0]) * invdet;
    res[5] = (m[1*3+ 0] * m[0*3+ 2] - m[0*3+ 0] * m[1*3+ 2]) * invdet;
    res[6] = (m[1*3+ 0] * m[2*3+ 1] - m[2*3+ 0] * m[1*3+ 1]) * invdet;
    res[7] = (m[2*3+ 0] * m[0*3+ 1] - m[0*3+ 0] * m[2*3+ 1]) * invdet;
    res[8] = (m[0*3+ 0] * m[1*3+ 1] - m[1*3+ 0] * m[0*3+ 1]) * invdet;
}

MAT_API void matrix_inverse3x3_float2(float* m)
{
    float det = m[0] * (m[1*3+ 1] * m[3*2+ 2] - m[3*2+ 1] * m[3*1+ 2]) -
            m[0+ 1] * (m[1*3+ 0] * m[2*3+ 2] - m[3*1+ 2] * m[2*3+ 0]) +
            m[0+ 2] * (m[1*3+ 0] * m[2*3+ 1] - m[1*3+ 1] * m[2*3+ 0]);

    float invdet = 1.0f / det;

    float res[9];
    // printf("det %f \n",det);
    res[0] = (m[1*3+ 1] * m[2*3+ 2] - m[2*3+ 1] * m[1*3+ 2]) * invdet;
    res[1] = (m[0*3+ 2] * m[2*3+ 1] - m[0*3+ 1] * m[2*3+ 2]) * invdet;
    res[2] = (m[0*3+ 1] * m[1*3+ 2] - m[0*3+ 2] * m[1*3+ 1]) * invdet;
    res[3] = (m[1*3+ 2] * m[2*3+ 0] - m[1*3+ 0] * m[2*3+ 2]) * invdet;
    res[4] = (m[0*3+ 0] * m[2*3+ 2] - m[0*3+ 2] * m[2*3+ 0]) * invdet;
    res[5] = (m[1*3+ 0] * m[0*3+ 2] - m[0*3+ 0] * m[1*3+ 2]) * invdet;
    res[6] = (m[1*3+ 0] * m[2*3+ 1] - m[2*3+ 0] * m[1*3+ 1]) * invdet;
    res[7] = (m[2*3+ 0] * m[0*3+ 1] - m[0*3+ 0] * m[2*3+ 1]) * invdet;
    res[8] = (m[0*3+ 0] * m[1*3+ 1] - m[1*3+ 0] * m[0*3+ 1]) * invdet;

    for (int var = 0; var < 9; ++var) {
        m[var]=res[var];
    }
}


MAT_API void matrix_multiply3x3_vector_float(float *resultvector, const float *matrix, const float *pvector)
{
    resultvector[0]=matrix[0]*pvector[0]+matrix[1]*pvector[1]+matrix[2]*pvector[2];
    resultvector[1]=matrix[3]*pvector[0]+matrix[4]*pvector[1]+matrix[5]*pvector[2];
    resultvector[2]=matrix[6]*pvector[0]+matrix[7]*pvector[1]+matrix[8]*pvector[2];
}


MAT_API bool matrix_inverse4x4(const double * m , double *invOut )
{
    double inv[16], det;


    inv[0] = m[5]  * m[10] * m[15] -
            m[5]  * m[11] * m[14] -
            m[9]  * m[6]  * m[15] +
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] -
            m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
            m[4]  * m[11] * m[14] +
            m[8]  * m[6]  * m[15] -
            m[8]  * m[7]  * m[14] -
            m[12] * m[6]  * m[11] +
            m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
            m[4]  * m[11] * m[13] -
            m[8]  * m[5] * m[15] +
            m[8]  * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] -
            m[8]  * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
            m[1]  * m[11] * m[14] +
            m[9]  * m[2] * m[15] -
            m[9]  * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
            m[0]  * m[11] * m[14] -
            m[8]  * m[2] * m[15] +
            m[8]  * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
            m[0]  * m[11] * m[13] +
            m[8]  * m[1] * m[15] -
            m[8]  * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
            m[0]  * m[10] * m[13] -
            m[8]  * m[1] * m[14] +
            m[8]  * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
            m[1]  * m[7] * m[14] -
            m[5]  * m[2] * m[15] +
            m[5]  * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
            m[0]  * m[7] * m[14] +
            m[4]  * m[2] * m[15] -
            m[4]  * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
            m[0]  * m[7] * m[13] -
            m[4]  * m[1] * m[15] +
            m[4]  * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
            m[0]  * m[6] * m[13] +
            m[4]  * m[1] * m[14] -
            m[4]  * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;
    int i;
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}


MAT_API void matrix_inverse4x4_float( float *m)
{
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] -
            m[5]  * m[11] * m[14] -
            m[9]  * m[6]  * m[15] +
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] -
            m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
            m[4]  * m[11] * m[14] +
            m[8]  * m[6]  * m[15] -
            m[8]  * m[7]  * m[14] -
            m[12] * m[6]  * m[11] +
            m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
            m[4]  * m[11] * m[13] -
            m[8]  * m[5] * m[15] +
            m[8]  * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] -
            m[8]  * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
            m[1]  * m[11] * m[14] +
            m[9]  * m[2] * m[15] -
            m[9]  * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
            m[0]  * m[11] * m[14] -
            m[8]  * m[2] * m[15] +
            m[8]  * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
            m[0]  * m[11] * m[13] +
            m[8]  * m[1] * m[15] -
            m[8]  * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
            m[0]  * m[10] * m[13] -
            m[8]  * m[1] * m[14] +
            m[8]  * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
            m[1]  * m[7] * m[14] -
            m[5]  * m[2] * m[15] +
            m[5]  * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
            m[0]  * m[7] * m[14] +
            m[4]  * m[2] * m[15] -
            m[4]  * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
            m[0]  * m[7] * m[13] -
            m[4]  * m[1] * m[15] +
            m[4]  * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
            m[0]  * m[6] * m[13] +
            m[4]  * m[1] * m[14] -
            m[4]  * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    det = 1.0f / det;

    matrix_zero_float(m,4*4);
    for (i = 0; i < 16; i++)
        m[i] = inv[i] * det;


}



MAT_API  void matrix_translate4x4_float(float * srcmat4x4 ,float * translate_vector3)
{
    float translate_mat[16];

    matrix_identity_float(translate_mat,4);

    translate_mat[12]=translate_vector3[0];
    translate_mat[13]=translate_vector3[1];
    translate_mat[14]=translate_vector3[2];

    float src_temp[16];
    matrix_copy_float( srcmat4x4,src_temp,4);

    // matrix_print_float(translate_mat,4,4,"floatmat");
    matrix_mul_matrix_float(translate_mat,src_temp,srcmat4x4,4,4,4,4);

}

MAT_API  void matrix_rotate4x4_x_float(float * srcmat4x4 ,float  angle)
{
    float rads=( angle/180.f)*M_PI;
    float rotx [16];

    matrix_identity_float(rotx,4);

    rotx[5]= cosf(rads);
    rotx[6]= sinf(rads);
    rotx[9]= -sinf(rads);
    rotx[10]=cosf(rads);

    float src_temp[16];
    matrix_copy_float( srcmat4x4,src_temp,4);

    // matrix_print_float(translate_mat,4,4,"floatmat");
    matrix_mul_matrix_float(rotx,src_temp,srcmat4x4,4,4,4,4);

}


MAT_API  void matrix_getVec3_float_t(const float *m, float *vec3)
{
    vec3[0]=m[13];
    vec3[1]=m[14];
    vec3[2]=m[15];
}

MAT_API  void matrix_getVec3_float(const float *m, float *vec3)
{
    vec3[0]=m[3];
    vec3[1]=m[7];
    vec3[2]=m[11];
}


MAT_API  void matrix_build_model_float(const float *rot, const float *translate, float *out )
{

    out[0]=rot[0];
    out[1]=rot[1];
    out[2]=rot[2];
    out[3]=translate[0];


    out[4]=rot[3];
    out[5]=rot[4];
    out[6]=rot[5];
    out[7]=translate[1];

    out[8]=rot[6];
    out[9]=rot[7];
    out[10]=rot[8];
    out[11]=translate[2];

    out[12]=0.0f;
    out[13]=0.0f;
    out[14]=0.0f;
    out[15]=1.0f;
}

MAT_API  void matrix_build_model_float_from_blender(const float *rot, const float *translate, float *out2 )
{
    float out[16];
    out[0]=rot[0];
    out[1]=rot[2];
    out[2]=-rot[1];
    out[3]=translate[0];


    out[4]=rot[3];
    out[5]=rot[5];
    out[6]=-rot[4];
    out[7]=translate[2];

    out[8]=rot[6];
    out[9]=rot[8];
    out[10]=-rot[7];
    out[11]=-translate[1];

    out[12]=0.0f;
    out[13]=0.0f;
    out[14]=0.0f;
    out[15]=1.0f;

    float r[16];
    r[0]=1.0f;
    r[1]=0.0f;
    r[2]=0.0f;
    r[3]=0.0f;


    r[4]=0.0f;
    r[5]=-1.0f;
    r[6]=0.0f;
    r[7]=0.0f;

    r[8]=0.0f;
    r[9]=0.0f;
    r[10]=-1.0f;
    r[11]=0.0f;

    r[12]=0.0f;
    r[13]=0.0f;
    r[14]=0.0f;
    r[15]=1.0f;



    matrix_mul_matrix_float(out,r,out2,4,4,4,4);
}



MAT_API  void FPSViewRH( vec3 eye, float pitch, float yaw, float *out)
{
    // I assume the values are already converted to radians.
    float cosPitch = cosf(pitch);
    float sinPitch = sinf(pitch);
    float cosYaw = cosf(yaw);
    float sinYaw = sinf(yaw);

    vec3 xaxis = vec3_create( cosYaw, 0, -sinYaw );
    vec3 yaxis =  vec3_create( sinYaw * sinPitch, cosPitch, cosYaw * sinPitch );
    vec3 zaxis =  vec3_create( sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw  );

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors




    out[0]=(float) xaxis.x;
    out[1]=(float) yaxis.x;
    out[2]=(float) zaxis.x;
    out[3]=0;

    out[4]=(float) xaxis.y;
    out[5]=(float) yaxis.y;
    out[6]=(float) zaxis.y;
    out[7]=0;

    out[8]=(float) xaxis.z;
    out[9]=(float) yaxis.z;
    out[10]=(float)zaxis.z;
    out[11]=0;

    out[12]=(float)-vec3_dot(&xaxis, &eye);
    out[13]=(float)-vec3_dot(& yaxis, &eye );
    out[14]=(float)-vec3_dot(&zaxis, &eye );
    out[15]=1;

}


MAT_API  void vec3_copy_float(const float *src,float *dst  )
{
    dst[0]=src[0];
    dst[1]=src[1];
    dst[2]=src[2];
}

MAT_API  void matrix4x4_mul_vec4_float(float *A, vec4 *B, vec4  *res)
{

    //                 a b c d
    //            M = e f g h
    //                 i j k l
    //                  m n o p

    res->vals[0]= A[0]*B->vals[0]   +  A[1]*B->vals[1]   +    A[2]*B->vals[2]   +   A[3]*B->vals[3];
    res->vals[1]= A[4]*B->vals[0]   +  A[5]*B->vals[1]   +    A[6]*B->vals[2]   +   A[7]*B->vals[3];

    res->vals[2]= A[8]*B->vals[0]   +  A[9]*B->vals[1]   +    A[10]*B->vals[2]   +   A[11]*B->vals[3];
    res->vals[3]= A[12]*B->vals[0]  +  A[13]*B->vals[1]  +    A[14]*B->vals[2]   +   A[15]*B->vals[3];
}



MAT_API  void vec4_mul_matrix4x4_float(vec4 *B,float *A,  vec4  *res)
{

    //                 a b c d
    //            M = e f g h
    //                 i j k l
    //                  m n o p

    float a=A[0]; float b=A[1];  float c=A[2];float d=A[3];
    float e=A[4]; float f=A[5];  float g=A[6];float h=A[7];
    float i=A[8]; float j=A[9];  float k=A[10];float l=A[11];
    float m=A[12]; float n=A[13];  float o=A[14];float p=A[15];

    float x=(float)B->vals[0];
    float y=(float)B->vals[1];
    float z=(float)B->vals[2];
    float w=(float)B->vals[3];

    res->vals[0]=  x*a + y*e + z*i + w*m;
    res->vals[1]=  x*b + y*f + z*j + w*n;
    res->vals[2]=   x*c + y*g + z*k + w*o;
    res->vals[3]=  x*d + y*h + z*l + w*p;






}




