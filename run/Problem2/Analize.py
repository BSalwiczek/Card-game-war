import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import PercentFormatter

data = pd.read_csv('AB3.txt') # 1 - wygrał gracz pierwszy, 0 - gracz drugi
data.drop(['Deck size','Moves','Variant','Rank2'],axis=1,inplace=True)
print(data.corr())
plt.plot(data.Rank1, data.Winner, 'ro')
plt.xlabel("Ranga")
plt.ylabel("Wygrana")
plt.show()


data = data.groupby('Rank1',as_index=False).mean()
print(data.corr())
plt.plot(data.Rank1, data.Winner, 'ro')
plt.xlabel("Ranga")
plt.ylabel("Prawdopodobieństwo wygranej")
plt.show()