#!/usr/bin/env python3
"""
Fait approximation pi
"""

from sys import argv
from random import uniform


def main_appro(nb_point):
    """
    Fait l'approximation
    """
    nb_pt_in = 0

    if __name__ == "__main__":

        #Si l'on l'execute pour les tests

        for _ in range(nb_point):
            absi, ordo = uniform(-1, 1), uniform(-1, 1)
            nb_pt_in += int(absi**2+ordo**2 <= 1)
        print(4*nb_pt_in/nb_point)

    else:

        #Si l'on l'execute pour draw afin de retourner uniquement ce qui est nécessaire

        list_point = []
        for _ in range(nb_point):
            absi, ordo = uniform(-1, 1), uniform(-1, 1)
            list_point.append(((absi, ordo), absi**2+ordo**2 <= 1))
        return list_point

if __name__ == "__main__":
    main_appro(int(argv[1]))
