

#include <containers/vector.h>
#include <polynominals/polynominals.h>
#pragma once
#define CAM_API extern

#define ROOT_NOT_FOUND -999
#define INITIAL_GUESS 0.0
#define LIMIT 0.00001
#define MAX_ITERATIONS 1000

///Запись полинома должна быть "наоборот"
//b=[8 ,2 , -25, 6]
//6x^3−25x^2+2x+8=0
///--------------------------------------

//usage example

//FILE *f;
//f = fopen("equation_main.txt", "r");
////f = fopen("equation.txt", "r");
//Vector v;
//vector_load_text_double(f,&v);
//fclose(f);
//vector_print_double(&v);



//Vector res=find_poly_roots(&v );
//vector_print_double(&res);
//printf("Hello World!\n");

//Answer_xy a= solve_linear_system_2d(2,4,8,4,9,15);
//print_answer(&a);













#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct Answer_xy_t
{
    double x;
    double y;
}Answer_xy;


CAM_API double f(Vector * poly,double x);
CAM_API double df(Vector * poly_deriative,double x);
CAM_API double ddf(Vector * poly_2deriative,double x);

//algorithm
CAM_API double find_root(Vector * poly,Vector * poly_deriative,Vector * poly_2deriative,
                 double initial_guess,double limit,int max_iterations);

CAM_API double find_poly_root(Vector * poly);
//main poly root func
CAM_API Vector find_poly_roots(Vector * poly);
//Solves the system of equations
//a*x + b*y = c, d*x + e*y = e
//using Gaussian Elimination (for numerical stability).

CAM_API Answer_xy solve_linear_system_2d(double a,double b,double c,
                                         double d,double e,double f);

CAM_API void print_answer(Answer_xy * a);


CAM_API void swap(double *a, double *b);









#ifdef __cplusplus
}
#endif // __cplusplus
