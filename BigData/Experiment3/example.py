# -*- coding:utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn.neighbors import KNeighborsClassifier
from sklearn.datasets import load_iris

""" 加载数据
    X   150 组点 为方便画图，仅采用数据的其中两个特征
    y   150 ndarray 1 dim，0 到 2 三种可能
"""
iris = load_iris()
X = iris.data[:, :2]
y = iris.target

""" iris 数据集描述
"""
# print(iris.DESCR)
# print(iris.feature_names)


""" 颜色设置
    cmap_light    图像的颜色（轻）大块的背景颜色
    cmap_bold     图像的颜色（重）点的颜色
"""
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])

""" 分类器设置
    n_neighbors   临近点的数量，默认为 5
    weights       在预测中使用的权重方程
                    参数 uniform 意味着所有点拥有相同的权重
"""
clf = KNeighborsClassifier(n_neighbors=15, weights='uniform', algorithm='brute')    # 初始化分类器对象

""" 训练
    为什么需要训练？
"""
clf.fit(X, y)

""" 画出决策边界，用不同的颜色表示
    先获得 X 集合中 0 列内最小的值减 1，获得 0 列内最大的值加 1 获得 x_min 和 x_max
    再获得 X 集合中 1 列内最小的值减 1，获得 1 列内最大的值加 1 获得 y_min 和 y_max
    
    利用 np.arrange 生成两个向量 [x_min, x_max) step=0.02；[y_min, y_max) step=0.02
    分别命名为 x_create 长度 280 和 y_create 长度 220
    
    np.meshgrid 将会为 x_create 和 y_create 生成两个矩阵 xx (220, 280) 和 yy (220 280)
    xx 相当于将 x_create 复制了 220 遍 （每行都相同的内容）
    yy 相当于将 y_create 的每个元素复制了 280 遍 （每行元素都相同）
    
    xx, yy 实际上是颜色采样点
"""
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1

x_create = np.arange(x_min, x_max, 0.02)
y_create = np.arange(y_min, y_max, 0.02)

xx, yy = np.meshgrid(x_create, y_create)

""" 预测
    xx.ravel 将 xx 向量化，长度为 220 * 280 = 61600
    yy.ravel 将 yy 向量化，长度为 220 * 280 = 61600
    
    np.c_ 在行方向上对 xx.ravel 和 yy.ravel 的结果进行合并得到 array_in_col (61600, 2)
    
    预测结果 Z 被设置为 xx 的尺寸 (220, 280)
"""
array_in_col = np.c_[xx.ravel(), yy.ravel()]
Z = clf.predict(array_in_col).reshape(xx.shape)

plt.figure()
plt.pcolormesh(xx, yy, Z, cmap=cmap_light)    # 绘制预测结果图
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold)    # 补充训练数据点
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.title("3-Class classification (k = 15, weights = 'uniform')")
plt.show()
