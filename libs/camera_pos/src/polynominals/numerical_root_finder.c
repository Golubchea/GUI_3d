#include <polynominals/numerical_root_finder.h>


#include <stdlib.h>
#include <math.h>
CAM_API double f(Vector *poly, double x)
{
    return poly_eval(poly,x);
}

CAM_API double df(Vector *poly_deriative, double x)
{
    return poly_eval(poly_deriative,x);
}

CAM_API double ddf(Vector *poly_2deriative, double x)
{
    return poly_eval(poly_2deriative,x);
}


CAM_API double find_root(Vector * poly,Vector * poly_deriative,Vector * poly_2deriative ,
                         double initial_guess, double limit ,int max_iterations)
{


    double xn_1=initial_guess;
    unsigned i=0;
    while (i<max_iterations)
    {
        double fx =f(poly,xn_1);
        double dfx =df(poly_deriative,xn_1);
        double ddfx =ddf(poly_2deriative,xn_1);
        double xn= xn_1 - 2 * fx * dfx / (2 * dfx * dfx - fx * ddfx);
        if ( fabs(xn-xn_1)<limit )
        {
            return xn;
        }
        xn_1=xn;
        i++;
    }
    return ROOT_NOT_FOUND;
}

CAM_API double find_poly_root(Vector *poly)
{
    Vector dpoly=poly_deriative(poly);
    Vector ddpoly=poly_deriative(&dpoly);
    double ans =find_root(poly,&dpoly,&ddpoly,INITIAL_GUESS,LIMIT,MAX_ITERATIONS);
    vector_destroy(&dpoly);
    vector_destroy(&ddpoly);
    return ans;

}


CAM_API Vector find_poly_roots(Vector *poly)
{
    Vector solutions;
    vector_setup(&solutions,0,sizeof (double) );

    unsigned int q=0;
    printf("--------------%lu----------------\n",poly_order(poly)  );
    for (q=0;q<=poly_order(poly)-2;  ++q)
    {
        double x= find_poly_root(poly);
        //printf("\n polys=%u -[%lf]-\n",q,x );
        if(x==ROOT_NOT_FOUND)
        {
            break;
        }
        vector_push_back(&solutions,&x);
        Vector dive;
        vector_setup(&dive,0,sizeof (double) );
        double t =1.0;
        x=-x;
        vector_push_back(&dive,&x);
        vector_push_back(&dive,&t);
        Vector temp= poly_div(poly,&dive);
        vector_move(poly,&temp);
        vector_clear(&temp);
        vector_destroy(&temp);
        vector_destroy(&dive);
    }
    //Find the rest of the roots analytically
    if(poly_order(poly)==1)
    {
        double x=-poly_coeff(poly,1)/poly_coeff(poly,0);
        vector_push_back(&solutions,&x);
    }
    else if (poly_order(poly)==2)
    {
        double a=poly_coeff(poly,2);
        double b=poly_coeff(poly,1);
        double c=poly_coeff(poly,0);
        double d=b*b -4*a*c;
        if(d==0.0)
        {
            double x=-b / (2 * a);
            vector_push_back(&solutions,&x);
        }
        else if (d>0.0)
        {
            double x1=(- b + sqrt(d)) / (2 * a);
            double x2=(- b - sqrt(d)) / (2 * a);
            vector_push_back(&solutions,&x1);
            vector_push_back(&solutions,&x2);

        }
    }





    return solutions;
}



//Swap function definition
void swap(double *a, double *b)
{
    double t;

    t  = *b;
    *b = *a;
    *a = t;
}

Answer_xy solve_linear_system_2d(double a, double b, double c, double d, double e, double f)
{
    printf("solve linear \n");
    printf("abcdef (a=%lf,b=%lf,c=%lf,d=%lf,e=%lf,f=%lf)\n",a,  b,  c,   d,   e,   f);
    Answer_xy ans;
    if ( fabs(d)> fabs(a) )
    {
        swap(&a,&d);
        swap(&b,&e);
        swap(&c,&f);
    }
    if (a==0.0)
    {
        ans.x=ROOT_NOT_FOUND;
        ans.y=ROOT_NOT_FOUND;
        return ans;
    }
    double tmp=e - d * b / a;
    printf("tmp=%lf\n",tmp);
    if (tmp==0.0)
    {
        ans.x=ROOT_NOT_FOUND;
        ans.y=ROOT_NOT_FOUND;
        return ans;
    }
    ans.y = (f - d * c / a) / tmp;
    ans.x = (c - b * ans.y) / a;
    printf("yx=%lf,%lf \n",ans.y,ans.x);
    return ans;
}



















void print_answer(Answer_xy *a)
{
    printf("answer: %lf, %lf",a->x,a->y);
}
