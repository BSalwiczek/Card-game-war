import pandas as pd
import numpy as np

fvsf = pd.read_csv('fvsf.txt')
fvsp = pd.read_csv('fvsp.txt')
lvsf = pd.read_csv('lvsf.txt')
lvsl = pd.read_csv('lvsl.txt')
lvsp = pd.read_csv('lvsp.txt')
pvsp = pd.read_csv('pvsp.txt')

pvsp = pvsp[pvsp['Moves'] != -1]
print("Odsetek skończonych rozgrywek w pvsp: ")
print(len(pvsp)/1000)

print('fvsf')
print(fvsf.groupby(['Player1Strategy']).sum()['Winner']/1000)
print(1 - fvsf.groupby(['Player2Strategy']).sum()['Winner']/1000)

print('\nfvsp')
print(fvsp.groupby(['Player1Strategy']).sum()['Winner']/1000)
print(1 - fvsp.groupby(['Player2Strategy']).sum()['Winner']/1000)

print('\nlvsf')
print(lvsf.groupby(['Player1Strategy']).sum()['Winner']/1000)
print(1 - lvsf.groupby(['Player2Strategy']).sum()['Winner']/1000)

print('\nlvsl')
print(lvsl.groupby(['Player1Strategy']).sum()['Winner']/1000)
print(1 - lvsl.groupby(['Player2Strategy']).sum()['Winner']/1000)

print('\nlvsp')
print(lvsp.groupby(['Player1Strategy']).sum()['Winner']/1000)
print(1 - lvsp.groupby(['Player2Strategy']).sum()['Winner']/1000)


print('\npvsp')
print(pvsp.groupby(['Player1Strategy']).sum()['Winner']/len(pvsp))
print(1 - pvsp.groupby(['Player2Strategy']).sum()['Winner']/len(pvsp))