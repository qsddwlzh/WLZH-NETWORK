/*
    作者：良州（b站号）
    运行程序后目录下的out.txt为运行日志，里面有当前正在识别的图像和损失函数输出值
    x1.txt、x2.txt、x3.txt均为训练后的权重，按S键可输出权重
    另外一份程序的注释就不写了，代码都差不多
    本程序使用MNIST数据集训练
*/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //用于按键检测，从https://blog.csdn.net/cd_yourheart/article/details/127287605抄来的
#define row 28//输入图像大小
#define line 28
int runx_num = 900;//要跑多少张图片

using namespace std;
double in[row][line];//输入图像数组
double x1[row][line][row][line];//第一层隐藏层权重
double x2[row][line][row][line];//第二层隐藏层权重


double x3_want[row][line][10];//用于运行反向传播
double x1_want[row][line][row][line];
double x2_want[row][line][row][line];
double x_last_want[row][line][2];//用于优化反向传播算法速度

double x3[10][row][line];//输出层层权重
double jg[10];//输出
double gc[row][line];//用来存储神经网络运算过程中的临时数据
double gc_[row][line];//同上

double gc_need[row][line];//用来存储神经网络运算过程中的临时数据和给反向传播算法提供数据
double gc__need[row][line];//同上
double jg_need[10];//同上

double mb[10];//用来存目标结果的数组
double dq;//损失函数输出值
double learing_rate = 0.01;//学习率
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

//神经网络执行部分
double zx() {
    //要求和,所以先初始化
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc_need[i1][i2] = 0;
        }
    }
    //i1和i2为遍历第一个内层的每个神经元i3,i4遍历图像
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc_need[i1][i2] += in[i3][i4] * x1[i1][i2][i3][i4];
                }
            }
            gc[i1][i2] = sz_hs(gc_need[i1][i2]);
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc__need[i1][i2] = 0;
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc__need[i1][i2] += gc[i3][i4] * x2[i1][i2][i3][i4];
                }
            }
            gc_[i1][i2] = sz_hs(gc__need[i1][i2]);
        }
    }
    //最后一层神经元，看过神经网络的应该都能看懂
    for (int i1 = 0; i1 < 10; i1++) {
        jg_need[i1] = 0;
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                jg_need[i1] += gc_[i2][i3] * x3[i1][i2][i3];
            }
        }
        jg[i1] = sz_hs_last(jg_need[i1]);
    }
    //计算损失函数
    double y = 0;
    for (int i = 0; i < 10; i++) {
        y += (jg[i] - mb[i]) * (jg[i] - mb[i]);
    }
    return y;
}
//以下代码用来写入权重
void write_x() {
    freopen("x1.txt", "w", stdout);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cout << x1[i1][i2][i3][i4] << ' ';
                }
            }
        }
    }
    freopen("x2.txt", "w", stdout);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cout << x2[i1][i2][i3][i4] << ' ';
                }
            }
        }
    }
    freopen("x3.txt", "w", stdout);
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                cout << x3[i1][i2][i3] << ' ';
            }
        }
    }
    freopen("out.txt", "w", stdout);
}

int main(int argc, const char* argv[]) {
    srand(time(0));
    freopen("out.txt", "w", stdout);
    //随机初始化权重，若设置为相同的权重则无法训练
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    x1[i1][i2][i3][i4] = rand() * 1.0 / 500000 * pow(-1, (rand() % 2));
                    x2[i1][i2][i3][i4] = rand() * 1.0 / 500000 * pow(-1, (rand() % 2));
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
    short x, y;
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
            for (int i1 = 0; i1 < 28; i1++) {//把文件中的图像录入到数组中
                for (int i2 = 0; i2 < 28; i2++) {
                    infile_IMAGE.read((char*)&x, sizeof(short) / 2);
                    in[i1][i2] = (((((x << 8) + (x >> 8)) & ((2 << 15) - 1))) >> 8) * 1.0 / 255;//这一行因为高低位优先问题要进行转化
                }
            }
            infile_LABE.read((char*)&y, sizeof(short) / 2);
            y = (((((y << 8) + (y >> 8)) & ((2 << 15) - 1))) >> 8);//这一行也一样
            for (int ixn = 0; ixn < 10; ixn++) {//初始化目标数组
                mb[ixn] = 0;
            }
            cout << endl << 'y' << y << endl;//写入日志
            mb[y] = 1;//初始化
            dq = zx();//执行神经网络并返回损失函数值
            cout << dq << endl;//写入日志
            //初始化下面反向传播需要的数组
            for (int i1 = 0; i1 < row; i1++) {
                for (int i2 = 0; i2 < line; i2++) {
                    x_last_want[i1][i2][0] = 0;
                    x_last_want[i1][i2][1] = 0;
                }
            }
            //反向传播
            for (int i_last = 0; i_last < 10; i_last++) {
                for (int i1 = 0; i1 < row; i1++) {
                    for (int i2 = 0; i2 < line; i2++) {
                        x3_want[i1][i2][i_last] += x3[i_last][i1][i2] - learing_rate * (jg[i_last] - mb[i_last]) * gc_[i1][i2] * sz_hs_last_ds(jg_need[i_last]);//链式求导
                        x_last_want[i1][i2][0] += (jg[i_last] - mb[i_last]) * x3[i_last][i1][i2] * sz_hs_last_ds(jg_need[i_last]);//为下一层神经网络不做重复运算
                    }
                }
            }
            for (int i1 = 0; i1 < row; i1++) {
                for (int i2 = 0; i2 < line; i2++) {
                    for (int i3 = 0; i3 < row; i3++) {
                        for (int i4 = 0; i4 < line; i4++) {
                            x2_want[i1][i2][i3][i4] += x2[i1][i2][i3][i4] - learing_rate * x_last_want[i1][i2][0] * gc[i1][i2] * sz_hs_ds(gc__need[i1][i2]);
                            x_last_want[i1][i2][1] += x_last_want[i1][i2][0] * x2[i1][i2][i3][i4] * sz_hs_ds(gc__need[i1][i2]);
                        }
                    }
                }
            }
            for (int i1 = 0; i1 < row; i1++) {
                for (int i2 = 0; i2 < line; i2++) {
                    for (int i3 = 0; i3 < row; i3++) {
                        for (int i4 = 0; i4 < line; i4++) {
                            x1_want[i1][i2][i3][i4] += x1[i1][i2][i3][i4] - learing_rate * x_last_want[i1][i2][1] * in[i1][i2] * sz_hs_ds(gc_need[i1][i2]);//链式求导
                        }
                    }
                }
            }
            //方便输出训练结果
            if (KEY_DOWN('S')) {
                write_x();
            }
        }
        //求平均值并初始化所有值
        for (int i_last = 0; i_last < 10; i_last++) {
            for (int i1 = 0; i1 < row; i1++) {
                for (int i2 = 0; i2 < line; i2++) {
                    x3_want[i1][i2][i_last] = x3_want[i1][i2][i_last] / runx_num;
                    x3[i_last][i1][i2] = x3_want[i1][i2][i_last];
                    x3_want[i1][i2][i_last] = 0;
                }
            }
        }
        for (int i1 = 0; i1 < row; i1++) {
            for (int i2 = 0; i2 < line; i2++) {
                for (int i3 = 0; i3 < row; i3++) {
                    for (int i4 = 0; i4 < line; i4++) {
                        x2_want[i1][i2][i3][i4] = x2_want[i1][i2][i3][i4] / runx_num;
                        x2[i1][i2][i3][i4] = x2_want[i1][i2][i3][i4];
                        x2_want[i1][i2][i3][i4] = 0;
                        x1_want[i1][i2][i3][i4] = x1_want[i1][i2][i3][i4] / runx_num;
                        x1[i1][i2][i3][i4] = x1_want[i1][i2][i3][i4];
                        x1_want[i1][i2][i3][i4] = 0;
                    }
                }
            }
        }
        infile_LABE.close();
        infile_IMAGE.close();
    }


    return 0;
}