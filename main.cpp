//此例程中删除了神经网络权重输出部分

#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#include "WNOTEWORK.hpp"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //用于按键检测，从https://blog.csdn.net/cd_yourheart/article/details/127287605抄来的
#define row 28//输入图像大小
#define line 28
int runx_num = 10;//要跑多少张图片
using namespace std;
WLZH_network sz = WLZH_network(785, 800, 4, 10);
double dq = 10;//损失函数输出值
double learing_rate = 0.001;//学习率
double sz_hs(double in_) {//中间层激活函数，目前为Leaky Relu
    return max(in_, in_ * 0.1);
    //return tanh(in_);
}
double sz_hs_ds(double in_) {//中间层激活函数的导数，运行反向传播时用
    if (in_ >= 0) { return 1.0; }
    else if (in_ < 0) { return -0.1; }//Leaky Relu的导数
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

//损失函数任意一项的导数，输入为当前神经元输出值与期望值
/*
如神经网络输出值为a1、a2、a3...
期望值为b1、b2、b3...
且损失函数为(1/2)[(a1-b1)*(a2-b2)+(a2-b2)*(a3-b3)+(a3-b3)*(a3-b3)+...]
则此函数为(1/2)(ax-bx)*(ax-bx)的导数
为ax-bx
*/
double Function_based_Ds(double in_1, double in_2) {
    return in_1 - in_2;
}
double Function_B(double *in, double *x,size_t len) {//损失函数
    double out = 0;
    for (int i = 0; i < len; i++) {
        out += 0.5 * (in[i] - x[i]) * (in[i] - x[i]);
        //cout << in[i] << "  " << x[i] << ' ';
    }
    //cout << endl;
    //cout << out << endl << endl;
    return out;
}//损失函数
vector<double*> in_s = vector<double*>(runx_num);
double **mb;
int i_d = 0;
int main(int argc, const char* argv[]) {
    sz.INIT_RAND_WEIGHT(1000000,time(0));
    sz.SET_Function_based_Derivatives(Function_based_Ds);
    sz.SET_Activation_function_ELSE_NETWORK(sz_hs);
    sz.SET_Activation_function_last_NETWORK(sz_hs_last);
    sz.SET_Function_Based(Function_B);
    sz.SET_Learn_Rate(0.005);
    sz.SET_Activation_function_Derivatives_NETWORK_Else(sz_hs_ds);
    sz.SET_Activation_function_Derivatives_NETWORK_Last(sz_hs_last_ds);
    short x, y;

    for (int ix = 0; ix < runx_num; ix++) {//这个for是为了跑完指定数量的图像
        in_s[ix] = new double[row * line];
    }
    mb = new double*[runx_num];
    for (int ix = 0; ix < runx_num; ix++) {//这个for是为了跑完指定数量的图像
        mb[ix] = new double[10];
    }
    while (1) {
        //打开图像和标签文件并读取，具体写法参考MNIST文件格式
        ifstream infile_IMAGE("train-images.idx3-ubyte", ios::in | ios::binary);
        ifstream infile_LABE("train-labels.idx1-ubyte", ios::in | ios::binary);

        if (!infile_IMAGE || !infile_LABE) {
            cerr << "open error!" << endl;
            return 1;
        }
        for (int i = 0; i < 8; i++) {
            infile_IMAGE.read((char*)&x, sizeof(short));
        }
        for (int i = 0; i < 4; i++) {
            infile_LABE.read((char*)&y, sizeof(short));
        }

        for (int ix = 0; ix < runx_num; ix++) {//这个for是为了跑完指定数量的图像
            in_s[ix][784] = 1.0;
            for (int i1 = 0; i1 < 28; i1++) {//把文件中的图像录入到数组中
                for (int i2 = 0; i2 < 28; i2++) {
                    infile_IMAGE.read((char*)&x, sizeof(short) / 2);
                    in_s[ix][i1 * row + i2] = (((((x << 8) + (x >> 8)) & ((2 << 15) - 1))) >> 8) * 1.0 / 255;//这一行因为高低位优先问题要进行转化，当然有很多写法
                }
            }
            infile_LABE.read((char*)&y, sizeof(short) / 2);
            y = (((((y << 8) + (y >> 8)) & ((2 << 15) - 1))) >> 8);//这一行也一样
            for (int i = 0; i < 10; i++) {
                mb[ix][i] = 0;
            }
            mb[ix][y] = 1;
            //初始化下面反向传播需要的数组
            //方便输出训练结果
            
        }
        dq = sz.Back_Propagation(in_s, mb);
        cout << "损失函数输出值:" << dq << endl;
        infile_LABE.close();
        infile_IMAGE.close();
    }


    return 0;
}
