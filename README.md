# WLZH-NOTEWORK
一个用纯C++写的神经网络
写这个项目的目的有两个

一是想巩固我的神经网络类的知识

二是opencv的可移植性太差，而我想在单片机上跑人工智能

所以这个项目是纯C++写的，完全不依赖系统，可移植性很强

且它不止可以用于图像识别，也可以用于别的人工智能项目

训练部分目前已实现梯度下降和反向传播算法

如果你想把它改为C程序也不难，只需改class即可

下面是这个库的使用方法：

首先，添加库文件

      #include "WNOTEWORK.hpp"

其次，创建一个神经网络对象

  WLZH_network MY_network = WLZH_network(ROW, LINE, NETWORK_WEIGHT_NUM, LAST_NETWORK_WEIGHT_NUM);

ROW和LINE为输入数据的长宽，这是为了方便图像识别

NETWORK_WEIGHT_NUM为隐藏层层数

LAST_NETWORK_WEIGHT_NUM为输出层神经元数量

接下来，初始化神经网络

  MY_network.IN_NETWORK_WEIGHT(&NETWORK_WEIGHT_S);

NETWORK_WEIGHT_S为隐藏层权重，格式为vector<vector<vector<vector<vector<double>>>>>
  
具体写法参考例程，注意，就算是训练神经网络也要用随机数初始化
  
  MY_network.IN_LAST_NETWORK_WEIGHT(&x3);

x3为输出层权重，格式为vector<vector<vector<double>>>
  
具体写法参考例程，注意，就算是训练神经网络也要用随机数初始化

  MY_network.SET_Activation_function_ELSE_NETWORK(sz_hs);
  
sz_hs为隐藏层激活函数，格式为double sz_hs(double in)
  
  MY_network.SET_Activation_function_last_NETWORK(sz_hs_last);
  
sz_hs_last为输出层激活函数，格式为double sz_hs_last(double in)
  
然后，你就可以运行或训练
  
首先看运行
  
  MY_network.IN_IMAGE(&IN_IMAGE_DATA_I);
  
IN_IMAGE_DATA_I为输入数据，格式为vector<vector<double>>，用二维数组是为了方便图像识别
  
  MY_network.RUN_network();

运行神经网络，输出最后输出层的输出值，格式为vector<double>
  
训练
  
梯度下降
  
  MY_network.SET_Function_Based(Function_B);
  
Function_B为损失函数，格式：double Function_B(double in)
  
  MY_network.SET_Learn_Rate(0.01);
  
设置学习率
  
  MY_network.Gradient_Descent(in_s, mb);
  
in_s为输入数据集，格式：vector<vector<vector<double>>>，具体写法参考例程
  
mb为期望值，格式：vector<vector<double>>，具体写法参考例程

输出损失函数的输出
  
此函数很慢且只运行一次没什么效果，最好多跑几次
  
反向传播
  
  MY_network.SET_Activation_function_Derivatives_NETWORK_Else(sz_hs_ds);
  
设置隐藏层激活函数的导数，sz_hs_ds的格式和激活函数一样
  
  MY_network.SET_Activation_function_Derivatives_NETWORK_Last(sz_hs_last_ds);
 
设置输出层激活函数的导数，sz_hs_last_ds的格式和激活函数一样
  
  MY_network.SET_Function_Based(Function_B);
  
Function_B为损失函数，格式：double Function_B(double in)
  
此函数可不调用，只不过需要把WNOTEWORK.hpp中的第216行改为RUN_network();且在下面加一行ERR=0;
  
  MY_network.SET_Learn_Rate(0.01);
  
设置学习率
  
  MY_network.Back_Propagation(in_s, mb);
  
in_s为输入数据集，格式：vector<vector<vector<double>>>，具体写法参考例程
  
mb为期望值，格式：vector<vector<double>>，具体写法参考例程
  
输出损失函数的输出，若把WNOTEWORK.hpp中的第216行改为了RUN_network()
  
此函数只运行一次没什么效果，最好多跑几次
