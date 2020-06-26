#include <slay_solver/slay_gauss.h>
#include <slay_solver/slay_defs.h>
#include <algorithm>
#include <map>
#include <set>
#include <math.h>
#include <fstream>

slay_gauss::slay_gauss()
{

}

slay_gauss::~slay_gauss()
{

}

void slay_gauss::solve(vector<vector<double> > &A, vector<double> &B, vector<double> &answer,
                       vector<double> &sustainability_row,
                       double  &sustainability_diag)
{
    unsigned long N=A.back().size();
    addB(A,B);

    //001|a    |100|c
    //010|b  ->|010|b
    //100|c    |001|a
    sort_equations(A,sustainability_row,sustainability_diag);


    cout<<"\n"<<A;
    double  M;

    cout<<N;
    for (unsigned int k=0;k<N-1;++k)
    {
        for (unsigned int i=1+k;i<N;++i)
        {

            M = A[i][k]/A[k][k];
            A[i] = A[i] -A[k]*M;

        }
    }

    cout<<"\nПриведенная матрица к ступенчатому виду:\n";
    cout<<A;


    // "Обратный ход"

    vector<double> ans;
    for (unsigned int k = 0; k < N; ++k)
    {
        vector<double> d;
        unsigned long n=ans.size();

        for (unsigned int i = 1; i < ans.size()+1; ++i)
        {
            double a;
            a=*(&(&A.back()-n)->back()-i)* *(&ans.back()-(n-i)    );
            d.push_back(a);
        }

        double temp_1=*(&(&A.back()-n)->back()-0);
        for (unsigned int i = 0; i < ans.size(); ++i)
        {
            temp_1=temp_1  - d[i];
        }
        temp_1= temp_1/ *(&(&A.back()-n)->back()-(n+1));
        d.clear();
        ans.push_back(temp_1);
    }


    vector<double>::reverse_iterator rit = ans.rbegin();
    for (; rit!= ans.rend(); ++rit)
    {
        answer.push_back(*rit );
    }
    ans.clear();
    // cout<<answer;
}

void slay_gauss::solve(vector<vector<double> > &A, vector<double> &answer, vector<double> &sustainability_row, double &sustainability_diag)
{
    unsigned long N=A.back().size()-1;


    //001|a    |100|c
    //010|b  ->|010|b
    //100|c    |001|a
    sort_equations(A,sustainability_row,sustainability_diag);


    cout<<"\n"<<A;
    double  M;

    cout<<N;
    for (unsigned int k=0;k<N-1;++k)
    {
        for (unsigned int i=1+k;i<N;++i)
        {

            M = A[i][k]/A[k][k];
            A[i] = A[i] -A[k]*M;

        }
    }

    cout<<"\nПриведенная матрица к ступенчатому виду:\n";
    cout<<A;


    // "Обратный ход"

    vector<double> ans;
    for (unsigned int k = 0; k < N; ++k)
    {
        vector<double> d;
        unsigned long n=ans.size();

        for (unsigned int i = 1; i < ans.size()+1; ++i)
        {
            double a;
            a=*(&(&A.back()-n)->back()-i)* *(&ans.back()-(n-i)    );
            d.push_back(a);
        }

        double temp_1=*(&(&A.back()-n)->back()-0);
        for (unsigned int i = 0; i < ans.size(); ++i)
        {
            temp_1=temp_1  - d[i];
        }
        temp_1= temp_1/ *(&(&A.back()-n)->back()-(n+1));
        d.clear();
        ans.push_back(temp_1);
    }


    vector<double>::reverse_iterator rit = ans.rbegin();
    for (; rit!= ans.rend(); ++rit)
    {
        answer.push_back(*rit );
    }
    ans.clear();
    // cout<<answer;
}

void slay_gauss::load_matrix(const char *filepath, vector<vector<double> > &A)
{
    auto f=fstream(filepath);
    if (!f)
    {
        cout << "Файл не открыт\n\n";

    }
    else
    {
        cout << "Файл открыт:\n"<<filepath<<"\n";
    }
    vector<double>d;
    while( !f.eof() )
    {


        double s;
        char separator;
        f >> s;
        f >> separator;

        d.push_back(s);
    }
    d.pop_back();


    cout <<"считано:"<< d << endl;
    int N= -0.5+sqrt(1.0+4.0*d.size())/2.0;
    unsigned long indx=0;
    for (int i = 0; i < N; ++i)
    {
        vector<double> row;
A.push_back(row);
        for (int j = 0; j < N+1; ++j)
        {
            A.back().push_back(d[indx]);
        indx++;
        }

    }



    f.close();

}

typedef struct row_t
{
    double max;
    double min;
    vector<double> row;

}row_t;


struct compare {
    bool operator()(pair<int,row_t>  lhs,
                    pair<int,row_t>  rhs) const
    {
        return lhs.second.max < rhs.second.max;
    }
};

void slay_gauss::sort_equations(vector<vector<double> > &A
                                ,vector<double> &sustainability_row
                                ,double &sustainability_diag)
{



    std::multimap<int,row_t> sorted;
    for (unsigned int i = 0; i < A.size(); ++i)
    {
        std::vector<double>::iterator result_max;
        std::vector<double>::iterator result_min;
        result_max = std::max_element(A[i].begin(), A[i].end()-1);
        result_min = std::min_element(A[i].begin(), A[i].end()-1);
        int index=std::distance(A[i].begin(), result_max);
        std::cout << "\n индекс максимального элемента: " << index << '\n';
        row_t t{*result_max,*result_min,A[i]};
        sorted.insert({index,t});
        sustainability_row.push_back(*result_max/ *result_min);
    }
    multiset<pair<int,row_t>, compare> b;
    for (multimap<int,row_t, compare>::iterator i=sorted.begin(); i!=sorted.end(); i++)
        b.insert(pair<int,row_t>((*i).first, (*i).second));

    A.clear();
    int j=0;
    for (multiset<pair<int,row_t>, compare>::iterator i=b.begin(); i!=b.end(); i++)
    {
        A.push_back(i->second.row);
        j++;
    }
    b.clear();
    sorted.clear();

    vector<double> t;
    for (unsigned int i = 0; i < A.size(); ++i)
    {
        t.push_back(A[i][i]);
    }

    std::vector<double>::iterator result_max;
    std::vector<double>::iterator result_min;
    result_max = std::max_element(t.begin(), t.end() );
    result_min = std::min_element(t.begin(), t.end() );

    sustainability_diag=*result_max/ *result_min;
    t.clear();
}

void slay_gauss::addB(vector<vector<double> > &A, vector<double> &B)
{
    for (unsigned int i = 0; i < A.size(); ++i)
    {
        A[i].push_back(B[i]);
    }
}
