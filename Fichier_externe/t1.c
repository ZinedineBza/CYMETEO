#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Ouvrir les fichiers d'entrée et de sortie
    FILE *input = fopen("tmp.csv", "r");
    FILE *output = fopen("result.txt", "w");

    // Vérifier que les fichiers ont été ouverts correctement
    if (input == NULL || output == NULL) {
        printf("Impossible d'ouvrir les fichiers d'entrée ou de sortie\n");
        return 1;
    }

    // Tableau pour stocker les statistiques pour chaque identifiant
    int stats[1000][3];
    int id, value, count[1000], ids[1000], currentId, currentIndex = 0;
    float sum[1000];

    // Initialiser les statistiques
    for (int i = 0; i < 1000; i++) {
        stats[i][0] = 1000000; // min
        stats[i][1] = 0; // max
        sum[i] = 0;
        count[i] = 0;
    }

    // Lire les lignes du fichier d'entrée
    char line[1024];
    while (fgets(line, 1024, input)) {
        // Extraire l'identifiant et la valeur de la ligne
        sscanf(line, "%d;%d", &id, &value);

        // vérifie si on a déjà vu cet identifiant
        currentId = -1;
        for(int i = 0; i < currentIndex; i++){
            if(ids[i] == id) {
                currentId = i;
                break;
            }
        }
        if (currentId == -1) {
            currentId = currentIndex;
            currentIndex++;
            ids[currentId] = id;
        }
        // Mettre à jour les statistiques pour l'identifiant
        if (value < stats[currentId][0]) {
            stats[currentId][0] = value;

        }
        if (value > stats[currentId][1]) {
            stats[currentId][1] = value;
        }
        sum[currentId] += value;
        count[currentId]++;
    }

    // Écrire les résultats dans le fichier de sortie
    for (int i = 0; i < currentIndex; i++) {
        float avg = sum[i] / count[i];
        fprintf(output, "%d %d %d %.2f 0\n", ids[i], stats[i][0], stats[i][1], avg);
    }

    // Fermer les fichiers
    fclose(input);
    fclose(output);
    return 0;
}
