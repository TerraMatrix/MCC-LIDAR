#ifndef LUDECOMPOSITION_H
#define LUDECOMPOSITION_H
/*
 *  Copyright (C) 2003, 2004 by Jarno Elonen
 *
 *  Permission to use, copy, modify, distribute and sell this software
 *  and its documentation for any purpose is hereby granted without fee,
 *  provided that the above copyright notice appear in all copies and
 *  that both that copyright notice and this permission notice appear
 *  in supporting documentation.  The authors make no representations
 *  about the suitability of this software for any purpose.
 *  It is provided "as is" without express or implied warranty.
 */
#define BOOST_UBLAS_SINGULAR_CHECK
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/lu.hpp>
using namespace boost::numeric;

// Solve a linear equation system a*x=b using inplace LU decomposition.
//
// Stores x in 'b' and overwrites 'a' (with a pivotted LUD).
//
// Matrix 'b' may have any (>0) number of columns but
// must contain as many rows as 'a'.
//
// Possible return values:
//  0=success
//  1=singular matrix
//  2=a.rows != b.rows


template <typename T> int LU_Solve(
	boost::numeric::ublas::matrix<T>& a,
	boost::numeric::ublas::matrix<T>& b)
{
	// This routine is originally based on the public domain draft for JAMA,
	// Java matrix package available at http://math.nist.gov/javanumerics/jama/

	typedef boost::numeric::ublas::matrix<T> Matrix;
	typedef boost::numeric::ublas::matrix_row<Matrix> Matrix_Row;
	typedef boost::numeric::ublas::matrix_column<Matrix> Matrix_Col;

	if (a.size1() != b.size1())
		return 2;

	int m = a.size1(), n = a.size2();
	int pivsign = 0;
	int* piv = (int*)alloca(sizeof(int) * m);

	// PART 1: DECOMPOSITION
	//
	// For an m-by-n matrix A with m >= n, the LU decomposition is an m-by-n
	// unit lower triangular matrix L, an n-by-n upper triangular matrix U,
	// and a permutation vector piv of length m so that A(piv,:) = L*U.
	// If m < n, then L is m-by-m and U is m-by-n.
	{
		// Use a "left-looking", dot-product, Crout/Doolittle algorithm.
		for (int i = 0; i < m; ++i)
			piv[i] = i;
		pivsign = 1;

		// Outer loop.
		for (int j = 0; j < n; ++j)
		{
			// Make a copy of the j-th column to localize references.
			Matrix_Col LUcolj(a, j);

			// Apply previous transformations.
			for (int i = 0; i < m; ++i)
			{
				Matrix_Row LUrowi(a, i);

				// This dot product is very expensive.
				// Optimize for SSE2?
				int kmax = (i <= j) ? i : j;
				typename Matrix_Row::const_iterator ri_ite(LUrowi.begin());
				typename Matrix_Col::const_iterator cj_ite(LUcolj.begin());
				typename Matrix::value_type sum = 0.0;
				while (kmax-- > 0)
					sum += (*(ri_ite++)) * (*(cj_ite++));
				LUrowi[j] = LUcolj[i] -= sum;
			}

			// Find pivot and exchange if necessary.
			//
			// Slightly optimized version of:
			//  for (int i = j+1; i < m; ++i)
			//    if ( fabs(LUcolj[i]) > fabs(LUcolj[p]) )
			//      p = i;
			int p = j;
			typename Matrix::value_type coljp_abs = fabs(LUcolj[p]);
			for (typename Matrix_Col::const_iterator
				beg = LUcolj.begin(),
				ite = beg + j + 1,
				end = LUcolj.end();
				ite < end;
				++ite)
			{
				if (fabs(*ite) > coljp_abs)
				{
					p = ite - beg;
					coljp_abs = fabs(LUcolj[p]);
				}
			}

			if (p != j)
			{
				Matrix_Row raj(a, j);
				Matrix_Row(a, p).swap(raj);

				int tmp = piv[p];
				piv[p] = piv[j];
				piv[j] = tmp;
				pivsign = -pivsign;
			}

			// Compute multipliers.
			if (j < m && a(j, j) != 0.0)
				for (int i = j + 1; i < m; ++i)
					LUcolj[i] /= LUcolj[j];
		}
	}

	// PART 2: SOLVE

	// Check singluarity
	for (int j = 0; j < n; ++j)
		if (a(j, j) == 0)
			return 1;

	// Reorder b according to pivotting
	for (int i = 0; i < m; ++i)
	{
		if (piv[i] != i)
		{
			Matrix_Row b_ri(b, i);
			Matrix_Row(b, piv[i]).swap(b_ri);
			for (int j = i; j < m; ++j)
				if (piv[j] == i)
				{
					piv[j] = piv[i];
					break;
				}
		}
	}

	// Solve L*Y = B(piv,:)
	for (int k = 0; k < n; ++k)
	{
		const Matrix_Row& b_rk = Matrix_Row(b, k);
		for (int i = k + 1; i < n; ++i)
		{
			const typename Matrix_Row::value_type aik = a(i, k);
			Matrix_Row(b, i) -= b_rk * aik;
		}
	}

	// Solve U*X = Y;
	for (int k = n - 1; k >= 0; --k)
	{
		Matrix_Row(b, k) *= 1.0 / a(k, k);

		const Matrix_Row& b_rk = Matrix_Row(b, k);
		for (int i = 0; i < k; ++i)
		{
			const typename Matrix_Row::value_type aik = a(i, k);
			Matrix_Row(b, i) -= b_rk * aik;
		}
	}

	return 0;
}
template <typename T> int ublas_LU_Solve(
	boost::numeric::ublas::matrix<T>& a,
	boost::numeric::ublas::matrix<T>& b)
{
	boost::numeric::ublas::vector<T> c(b.size1());
	boost::numeric::ublas::permutation_matrix<size_t> pm(a.size1());
#pragma omp parallel for
	for (int i = 0; i < b.size1(); i++)
	{
		c(i) = b(i, 0);
	}
	try
	{
		if (0 != ublas::lu_factorize(a, pm))
		{
			return 1;
		}
		//		ublas::lu_factorize(a, pm);
		boost::numeric::ublas::lu_substitute(a, pm, c);

	}
	catch (ublas::singular())
	{
		return 1;
	}
#pragma omp parallel for
	for (int i = 0; i < b.size1(); i++)
	{
		b(i, 0) = c(i);
	}
	return 0;
}

#include "..\CUDAInit.h"
#include <cusolverSp.h>
#include <vector>

template <typename T> int CUDA_LU_Solve(
	boost::numeric::ublas::matrix<T>& a,
	boost::numeric::ublas::matrix<T>& b)
{
	std::vector<T> ValA;
	std::vector<int> RowA;
	std::vector<int> ColIndexA;

	RowA.push_back(0);
	for (int i = 0; i < a.size1(); i++)
	{
		for (int j = 0; j < a.size2(); j++)
		{
			if (a(i, j) != 0.0)
			{
				ValA.push_back(a(i, j));
				ColIndexA.push_back(j);
			}
		}
		RowA.push_back(ValA.size());
	}

	std::vector<T> ValB(b.size1());
	for (int i = 0; i < b.size1(); i++)
	{
		ValB[i] = b(i, 0);
	}
	std::vector<T> ValX(b.size1());
	int sing;
	cusolverStatus_t cso;

	//判断模板类型
	if (sizeof(T) == sizeof(double))
	{
		cso = cusolverSpDcsrlsvluHost(solver_handle, a.size1(), ValA.size(), descr, (const double*)&ValA[0], &RowA[0], &ColIndexA[0], (const double*)&ValB[0], 0.0, 0, (double*)&ValX[0], &sing);

	}
	else if (sizeof(T) == sizeof(float))
	{
		cso = cusolverSpScsrlsvluHost(solver_handle, a.size1(), ValA.size(), descr, (const float*)&ValA[0], &RowA[0], &ColIndexA[0], (const float*)&ValB[0], 0.0, 0, (float*)&ValX[0], &sing);

	}

	if (cso != CUSOLVER_STATUS_SUCCESS || sing != -1)
	{
		return 1;
	}
	for (int i = 0; i < b.size1(); i++)
	{
		b(i, 0) = ValX[i];
	}
	return 0;
}
#endif // LUDECOMPOSITION_H
