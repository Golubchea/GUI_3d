

///------------------------------usage example
//FILE *f;
//f = fopen("a.txt", "r");

//Vector v;
//vector_load_text_double(f,&v);
//fclose(f);
//vector_print_double(&v);

//Vector v2;
//f = fopen("b.txt", "r");
//vector_load_text_double(f,&v2);
//fclose(f);
//vector_print_double(&v2);

//Vector res=poly_scale(&v,10);  //ok
//Vector res=poly_substract(&v,&v2);//ok
//Vector res=poly_reduce(&v);//ok
//Vector res=poly_deriative(&v);//ok
//printf("d=%lf",poly_eval(&v,2) ); //ok
//printf("\norder=%lu\n",poly_order(&v) ); //ok
//printf("\n coeff=%lf\n",poly_coeff(&v,1) );//ok
//Vector res=poly_div(&v,&v2);//ok
//vector_print_double(&res); //ok
///------------------------------usage example

#include <containers/vector.h>
#define POLY_API extern
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

POLY_API Vector poly_norm(Vector *coeffs);
POLY_API Vector poly_substract(Vector *a,Vector *b);
POLY_API Vector poly_scale(Vector *coeffs ,double scale);
POLY_API Vector poly_reduce(Vector *v );
POLY_API Vector poly_deriative(Vector *v );
POLY_API double poly_eval(Vector *v ,double x);
POLY_API size_t poly_order(Vector *v);
POLY_API double poly_coeff(Vector *v,int index);
POLY_API Vector poly_div(Vector *a,Vector * b);




#ifdef __cplusplus
}
#endif // __cplusplus


