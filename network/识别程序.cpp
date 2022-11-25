#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#define row 28
#define line 28
int runx_num = 900;//max is 60000
using namespace std;
double in[row][line] = {};//这是输入数组
double x1[row][line][row][line];
double x2[row][line][row][line];
double x3[10][row][line];
double jg[10];
double gc[row][line];
double gc_[row][line];
double sz_hs(double in_) {
    return max(in_, in_ * 0.1);
    //return tanh(in_);
}
double sz_hs_last(double in_) {
    return tanh(in_);
    //return max(in_, in_ * 0.1);
}
void zx_network() {
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc[i1][i2] = 0;
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc[i1][i2] += in[i3][i4] * x1[i1][i2][i3][i4];
                }
            }
            gc[i1][i2] = sz_hs(gc[i1][i2]);
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc_[i1][i2] = 0;
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc_[i1][i2] += gc[i3][i4] * x2[i1][i2][i3][i4];
                }
            }
            gc_[i1][i2] = sz_hs(gc_[i1][i2]);
        }
    }

    for (int i1 = 0; i1 < 10; i1++) {
        jg[i1] = 0;
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                jg[i1] += gc_[i2][i3] * x3[i1][i2][i3];
            }
        }
        jg[i1] = sz_hs_last(jg[i1]);
    }
}
int main(int argc, const char* argv[]) {
    freopen("x1.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> x1[i1][i2][i3][i4];
                }
            }
        }
    }
    freopen("x2.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> x2[i1][i2][i3][i4];
                }
            }
        }
    }
    freopen("x3.txt", "r", stdin);
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                cin >> x3[i1][i2][i3];
            }
        }
    }
    freopen("out.txt", "w", stdout);
    short x, y;
    int max_kn_number;
    double max_knx = 0;
    ifstream infile_IMAGE("train-images.idx3-ubyte", ios::in | ios::binary);
    ifstream infile_LABE("train-labels.idx1-ubyte", ios::in | ios::binary);
    for (int i = 0; i < 8; i++) {
        infile_IMAGE.read((char*)&x, sizeof(short));
    }
    for (int i = 0; i < 4; i++) {
        infile_LABE.read((char*)&y, sizeof(short));
    }

    for (int ix = 0; ix < runx_num; ix++) {
        for (int i1 = 0; i1 < 28; i1++) {
            for (int i2 = 0; i2 < 28; i2++) {
                infile_IMAGE.read((char*)&x, sizeof(short) / 2);
                in[i1][i2] = (((((x << 8) + (x >> 8)) & ((2 << 15) - 1))) >> 8) * 1.0 / 255;
            }
        }
        infile_LABE.read((char*)&y, sizeof(short) / 2);
        y = (((((y << 8) + (y >> 8)) & ((2 << 15) - 1))) >> 8);
        zx_network();
        max_knx = 0;
        for (int i_now_number = 0; i_now_number < 10; i_now_number++) {
            if (jg[i_now_number] > max_knx) {
                max_kn_number = i_now_number;
                max_knx = jg[i_now_number];
            }
        }
        cout << "识别为:" << max_kn_number << " 可能性:" << max_knx << " 实际为:" << y << endl;
    }
    infile_IMAGE.close();
    infile_LABE.close();
}
//下面是另一份差不多的程序，但是便于封装到其他程序中
/*
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<windows.h>
#include <fstream>
#define row 28
#define line 28
int runx_num = 900;//max is 60000

using namespace std;
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
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
double x1[row][line][row][line];
double x2[row][line][row][line];
double x3[10][row][line];
double jg[10];
double gc[row][line];
double gc_[row][line];
double sz_hs(double in_) {
    return max(in_, in_ * 0.1);
    //return tanh(in_);
}
double sz_hs_last(double in_) {
    return tanh(in_);
    //return max(in_, in_ * 0.1);
}
void zx_network() {
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc[i1][i2] = 0;
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc[i1][i2] += in[i3][i4] * x1[i1][i2][i3][i4];
                }
            }
            gc[i1][i2] = sz_hs(gc[i1][i2]);
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            gc_[i1][i2] = 0;
        }
    }
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    gc_[i1][i2] += gc[i3][i4] * x2[i1][i2][i3][i4];
                }
            }
            gc_[i1][i2] = sz_hs(gc_[i1][i2]);
        }
    }

    for (int i1 = 0; i1 < 10; i1++) {
        jg[i1] = 0;
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                jg[i1] += gc_[i2][i3] * x3[i1][i2][i3];
            }
        }
        jg[i1] = sz_hs_last(jg[i1]);
    }
}
int main(int argc, const char* argv[]) {
    freopen("x1.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> x1[i1][i2][i3][i4];
                }
            }
        }
    }
    freopen("x2.txt", "r", stdin);
    for (int i1 = 0; i1 < row; i1++) {
        for (int i2 = 0; i2 < line; i2++) {
            for (int i3 = 0; i3 < row; i3++) {
                for (int i4 = 0; i4 < line; i4++) {
                    cin >> x2[i1][i2][i3][i4];
                }
            }
        }
    }
    freopen("x3.txt", "r", stdin);
    for (int i1 = 0; i1 < 10; i1++) {
        for (int i2 = 0; i2 < row; i2++) {
            for (int i3 = 0; i3 < line; i3++) {
                cin >> x3[i1][i2][i3];
            }
        }
    }
    int max_kn_number;
    double max_knx = 0;
        zx_network();
        max_knx = 0;
        for (int i_now_number = 0; i_now_number < 10; i_now_number++) {
            if (jg[i_now_number] > max_knx) {
                max_kn_number = i_now_number;
                max_knx = jg[i_now_number];
            }
        }
        cout << "识别为:" << max_kn_number << " 可能性:" << max_knx*100  << '%' << endl;
}}




*/