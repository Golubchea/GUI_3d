#ifndef SLAY_GAUSS_H
#define SLAY_GAUSS_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <ostream>


using namespace std;







class slay_gauss
{
public:
    slay_gauss();
    ~slay_gauss();
    ///
    /// \brief solve - решение линейных алгебраических уравнений
    /// \param A - матрица коэффициентов
    /// \param B - столбец решенний правой части уравнения
    /// \param answer - столбец ответов
    ///
    void solve(vector<vector <double> >&A,vector <double> &B,
               vector <double> &answer,
               vector<double> &sustainability_row,
                double  &sustainability_diag);
    void solve(vector<vector <double> >&A_addedB,
               vector <double> &answer,
               vector<double> &sustainability_row,
                double  &sustainability_diag);

    void load_matrix(const char * filepath,vector<vector <double> >&A);




private:
    void sort_equations(vector<vector <double> >&A,
                        vector<double> &sustainability_row,
                         double  &sustainability_diag);
    void addB(vector<vector <double> >&A , vector <double>&B);
};

#endif // SLAY_GAUSS_H
