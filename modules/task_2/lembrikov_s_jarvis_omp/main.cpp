// Copyright 2020 Lembrikov Stepan
#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include "./jarvis_omp.h"

TEST(Vector_Min_MPI, Test_On_3_Points_EQ) {
    size_t size = 3;
    int num_thr = 2;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> answer_omp;
    std::vector<std::pair<double, double>> pr(size);

    pr[0].first = 3;
    pr[0].second = 4;
    pr[1].first = 4;
    pr[1].second = 3;
    pr[2].first = 0;
    pr[2].second = 0;
    answer_omp = Jarvis_Omp(pr, num_thr);
    answer_seq = Jarvis_Seq(pr);

    EXPECT_EQ(answer_omp, answer_seq);
}

TEST(Vector_Min_MPI, Test_On_7_Points_EQ) {
    size_t size = 7;
    int num_thr = 2;
    std::vector<std::pair<double, double>> answer_omp;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> pr(size);

    pr[0].first = 0;
    pr[0].second = 0;
    pr[1].first = 5;
    pr[1].second = 0;
    pr[2].first = 1;
    pr[2].second = 2;
    pr[3].first = 5;
    pr[3].second = 3;
    pr[4].first = 3;
    pr[4].second = 4;
    pr[5].first = 1;
    pr[5].second = 5;
    pr[6].first = 3;
    pr[6].second = 6;
    answer_omp = Jarvis_Omp(pr, num_thr);
    answer_seq = Jarvis_Seq(pr);

    EXPECT_EQ(answer_omp, answer_seq);
}

TEST(Vector_Min_MPI, Test_On_1_Point_EQ) {
    size_t size = 1;
    int num_thr = 2;
    std::vector<std::pair<double, double>> answer_omp;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> pr(size);

    pr[0].first = 3;
    pr[0].second = 4;
    answer_omp = Jarvis_Omp(pr, num_thr);
    answer_seq = Jarvis_Seq(pr);

    EXPECT_EQ(answer_omp, answer_seq);
}

TEST(Vector_Min_MPI, Test_On_2_Equal_Points_EQ) {
    size_t size = 2;
    int num_thr = 2;
    std::vector<std::pair<double, double>> answer_omp;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> pr(size);

    pr[0].first = 3;
    pr[0].second = 4;
    pr[1].first = 3;
    pr[1].second = 4;
    answer_omp = Jarvis_Omp(pr, num_thr);
    answer_seq = Jarvis_Seq(pr);

    EXPECT_EQ(answer_omp, answer_seq);
}

TEST(Vector_Min_MPI, Test_On_Random_Points_EQ) {
    std::vector<std::pair<double, double>> a;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> answer_omp;
    size_t size_a = 10;
    int num_thr = 2;

    a = getRandomPoints(size_a);
    answer_seq = Jarvis_Seq(a);
    answer_omp = Jarvis_Omp(a, num_thr);

    EXPECT_EQ(answer_seq, answer_omp);
}

TEST(Vector_Min_MPI, Test_On_Negative_Points_EQ) {
    std::vector<std::pair<double, double>> a;
    std::vector<std::pair<double, double>> answer_seq;
    std::vector<std::pair<double, double>> answer_omp;
    size_t size_a = 10;
    int num_thr = 2;

    a = getNegativePoints(size_a);
    answer_omp = Jarvis_Omp(a, num_thr);
    answer_seq = Jarvis_Seq(a);

    EXPECT_EQ(answer_seq, answer_omp);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
