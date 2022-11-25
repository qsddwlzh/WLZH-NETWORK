#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#include <vector>
#include <deque>
#include "WNOTEWORK.hpp"
#define row 28
#define line 28
int runx_num = 900;//max is 60000

using namespace std;
vector<vector<double>> IN_IMAGE_DATA_I = vector<vector<double>>(row, vector<double>(line));
vector<vector<vector<vector<vector<double>>>>> NETWORK_WEIGHT_S = vector<vector<vector<vector<vector<double>>>>>(2, vector<vector<vector<vector<double>>>>(row, vector<vector<vector<double>>>(line, vector<vector<double>>(row, vector<double>(line)))));
vector<vector<vector<double>>> x3 = vector<vector<vector<double>>>(10, vector<vector<double>>(row, vector<double>(line)));
vector<double> jg;
double in[row][line] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

double sz_hs(double in_) {//中间层激活函数，目前为Leaky Relu
    return max(in_, in_ * 0.1);
    //return tanh(in_);
}
double sz_hs_ds(double in_) {//中间层激活函数的导数，运行反向传播时用
    if (in_ >= 0) { return 1.0; }
    else if (in_ < 0) { return 0.1; }//Leaky Relu的导数
    //return 1 - tanh(in_) * tanh(in_);//tanh的导数
}
double sz_hs_last(double in_) {//输出层激活函数，目前为tanh
    return tanh(in_);
    //return max(in_, in_ * 0.1);
}
double sz_hs_last_ds(double in_) {//输出层激活函数的导数，运行反向传播时用
    return 1 - tanh(in_) * tanh(in_);//tanh的导数
    //if (in_ >= 0) { return 1.0; }
    //else if (in_ < 0) { return 0.1; }////Leaky Relu的导数
}
int main(int argc, const char* argv[]) {
    for (int i = 0; i < row; i++) {
        for (int i1 = 0; i1 < line; i1++) {
            IN_IMAGE_DATA_I[i][i1] = in[i][i1];
        }
    }
    WLZH_network sz = WLZH_network(28, 28, 2, 10);
    sz.IN_IMAGE(&IN_IMAGE_DATA_I);
    //输入除最后一层外的所有权重
    freopen("x1.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> NETWORK_WEIGHT_S[0][i1][i2][i3][i4];
                }
            }
        }
    }
    freopen("x2.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> NETWORK_WEIGHT_S[1][i1][i2][i3][i4];
                }
            }
        }
    }
    //输入最后一层权重
    freopen("x3.txt", "r", stdin);
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                cin >> x3[i1][i2][i3];
            }
        }
    }
    sz.IN_NETWORK_WEIGHT(&NETWORK_WEIGHT_S);
    sz.IN_LAST_NETWORK_WEIGHT(&x3);
    sz.SET_Activation_function_ELSE_NETWORK(sz_hs);
    sz.SET_Activation_function_last_NETWORK(sz_hs_last);
    int max_kn_number;
    double max_knx = 0;
    freopen("out.txt", "w", stdout);
    jg = sz.RUN_network();
    max_knx = 0;
    for (int i_now_number = 0; i_now_number < 10; i_now_number++) {
        if (jg[i_now_number] > max_knx) {
            max_kn_number = i_now_number;
            max_knx = jg[i_now_number];
        }
    }
    cout << "识别为:" << max_kn_number << " 可能性:" << max_knx * 100 << '%' << endl;
}