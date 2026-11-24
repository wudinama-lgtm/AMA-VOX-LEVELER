AMA VOCAL LEVELER — projet source VST3 pour Cubase 12 / Windows 64 bits
======================================================================

IMPORTANT
---------
Ce ZIP contient le code source du plugin, pas encore un fichier VST3 compilé.
Il faut compiler le projet sur Windows avec JUCE et Visual Studio. Je ne peux
pas prétendre qu'un .vst3 a été produit tant que cette compilation n'a pas eu lieu.

FONCTIONS
---------
- Vocal rider en temps réel basé sur un détecteur de niveau RMS approché.
- Gain automatique plafonné par MAXIMUM BOOST et MAXIMUM REDUCTION.
- Contrôle des crêtes avec un plafond réglable.
- Réglage du niveau cible, de la vitesse et du gain de sortie.
- Bypass et mémorisation des paramètres dans Cubase.

PRÉREQUIS
---------
1. Windows 10/11 64 bits.
2. Visual Studio 2022 avec la charge de travail « Développement Desktop en C++ ».
3. JUCE 7 ou 8 (version récente) : https://juce.com/
4. CMake 3.22 ou plus récent.

COMPILATION
-----------
1. Installe JUCE et Visual Studio.
2. Décompresse ce dossier, par exemple dans C:\\Audio\\AMA_VOCAL_LEVELER.
3. Ouvre « x64 Native Tools Command Prompt for VS 2022 ».
4. Va dans le dossier du projet :
   cd C:\\Audio\\AMA_VOCAL_LEVELER
5. Configure (adapte C:/JUCE au chemin réel de JUCE) :
   cmake -S . -B build -DJUCE_DIR="C:/JUCE"
6. Compile :
   cmake --build build --config Release
7. Le VST3 est normalement créé dans :
   build/AMA_VOCAL_LEVELER_artefacts/Release/VST3/
8. Copie le dossier « AMA VOCAL LEVELER.vst3 » dans :
   C:\\Program Files\\Common Files\\VST3
9. Relance Cubase 12, puis vérifie le Gestionnaire de plug-ins VST.

RÉGLAGES DE DÉPART POUR UNE VOIX
-------------------------------
Target level : -18 dBFS
Maximum boost : 4 à 6 dB (commence bas)
Maximum reduction : 4 à 6 dB
Rider speed : 0.3 à 0.6 s
Peak ceiling : -3 dBFS pour commencer
Output : ajuste pour conserver un niveau de sortie comparable au bypass

CONSEILS
--------
- Utilise d'abord le plugin sur une piste vocale mono.
- Évite de demander +18 dB de boost : le bruit de fond sera remonté aussi.
- Pour une voix très irrégulière, corrige les gros écarts avec le volume/automation
  de Cubase avant le plugin.
- Le plafond de crête de cette première version est un contrôle de gain réactif,
  pas un limiteur look-ahead transparent.
- « Normalisation » ici signifie nivellement dynamique en temps réel. Une
  normalisation d'un fichier entier après analyse hors ligne est une fonction
  différente et n'est pas incluse dans cette première version.

VERSION
-------
Prototype source 1.0. À tester et valider sur des copies de sessions avant usage
sur des projets importants. Le projet doit être compilé avec JUCE/Visual Studio.
