import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('measure.csv', index_col=0)
mean = df.mean(axis=1)
std = df.std(axis=1)

plt.plot(mean, label='mean', color='green')
plt.plot(std, label='std', color='red')

plt.legend(title='legend')
plt.ylabel('Temps (en secondes)')
plt.xlabel('Nombre de threads')
plt.title('Test de performance de "read-write.c"')

plt.show()