ECG PROJECT
-----------

Description de l’application :

 - Lecture des données et reconstruction de la séquence complète de l’ECG pour la durée totale de celle-ci.

 - Une fenêtre de largeur 6 s (avec un bouton play/stop) permet de faire défiler le résultat à la bonne cadence.

 - Fonction de correction de la dérive du signal en Y en faisant une moyenne sur fenêtre glissante (par exemple). Cette dernière est réglable par l’utilisateur.

 - Fonction de gain/zoom vertical des données:

--------Compilation---------

- Sur systeme UNIX lancer l'exécutable ECG
- Sinon générer un Makefile avec : qmake -unix/macx/win32 ; qmake -makefile
- Puis make && ./ECG
