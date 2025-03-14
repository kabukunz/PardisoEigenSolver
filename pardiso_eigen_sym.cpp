/*******************************************************************************
 * Copyright 2004-2022 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted  materials,  and
 * your use of  them is  governed by the  express license  under which  they were
 * provided to you (License).  Unless the License provides otherwise, you may not
 * use, modify, copy, publish, distribute,  disclose or transmit this software or
 * the related documents without Intel's prior written permission.
 *
 * This software and the related documents  are provided as  is,  with no express
 * or implied  warranties,  other  than those  that are  expressly stated  in the
 * License.
 *******************************************************************************/

/*
 *   Content : Intel(R) oneAPI Math Kernel Library (oneMKL) PARDISO C example
 *
 ********************************************************************************
 */
/* -------------------------------------------------------------------- */
/* Example program to show the use of the "PARDISO" routine */
/* on symmetric linear systems */
/* -------------------------------------------------------------------- */
/* This program can be downloaded from the following site: */
/* www.pardiso-project.org */
/* */
/* (C) Olaf Schenk, Department of Computer Science, */
/* University of Basel, Switzerland. */
/* Email: olaf.schenk@unibas.ch */
/* -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mkl_pardiso.h"
#include "mkl_types.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <unsupported/Eigen/SparseExtra>

using namespace Eigen;

// Define the format to printf MKL_INT values
#if !defined(MKL_ILP64)
#define IFORMAT "%i"
#else
#define IFORMAT "%lli"
#endif

int main(void)
{
    /* Matrix data. */
    MKL_INT n = 8;
    MKL_INT ia[9] = {1, 5, 8, 10, 12, 15, 17, 18, 19};
    MKL_INT ja[18] =
        {1, 3, 6, 7,
         2, 3, 5,
         3, 8,
         4, 7,
         5, 6, 7,
         6, 8,
         7,
         8};
    double a[18] =
        {7.0, 1.0, 2.0, 7.0,
         -4.0, 8.0, 2.0,
         1.0, 5.0,
         7.0, 9.0,
         5.0, 1.0, 5.0,
         -1.0, 5.0,
         11.0,
         5.0};
    MKL_INT mtype = -2; /* Real symmetric matrix */
    /* RHS and solution vectors. */
    double b[8], x[8];
    MKL_INT nrhs = 1; /* Number of right hand sides. */
    /* Internal solver memory pointer pt, */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
    /* or void *pt[64] should be OK on both architectures */
    void *pt[64];
    /* Pardiso control parameters. */
    MKL_INT iparm[64];
    MKL_INT maxfct, mnum, phase, error, msglvl;
    /* Auxiliary variables. */
    MKL_INT i;
    double ddum;  /* Double dummy */
    MKL_INT idum; /* Integer dummy. */
                  /* -------------------------------------------------------------------- */
                  /* .. Setup Pardiso control parameters. */
                  /* -------------------------------------------------------------------- */
    for (i = 0; i < 64; i++)
    {
        iparm[i] = 0;
    }
    iparm[0] = 1;   /* No solver default */
    iparm[1] = 2;   /* Fill-in reordering from METIS */
    iparm[3] = 0;   /* No iterative-direct algorithm */
    iparm[4] = 0;   /* No user fill-in reducing permutation */
    iparm[5] = 0;   /* Write solution into x */
    iparm[6] = 0;   /* Not in use */
    iparm[7] = 2;   /* Max numbers of iterative refinement steps */
    iparm[8] = 0;   /* Not in use */
    iparm[9] = 13;  /* Perturb the pivot elements with 1E-13 */
    iparm[10] = 1;  /* Use nonsymmetric permutation and scaling MPS */
    iparm[11] = 0;  /* Not in use */
    iparm[12] = 0;  /* Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
    iparm[13] = 0;  /* Output: Number of perturbed pivots */
    iparm[14] = 0;  /* Not in use */
    iparm[15] = 0;  /* Not in use */
    iparm[16] = 0;  /* Not in use */
    iparm[17] = -1; /* Output: Number of nonzeros in the factor LU */
    iparm[18] = -1; /* Output: Mflops for LU factorization */
    iparm[19] = 0;  /* Output: Numbers of CG Iterations */
    maxfct = 1;     /* Maximum number of numerical factorizations. */
    mnum = 1;       /* Which factorization to use. */
    msglvl = 1;     /* Print statistical information in file */
    error = 0;      /* Initialize error flag */
                    /* -------------------------------------------------------------------- */
                    /* .. Initialize the internal solver memory pointer. This is only */
                    /* necessary for the FIRST call of the PARDISO solver. */
                    /* -------------------------------------------------------------------- */
    for (i = 0; i < 64; i++)
    {
        pt[i] = 0;
    }
    /* -------------------------------------------------------------------- */
    /* .. Reordering and Symbolic Factorization. This step also allocates */
    /* all memory that is necessary for the factorization. */
    /* -------------------------------------------------------------------- */
    phase = 11;
    PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if (error != 0)
    {
        printf("\nERROR during symbolic factorization: " IFORMAT, error);
        exit(1);
    }
    printf("\nReordering completed ... ");
    printf("\nNumber of nonzeros in factors = " IFORMAT, iparm[17]);
    printf("\nNumber of factorization MFLOPS = " IFORMAT, iparm[18]);
    /* -------------------------------------------------------------------- */
    /* .. Numerical factorization. */
    /* -------------------------------------------------------------------- */
    phase = 22;
    PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if (error != 0)
    {
        printf("\nERROR during numerical factorization: " IFORMAT, error);
        exit(2);
    }
    printf("\nFactorization completed ... ");
    /* -------------------------------------------------------------------- */
    /* .. Back substitution and iterative refinement. */
    /* -------------------------------------------------------------------- */
    phase = 33;
    iparm[7] = 2; /* Max numbers of iterative refinement steps. */
    /* Set right hand side to one. */
    for (i = 0; i < n; i++)
    {
        b[i] = 1;
    }
    PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, &idum, &nrhs, iparm, &msglvl, b, x, &error);
    if (error != 0)
    {
        printf("\nERROR during solution: " IFORMAT, error);
        exit(3);
    }
    printf("\nSolve completed ... ");
    printf("\nThe solution of the system is: ");
    for (i = 0; i < n; i++)
    {
        printf("\n x [" IFORMAT "] = % f", i, x[i]);
    }
    printf("\n");
    /* -------------------------------------------------------------------- */
    /* .. Termination and release of memory. */
    /* -------------------------------------------------------------------- */
    phase = -1; /* Release internal memory. */
    PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            &n, &ddum, ia, ja, &idum, &nrhs,
            iparm, &msglvl, &ddum, &ddum, &error);

    //
    // Eigen solver
    //

    printf("\n");
    printf("Starting Eigen solver\n");
    printf("\n");

    // fill A
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList;
    tripletList.reserve(sizeof(a));

    for (size_t k = 0; k < sizeof(a); k++)
    {
        double a_ = a[k];
        int i_ = ia[k];
        int j_ = ja[k];

        tripletList.push_back(T(i_, j_, a_));
    }

    printf("After T\n");

    Eigen::SparseMatrix<double> A;
    A.resize(sizeof(ia), sizeof(ja));
    A.setFromTriplets(tripletList.begin(), tripletList.end());
    A.makeCompressed();

    printf("After A\n");

    // fill b
    Eigen::VectorXd b_;
    b_.resize(sizeof(b));
    for (size_t i = 0; i < sizeof(b); i++)
        b_(i) = b[i];

    // 
    Eigen::MatrixXd AF = A;

    std::string sep = "\n----------------------------------------\n";

    IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", " << ", ";");
    IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    IOFormat OctaveFmt(StreamPrecision, 0, ", ", ";\n", "", "", "[", "]");
    IOFormat HeavyFmt(FullPrecision, 0, ", ", ";\n", "[", "]", "[", "]");

    std::cout << AF << sep;
    std::cout << AF.format(CommaInitFmt) << sep;
    std::cout << AF.format(CleanFmt) << sep;
    std::cout << AF.format(OctaveFmt) << sep;
    std::cout << AF.format(HeavyFmt) << sep;

    // CHECK: should be upper triangular

    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>, Eigen::Upper> solver;

    solver.compute(A);

    // decomposition failed
    if (solver.info() != Eigen::Success)
    {
        //    assert(false);
        return EXIT_FAILURE;
    }

    Eigen::VectorXd x_;
    x_ = solver.solve(b_);

    // solver failed
    if (solver.info() != Eigen::Success)
    {
        //    assert(false);
        return EXIT_FAILURE;
    }

    // TODO: from x to pardiso.result
    // for (size_t i = 0; i < x.size(); i++)
    //     pardiso.result.at(i) = x(i);

    Eigen::saveMarket(A, "A.mtx");
    Eigen::saveMarket(A, "A_SPD.mtx", Eigen::Symmetric); // if A is symmetric-positive-definite
    Eigen::saveMarketVector(b_, "b_.mtx");
    Eigen::saveMarketVector(x_, "x.mtx");

    //
    // Eigen solver
    //

    return 0;
}
