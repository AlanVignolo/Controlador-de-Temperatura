from ast import Continue        # Libreria para utilizar el comando continue
from re import ASCII            # Libreria para la decodificacion de la comunicacon serial
import serial                   # Libreria para la comunicacion serial con el arduino
from datetime import datetime   # Libreria para el manejo de tiempo
import socket                   # Libreria para la comunicacion con PC 2

Host = "192.168.1.39"           # Direccion del propio equipo
Port = 65123                    # Puerto de escucha. Cualquiea mayor a 1023 se puede usar pero debe ser el mismo en ambas PCs

#Realizamos la conexion con la PC 2
Server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
Server.bind((Host, Port))
Server.listen()

print("\nEsperando conexion con PC 2...")

conn, addr = Server.accept() #Espera mientras se conecta

print("/nConexion exitosa")

N = 5                           # Numero minimo de ciclos
i = 0                           # Contador
X = 0.005                       # Limite de variacion de las temperaturas
datos = [0.0 ,0.0 ,0.0 ,0.0 ,0.0]  # Lista para guardar las temperaturas
sum = 0
tendencia = 0
# Estados de los leds
leds0 = "1,1,1"                 
leds1 = "0,1,0"
leds2 = "1,0,0"
leds3 = "0,0,1"
bus = serial.Serial('COM7',9600)# Entrada en la que esta conectado arduino

#Eliminar los datos de las temperaturas preexistentes en el archivo de texto
with open("Temperatura.txt",'w') as f:
    pass

while True: 
    # Lectura del puerto serial
    i=i+1
    canal = bus.readline().decode('ascii')
    # Descomponemos el string que llega a traves canal serial
    pos = canal.index(":")
    label = canal[:pos]
    value = float(canal[pos+1:])

    if label == 'Temperatura':

        if  i < N:
            datos[i] = value

        # Logica de Leds
        if  N < i:
            datos[0] = datos[1] # Desplazo los datos reutilizar memoria y descartar valores que no uso
            datos[1] = datos[2]
            datos[2] = datos[3]
            datos[3] = datos[4]
            datos[4] = value    # Guardo el valor leido
            sum = datos[1] + datos[2] + datos[3] + datos[4] + datos[0]
            variacion = value*N/sum

            if variacion <= 1+X and variacion >= 1-X:
                bus.write(leds1.encode('ascii'))
                tendencia = "1" # Tendenccia en estable

            elif variacion >= 1+X:
                bus.write(leds2.encode('ascii'))
                tendencia = "2" # Tendencia en alta

            elif variacion <= 1-X:
                bus.write(leds3.encode('ascii'))
                tendencia = "0" # Tendencia en baja
        else:
            bus.write(leds0.encode('ascii')) 
            tendencia = "-"     # No tiene tendencia

        print('\n-----------------------------------------\n')
        print("Temperatura medida: ", value)

        if sum != 0:
            print("Promedio: " , sum/N)

        else:
            print("Datos insuficientes para calcular un promedio")

        a=str(value)

    # Calculo del tiempo
    if label == 'Tiempo':
        if value < 2500:
            print("Se cerrara el programa")
            break

        elif value>=2500 and value <= 10000:
            print(f"Se actualizo el cicloo: {value/1000} segundos")

        elif value>10000:
            print(f"Se actualizo el cicloo: 10 segundos")
    
    fecha = str(datetime.now().strftime('%Y-%m-%d'))
    hora = str(datetime.now().strftime('%H:%M:%S'))

    #Datos guardados en un archivo de texto. Metodo: ABRIR-ESCRIBIR-CERRAR
    Temperaturatxt = open('Temperatura.txt','a')
    Temperaturatxt.write(str(a) + '|' + str(tendencia) + '|' + hora + '|' + fecha + '\n')
    Temperaturatxt.close()

    print(f"{fecha}  {hora}")

    try:
        conn.sendall(bytes(a, encoding = "utf-8"))
        
    except:
        Continue