#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// ---------------- STRUCTS ----------------
struct User {
    char email[200];
    char name[50];
    char password[50];
    int calories;
    int protein;
    int carbs;
    int fat;
};

struct Food {
    char name[50];
    int calories;
    int protein;
    int carbs;
    int fat;
};

// ---------------- HELPER ----------------
void trimNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

// ---------------- FIND FOOD ----------------
int findFood(struct Food *f) {
    FILE *file = fopen("Foods.txt", "r");
    char buffer[BUFFER_SIZE];
    char search[100];

    if (!file) {
        printf("Foods.txt not found!\n");
        return 0;
    }

    printf("Enter food name: ");
    fgets(search, sizeof(search), stdin);
    trimNewline(search);

    while (fgets(buffer, BUFFER_SIZE, file)) {
        struct Food temp;

        sscanf(buffer, "%[^,],%d,%d,%d,%d",
               temp.name,
               &temp.calories,
               &temp.protein,
               &temp.carbs,
               &temp.fat);

        if (strcmp(temp.name, search) == 0) {
            *f = temp;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// ---------------- CREATE USER ----------------
void createUser() {
    struct User u;
    FILE *file = fopen("Users.txt", "a+");
    char buffer[BUFFER_SIZE];

    if (!file) {
        printf("Error opening Users.txt\n");
        return;
    }

    printf("Enter email: ");
    fgets(u.email, sizeof(u.email), stdin);
    trimNewline(u.email);

    while (strchr(u.email, '@') == NULL) {
        printf("Invalid email! Must contain '@'. Try again: ");
        fgets(u.email, sizeof(u.email), stdin);
        trimNewline(u.email);
    }

    rewind(file);

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char existingEmail[200];
        sscanf(buffer, "%[^,]", existingEmail);

        if (strcmp(existingEmail, u.email) == 0) {
            printf("User already exists!\n");
            fclose(file);
            return;
        }
    }

    printf("Enter name: ");
    fgets(u.name, sizeof(u.name), stdin);
    trimNewline(u.name);

    printf("Enter password: ");
    fgets(u.password, sizeof(u.password), stdin);
    trimNewline(u.password);

    char temp[50];

    printf("Calories goal: ");
    fgets(temp, sizeof(temp), stdin);
    u.calories = atoi(temp);

    printf("Protein goal: ");
    fgets(temp, sizeof(temp), stdin);
    u.protein = atoi(temp);

    printf("Carbs goal: ");
    fgets(temp, sizeof(temp), stdin);
    u.carbs = atoi(temp);

    printf("Fat goal: ");
    fgets(temp, sizeof(temp), stdin);
    u.fat = atoi(temp);

    fprintf(file, "%s,%s,%s,%d,%d,%d,%d\n",
            u.email, u.name, u.password,
            u.calories, u.protein, u.carbs, u.fat);

    fclose(file);
    printf("User created successfully!\n");
}

// ---------------- LOGIN ----------------
int login(struct User *u) {
    FILE *file = fopen("Users.txt", "r");
    char buffer[BUFFER_SIZE];
    char email[200], password[50];

    if (!file) {
        printf("No users found. Create one first.\n");
        return 0;
    }

    printf("Email: ");
    fgets(email, sizeof(email), stdin);
    trimNewline(email);

    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    trimNewline(password);

    while (fgets(buffer, BUFFER_SIZE, file)) {
        struct User temp;

        sscanf(buffer, "%[^,],%[^,],%[^,],%d,%d,%d,%d",
               temp.email,
               temp.name,
               temp.password,
               &temp.calories,
               &temp.protein,
               &temp.carbs,
               &temp.fat);

        if (strcmp(temp.email, email) == 0 &&
            strcmp(temp.password, password) == 0) {

            *u = temp;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// ---------------- VIEW LOGS ----------------
void viewLogs(struct User *u) {
    FILE *file = fopen("logs.txt", "r");
    char buffer[BUFFER_SIZE];

    if (!file) {
        printf("No logs found.\n");
        return;
    }

    int totalCalories = 0;
    int totalProtein = 0;
    int totalCarbs = 0;
    int totalFat = 0;

    printf("\n--- YOUR DAILY LOGS ---\n");

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char email[200], foodName[50];
        int calories, protein, carbs, fat;

        sscanf(buffer, "%[^,],%[^,],%d,%d,%d,%d",
               email, foodName,
               &calories, &protein, &carbs, &fat);

        if (strcmp(email, u->email) == 0) {
            printf("Food: %s | Cal: %d | P: %d | C: %d | F: %d\n",
                   foodName, calories, protein, carbs, fat);

            totalCalories += calories;
            totalProtein += protein;
            totalCarbs += carbs;
            totalFat += fat;
        }
    }

    fclose(file);

    printf("\n--- TOTAL ---\n");
    printf("Calories: %d\n", totalCalories);
    printf("Protein: %d\n", totalProtein);
    printf("Carbs: %d\n", totalCarbs);
    printf("Fat: %d\n", totalFat);
}

// ---------------- MAIN ----------------
int main() {
    struct User currentUser;
    struct Food food;
    int loggedIn = 0;

    char choice[10];

    // -------- LOGIN LOOP --------
    while (!loggedIn) {
        printf("\n1. Login\n2. Create Account\nChoice: ");
        fgets(choice, sizeof(choice), stdin);

        if (choice[0] == '1') {
            if (login(&currentUser)) {
                printf("Welcome %s!\n", currentUser.name);
                loggedIn = 1;
            } else {
                printf("Login failed!\n");
            }
        } else if (choice[0] == '2') {
            createUser();
        } else {
            printf("Invalid option\n");
        }
    }

    // -------- MAIN MENU --------
    while (1) {
        printf("\n1. Add Food\n2. View Today\n3. View Logs\n4. Exit\nChoice: ");
        fgets(choice, sizeof(choice), stdin);

        if (choice[0] == '1') {
            if (findFood(&food)) {

                printf("\nFound: %s\nCalories: %d\nProtein: %d\nCarbs: %d\nFat: %d\n",
                       food.name, food.calories, food.protein, food.carbs, food.fat);

                FILE *logFile = fopen("logs.txt", "a");
                if (!logFile) {
                    printf("Error opening logs file\n");
                    continue;
                }

                fprintf(logFile, "%s,%s,%d,%d,%d,%d\n",
                        currentUser.email,
                        food.name,
                        food.calories,
                        food.protein,
                        food.carbs,
                        food.fat);

                fclose(logFile);

                printf("Food logged successfully!\n");

            } else {
                printf("Food not found\n");
            }

        } else if (choice[0] == '2') {
            printf("View Today (coming next)\n");

        } else if (choice[0] == '3') {
            viewLogs(&currentUser);

        } else if (choice[0] == '4') {
            printf("Goodbye!\n");
            break;

        } else {
            printf("Invalid option\n");
        }
    }

    return 0;
}