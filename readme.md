# Minishell - Documentation des Fonctions Externes et Builtins

---

## 📚 Fonctions Externes (External Functions)


---

### **1. Gestion de la Mémoire**
- **`perror`** / **`strerror`** :  
  Affichent des messages d’erreur système.  
  **Exemple** : Si `open()` échoue, `perror("open")` affiche "open: No such file or directory".  

---

### **2. Entrées/Sorties et Fichiers**
- **`opendir`**, **`readdir`**, **`closedir`** :  
  Parcourent les répertoires. Utiles pour l’expansion des wildcards `*` (bonus).  

- **`stat`**, **`lstat`**, **`fstat`** :  
  Récupèrent des informations sur un fichier (ex : vérifier si un chemin est un exécutable).  

---

### **3. Processus et Signaux**
- **`signal`**, **`sigaction`** :  
  Gèrent les signaux (ex : `SIGINT` pour Ctrl+C).  
  **Comportement attendu** :  
  - `Ctrl+C` : Affiche un nouveau prompt.  
  - `Ctrl+\` : Ignoré.  

---

### **4. Environnement et Chemins**
- **`getenv`** :  
  Récupère la valeur d’une variable d’environnement (ex : `PATH` pour chercher les exécutables).  

- **`getcwd`** :  
  Retourne le répertoire de travail actuel (utilisé par `pwd`).  

- **`chdir`** :  
  Change le répertoire courant (utilisé par `cd`).  

---

### **5. Terminal et Readline**
- **`readline`** :  
  Lit une ligne de saisie utilisateur avec édition de ligne et historique.  
  **Exemple** : `char *input = readline("minishell> ");`  

- **`add_history`** :  
  Ajoute une commande à l’historique (accessible avec les flèches ↑/↓).  

- **`isatty`** :  
  Vérifie si un descripteur de fichier est un terminal (ex : `STDIN_FILENO`). 

- **`rl_*`** :
  functions: Manage prompt behavior (e.g., refresh the display after signals).  

---

### **6. Miscellaneous**

- `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`  
   Terminal capability functions (used for cursor movement or screen manipulation, if needed)

---

## 🛠 Builtins

Commandes intégrées au shell :

---

### **1. `echo`**
- **Usage** : `echo [-n] [text]`  
- **Fonction** :  
  Affiche les arguments.  
  - `-n` : Supprime le saut de ligne final.  
  **Exemple** :  
  ```bash
  echo "Hello World"   # Affiche "Hello World\n"
  echo -n "Hello"      # Affiche "Hello"

---

### **2. `cd`**
- **Usage** : `cd [chemin] `
- **Fonction** :
 Change le répertoire courant.
    - Sans argument : change vers le répertoire défini dans $HOME
- **Error case** 
    -Droit d'accès insuffisant : affiche une erreur
    -Chemin inexistant : affiche une erreur
    -Chemin n'est pas un répertoire : affiche une erreur
     **Exemple** : 
     ```bash
     cd            # Va dans le home directory
     cd /tmp       # Va dans /tmp
     cd ..         # Remonte d'un niveau
     cd Documents  # Va dans le sous-répertoire Documents

---

### **3. `pwd`**
- **Usage** : `pwd`
- **Fonction** :
    Affiche le chemin absolu du répertoire de travail actuel.
    **Exemple** :
    ```bash
    pwd  # Affiche "/home/user/mil03/minishell"

---

### **4. `export`**
- **Usage** : `export [nom[=valeur] ...]`
- **Fonction** :   
    Définit des variables d'environnement.
     - Sans arguments : affiche toutes les variables exportées.
     - Avec arguments : définit ou modifie des variables d'environnement
    **Exemple** :
    ```bash
    export nom=valeur    # Définition simple
    export nom="valeur"  # Définition avec guillemets
    export nom='valeur'  # Définition avec apostrophes
    export nom          # Exporte une variable existante

---
### **5. `unset`**
- **Usage** : `unset [nom ...]`
- **Fonction** :   
    Supprime des variables d'environnement.
     **Exemple** :
     ```bash
     unset NAME     # Supprime la variable NAME
     unset VAR1 VAR2  # Supprime plusieurs variables

---

### **6. `env`**
- **Usage**  : `env`
- **Fonction** :
    Affiche toutes les variables d'environnement.
    **Exemple** :
    ```bash
    env  # Affiche USER=john PATH=/usr/bin HOME=/home/john etc.

---

### **7. `exit`**
- **Usage**  : `exit [n]`
- **Fonction** :
    Quitte le shell avec le code de retour spécifié.
     - Sans argument : quitte avec le statut de la dernière commande.  
     - Avec argument numérique : quitte avec ce code de retour.
    **Exemple** :
    ```bash
    exit      # Quitte avec le dernier code de retour
    exit 1    # Quitte avec le code 1  

---

## 🛠 To do

faire $?
secu for 1024 PROCESUS
here doc sans cat avant ou cmd
signal 
prob ast quand ls > out.txt | w > out2.txt
cat << eof bug
<< eof bug car je skip la node 