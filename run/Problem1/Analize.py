import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


a1 = pd.read_csv("A1.txt")
print(a1['Moves'].mean())