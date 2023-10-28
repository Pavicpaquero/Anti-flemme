#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void ajouterTache(FILE *outputFile) {
    char action[100];
    int taskTime;
    int day, month;

    printf("Précisez l'action, l'heure et la date : \n");
    scanf("%s", action);
    printf("Vous avez saisi : %s\n", action);

    scanf("%d", &taskTime);
    printf("Vous avez saisi : %d heure\n", taskTime);

    scanf("%d/%d", &day, &month);
    printf("La date concaténée est : %d/%d\n", day, month);

    fprintf(outputFile, "%s,%d,%d,%d\n", action, taskTime, day, month);
}

void supprimerTache(FILE *outputFile) {
    char action[100];
    printf("Sélectionnez l'action que vous voulez supprimer : ");
    scanf("%s", action);

    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier temporaire");
        return;
    }

    char line[1024];
    int found = 0;

    while (fgets(line, sizeof(line), outputFile) != NULL) {
        if (strstr(line, action) == NULL) {
            fputs(line, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(tempFile);

    if (found) {
        remove("output.csv");
        rename("temp.csv", "output.csv");
        printf("Tâche supprimée avec succès.\n");
    } else {
        printf("Tâche non trouvée dans l'agenda.\n");
    }
}

void afficherAgenda(FILE *outputFile) {
    char line[1024];
    while (fgets(line, sizeof(line), outputFile) != NULL) {
        printf("%s", line);
    }
}

int execute(FILE *outputFile) {
    time_t currentTime;
    struct tm *timeInfo;

    // Obtenir l'heure actuelle
    time(&currentTime);
    timeInfo = localtime(&currentTime);

    int currentMonth = timeInfo->tm_mon + 1;  // Récupérer le mois actuel (ajouter 1 car janvier est 0)
    int currentDay = timeInfo->tm_mday;  // Récupérer le jour du mois
    int currentHour = timeInfo->tm_hour; // Récupérer l'heure actuelle

    char line[1024];
    int found = 0; // Indicateur de correspondance

    while (fgets(line, sizeof(line), outputFile) != NULL) {
        // Extraire les informations du fichier CSV
        char action[100];
        int taskTime, day, month;
        if (sscanf(line, "%[^,],%d,%d,%d", action, &taskTime, &day, &month) == 4) {
            if (currentMonth == month && currentDay == day && currentHour >= taskTime) {
                // Générer des coordonnées aléatoire

                // Utiliser zenity pour afficher une fenêtre d'alerte avec des coordonnées aléatoires
                char alertMessage[256];
                snprintf(alertMessage, sizeof(alertMessage), "Fais cette tache connard :%s", action);
                char command[512];
                snprintf(command, sizeof(command), "zenity --info --title='Alerte' --text='%s' --width=600 --height=400", alertMessage);
                system(command);
                system(command);
                system(command);

                found = 1;
            }
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    FILE *outputFile = fopen("output.csv", "a+");

    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier en écriture");
        return 1;
    }

    if (argc != 2) {
        printf("Un seul argument est attendu.\n");
        return 1;
    }

    if (strcmp(argv[1], "-show") == 0) {
        afficherAgenda(outputFile);
    } else if (strcmp(argv[1], "-add") == 0) {
        ajouterTache(outputFile);
    } else if (strcmp(argv[1], "-remove") == 0) {
        supprimerTache(outputFile);
    } else if (strcmp(argv[1], "-execute") == 0) {
        execute(outputFile);
    }

    fclose(outputFile);

    return 0;
}
