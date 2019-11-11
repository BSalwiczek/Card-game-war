import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import PercentFormatter

a1 = pd.read_csv("A1.txt")
a2 = pd.read_csv("A2.txt")
a3 = pd.read_csv("A3.txt")
a4 = pd.read_csv("A4.txt")
a5 = pd.read_csv("A5.txt")
a6 = pd.read_csv("A6.txt")
a7 = pd.read_csv("A7.txt")
a8 = pd.read_csv("A8.txt")
a9 = pd.read_csv("A9.txt")

b1 = pd.read_csv("B1.txt")
b2 = pd.read_csv("B2.txt")
b3 = pd.read_csv("B3.txt")
b4 = pd.read_csv("B4.txt")
b5 = pd.read_csv("B5.txt")
b6 = pd.read_csv("B6.txt")
b7 = pd.read_csv("B7.txt")
b8 = pd.read_csv("B8.txt")
b9 = pd.read_csv("B9.txt")

print(a9['Moves'].max())
print(b9['Moves'].max())

data = pd.concat([a1, a2, a3, a4, a5, a6, a7, a8, a9,b1, b2, b3, b4, b5, b6, b7, b8, b9])
data_inf = data[data['Moves'] == -1].groupby(['Deck size','Variant'],as_index=False).count()
data_inf = data_inf.drop(['Winner'], axis=1)
# data_inf.columns = ['Inf count']
print(data_inf)

# plt.plot(data_inf[data_inf['Variant']==0]['Deck size'],data_inf[data_inf['Variant']==0]['Moves']/1000,label="Wariant A", marker='o',ls='--')
# plt.plot(data_inf[data_inf['Variant']==1]['Deck size'],data_inf[data_inf['Variant']==1]['Moves']/1000,label="Wariant B", marker='o', ls='--')
# plt.xlabel('Ilość kart w talii')
# plt.ylabel('Szansa na nieskończoną rozgrywkę')
# plt.gca().yaxis.set_major_formatter(PercentFormatter(1))
# plt.legend()
# plt.show()

# drop where moves are infinity (-1)
data = data[data['Moves'] != -1]

data_mean = data.groupby(['Deck size','Variant'],as_index=False).mean()
data_mean = data_mean.drop(['Winner'], axis=1)
data_mean.columns = ['Deck size','Variant','Moves mean']
# data_mean.plot.line()

plt.plot(data_mean[data_mean['Variant']==0]['Deck size'],data_mean[data_mean['Variant']==0]['Moves mean'],label="Wariant A", marker='o',ls='--')
plt.plot(data_mean[data_mean['Variant']==1]['Deck size'],data_mean[data_mean['Variant']==1]['Moves mean'],label="Wariant B", marker='o', ls='--')
plt.xlabel('Ilość kart w talii')
plt.ylabel('Średnia ilość ruchów')


print(data_mean)
plt.legend()
plt.show()
