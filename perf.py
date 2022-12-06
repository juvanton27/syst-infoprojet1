import pandas as pd
import matplotlib.pyplot as plt
import os

filename = input("Enter filename ('q' to quit) : ")
while filename != 'q':
  # Doing simple graph
  df = pd.read_csv(f'data/{filename}.csv', index_col=0)
  mean = df.mean(axis=1)
  std = df.std(axis=1)

  plt.plot(mean, label='mean', color='green')
  plt.plot(std, label='std', color='red')

  plt.legend(title='legend')
  plt.ylabel('Temps (en secondes)')
  plt.xlabel('Nombre de threads')
  plt.title(f'Test de performance de "{filename}"')

  plt.savefig(f'images/{filename}.png')
  plt.clf()

  # Doing comparative graph
  df = []
  mean = []
  index = 0
  colors = ['red', 'blue', 'green', 'yellow']
  for file in os.listdir('data/'):
    if file.startswith(filename):
      df.append({'data': pd.read_csv(f'data/{file}', index_col=0), 'filename': file, 'color': colors[index]})
      index+=1

  for data in df:
    mean.append({'data': data['data'].mean(axis=1), 'filename': data['filename'], 'color': data['color']})

  for m in mean:
    plt.plot(m['data'], label=f"mean of {m['filename']}", color=m['color'])

    plt.legend(title='legend')
    plt.ylabel('Temps (en secondes)')
    plt.xlabel('Nombre de threads')
    plt.title(f'Comparatif des tests de performance de "{filename}"')

  plt.savefig(f'images/full-{filename}.png')
  plt.clf()

  filename = input("Enter filename ('q' to quit) : ")

