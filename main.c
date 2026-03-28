#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// ---------------- NEW: DATE ----------------
void getToday(char *dateStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(dateStr, 20, "%Y-%m-%d", tm);
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

// ---------------- NEW: DASHBOARD ----------------
void showDashboard(struct User *u, int cal, int pro, int carb, int fat) {
    printf("\n--- DASHBOARD ---\n");

    printf("Remaining Calories: %d\n", u->calories - cal);
    printf("Remaining Protein: %d\n", u->protein - pro);
    printf("Remaining Carbs: %d\n", u->carbs - carb);
    printf("Remaining Fat: %d\n", u->fat - fat);

    printf("\n--- PROGRESS ---\n");

    if (u->calories > 0)
        printf("Calories: %.0f%%\n", (cal * 100.0) / u->calories);

    if (u->protein > 0)
        printf("Protein: %.0f%%\n", (pro * 100.0) / u->protein);

    if (u->carbs > 0)
        printf("Carbs: %.0f%%\n", (carb * 100.0) / u->carbs);

    if (u->fat > 0)
        printf("Fat: %.0f%%\n", (fat * 100.0) / u->fat);
}

// ---------------- NEW: VIEW TODAY ----------------
void viewToday(struct User *u) {
    FILE *file = fopen("logs.txt", "r");
    char buffer[BUFFER_SIZE];
    char today[20];

    getToday(today);

    int totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;

    printf("\n--- TODAY (%s) ---\n", today);

    if (!file) {
        printf("No logs found.\n");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char email[200], date[20], foodName[50];
        int calories, protein, carbs, fat;

        if (sscanf(buffer, "%[^,],%[^,],%[^,],%d,%d,%d,%d",
           email, date, foodName,
           &calories, &protein, &carbs, &fat) != 7) {
        continue;
}

        if (strcmp(email, u->email) == 0 && strcmp(date, today) == 0) {
            printf("Food: %s | Cal: %d\n", foodName, calories);

            totalCalories += calories;
            totalProtein += protein;
            totalCarbs += carbs;
            totalFat += fat;
        }
    }

    fclose(file);

    printf("\nCalories: %d / %d\n", totalCalories, u->calories);
    printf("Protein: %d / %d\n", totalProtein, u->protein);
    printf("Carbs: %d / %d\n", totalCarbs, u->carbs);
    printf("Fat: %d / %d\n", totalFat, u->fat);
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
        char email[200], date[20], foodName[50];
        int calories, protein, carbs, fat;

       if (sscanf(buffer, "%[^,],%[^,],%[^,],%d,%d,%d,%d",
           email, date, foodName,
           &calories, &protein, &carbs, &fat) != 7) {
           continue; // skip bad/old lines
}

        if (strcmp(email, u->email) == 0) {
            printf("Food: %s (%s) | Cal: %d\n", foodName, date, calories);

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

    showDashboard(u, totalCalories, totalProtein, totalCarbs, totalFat);
}

// ---------------- UPDATE GOALS ----------------
void updateGoals(struct User *u) {
    FILE *file = fopen("Users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    char buffer[BUFFER_SIZE];

    if (!file || !tempFile) {
        printf("Error opening file\n");
        return;
    }

    char temp[50];

    printf("\n--- UPDATE GOALS ---\n");

    printf("New Calories goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->calories = atoi(temp);

    printf("New Protein goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->protein = atoi(temp);

    printf("New Carbs goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->carbs = atoi(temp);

    printf("New Fat goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->fat = atoi(temp);

    while (fgets(buffer, BUFFER_SIZE, file)) {
        struct User tempUser;

        sscanf(buffer, "%[^,],%[^,],%[^,],%d,%d,%d,%d",
               tempUser.email,
               tempUser.name,
               tempUser.password,
               &tempUser.calories,
               &tempUser.protein,
               &tempUser.carbs,
               &tempUser.fat);

        if (strcmp(tempUser.email, u->email) == 0) {
            fprintf(tempFile, "%s,%s,%s,%d,%d,%d,%d\n",
                    u->email, u->name, u->password,
                    u->calories, u->protein, u->carbs, u->fat);
        } else {
            fprintf(tempFile, "%s,%s,%s,%d,%d,%d,%d\n",
                    tempUser.email, tempUser.name, tempUser.password,
                    tempUser.calories, tempUser.protein,
                    tempUser.carbs, tempUser.fat);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("Users.txt");
    rename("temp.txt", "Users.txt");

    printf("Goals updated successfully!\n");
}

// ---------------- MAIN ----------------
int main() {
    struct User currentUser;
    struct Food food;
    int loggedIn = 0;
    char choice[10];

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
        }
    }

    while (1) {
        printf("\n1. Add Food\n2. View Today\n3. View Logs\n4. Update Goals\n5. Exit\nChoice: ");
        fgets(choice, sizeof(choice), stdin);

        if (choice[0] == '1') {
            if (findFood(&food)) {

                char date[20];
                getToday(date);

                FILE *logFile = fopen("logs.txt", "a");

                fprintf(logFile, "%s,%s,%s,%d,%d,%d,%d\n",
                        currentUser.email,
                        date,
                        food.name,
                        food.calories,
                        food.protein,
                        food.carbs,
                        food.fat);

                fclose(logFile);

                printf("Food logged successfully!\n");
            }
        } else if (choice[0] == '2') {
            viewToday(&currentUser);
        } else if (choice[0] == '3') {
            viewLogs(&currentUser);
        } else if (choice[0] == '4') {
            updateGoals(&currentUser);
        } else if (choice[0] == '5') {
            break;
        }
    }

    return 0;
}