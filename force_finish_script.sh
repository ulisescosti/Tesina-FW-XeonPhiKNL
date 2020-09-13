#!/bin/bash

#USO: Pasar por parámetro el nombre del proceso ($1) y la cantidad de veces que se quiere ejecutar el killall ($2)

for((i=1;i<=$2;i++)); do
	killall -9 $1
done
