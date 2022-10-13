#!/usr/bin/env python3
"""System module."""

from subprocess import run
from sys import argv
from approximate_pi import main_appro

def generate_ppm_file(taille_image, dico_point, fichier):
    """
    Genere l'image ppm
    """

    print(f"P3 \n{taille_image} {taille_image} \n1", file=fichier)
    #   On assimile l'image au dictionnaire
    for ligne in dico_point.values():
        abscisse = 0
        while abscisse < taille_image:

            try:
                result = ligne[f"{abscisse}"]
            except KeyError:
                #   On attrape l'erreur (equivalent à: rien d'associer à la case, donc blanc)
                print("1 1 1", file=fichier)
            else:
                if result is None:
                    #   Correspond à un bit pour écrire un chiffre
                    print("0 0 0", file=fichier)
                elif result:
                    #   In
                    print("1 0 1", file=fichier)
                elif not result:
                    #   Out
                    print("0 0 1", file=fichier)
            abscisse += 1

def creation_l10(l_resultat, nb_point):
    '''
    Création du tableau dynamique l_in contenant les points in
    parmis les (1/10)*nb_point points et l_point_partiel
    '''
    compte_in = 0
    compte_tour = 0
    l_in = []
    ind_parti_liste = 0
    l_point_partiel = []

    for i in range(10):
        #   Création du tableau dynamique l_point_partiel suivant :
        #   taille image = n, l_point_partiel = [n/10, 2n/10, 3n/10 ..., n]
        l_point_partiel.append(int(((i+1)/10)*nb_point))

    while compte_tour < nb_point:
        if l_resultat[compte_tour][1]:
            #   incrémente de 1 si point est dans le cercle
            compte_in += 1
        if compte_tour >= l_point_partiel[ind_parti_liste]:
            #   permet de savoir le nb de point in par n/10 de points
            ind_parti_liste += 1
            l_in.append(compte_in)
        compte_tour += 1
    l_in.append(compte_in)

    return l_point_partiel, l_in

def transfo_point(point, taille_image):
    """
    Translate les points de la grille x in [-1,1], y in [-1,1] vers le dico
    """
    abscisse = int((point[0]+1)*taille_image/2)
    ordonnee = int((point[1]+1)*taille_image/2)

    return abscisse, ordonnee

def modif_dico(dico_point, taille_image, liste_point_a_placer):
    """
    Modifie le dico actuelle en ajoutant les nouveaux points tirés sur l'ancien
    pour ne pas avoir à en créer un nouveau
    """
    for point, resultat in liste_point_a_placer:
        absci, ordo = transfo_point(point, taille_image)
        dico_point[f"{ordo}"][f"{absci}"] = resultat

    return dico_point

def afficher_pi(dico, pii, taille_image):
    """
    Imprime pi sur le dico à l'aide d'un afficheur 7 segments,
    le point (coordx,coordy) repere le point en bas a gauche du digit
    """
    #   Définition des paramètres de l'afficheur 7 segments en fonctions de la taille des images
    largeur_seg = int(taille_image/20)
    hauteur_seg = int(taille_image/20)
    ecartx = int(taille_image/50)
    largeur_point = int(taille_image/50)
    coordy, coordx = int(taille_image/2+hauteur_seg),\
         int(taille_image/2-int((len(pii)-1)/2)*(largeur_seg+ecartx)-largeur_point-ecartx)

    #   Attribution spéciale des numéros marquant les segment de l'afficheur
    #   (pair : verticaux, impair : horizontaux)
    lcarac = ((0, 1, 2, 4, 5, 6), (2, 6), (0, 1, 3, 5, 6), (1, 2, 3, 5, 6), (2, 3, 4, 6),\
         (1, 2, 3, 4, 5), (0, 1, 2, 3, 4, 5), (2, 5, 6), (0, 1, 2, 3, 4, 5, 6), (1, 2, 3, 4, 5, 6))

    for carac in pii:

        if carac == ".":
            # Spécial pour le point pour ne pas avoir le meme decalage en x que pour les chiffres
            # (ex: 42.1, 42. 1)
            dico = afficher_point(dico, largeur_point, coordy, coordx)
            coordx += ecartx + largeur_point

        else:
            segment = lcarac[int(carac)]
            for element in segment:

                if element%2: #impair -> horizontaux
                    dico = segmenthorizontaux(dico, largeur_seg, coordx,\
                         coordy-((element-1)//2)*hauteur_seg)
                         #  Fonction renvoyant 0 pour 1 (donc horizontaux bas),
                         #  1 pour 3 (horizontaux intermédiaire), 2 pour 5 (horizontaux haut)

                else:
                    dico = segmentverticaux(dico, hauteur_seg, int(coordx + \
                        largeur_seg*(element%4)/2), coordy - hauteur_seg*(element > 3))
                        #   Même principe que pour les horizontaux mais en jouant sur le x et y

            coordx += ecartx + largeur_seg

    return dico

def segmenthorizontaux(dico, largeur, coordx, coordy):
    '''
    Imprime un segment horizontale
    '''
    epaisseur = (-1, 0, 1) #Permet d'avoir un trait épais pour que ce soit plus lisible
    for val_epais in epaisseur:
        ind = 0
        while ind < largeur:
            dico[f'{coordy+val_epais}'][f'{coordx + ind}'] = None #####
            ind += 1
    return dico

def segmentverticaux(dico, hauteur, coordx, coordy):
    '''
    Imprime un segment horizontale
    '''
    epaisseur = (-1, 0, 1) #Permet d'avoir un trait épais pour que ce soit plus lisible
    for val_epais in epaisseur:
        ind = 0
        while ind < hauteur:
            dico[f'{coordy-ind}'][f'{coordx+val_epais}'] = None ###
            ind += 1
    return dico

def afficher_point(dico, largeur_point, coordy, coordx):
    """"
    imprime le point de pi sur le dico de dim larheur*largeur
    """
    indy = 0
    while indy < largeur_point:
        indx = 0
        while indx < largeur_point:
            dico[f'{coordy-indy+1}'][f'{coordx+indx}'] = None
            indx += 1
        indy += 1
    return dico

def main():
    """
    Pylint t'es chaint
    """

    #récupération des variables
    taille_image = int(argv[1])
    nb_point = int(argv[2])
    ch_apresvirgule = int(argv[3])


    #Test si elles sont valide sinon on lève une erreur
    if nb_point < 100 or taille_image < 100 or ch_apresvirgule < 1:
        raise ValueError("Argument non valide")


    #   Crétation du dictionnaire qui va servir de "grille",
    #   stock uniquement les les points tirés ET NON pi (pi pas imprimé).
    #   (Pas spécifié->Pas tiré, True->in, False->out, None->noir pour le pi)
    dico_point = {}
    compt = 0
    while compt != taille_image:
        dico_point[f"{compt}"] = {}
        compt += 1


    #   Création des points et création des tableaux dynamiques
    #   permettant de creer les 10 images contenant les bons points
    print("Attente calcul approximation de pi")
    l_resultat = main_appro(nb_point)
    l_point_partiel, l_in = creation_l10(l_resultat, nb_point)


    #   Création des 10 images
    for i in range(10):
        print(f"Impression image {i}")
        #   Evaluation du pi (pour le bon nb de points) et creation de la bonne str pour le titre
        pii = 4*l_in[i]/l_point_partiel[i]
        pi_str = f"{pii:.{ch_apresvirgule}f}"
        nom = f"img{i}_{pi_str[0]}-{pi_str[-ch_apresvirgule:]}.ppm"
        fichier = open(nom, "w")


        #   Modification du dico actuel pour y ajouter les nouveaux points
        if i == 0:
            dico_point = modif_dico(dico_point, taille_image, l_resultat[:l_point_partiel[0]])
        else:
            dico_point = modif_dico(dico_point, taille_image,\
                 l_resultat[l_point_partiel[i-1]:l_point_partiel[i]])


        #   Création du dico qui va contenir les points tirés ET pi
        compt = 0
        dico_afficher = {}
        while compt < taille_image:
            dico_afficher[f"{compt}"] = dico_point[f"{compt}"].copy()
            compt += 1
        dico_afficher = afficher_pi(dico_afficher, pi_str, taille_image)

        #   Imprime l'image
        generate_ppm_file(taille_image, dico_afficher, fichier)
        fichier.close()

    #   Génération du gif
    print("Création du gif")
    run('convert -delay 50 img* pi.gif', check=True, shell=True)

if __name__ == "__main__":
    main()
