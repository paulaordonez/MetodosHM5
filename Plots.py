from  numpy import genfromtxt
from  numpy import arange
from  numpy import exp
import numpy as np
from matplotlib.pyplot import *

#importacion de datos 
parametros = genfromtxt('parametros.dat', delimiter='\t')
datos = genfromtxt('RadialVelocities.dat')

#separacion de columnas de datos a usar
radio = datos[:,0]
velocidad = datos[:,1]

Mb = abs(parametros[1])
Md = abs(parametros[2])
Mh = abs(parametros[3])

#constantes dadas en el enunciado
bb = 0.2497
bd = 5.16
ad = 0.3105
ah = 64.3

#calculo 
v_ecuacion = Mb**0.5*radio/((radio**2+bb**2)**(3.0/4.0))
v_ecuacion = v_ecuacion + Md**0.5*radio/((radio**2+(bd+ad)*(bd+ad))**(3.0/4.0))
v_ecuacion = v_ecuacion + Md**0.5/((radio*radio+ah*ah)**(1.0/4.0))

#figura 
figure(figsize=(10,10))
scatter(radio, velocidad)
plot(radio, v_ecuacion)
xlabel("radio")
ylabel("Velocidad")
title("Superposicion datos y ecuacion")
savefig('grafica.png')
