# ce Makefile est très basique, on peut faire mieux avec les cibles prédéfinies

default: exo1 exo2 exo2bis rectangle

exo1: exo1.c
	gcc -Wall -o exo1 exo1.c

exo2: exo2.c
	gcc -Wall -o exo2 exo2.c

# Astuce:
# $@ permet d'éviter la répétition du nom du programme et du nom du .c
# si le programme doit porter le même nom que le .c

exo2bis: exo2bis.c
	gcc -Wall -o $@ exo2bis.c

rectangle: rectangle.c
	gcc -Wall -o $@ rectangle.c
