/*
    ���ߣ����ݣ�bվ�ţ�
    ���г����Ŀ¼�µ�out.txtΪ������־�������е�ǰ����ʶ���ͼ�����ʧ�������ֵ
    x1.txt��x2.txt��x3.txt��Ϊѵ�����Ȩ�أ���S�������Ȩ��
    ����һ�ݳ����ע�;Ͳ�д�ˣ����붼���
    ������ʹ��MNIST���ݼ�ѵ��
*/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //���ڰ�����⣬��https://blog.csdn.net/cd_yourheart/article/details/127287605������
#define row 28//����ͼ���С
#define line 28
int runx_num = 900;//Ҫ�ܶ�����ͼƬ

using namespace std;
double in[row][line];//����ͼ������
double x1[row][line][row][line];//��һ�����ز�Ȩ��
double x2[row][line][row][line];//�ڶ������ز�Ȩ��


double x3_want[row][line][10];//�������з��򴫲�
double x1_want[row][line][row][line];
double x2_want[row][line][row][line];
double x_last_want[row][line][2];//�����Ż����򴫲��㷨�ٶ�

double x3[10][row][line];//������Ȩ��
double jg[10];//���
double gc[row][line];//�����洢��������������е���ʱ����
double gc_[row][line];//ͬ��

double gc_need[row][line];//�����洢��������������е���ʱ���ݺ͸����򴫲��㷨�ṩ����
double gc__need[row][line];//ͬ��
double jg_need[10];//ͬ��

double mb[10];//������Ŀ����������
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

//������ִ�в���
double zx() {
    //Ҫ���,�����ȳ�ʼ��
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc_need[i1][i2] = 0;
        }
    }
    //i1��i2Ϊ������һ���ڲ��ÿ����Ԫi3,i4����ͼ��
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
    //���һ����Ԫ�������������Ӧ�ö��ܿ���
    for (int i1 = 0; i1 < 10; i1++) {
        jg_need[i1] = 0;
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                jg_need[i1] += gc_[i2][i3] * x3[i1][i2][i3];
            }
        }
        jg[i1] = sz_hs_last(jg_need[i1]);
    }
    //������ʧ����
    double y = 0;
    for (int i = 0; i < 10; i++) {
        y += (jg[i] - mb[i]) * (jg[i] - mb[i]);
    }
    return y;
}
//���´�������д��Ȩ��
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
    //�����ʼ��Ȩ�أ�������Ϊ��ͬ��Ȩ�����޷�ѵ��
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
                    in[i1][i2] = (((((x << 8) + (x >> 8)) & ((2 << 15) - 1))) >> 8) * 1.0 / 255;//��һ����Ϊ�ߵ�λ��������Ҫ����ת��
                }
            }
            infile_LABE.read((char*)&y, sizeof(short) / 2);
            y = (((((y << 8) + (y >> 8)) & ((2 << 15) - 1))) >> 8);//��һ��Ҳһ��
            for (int ixn = 0; ixn < 10; ixn++) {//��ʼ��Ŀ������
                mb[ixn] = 0;
            }
            cout << endl << 'y' << y << endl;//д����־
            mb[y] = 1;//��ʼ��
            dq = zx();//ִ�������粢������ʧ����ֵ
            cout << dq << endl;//д����־
            //��ʼ�����淴�򴫲���Ҫ������
            for (int i1 = 0; i1 < row; i1++) {
                for (int i2 = 0; i2 < line; i2++) {
                    x_last_want[i1][i2][0] = 0;
                    x_last_want[i1][i2][1] = 0;
                }
            }
            //���򴫲�
            for (int i_last = 0; i_last < 10; i_last++) {
                for (int i1 = 0; i1 < row; i1++) {
                    for (int i2 = 0; i2 < line; i2++) {
                        x3_want[i1][i2][i_last] += x3[i_last][i1][i2] - learing_rate * (jg[i_last] - mb[i_last]) * gc_[i1][i2] * sz_hs_last_ds(jg_need[i_last]);//��ʽ��
                        x_last_want[i1][i2][0] += (jg[i_last] - mb[i_last]) * x3[i_last][i1][i2] * sz_hs_last_ds(jg_need[i_last]);//Ϊ��һ�������粻���ظ�����
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
                            x1_want[i1][i2][i3][i4] += x1[i1][i2][i3][i4] - learing_rate * x_last_want[i1][i2][1] * in[i1][i2] * sz_hs_ds(gc_need[i1][i2]);//��ʽ��
                        }
                    }
                }
            }
            //�������ѵ�����
            if (KEY_DOWN('S')) {
                write_x();
            }
        }
        //��ƽ��ֵ����ʼ������ֵ
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