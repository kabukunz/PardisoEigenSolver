
pardiso_sym.cpp

preso dagli esempi mkl in:

C:\Program Files (x86)\Intel\oneAPI\mkl\2023.0.0\examples\examples_core_c.zip

examples_core_c\c\sparse_directsolvers\source

-----

pardiso uses:

compressed sparse row format

https://en.wikipedia.org/wiki/Sparse_matrix#:~:text=The%20compressed%20sparse%20row%20(CSR,row%20indices%2C%20hence%20the%20name.

V         = [ 10 20 30 40 50 60 70 80 ]
COL_INDEX = [  0  1  1  3  2  3  4  5 ]   
ROW_INDEX = [  0  2  4  7  8 ]
The whole is stored as 21 entries: 8 in V, 8 in COL_INDEX, and 5 in ROW_INDEX.

ROW_INDEX splits the array V into rows: (10, 20) (30, 40) (50, 60, 70) (80), indicating the index of V (and COL_INDEX) where each row starts and ends;
COL_INDEX aligns values in columns: (10, 20, ...) (0, 30, 0, 40, ...)(0, 0, 50, 60, 70, 0) (0, 0, 0, 0, 0, 80).

last row index (8) is end of matrix (fictitious)

pardiso:

/* Matrix data. */
MKL_INT n = 8;
MKL_INT ia[9] = { 1, 5, 8, 10, 12, 15, 17, 18, 19};
MKL_INT ja[18] = 
{ 1,    3,       6, 7,
        2, 3,    5,
        3,             8,
            4,       7,
                5, 6, 7,
                    6,    8,
                    7,
                        8
};
double a[18] = 
{ 7.0,      1.0,           2.0, 7.0,
        -4.0, 8.0,      2.0,
            1.0,                     5.0,
                    7.0,           9.0,
                        5.0, 1.0, 5.0,
                            -1.0,      5.0,
                                11.0,
                                        5.0
};
MKL_INT mtype = -2;       /* Real symmetric matrix */

cominciano da 1 e ia è una in più.

Eigen

https://eigen.tuxfamily.org/dox/group__TutorialSparse.html

The SparseMatrix class

The class SparseMatrix is the main sparse matrix representation of Eigen's sparse module; it offers high performance and low memory usage. It implements a more versatile variant of the widely-used Compressed Column (or Row) Storage scheme. It consists of four compact arrays:

Values: stores the coefficient values of the non-zeros.
InnerIndices: stores the row (resp. column) indices of the non-zeros.
OuterStarts: stores for each column (resp. row) the index of the first non-zero in the previous two arrays.
InnerNNZs: stores the number of non-zeros of each column (resp. row). The word inner refers to an inner vector that is a column for a column-major matrix, or a row for a row-major matrix. The word outer refers to the other direction.

and one of its possible sparse, column major representation:

Values:	        22	7	_	3	5	14	_	_	1	_	17	8
InnerIndices:	1	2	_	0	2	4	_	_	2	_	1	4
OuterStarts:	0	3	5	8	10	12
InnerNNZs:	    2	2	1	1	2	

