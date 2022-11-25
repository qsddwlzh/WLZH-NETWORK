#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#include "WNOTEWORK.hpp"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //���ڰ�����⣬��https://blog.csdn.net/cd_yourheart/article/details/127287605������
#define row 28//����ͼ���С
#define line 28
int runx_num = 900;//Ҫ�ܶ�����ͼƬ

using namespace std;
WLZH_network sz = WLZH_network(28, 28, 2, 10);
vector<vector<vector<vector<vector<double>>>>> NETWORK_WEIGHT_ = vector<vector<vector<vector<vector<double>>>>>(2, vector<vector<vector<vector<double>>>>(row, vector<vector<vector<double>>>(line, vector<vector<double>>(row, vector<double>(line)))));
double dq;//��ʧ�������ֵ
double learing_rate = 0.01;//ѧϰ��
double sz_hs(double in_) {//�м�㼤�����ĿǰΪLeaky Relu
    return max(in_, in_ * 0.1);
    //return tanh(in_);
}
double sz_hs_ds(double in_) {//�м�㼤����ĵ��������з��򴫲�ʱ��
    if (in_ >= 0) { return 1.0; }
    else if (in_ < 0) { return 0.1; }//Leaky Relu�ĵ���
    //return 1 - tanh(in_) * tanh(in_);//tanh�ĵ���
}
double sz_hs_last(double in_) {//����㼤�����ĿǰΪtanh
    return tanh(in_);
    //return max(in_, in_ * 0.1);
}
double sz_hs_last_ds(double in_) {//����㼤����ĵ��������з��򴫲�ʱ��
    return 1 - tanh(in_) * tanh(in_);//tanh�ĵ���
    //if (in_ >= 0) { return 1.0; }
    //else if (in_ < 0) { return 0.1; }////Leaky Relu�ĵ���
}
double Function_B(vector<double> in, vector<double> x) {
    double out = 0;
    for (int i = 0; i < x.size(); i++) {
        out += (in[i] - x[i]) * (in[i] - x[i]);
    }
    return out;
}//��ʧ����
//�����������Ȩ��
void write_x() {
    freopen("x1.txt", "w", stdout);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cout << sz.NETWORK_WEIGHT[0][i1][i2][i3][i4] << ' ';
                }
            }
        }
    }
    freopen("x2.txt", "w", stdout);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cout << sz.NETWORK_WEIGHT[1][i1][i2][i3][i4] << ' ';
                }
            }
        }
    }
    freopen("x3.txt", "w", stdout);
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                cout << sz.LAST_NETWORK_WEIGHT[i1][i2][i3] << ' ';
            }
        }
    }
    freopen("out.txt", "w", stdout);
}
vector<vector<vector<double>>> x3 = vector<vector<vector<double>>>(10, vector<vector<double>>(row, vector<double>(line)));
vector<vector<vector<double>>> in_s = vector<vector<vector<double>>>(runx_num, vector<vector<double>>(row, vector<double>(line)));
vector<vector<double>> mb = vector<vector<double>>(runx_num, vector<double>(10));
int main(int argc, const char* argv[]) {
    srand(time(0));
    //freopen("out.txt", "w", stdout);
    //�����ʼ��Ȩ�أ�������Ϊ��ͬ��Ȩ�����޷�ѵ��

    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    NETWORK_WEIGHT_[0][i1][i2][i3][i4] = rand() * 1.0 / 500000 * pow(-1, (rand() % 2));
                    NETWORK_WEIGHT_[1][i1][i2][i3][i4] = rand() * 1.0 / 500000 * pow(-1, (rand() % 2));
                }
            }
        }
    }
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                x3[i1][i2][i3] = rand() * 1.0 / 500000 * pow(-1, (rand() % 2));
            }
        }
    }
    sz.IN_NETWORK_WEIGHT(&NETWORK_WEIGHT_);
    sz.IN_LAST_NETWORK_WEIGHT(&x3);
    sz.SET_Activation_function_ELSE_NETWORK(sz_hs);
    sz.SET_Activation_function_last_NETWORK(sz_hs_last);
    sz.SET_Function_Based(Function_B);
    sz.SET_Learn_Rate(0.01);
    sz.SET_Activation_function_Derivatives_NETWORK_Else(sz_hs_ds);
    sz.SET_Activation_function_Derivatives_NETWORK_Last(sz_hs_last_ds);
    short x, y;
    while (1) {
        //��ͼ��ͱ�ǩ�ļ�����ȡ������д���ο�MNIST�ļ���ʽ
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

        for (int ix = 0; ix < runx_num; ix++) {//���for��Ϊ������ָ��������ͼ��
            for (int i1 = 0; i1 < 28; i1++) {//���ļ��е�ͼ��¼�뵽������
                for (int i2 = 0; i2 < 28; i2++) {
                    infile_IMAGE.read((char*)&x, sizeof(short) / 2);
                    in_s[ix][i1][i2] = (((((x << 8) + (x >> 8)) & ((2 << 15) - 1))) >> 8) * 1.0 / 255;//��һ����Ϊ�ߵ�λ��������Ҫ����ת������Ȼ�кܶ�д��
                }
            }
            infile_LABE.read((char*)&y, sizeof(short) / 2);
            y = (((((y << 8) + (y >> 8)) & ((2 << 15) - 1))) >> 8);//��һ��Ҳһ��
            for (int i = 0; i < 10; i++) {
                mb[ix][i] = 0;
            }
            mb[ix][y] = 1;
            //��ʼ�����淴�򴫲���Ҫ������
            //�������ѵ�����
            if (KEY_DOWN('S')) {
                write_x();
            }
        }
        dq = sz.Back_Propagation(in_s, mb);
        cout << y << ' ' << dq << endl;
        infile_LABE.close();
        infile_IMAGE.close();
    }


    return 0;
}