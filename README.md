# Scop
This project has been created as part of the 42 curriculum by lboudjem

# Description

# Instructions

Installation:

    git clone git@github.com:Louisa4124/Scop.git
    cd ./scop
    make

Utilisation:

    ./scop [fichier .obj]

Exemple:

    ./scop ./assets/42.obj


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

Dans le cas de polygones, etant donner que OpenGL ne sait pas afficher directements ces formes avec ```GL_TRIANGLES```, on doit effectuer une triangulation et decouper les polygones en triangles.

Les faces peuvent aussi avoir des textures/normales, et se presentent sous les formes suivantes:

- Position seule : **f v1 v2 v3**
- Position et Coordonnées de Texture : **f v1/vt1 v2/vt2 v3/vt3**
- Position, Texture et Normale : **f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3**
- Position et Normale : **f v1//vn1 v2//vn2 v3//vn3**

# Resources

- https://opengl.developpez.com/tutoriels/apprendre-opengl/
- https://www.scratchapixel.com/lessons/3d-basic-rendering/obj-file-format//obj-file-format.html