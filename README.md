# TP3 - Histogrammes et Quantification

> BINP - Université de Rennes 1  
> Étudiants :  
> - **Damien Vailland**  
> - **Axel Plessis**  
> Date : 24/09/2024

## 🧠 Objectifs

Ce TP a pour but de manipuler les histogrammes d'images en niveaux de gris, à travers :

- Le calcul et l'affichage d'un histogramme et d'un histogramme cumulé
- L'analyse statistique de l'image (moyenne, écart-type, dynamique, nombre de niveaux de gris distincts)
- L'application de transformations comme l’anamorphose (linéaire, seuil), l'égalisation d'histogramme
- La quantification uniforme de l'image

Le tout s'appuie sur la bibliothèque **ViSP**.

---

## 📁 Structure du projet

```
.
├── tp3.cpp           # Code principal du TP
├── images/           # Répertoire contenant les images en niveaux de gris utilisées
└── README.md         # Ce fichier
```

---

## ▶️ Compilation & Exécution

### Pré-requis

- CMake
- Bibliothèque [ViSP](https://visp.inria.fr/) installée

### Compilation

```bash
g++ tp3.cpp -o tp3 -lvisp_core -lvisp_io -lvisp_gui
```

> **Note** : Il peut être nécessaire d'ajuster les options de compilation en fonction de votre installation de ViSP.

### Exécution

```bash
./tp3
```

---

## 💡 Fonctionnalités

### 1. Affichage des statistiques d'une image

- Calcul et affichage de :
  - Histogramme
  - Histogramme cumulé
  - Moyenne, écart-type
  - Dynamique de l’image
  - Nombre de niveaux de gris distincts

### 2. Anamorphoses

- **Anamorphose 1** : transformation linéaire avec pente
- **Anamorphose 2** : seuillage suivi d’une mise à l’échelle

### 3. Égalisation d’histogramme

- Augmente le contraste de l’image en répartissant les niveaux de gris uniformément

### 4. Quantification uniforme

- Réduction du nombre de niveaux de gris (selon un nombre de bits)
- Affichage de l’EQM (Erreur Quadratique Moyenne) avec l’image d’origine

---

## 📷 Exemple d'utilisation

Lancement du programme :

```bash
./tp3
```

Choisir une des options du menu :

```
1. Affichage des stats d'une image
2. Anamorphoses
3. Egalisation d'histogramme
4. Quantification uniforme
5. Quitter
```

Saisir le nom de l’image (ex. `baboon.pgm`) — le programme va automatiquement chercher dans `../images/`.

---

## 📌 Remarques

- Les images doivent être en **niveaux de gris** (PGM ou similaire).
- L’histogramme est affiché dans une fenêtre graphique via ViSP.

---

## 📚 Références

- [ViSP Documentation](https://visp.inria.fr/documentation/)
- [TP Imagerie Numérique — ESIR Rennes 1](https://esir.univ-rennes1.fr/)

---

## 🛠️ Auteurs

- Damien Vailland
- Axel Plessis
