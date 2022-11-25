#include <vector>
using namespace std;
typedef double (*Activation_function_NETWORK_) (double in_);
typedef double (*Activation_function_Derivatives_NETWORK_) (double in_);
typedef double (*Function_based) (vector<double> target_, vector<double> Network_OUT);
class WLZH_network {
public:
    int IN_ROW = 28, IN_LINE = 28;
    int LAST_NETWORK_WEIGHT_NUM = 10;//���һ����������Ԫ����
    int NETWORK_WEIGHT_NUM = 2;//�����һ�������������
    double learn_rate;
    //��ʼ������Ϊ�����С��С������һ������������������һ����������Ԫ����
    WLZH_network(int IN_ROW_, int IN_LINE_, int NETWORK_WEIGHT_NUM_, int LAST_NETWORK_WEIGHT_NUM_) {
        IN_ROW = IN_ROW_;
        IN_LINE = IN_LINE_;
        NETWORK_WEIGHT_NUM = NETWORK_WEIGHT_NUM_;
        LAST_NETWORK_WEIGHT_NUM = LAST_NETWORK_WEIGHT_NUM_;
    }
    vector<vector<double>> IN_IMAGE_DATA = vector<vector<double>>(IN_ROW, vector<double>(IN_LINE));//��������
    vector<vector<vector<double>>> RUN_NETWORK_PROCESS = vector<vector<vector<double>>>(NETWORK_WEIGHT_NUM, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)));//���ڴ洢�������������
    vector<vector<vector<double>>> RUN_NETWORK_PROCESS_FOR_BP = vector<vector<vector<double>>>(NETWORK_WEIGHT_NUM, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)));//���ڴ洢�������������Ϊ�˷��򴫲��㷨
    vector<double> RUN_NETWORK_PROCESS_LAST_FOR_BP = vector<double>(LAST_NETWORK_WEIGHT_NUM);//���ڴ洢���һ�������������������Ϊ�˷��򴫲��㷨
    vector<double> RUN_NETWORK_PROCESS_LAST = vector<double>(LAST_NETWORK_WEIGHT_NUM);//���ڴ洢���һ�������������������
    vector<vector<vector<vector<vector<double>>>>> NETWORK_WEIGHT = vector<vector<vector<vector<vector<double>>>>>(NETWORK_WEIGHT_NUM, vector<vector<vector<vector<double>>>>(IN_ROW, vector<vector<vector<double>>>(IN_LINE, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)))));//�����һ���������Ȩ��
    vector<vector<vector<double>>> LAST_NETWORK_WEIGHT = vector<vector<vector<double>>>(LAST_NETWORK_WEIGHT_NUM, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)));//���һ���Ȩ��
    void IN_IMAGE(vector<vector<double>>* IN_IMAGE_) {//������������,ִ�з��򴫲�ʱ����ִ��
        IN_IMAGE_DATA = *IN_IMAGE_;
    }
    void IN_NETWORK_WEIGHT(vector<vector<vector<vector<vector<double>>>>>* NETWORK_WEIGHT_) {//��������һ���������Ȩ�أ�ѵ��ģʽ�½��������ʼ��
        NETWORK_WEIGHT = *NETWORK_WEIGHT_;
    }
    void IN_LAST_NETWORK_WEIGHT(vector<vector<vector<double>>>* LAST_NETWORK_WEIGHT_) {//�������һ���Ȩ�أ�ѵ��ģʽ�½��������ʼ��
        LAST_NETWORK_WEIGHT = *LAST_NETWORK_WEIGHT_;
    }
    Activation_function_NETWORK_ Activation_function_NETWORK;//�����һ����ļ����
    Activation_function_NETWORK_ Activation_function_last_NETWORK;//���һ�㼤���
    Function_based Function_Based_;//��ʧ����
    Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Else;//�����һ����ļ�����ĵ���
    Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Last;//���һ��ļ�����ĵ���
    void SET_Activation_function_last_NETWORK(Activation_function_NETWORK_ Activation_function_NETWORK_IN) {//��������һ����ļ����
        Activation_function_last_NETWORK = Activation_function_NETWORK_IN;
    }
    void SET_Activation_function_ELSE_NETWORK(Activation_function_NETWORK_ Activation_function_NETWORK_IN_) {//�������һ��ļ����
        Activation_function_NETWORK = Activation_function_NETWORK_IN_;
    }
    void SET_Function_Based(Function_based Function_based_) {//������ʧ����
        Function_Based_ = Function_based_;
    }
    void SET_Learn_Rate(double learn_rate_) {//����ѧϰ��
        learn_rate = learn_rate_;
    }
    void SET_Activation_function_Derivatives_NETWORK_Else(Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Else_) {//��������һ�������Ԫ������ĵ���
        Activation_function_Derivatives_NETWORK_Else = Activation_function_Derivatives_NETWORK_Else_;
    }
    void SET_Activation_function_Derivatives_NETWORK_Last(Activation_function_Derivatives_NETWORK_ Activation_function_Derivatives_NETWORK_Last_) {//��������һ�����Ԫ������ĵ���
        Activation_function_Derivatives_NETWORK_Last = Activation_function_Derivatives_NETWORK_Last_;
    }
    vector<double> RUN_network() {//����������
        for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
            for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                RUN_NETWORK_PROCESS_FOR_BP[0][i1_network][i2_network] = 0;
            }
        }
        for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
            for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                    for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {

                        RUN_NETWORK_PROCESS_FOR_BP[0][i1_network][i2_network] += IN_IMAGE_DATA[i3_network][i4_network] * NETWORK_WEIGHT[0][i1_network][i2_network][i3_network][i4_network];
                    }
                }
                RUN_NETWORK_PROCESS[0][i1_network][i2_network] = Activation_function_NETWORK(RUN_NETWORK_PROCESS_FOR_BP[0][i1_network][i2_network]);
            }
        }
        for (int i_NETWORK_WEIGHT_NUM = 1; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {

            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] = 0;
                }
            }
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                        for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {
                            RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] += RUN_NETWORK_PROCESS[i_NETWORK_WEIGHT_NUM - 1][i3_network][i4_network] * NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network];
                        }
                    }
                    RUN_NETWORK_PROCESS[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] = Activation_function_NETWORK(RUN_NETWORK_PROCESS_FOR_BP[i_NETWORK_WEIGHT_NUM][i1_network][i2_network]);
                }
            }
        }
        for (int i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM; i1_network++) {
            RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network] = 0;
            for (int i2_network = 0; i2_network < IN_ROW; i2_network++) {
                for (int i3_network = 0; i3_network < IN_LINE; i3_network++) {
                    RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network] += RUN_NETWORK_PROCESS[NETWORK_WEIGHT_NUM - 1][i2_network][i3_network] * LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network];
                }
            }
            RUN_NETWORK_PROCESS_LAST[i1_network] = Activation_function_last_NETWORK(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
        }
        return RUN_NETWORK_PROCESS_LAST;
    }
    double Gradient_Descent(vector<double> target_) {//�ݶ��½��㷨,����Ŀ��ֵ,�����ʧ��������������㷨������������ʹ��
        double ERR = Function_Based_(target_, RUN_network());
        double ERR_;
        for (int i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                        for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {
                            NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] += learn_rate;
                            ERR_ = Function_Based_(target_, RUN_network());
                            if (ERR_ < ERR) {
                                ERR = ERR_;
                            }
                            else {
                                NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] -= learn_rate;
                            }
                            NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] -= learn_rate;
                            ERR_ = Function_Based_(target_, RUN_network());
                            if (ERR_ < ERR) {
                                ERR = ERR_;
                            }
                            else {
                                NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] += learn_rate;
                            }
                        }
                    }
                }
            }
        }
        for (int i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM; i1_network++) {
            for (int i2_network = 0; i2_network < IN_ROW; i2_network++) {
                for (int i3_network = 0; i3_network < IN_LINE; i3_network++) {
                    LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] += learn_rate;
                    ERR_ = Function_Based_(target_, RUN_network());
                    if (ERR_ < ERR) {
                        ERR = ERR_;
                    }
                    else {
                        LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] -= learn_rate;
                    }
                    LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] -= learn_rate;
                    ERR_ = Function_Based_(target_, RUN_network());
                    if (ERR_ < ERR) {
                        ERR = ERR_;
                    }
                    else {
                        LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] += learn_rate;
                    }
                }
            }
        }
        return ERR;
    }
    double Back_Propagation(vector<vector<vector<double>>> IN_IMAGES_DATA, vector<vector<double>> target_) {//���򴫲��㷨,����ͼ�������ֵ����������飩
        int len_IN = IN_IMAGES_DATA.size();
        double ERR;
        vector<vector<vector<vector<vector<double>>>>> NETWORK_WEIGHT_WANT = vector<vector<vector<vector<vector<double>>>>>(NETWORK_WEIGHT_NUM, vector<vector<vector<vector<double>>>>(IN_ROW, vector<vector<vector<double>>>(IN_LINE, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)))));
        vector<vector<vector<double>>> LAST_NETWORK_WEIGHT_WANT = vector<vector<vector<double>>>(LAST_NETWORK_WEIGHT_NUM, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)));
        vector<vector<vector<double>>> NETWORK_LAST_WANT = vector<vector<vector<double>>>(NETWORK_WEIGHT_NUM, vector<vector<double>>(IN_ROW, vector<double>(IN_LINE)));
        for (int i_network = 0; i_network < LAST_NETWORK_WEIGHT_NUM; i_network++) {
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    LAST_NETWORK_WEIGHT_WANT[i_network][i1_network][i2_network] = 0;
                }
            }
        }
        for (int i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                        for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {
                            NETWORK_WEIGHT_WANT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] = 0;
                        }
                    }
                }
            }
        }
        for (int ix_network = 0; ix_network < len_IN; ix_network++) {//�������������
            IN_IMAGE_DATA = IN_IMAGES_DATA[ix_network];
            ERR = Function_Based_(target_[ix_network], RUN_network());
            for (int i1_network = 0; i1_network < NETWORK_WEIGHT_NUM; i1_network++) {
                for (int i2_network = 0; i2_network < IN_ROW; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_LINE; i3_network++) {
                        //cout << ix_network << ' ' << i1_network << ' ' << i2_network << ' ' << i3_network << endl;
                        NETWORK_LAST_WANT[i1_network][i2_network][i3_network] = 0;
                        
                    }
                }
            }
            for (int i1_network = 0; i1_network < LAST_NETWORK_WEIGHT_NUM; i1_network++) {
                for (int i2_network = 0; i2_network < IN_ROW; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_LINE; i3_network++) {
                        LAST_NETWORK_WEIGHT_WANT[i1_network][i2_network][i3_network] += LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] - learn_rate * (RUN_NETWORK_PROCESS_LAST[i1_network] - target_[ix_network][i1_network]) * Activation_function_Derivatives_NETWORK_Last(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
                        NETWORK_LAST_WANT[0][i2_network][i3_network] += (RUN_NETWORK_PROCESS_LAST[i1_network] - target_[ix_network][i1_network]) * LAST_NETWORK_WEIGHT[i1_network][i2_network][i3_network] * Activation_function_Derivatives_NETWORK_Last(RUN_NETWORK_PROCESS_LAST_FOR_BP[i1_network]);
                    }
                }
            }
            for (int i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
                for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                    for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                        for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                            for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {
                                if (NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 2 >= 0) {
                                    NETWORK_WEIGHT_WANT[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network][i3_network][i4_network] += NETWORK_WEIGHT[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network][i3_network][i4_network] - learn_rate * NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] * RUN_NETWORK_PROCESS[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 2][i1_network][i2_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FOR_BP[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network]);
                                    NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM + 1][i2_network][i3_network] += NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] * NETWORK_WEIGHT[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network][i3_network][i4_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FOR_BP[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network]);
                                }
                                else {
                                    NETWORK_WEIGHT_WANT[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network][i3_network][i4_network] += NETWORK_WEIGHT[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network][i3_network][i4_network] - learn_rate * NETWORK_LAST_WANT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network] * IN_IMAGE_DATA[i1_network][i2_network] * Activation_function_Derivatives_NETWORK_Else(RUN_NETWORK_PROCESS_FOR_BP[NETWORK_WEIGHT_NUM - i_NETWORK_WEIGHT_NUM - 1][i1_network][i2_network]);
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int i_NETWORK_WEIGHT_NUM = 0; i_NETWORK_WEIGHT_NUM < NETWORK_WEIGHT_NUM; i_NETWORK_WEIGHT_NUM++) {
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    for (int i3_network = 0; i3_network < IN_ROW; i3_network++) {
                        for (int i4_network = 0; i4_network < IN_LINE; i4_network++) {
                            NETWORK_WEIGHT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] = NETWORK_WEIGHT_WANT[i_NETWORK_WEIGHT_NUM][i1_network][i2_network][i3_network][i4_network] / len_IN;
                        }
                    }
                }
            }
        }
        for (int i_network = 0; i_network < LAST_NETWORK_WEIGHT_NUM; i_network++) {
            for (int i1_network = 0; i1_network < IN_ROW; i1_network++) {
                for (int i2_network = 0; i2_network < IN_LINE; i2_network++) {
                    LAST_NETWORK_WEIGHT[i_network][i1_network][i2_network] = LAST_NETWORK_WEIGHT_WANT[i_network][i1_network][i2_network] / len_IN;
                }
            }
        }
        return ERR;
    }
};