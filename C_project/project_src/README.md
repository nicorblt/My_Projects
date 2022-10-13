# Projet Interface Graphique - Équipe 7

Projet C de fin de 1ère année ENSIMAG. 
*** ***

Ce projet a été réalisé par PRIOUR Benjamin, NAVET Éloi et ROBLET Nicolas.

Le but du projet est la construction d'une bibliothèque en C permettant de réaliser une interface graphique composée de fenêtres et d’interacteurs tels que boutons, champs de saisie, etc.


## Détail des tests :

Les tests sont détaillés dans l'ordre alphabétique (et non dans l'ordre logique de réflexion).
À travers tout les tests graphiques, on teste _ei_place_ avec différents arugment pour _placer_.

### Tests globaux :

* *button.c* : &emsp;&emsp; test graphique
	> Vérification du bon fonctionnement du widget _button_ : création d'un bouton contenant un texte sur lequel on peut cliquer et effectue la fonction callback. Permet aussi de vérifier la _pick_surface_ du bouton et la gestion des entrées avec la fermeture de la fenêtre grâce à la touche échap.

* *button_limit.c* : &emsp;&emsp; test graphique
	> Vérification des limites de l'implémentation du widget _button_ : création d'un bouton contenant une image, un bouton dépassant les limites de la _root_surface_, un bouton pour lequel on ne précise pas sa taille (elle doit alors s'adapter à la taille de son contenu). Teste aussi les cas limites des couleurs de nos reliefs (blanc et noir).

* *frame.c* : &emsp;&emsp; test graphique
	> Vérification de l'implémentation de la _frame_ avec ses paramètres.

* *frame_image.c* : &emsp;&emsp; test graphique
	> Vérification des limites de l'implémentation de l'affichage d'une image dans un widget _frame_ : rognage d'une image, teste les anchor, définition du placement et taille avec des paramètres relatifs ou absolus de _ei_place_, la zone à copier de l'image lorsque le widget sort de la _root_surface_, est trop petit, lorsqu'il y a un relief ... À CHANGER

* *frame_multiple.c* : &emsp;&emsp; test graphique
	> Vérification du bon l'affichage de plusieurs widgets _frame_ : affichage de widget en frère et de widget enfant (pour _frame_ et _button_), permet aussi de vérifier le bon fonctionnement de _ei_copy_ avec de la transparence.

* *frame_relief.c* : &emsp;&emsp; test graphique
	> Vérification de l'affichage de _frame_ avec du relief : replacement du contenu interne.

* *frame_text.c* : &emsp;&emsp; test graphique
	> Vérification de l'affichage de _frame_ avec du texte : ajustement automatique de la taille de la frame, texte qui dépasse.

* *radiobutton.c* : &emsp;&emsp; test graphique
	> Vérification des fonctions associées à l'extension _radiobutton_ : liste chainées de radio-button (avec _form_), unicité de la sélection d'un bouton, affichage du texte et clippage dans une _toplevel_.

### Tests unitaires : 

* *test_ei_draw.c* : &emsp;&emsp; test fonctions graphiques non affiché
	> Vérifie la correction des fonctions de *ei_draw.c* : on teste les fonctions de fill, de map_rgba, d'intersectino entre deux rectangles, la copy d'une surface et la récupération d'une pick color depuis un id.

* *test_ei_frameclass.c* : &emsp;&emsp; test unitaire graphique 
	> Vérifie la correction de _frame_drawer_ grâce à des tests unitaires et des paramètres limites bien choisis; va de pair avec le test *frame.c* qui affiche le résultat.

* *test_ei_geometry_helpers.c* : &emsp;&emsp; test graphique
	> Après avoir testé les fonctions de manipulation des listes chainées de points, ce test vérifie le fonctionnement des fonctions de dessin (arc, frame arrondie, rectangle, triangle, losange, etc) codés à partir des primitives graphiques. Les tests unitaires se concentrent sur les cas limites des fonctions.

* *test_ei_widget.c*
	> Vérifie les fonctions de création et de destruction d'un _widget_.

* *test_linked_widget.c*
	> Vérifie la correction des fonctions de manipulation des listes chainées de _widget_ : ajout et suppression d'un _widget_ de la liste et enregistrement et désenregistreemnt d'un traitant associé au widget.

* *test_linked_callback.c*
	> Vérifie la correction des fonctions de manipulation des listes chainées de _callback_ : teste l'ajout et la suppression de traitants dans une liste.

* *test_widget_hashmap.c*
	> Vérifie l'implémentation de la hashmap de _widget_ : ajout et suppression d'une clé (qui est une liste chainée de callback) dans la hashmap qui est redimensionnée dynamiquement. Teste aussi l'appartenance d'une clé à la hashmap. 

* *test_linked_tag_and_widgets.c* 
	> Vérifie la correction de l'implémentation des listes chainées de _tag_ et _widget_ : ajout, suppression, recherche, enregistrement et désenregistrement d'un ou plusieurs traitants.

* *test_event_internal.c*
	> Vérification de la structure de stockage des _events_ : crée un tableau d'évènements et recherche dans ce tableau. Appelle les traitants associés aux évènements.

## Conclusion :
Merci d'avoir pris le temps de lire notre code !