import threading 
import time 
import requests
from collections import Counter
import matplotlib.pyplot as plt
#from bs4 import BeautifulSoup
def GraficoLetras(CaractereContado):
    caractere = list(CaractereContado.keys())
    contador = list(CaractereContado.values())
    plt.bar(caractere, contador)
    plt.xlabel('Caracteres')
    plt.ylabel('Quantidade')
    plt.title('Contagem de Caracteres')
    plt.show()
def ApresentaInfoCaracter(cont):
    for caractere, contador in cont.items():
        print(f"Caractere '{caractere}': {contador} vezes'")
def LeituraPeriodica(url, periodo):
    while True:
        text = ConteudoUrl(url)
        if text:
            caracterescontados = Counter(text)
            ApresentaInfoCaracter(caracterescontados)
            GraficoLetras(caracterescontados)
        time.sleep(periodo)
def ConteudoUrl(url):
    leitura = requests.get(url)
    if leitura.status_code == 200:
        return leitura.text
    else:
        print("Falha ao obter o conteúdo da URL")
        return None 

if __name__ == "__main__":
    url = "https://g1.globo.com/"
    periodo = 10
    LeituraPeriodica(url,periodo)