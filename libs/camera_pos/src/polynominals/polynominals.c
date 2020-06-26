#include <polynominals/polynominals.h>
#include <math.h>
POLY_API Vector poly_norm(Vector *v)
{
    //x = *(double*)vector_get(&v, 0);
    double max=*(double*)vector_back(v);
    unsigned int i;

    Vector res;
    vector_setup(&res,v->size,sizeof (double) );
    double p;
    for (i = 0; i < v->size; ++i)
    {
        double x = *(double*)vector_get(v, i);
        p=x/max;
        vector_push_back(&res,&p);
    }

    return res;
}

POLY_API Vector poly_substract(Vector *a, Vector *b)
{
    size_t siz= MAX(a->size,b->size);
    size_t siz_min= MIN(a->size,b->size);

    unsigned int i;
    Vector aa;
    vector_setup(&aa,0,sizeof (double) );
    for (i = 0; i < a->size; ++i)
    {
        vector_push_back(&aa,(double*)vector_get(a, i)  );
    }
    if(aa.size==siz_min)
    {
        for (i = siz_min-1; i < siz; ++i)
        {
            double p=0.0;
            vector_push_back(&aa,&p);
        }
    }


    Vector bb;
    vector_setup(&bb,0,sizeof (double) );

    for (i = 0; i < b->size; ++i)
    {
        vector_push_back(&bb,(double*)vector_get(b, i)  );
    }
    if(bb.size==siz_min)
    {
        for (i = siz_min; i < siz; ++i)
        {
            double p=0.0;
            vector_push_back(&bb,&p);
        }
    }

    Vector res;
    vector_setup(&res,0,sizeof (double) );

    for (i = 0; i < siz; ++i)
    {
        double  x = *(double*)vector_get(&aa, i);
        double  y = *(double*)vector_get(&bb, i);
        double ans=x-y;
        vector_push_back(&res,&ans);
    }
    vector_destroy(&aa);
    vector_destroy(&bb);

    return res;

}

POLY_API Vector poly_scale(Vector *v, double scale)
{
    unsigned int i;

    Vector res;
    vector_setup(&res,v->size,sizeof (double) );
    double p;
    for (i = 0; i < v->size; ++i)
    {
        double x = *(double*)vector_get(v, i);
        p=x*scale;
        vector_push_back(&res,&p);
    }
    return res;
}

POLY_API Vector poly_reduce(Vector *v )
{
    unsigned int i=0;

    Vector res;
    //res.data=NULL;
    vector_setup(&res,0,sizeof (double)  );
    vector_copy_assign( &res,v);


    for (i = 0; i<v->size; ++i)
    {
        double x = *(double*)vector_back(&res);
        if(x>-0.00001 && x<0.00001)
        {
            vector_pop_back(&res);
        }
        else
        {
            break;
        }
    }
    return res;
}


POLY_API Vector poly_deriative(Vector *v )
{
    unsigned int i=0;

    Vector res;
    //res.data=NULL;
    vector_setup(&res,0,sizeof (double)  );

    for (i = 1; i< v->size; ++i)
    {
        double x = *(double*)vector_get(v, i);
        x=x*(double)i;
        vector_push_back(&res,&x);
    }
    return res;
}





POLY_API double poly_eval(Vector *v, double x)
{
    unsigned int i=0;
    double res=0.0;


    for (i = 0; i< v->size; ++i)
    {
        double p = *(double*)vector_get(v, i);
        res+=p*pow(x,(double)i) ;

    }
    return res;
}

POLY_API size_t poly_order(Vector *v)
{
    return v->size-1;
}

POLY_API double poly_coeff(Vector *v, int index)
{
    if(index> v->size-1)
    {
        return 0.0;
    }
    else if (index>=0)
    {
        double p = *(double*)vector_get(v, index);
        return p;
    }

}

POLY_API Vector poly_div(Vector *a, Vector *b)
{
    size_t na=poly_order(a);
    size_t nb=poly_order(b);

    Vector res;
    //res.data=NULL;
    vector_setup(&res, 0,sizeof (double)  );


    size_t i=0;
    for (i=na;i>nb-1;i--)
    {
        double f=*(double*)vector_get(a, i)/ *(double*)vector_back(b);
       // printf("f%d=%lf \n",i,f);
        vector_push_front(&res, &f);

        Vector vv=poly_scale(b,f);
        //printf("scale ");
        //vector_print_double(&vv);
        unsigned j=0;
        for (j=0;j<i-nb;++j)
        {
            double isnull=0.0;
            vector_push_front(&vv,&isnull);
        }

        Vector vv_2 =poly_substract(a,&vv);
        //vector_print_double(&vv_2);
        vector_clear(a);
        vector_move(a,&vv_2);

        vector_destroy(&vv);
        vector_destroy(&vv_2);
    }

    return res;
}













