import socket # Libreria para la comunicacion con PC 1
import matplotlib.pyplot as plt 
from matplotlib import pyplot
from matplotlib.animation import FuncAnimation # MATPLOTLIB: Libreria para realizar graficos. #MATPLOTLIB.ANIMATION: Realizar graficos en tiempo real
from datetime import datetime # Libreria que proporciona el tiempo

plt.style.use('ggplot') # Agrega estilo al grafico

Host="192.168.1.39" # IP del servidor
Port= 65123 # Puerto de envio

print("Conectando a la PC 1...")

# SOCKET, creando conexion con PC1
Cliente = socket.socket(socket.AF_INET,socket.SOCK_STREAM) 
Cliente.connect((Host,Port)) 

print("Conexion aceptada")

# Listas utilizadas en el grafico, eje x(Tiempo) - eje y (Temperatura)
x_data=[]
y_data=[]

# Funcion grafico en tiempo real
def Grafico(frame): # Funcion que recibe un cuadro como parametro 
    x_data.append(datetime.now()) # Tiempo actual
    y_data.append(float(Cliente.recv(1024).decode("utf-8"))) # Recepcion de temperatura
    line.set_data(x_data,y_data) # Inserta linea con los datos de las listas
    # Edicion grafico
    figure.gca().relim() 
    figure.gca().autoscale_view()
    plt.gcf().suptitle('Estación de monitoreo', fontsize=20)
    plt.xlabel('Tiempo', fontsize=16)
    plt.ylabel('Temperatura', fontsize=16)
    plt.gcf().autofmt_xdate()
    return line

# Creamos una figura
figure = pyplot.figure()
line, = pyplot.plot_date(x_data,y_data, '-',color = 'tab:red') # Linea continua en color rojo
grafico = FuncAnimation(figure , Grafico , interval = 2500) # Realiza el grafico en tiempo real con un intervalo de 2,5 segundos (Minimo 2,5 en programa principa)
plt.show() # Realizacion de grafico 