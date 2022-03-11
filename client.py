from ast import arguments
import sys

def main():
    ans = -1
    while ans == -1:
        print("El cliente está habilitado para las siguientes funciones:")
        print("1) Crear contenedor")
        print("2) Listar contenedores")
        print("3) Detener contenedor")
        print("4) Borar contenedor")
        ans = int(input())
        if ans < 1 or ans > 4:
            ans = -1
            print("No se ha podido ejecutar. Comando no valido.")
        else:
            if ans == 1:
                print("Crear xd")
            elif ans == 2:
                print("Listar xd")
            elif ans == 3:
                print("Detener xd")
            else:
                print("Crear xd")

main()