import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import PercentFormatter

data = pd.read_csv('wyniki.txt') # 1 - wygrał gracz pierwszy, 0 - gracz drugi
data.drop(['Winner'],axis=1,inplace=True)

# drop where moves are infinity (-1)
data = data[data['Moves'] != -1]

data_mean = data.groupby(['Deck size'],as_index=False).mean()
data_mean.columns = ['Deck size','Moves mean']
# data_mean.plot.line()

plt.plot(data_mean['Deck size'],data_mean['Moves mean'],label="Wariant A", marker='o',ls='--')
plt.xlabel('Ilość kart w talii')
plt.ylabel('Średnia ilość ruchów')


print(data_mean)
# plt.legend()
plt.show()
