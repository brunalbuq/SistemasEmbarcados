import threading
import time
import sched

def TarefaPeriodica(nome, Periodo):
    print(f"A {nome} foi executada ")
    scheduler.enter(Periodo, 1, TarefaPeriodica, (nome, Periodo))
    
if __name__ == "__main__":   
    scheduler = sched.scheduler(time.time, time.sleep)
    scheduler.enter(0, 1, TarefaPeriodica, ("Tarefa 1", 5))
    scheduler.enter(0, 2, TarefaPeriodica, ("Tarefa 2", 5))
    
    thread = threading.Thread(target = scheduler.run)
    thread.start()
    thread.join()