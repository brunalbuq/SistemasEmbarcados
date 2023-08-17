import threading
import time

class TarefaTempoReal(threading.Thread):
    def __init__(self, nome, periodo, deadline, tExe):
        super().__init__()
        self.nome = nome
        self.periodo = periodo
        self.deadline = deadline
        self.tExe = tExe
        self.TempoZero = time.time()
    
    def run(self):
        while True:
            TempoPassado = time.time() - self.TempoZero
            
            if TempoPassado >= self.periodo:
                self.TempoZero = time.time()
                self.VisualizarInfos()
                break
            
        #self.VisualizarInfos()
        
    def VisualizarInfos (self):
        print(f"O tempo de início da execução da {self.nome} é {self.TempoZero}")
        time.sleep(self.tExe)
        print(f"O tempo de finalização da execução da {self.nome} é {time.time()} \n")

TarefaUm = TarefaTempoReal(nome = "Tarefa 1", periodo = 5, deadline = 5, tExe = 2)
TarefaDois = TarefaTempoReal(nome = "Tarefa 2", periodo = 10, deadline = 10, tExe = 4)
TarefaTres= TarefaTempoReal(nome = "Tarefa 3", periodo = 15, deadline = 10, tExe = 6)

"""
TarefaUm.VisualizarInfos()
TarefaDois.VisualizarInfos()
TarefaTres.VisualizarInfos()
"""

TarefaUm.start()
TarefaDois.start()
TarefaTres.start()

TarefaUm.join()