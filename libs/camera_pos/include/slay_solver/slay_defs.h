#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <ostream>
using namespace std;
 
const vector <double>  operator-(const vector <double>& left, const vector <double>& right)
    {
        vector <double> res;
        res.resize(left.size());
        for (unsigned int i = 0; i < left.size(); ++i)
        {
            res[i]=left[i]-right[i];
        }

        return res;
    }
const vector <double>  operator*(const vector <double>& left,  double & scale)
    {
        vector <double> res;
        res.resize(left.size());
        for (unsigned int i = 0; i < left.size(); ++i)
        {
            res[i]=left[i]*scale;
        }

        return res;
    }
 ostream& operator<<(ostream& os,const  vector <double>  &A)
    {
        os.precision(6);
        os << "[";
        for (int i = 0; i < A.size(); ++i)
        {
            os << A[i]<<setw(10);
        }
        os << "]";
        return os;
    }
 ostream& operator<<(ostream& os,const vector<vector <double> >&A)
    {
        for (int i = 0; i < A.size(); ++i)
        {
            os << A[i]<<endl;
        }
        return os;
    }
