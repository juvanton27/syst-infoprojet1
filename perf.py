import pandas as pd
import matplotlib.pyplot as plt

filename = input("Enter filename ('q' to quit) : ")
while filename != 'q':

  df = pd.read_csv(f'{filename}.csv', index_col=0)
  mean = df.mean(axis=1)
  std = df.std(axis=1)

  plt.plot(mean, label='mean', color='green')
  plt.plot(std, label='std', color='red')

  plt.legend(title='legend')
  plt.ylabel('Temps (en secondes)')
  plt.xlabel('Nombre de threads')
  plt.title(f'Test de performance de "{filename}.c"')

  plt.show()

  filename = input("Enter filename ('q' to quit) : ")