# Scop
This project has been created as part of the 42 curriculum by lboudjem

# Description

# Instructions

# Resources

# Fonctionnement 

### Parsing

Le format **.obj** est un fichier texte où chaque ligne commence par un identifiant indiquant la nature de la donnée.

Les principaux identifiants sont les suivants:

- **v** : Position d'un sommet *(x, y, z)*
- **vt** : Coordonnées de texture *(u, v)*
- **vn**: Vecteur normal *(x, y, z)*
- **f** : Une face

Exemple:

```obj
# Pyramide à base carrée
# ---------------------

# 1. Sommets (v x y z)
v -0.5 0.0 -0.5  # 1 : Coin bas-gauche
v  0.5 0.0 -0.5  # 2 : Coin bas-droit
v  0.5 0.0  0.5  # 3 : Coin haut-droit
v -0.5 0.0  0.5  # 4 : Coin haut-gauche
v  0.0 1.0  0.0  # 5 : Sommet de la pyramide

# 2. Faces (f v1 v2 v3 ...)
# Remarque : Les indices commencent à 1 en format OBJ.

# Base de la pyramide (2 triangles)
f 1 2 3
f 1 3 4

# Faces latérales de la pyramide (4 triangles)
f 1 2 5
f 2 3 5
f 3 4 5
f 4 1 5
```

#### Structure d'une face

Chaque ligne **f** définit un triangle/polygone. Les valeurs font référence aux indices des listes **v**, **vt**, et **vn** lues plus haut dans le fichier.