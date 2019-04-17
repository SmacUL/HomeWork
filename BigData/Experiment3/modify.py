#!/usr/bin/env python 
# -*- coding: utf-8 -*- 
"""
Created on 4/6/2019 13:20

@author: SmacUL 
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from sklearn.datasets import load_iris


iris = load_iris()
X = iris.data[:, :2]
y = iris.target

n_neighbors = 15

x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
x_create = np.arange(x_min, x_max, 0.02)
y_create = np.arange(y_min, y_max, 0.02)
xx, yy = np.meshgrid(x_create, y_create)
array_in_col = np.c_[xx.ravel(), yy.ravel()]

target_labels = []
for target in array_in_col:
    dists = []
    for x in X:
        dist = np.linalg.norm(target - x)
        dists.append(dist)

    labels = []
    for i in range(n_neighbors):
        index = dists.index(min(dists))
        labels.append(y[index])
        dists.remove(dists[index])

    counters = [0, 0, 0]
    for lab in labels:
        counters[lab] += 1
    target_labels.append(counters.index(max(counters)))

Z = np.reshape(np.array(target_labels), xx.shape)

cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])

plt.figure()
plt.pcolormesh(xx, yy, Z, cmap=cmap_light)
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=cmap_bold)
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.title("3-Class classification (k = 15, weights = 'uniform')")
plt.show()
