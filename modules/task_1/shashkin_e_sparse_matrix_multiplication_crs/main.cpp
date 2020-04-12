// Copyright 2020 Shashkin Evgeny
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../../modules/task_1/shashkin_e_sparse_matrix_multiplication_crs/sparse_matrix_multiplication_crs.h"

TEST(SparceMatrixMultiplication, throw_when_num_of_rows_or_num_of_cols_is_negative) {
  ASSERT_ANY_THROW(SparseComplexMatrix mat(10, -1));
  ASSERT_ANY_THROW(SparseComplexMatrix mat(-10, 1));
  ASSERT_ANY_THROW(SparseComplexMatrix mat(-10, -10));
}

TEST(SparceMatrixMultiplication, can_convert_regular_matrix_to_csr) {
  std::vector<std::vector<std::complex<double>>> mat;
  SparseComplexMatrix csrMat;
  mat = randomMatrix(5, 5);
  ASSERT_NO_THROW(csrMat.matrixToCRS(mat));
}

TEST(SparceMatrixMultiplication, can_transpose) {
  std::vector<std::complex<double>> vals_trans = { std::complex<double>(0, 5), std::complex<double>(7, 9) };
  std::vector<int> col_ind_trans = { 0, 0 };
  std::vector<int> row_ind_trans = { 0, 0, 1, 1, 2, 2, 2 };

  std::vector<std::complex<double>> vals = { std::complex<double>(0, 5), std::complex<double>(7, 9) };
  std::vector<int> col_ind = { 1, 3 };
  std::vector<int> row_ind = { 0, 2, 2, 2 };

  SparseComplexMatrix mat(3, 6, vals, col_ind, row_ind);
  SparseComplexMatrix mat_trans(6, 3, vals_trans, col_ind_trans, row_ind_trans);
  SparseComplexMatrix tmp;
  tmp = mat.transpose();

  ASSERT_TRUE(mat_trans == tmp);
}

TEST(SparceMatrixMultiplication, can_multimply_square_csr_matrices) {
  int size = 5;
  std::vector<std::vector<std::complex<double>>> mat1;
  std::vector<std::vector<std::complex<double>>> mat2;
  SparseComplexMatrix csrMat1;
  SparseComplexMatrix csrMat2;
  SparseComplexMatrix csrMat3;
  mat1 = randomMatrix(size, size);
  mat2 = randomMatrix(size, size);
  csrMat1.matrixToCRS(mat1);
  csrMat2.matrixToCRS(mat2);
  csrMat2.transpose();
  ASSERT_NO_THROW(csrMat3 = csrMat1 * csrMat2);
}

TEST(SparceMatrixMultiplication, can_multimply_not_square_csr_matrices) {
  int rows1 = 5;
  int cols1 = 3;
  int rows2 = 3;
  int cols2 = 6;
  std::vector<std::vector<std::complex<double>>> mat1;
  std::vector<std::vector<std::complex<double>>> mat2;
  SparseComplexMatrix csrMat1;
  SparseComplexMatrix csrMat2;
  SparseComplexMatrix csrMat2T;
  SparseComplexMatrix csrMat3;
  mat1 = randomMatrix(rows1, cols1);
  mat2 = randomMatrix(rows2, cols2);
  csrMat1.matrixToCRS(mat1);
  csrMat2.matrixToCRS(mat2);
  csrMat2T = csrMat2.transpose();
  ASSERT_NO_THROW(csrMat3 = csrMat1 * csrMat2T);
}

TEST(SparceMatrixMultiplication, can_multimply_not_square_csr_matrices_correctly) {
  int rows1 = 10;
  int cols1 = 5;
  int rows2 = 5;
  int cols2 = 12;
  std::vector<std::complex<double>> vals1 = {
    std::complex<double>(8, 5),
    std::complex<double>(5, 2),
    std::complex<double>(0, 7),
    std::complex<double>(7, 2),
    std::complex<double>(8, 3),
    std::complex<double>(8, 1),
    std::complex<double>(8, 7)
  };
  std::vector<int> col_ind1 = { 2, 2, 3, 0, 3, 4, 2 };
  std::vector<int> row_ind1 = { 0, 0, 1, 2, 3, 3, 3, 6, 6, 6, 7 };

  std::vector<std::complex<double>> vals2 = {
    std::complex<double>(8, 5),
    std::complex<double>(5, 2),
    std::complex<double>(0, 7),
    std::complex<double>(7, 2),
    std::complex<double>(8, 3),
    std::complex<double>(8, 1),
    std::complex<double>(8, 7),
    std::complex<double>(8, 0),
    std::complex<double>(2, 6),
    std::complex<double>(6, 2),
    std::complex<double>(7, 5)
  };
  std::vector<int> col_ind2 = { 7, 0, 6, 6, 9, 10, 11, 5, 6, 7, 11 };
  std::vector<int> row_ind2 = { 0, 1, 3, 6, 7, 11 };

  std::vector<std::complex<double>> res = {
    std::complex<double>(46, 51),
    std::complex<double>(49, 64),
    std::complex<double>(59, 48),
    std::complex<double>(31, 24),
    std::complex<double>(34, 31),
    std::complex<double>(38, 21),
    std::complex<double>(-49, 56),
    std::complex<double>(64, 8),
    std::complex<double>(10, 50),
    std::complex<double>(92, 73),
    std::complex<double>(94, 127),
    std::complex<double>(42, 65),
    std::complex<double>(43, 80),
    std::complex<double>(57, 64)
  };
  std::vector<int> col_ind_res = { 6, 9, 10, 6, 9, 10, 11, 5, 6, 7, 6, 9, 10 };
  std::vector<int> row_ind_res = { 0, 0, 3, 6, 7, 7, 7, 11, 11, 11, 14 };

  SparseComplexMatrix csrMat1(rows1, cols1, vals1, col_ind1, row_ind1);
  SparseComplexMatrix csrMat2(rows2, cols2, vals2, col_ind2, row_ind2);
  SparseComplexMatrix res_mat(cols1, rows2, res, col_ind_res, row_ind_res);
  SparseComplexMatrix csrMat2T;
  SparseComplexMatrix csrMat3;
  csrMat2T = csrMat2.transpose();
  csrMat3 = csrMat1 * csrMat2T;
  ASSERT_TRUE(csrMat3 == res_mat);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
