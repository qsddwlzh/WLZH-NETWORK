#include <vector>
using namespace std;
typedef double (*Activation_function_NETWORK_) (double in_);
typedef double (*Activation_function_Derivatives_NETWORK_) (double in_);

//target_为期望值数组，Network_OUT为输出值数组，len为数组长度
typedef double (*Function_based) (double *target_, double *Network_OUT, size_t len_);

//第一个输入为当前神经元的期望值，第二个为当前末层神经元的输出值
typedef double (*Function_based_Derivatives) (double target_I, double Network_OUT_I);
class WLZH_network {
public:
    size_t IN_NUM;//输入数据个数也是除最后一层外每层神经元的数量
    size_t IN_NUM_FIRST;//输入数据个数
    size_t LAST_NETWORK_WEIGHT_NUM = 10;//最后一层神经网络神经元数量
    size_t NETWORK_WEIGHT_NUM;//除最后一层外神经网络层数
    double learn_rate;
    
    double *IN_IMAGE_DATA;//输入数据

    double *RUN_NETWORK_PROCESS;//用于存储神经网络运算过程
    double *RUN_NETWORK_PROCESS_FOR_BP;//用于存储神经网络运算过程为了反向传播算法

    double *RUN_NETWORK_PROCESS_LAST_FOR_BP;//用于存储最后一层神经网络运算过程数据为了反向传播算法
    double *RUN_NETWORK_PROCESS_LAST;//用于存储最后一层神经网络运算过程数据

    double* RUN_NETWORK_PROCESS_FIRST_FOR_BP;//用于存储第一层神经网络运算过程数据为了反向传播算法

    double *FIRST_NETWORK_WEIGHT;//第一层的权重
    double *NETWORK_WEIGHT;//除最后一层外的所有权重
    double *LAST_NETWORK_WEIGHT;//最后一层的权重
    //初始化依次为输入行、列、除最后一层外神经网络层数、最后一层神经网络神经元数量
    WLZH_network(size_t IN_NUM_FIRST_,size_t IN_NUM_, size_t NETWORK_WEIGHT_NUM_, size_t LAST_NETWORK_WEIGHT_NUM_) {
        IN_NUM = IN_NUM_;
        IN_NUM_FIRST = IN_NUM_FIRST_;
        LAST_NETWORK_WEIGHT_NUM = LAST_NETWORK_WEIGHT_NUM_;

        NETWORK_WEIGHT_NUM = NETWORK_WEIGHT_NUM_;

        IN_IMAGE_DATA = new double[IN_NUM];

        RUN_NETWORK_PROCESS = new double[(NETWORK_WEIGHT_NUM+1) * IN_NUM];
        RUN_NETWORK_PROCESS_FOR_BP = new double[NETWORK_WEIGHT_NUM* IN_NUM];

        FIRST_NETWORK_WEIGHT = new double[IN_NUM * IN_NUM_FIRST];
        NETWORK_WEIGHT = new double[NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM];
        LAST_NETWORK_WEIGHT = new double[LAST_NETWORK_WEIGHT_NUM * IN_NUM];


        RUN_NETWORK_PROCESS_LAST_FOR_BP = new double[LAST_NETWORK_WEIGHT_NUM];
        RUN_NETWORK_PROCESS_LAST = new double[LAST_NETWORK_WEIGHT_NUM];

        RUN_NETWORK_PROCESS_FIRST_FOR_BP = new double[IN_NUM];
    }
    //_gain_为初始化所需的一个常数，它越小神经网络初始权重越大
    //seed随机数种子
    void INIT_RAND_WEIGHT(size_t _GAIN_,size_t seed) {//一个函数随机初始化神经网络,而且如果要输出神经网络的所有权重照着这个写就好
        srand(seed);
        for (size_t _i1 = 0; _i1 < IN_NUM; _i1++) {
            for (size_t _i2 = 0; _i2 < IN_NUM_FIRST; _i2++) {
                FIRST_NETWORK_WEIGHT[_i1 * IN_NUM_FIRST + _i2] = rand() * 1.0 / (1.0 * RAND_MAX * _GAIN_ / 32767) * pow(-1, (rand() % 2));
            }
        }
        for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                    NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] = rand() * 1.0 / (1.0 * RAND_MAX * _GAIN_ / 32767) * pow(-1, (rand() % 2));
                }
            }
        }
        for (size_t i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM * IN_NUM; i1_network++) {
            LAST_NETWORK_WEIGHT[i1_network] = rand() * 1.0 / (1.0 * RAND_MAX * _GAIN_ / 32767) * pow(-1, (rand() % 2));
        }
    }
    void IN_IMAGE(double *IN_IMAGE_) {//传入输入数据,执行反向传播时不用执行
        for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
            IN_IMAGE_DATA[i1_network] = IN_IMAGE_[i1_network];
        }
    }
    void IN_NETWORK_WEIGHT_FIRST(double* NETWORK_FIRST_IN_) {
        for (size_t _i1 = 0; _i1 < IN_NUM; _i1++) {
            for (size_t _i2 = 0; _i2 < IN_NUM_FIRST; _i2++) {
                FIRST_NETWORK_WEIGHT[_i1 * IN_NUM_FIRST + _i2] = NETWORK_FIRST_IN_[_i1 * IN_NUM_FIRST + _i2];
            }
        }
    }
    void IN_NETWORK_WEIGHT(double *NETWORK_WEIGHT_) {//传入除最后一层外的所有权重，训练模式下建议随机初始化
        for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                    for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                            NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] = NETWORK_WEIGHT_[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network];
                    }
            }
        }
    }
    void IN_LAST_NETWORK_WEIGHT(double* LAST_NETWORK_WEIGHT_) {//传入最后一层的权重，训练模式下建议随机初始化
        for (size_t i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM * IN_NUM; i1_network++) {
            LAST_NETWORK_WEIGHT[i1_network] = LAST_NETWORK_WEIGHT_[i1_network];
        }
    }
    Activation_function_NETWORK_ Activation_function_NETWORK;//除最后一层外的激活函数
    Activation_function_NETWORK_ Activation_function_last_NETWORK;//最后一层激活函数
    Function_based Function_Based_;//损失函数
    Function_based_Derivatives Function_based_Derivatives_;//损失函数任意一项的导数
    Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Else;//除最后一层外的激活函数的导数
    Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Last;//最后一层的激活函数的导数
    void SET_Activation_function_last_NETWORK(Activation_function_NETWORK_ Activation_function_NETWORK_IN) {//传入除最后一层外的激活函数
        Activation_function_last_NETWORK = Activation_function_NETWORK_IN;
    }
    void SET_Activation_function_ELSE_NETWORK(Activation_function_NETWORK_ Activation_function_NETWORK_IN_) {//传入最后一层的激活函数
        Activation_function_NETWORK = Activation_function_NETWORK_IN_;
    }
    void SET_Function_Based(Function_based Function_based_) {//传入损失函数
        Function_Based_ = Function_based_;
    }
    //损失函数任意一项的导数，输入为当前神经元输出值与期望值
    /*
        如神经网络输出值为a1、a2、a3...
        期望值为b1、b2、b3...
        且损失函数为(1/2)[(a1-b1)*(a2-b2)+(a2-b2)*(a3-b3)+(a3-b3)*(a3-b3)+...]
        则此函数为(1/2)(ax-bx)*(ax-bx)的导数为ax-bx
    */
    void SET_Function_based_Derivatives(Function_based_Derivatives Function_based_Derivatives__) {//传入损失函数任意一项的导数
        Function_based_Derivatives_ = Function_based_Derivatives__;
    }
    void SET_Learn_Rate(double learn_rate_) {//设置学习率
        learn_rate = learn_rate_;
    }
    void SET_Activation_function_Derivatives_NETWORK_Else(Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Else_) {//传入除最后一层外的神经元激活函数的导数
        Activation_function_Derivatives_NETWORK_Else = Activation_function_Derivatives_NETWORK_Else_;
    }
    void SET_Activation_function_Derivatives_NETWORK_Last(Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Last_) {//传入除最后一层的神经元激活函数的导数
        Activation_function_Derivatives_NETWORK_Last = Activation_function_Derivatives_NETWORK_Last_;
    }
    double *RUN_network() {//运行神经网络
        for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                RUN_NETWORK_PROCESS_FIRST_FOR_BP[i1_network] = 0;
                for (size_t i3_network = 0; i3_network < IN_NUM_FIRST; i3_network++) {
                        RUN_NETWORK_PROCESS_FIRST_FOR_BP[i1_network] += IN_IMAGE_DATA[i3_network] * FIRST_NETWORK_WEIGHT[i1_network * IN_NUM_FIRST + i3_network];
                }
                RUN_NETWORK_PROCESS[i1_network] = Activation_function_NETWORK(RUN_NETWORK_PROCESS_FIRST_FOR_BP[i1_network]);
                
        }
        for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                    RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM * IN_NUM + i1_network] = 0;
                    for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                            RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM* IN_NUM + i1_network] += RUN_NETWORK_PROCESS[i_NETWORK_WEIGHT_NUM * IN_NUM + i3_network] * NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network];
                    }
                    RUN_NETWORK_PROCESS[(i_NETWORK_WEIGHT_NUM+1)* IN_NUM + i1_network] = Activation_function_NETWORK(RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM* IN_NUM + i1_network]);
                    
            }
        }
        for (size_t i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM; i1_network++) {
            
            RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network] = 0;
            for (size_t i2_network = 0; i2_network < IN_NUM; i2_network++) {
                    RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network] += RUN_NETWORK_PROCESS[NETWORK_WEIGHT_NUM * IN_NUM + i2_network] * LAST_NETWORK_WEIGHT[i1_network * IN_NUM + i2_network];
            }
            RUN_NETWORK_PROCESS_LAST[i1_network] = Activation_function_last_NETWORK(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
            
            
        }
        return RUN_NETWORK_PROCESS_LAST;
    }
    double Back_Propagation(vector<double*> IN_IMAGES_DATA, double **target_) {//反向传播算法,输入图像和期望值（可输入多组）输出损失函数的输出
        size_t len_IN = IN_IMAGES_DATA.size();
        //int xxxxx;
        double ERR;
        double ERR_MAX = 0;
        double *NETWORK_WEIGHT_WANT = new double[NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM];
        double *LAST_NETWORK_WEIGHT_WANT = new double[LAST_NETWORK_WEIGHT_NUM * IN_NUM];
        double *FIRST_NETWORK_WEIGHT_WANT = new double[IN_NUM_FIRST * IN_NUM];
        double *NETWORK_LAST_WANT = new double[(NETWORK_WEIGHT_NUM+2) * IN_NUM];
        
        for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
            for (size_t i2_network = 0; i2_network < IN_NUM_FIRST; i2_network++) {
                FIRST_NETWORK_WEIGHT_WANT[i1_network * IN_NUM_FIRST + i2_network] = 0;
            }
        }

        for (size_t i_network = 0; i_network < LAST_NETWORK_WEIGHT_NUM; i_network++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                    LAST_NETWORK_WEIGHT_WANT[i_network * IN_NUM + i1_network] = 0;
            }
        }
        
        for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                    for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                            NETWORK_WEIGHT_WANT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] = 0;
                    }
            }
        }
        
        for (size_t ix_network = 0; ix_network < len_IN; ix_network++) {//遍历输入的数据
            //把其中一张图片复制进IN_IMAGE_DATA
            IN_IMAGE_DATA = IN_IMAGES_DATA[ix_network];
            ERR = Function_Based_(target_[ix_network], RUN_network(), LAST_NETWORK_WEIGHT_NUM);
            //if (ERR > ERR_MAX) { ERR_MAX = ERR; xxxxx = ix_network; }
            ERR_MAX += ERR;
            for (size_t i1_network = 0; i1_network < NETWORK_WEIGHT_NUM; i1_network++) {
                for (size_t i2_network = 0; i2_network < IN_NUM; i2_network++) {
                        NETWORK_LAST_WANT[i1_network * IN_NUM + i2_network] = 0;
                }
            }
            for (size_t i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM; i1_network++) {
                for (size_t i2_network = 0; i2_network < IN_NUM; i2_network++) {
                        LAST_NETWORK_WEIGHT_WANT[i1_network * IN_NUM + i2_network] += LAST_NETWORK_WEIGHT[i1_network * IN_NUM + i2_network] + learn_rate * Function_based_Derivatives_(target_[ix_network][i1_network],RUN_NETWORK_PROCESS_LAST[i1_network]) * RUN_NETWORK_PROCESS[NETWORK_WEIGHT_NUM * IN_NUM + i2_network] * Activation_function_Derivatives_NETWORK_Last(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
                        NETWORK_LAST_WANT[0 * IN_NUM + i2_network] += (RUN_NETWORK_PROCESS_LAST[i1_network] - target_[ix_network][i1_network]) * LAST_NETWORK_WEIGHT[i1_network* IN_NUM + i2_network] * Activation_function_Derivatives_NETWORK_Last(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
                }
            }
            for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
                for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                        for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                            NETWORK_WEIGHT_WANT[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] += NETWORK_WEIGHT[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] - learn_rate * NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM * IN_NUM + i1_network] * RUN_NETWORK_PROCESS[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM + i3_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FOR_BP[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM + i1_network]);
                            NETWORK_LAST_WANT[(i_NETWORK_WEIGHT_NUM + 1) * IN_NUM + i3_network] += NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM * IN_NUM + i1_network] * NETWORK_WEIGHT[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FOR_BP[(NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1) * IN_NUM + i1_network]);
                        }
                }
            }
            
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                for (size_t i3_network = 0; i3_network < IN_NUM_FIRST; i3_network++) {
                    FIRST_NETWORK_WEIGHT_WANT[i1_network * IN_NUM_FIRST + i3_network] += FIRST_NETWORK_WEIGHT[i1_network * IN_NUM_FIRST + i3_network] - learn_rate * NETWORK_LAST_WANT[(NETWORK_WEIGHT_NUM) * IN_NUM + i1_network] * IN_IMAGE_DATA[i3_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FIRST_FOR_BP[i1_network]);
                }
            }
            
        }
        for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
            for (size_t i2_network = 0; i2_network < IN_NUM_FIRST; i2_network++) {
                FIRST_NETWORK_WEIGHT[i1_network * IN_NUM_FIRST + i2_network] = FIRST_NETWORK_WEIGHT_WANT[i1_network * IN_NUM_FIRST + i2_network] / len_IN;
            }
        }
        for (size_t i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                    for (size_t i3_network = 0; i3_network < IN_NUM; i3_network++) {
                            NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] = NETWORK_WEIGHT_WANT[i_NETWORK_WEIGHT_NUM * IN_NUM * IN_NUM + i1_network * IN_NUM + i3_network] / len_IN;
                    }
            }
        }
        for (size_t i_network = 0; i_network < LAST_NETWORK_WEIGHT_NUM; i_network++) {
            for (size_t i1_network = 0; i1_network < IN_NUM; i1_network++) {
                LAST_NETWORK_WEIGHT[i_network * IN_NUM + i1_network] = LAST_NETWORK_WEIGHT_WANT[i_network * IN_NUM + i1_network] / len_IN;
            }
        }
        delete []NETWORK_WEIGHT_WANT;
        delete []LAST_NETWORK_WEIGHT_WANT;
        delete []NETWORK_LAST_WANT;
        delete []FIRST_NETWORK_WEIGHT_WANT;
        return ERR;
    }
}; 
