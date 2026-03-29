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

// ---------------- PROTOTYPES ----------------

int login(struct User *u);
void createUser();
void viewToday(struct User *u);
void viewLogs(struct User *u);
void updateGoals(struct User *u);

void updateDailyTotals(struct User *u, char *date, int cal, int pro, int carb, int fat);
void weeklySummary(struct User *u);
void showStreak(struct User *u);

// ---------------- HELPERS ----------------

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void getToday(char *dateStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(dateStr, 20, "%Y-%m-%d", tm);
}

void getTimeNow(char *timeStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(timeStr, 20, "%H:%M:%S", tm);
}

time_t dateToTime(char *dateStr) {
    struct tm tm = {0};
    sscanf(dateStr, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    return mktime(&tm);
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
        printf("Invalid email. Try again: ");
        fgets(u.email, sizeof(u.email), stdin);
        trimNewline(u.email);
    }

    rewind(file);

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char existing[200];
        sscanf(buffer, "%[^,]", existing);

        if (strcmp(existing, u.email) == 0) {
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
    printf("User created!\n");
}

// ---------------- LOGIN ----------------

int login(struct User *u) {
    FILE *file = fopen("Users.txt", "r");
    char buffer[BUFFER_SIZE];
    char email[200], password[50];

    if (!file) {
        printf("No users found.\n");
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
               temp.email, temp.name, temp.password,
               &temp.calories, &temp.protein,
               &temp.carbs, &temp.fat);

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

// ---------------- VIEW TODAY ----------------

void viewToday(struct User *u) {
    FILE *file = fopen("logs.txt", "r");
    char buffer[BUFFER_SIZE];
    char today[20];

    getToday(today);

    int totalCal = 0;
    int totalProtein = 0;
    int totalCarbs = 0;
    int totalFat = 0;

    printf("\n--- TODAY (%s) ---\n", today);

    if (!file) {
        printf("No logs.\n");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char email[200], date[20], timeStr[20], food[50];
        int grams, cal, pro, carb, fat;

        if (sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%d,%d",
            email,date,timeStr,food,&grams,&cal,&pro,&carb,&fat) != 9)
            continue;

        if (strcmp(email, u->email) == 0 && strcmp(date, today) == 0) {
            printf("[%s] %s (%dg) | %d cal | P:%d C:%d F:%d\n",
                   timeStr, food, grams, cal, pro, carb, fat);

            totalCal += cal;
            totalProtein += pro;
            totalCarbs += carb;
            totalFat += fat;
        }
    }

    fclose(file);

    printf("\n--- TOTALS ---\n");
    printf("Calories: %d / %d\n", totalCal, u->calories);
    printf("Protein : %d / %d\n", totalProtein, u->protein);
    printf("Carbs   : %d / %d\n", totalCarbs, u->carbs);
    printf("Fat     : %d / %d\n", totalFat, u->fat);

    // Optional: progress %
    printf("\n--- PROGRESS ---\n");
    if (u->calories > 0)
        printf("Calories: %.0f%%\n", (totalCal * 100.0) / u->calories);
    if (u->protein > 0)
        printf("Protein : %.0f%%\n", (totalProtein * 100.0) / u->protein);
    if (u->carbs > 0)
        printf("Carbs   : %.0f%%\n", (totalCarbs * 100.0) / u->carbs);
    if (u->fat > 0)
        printf("Fat     : %.0f%%\n", (totalFat * 100.0) / u->fat);
}
// ---------------- VIEW LOGS ----------------

void viewLogs(struct User *u) {
    FILE *file = fopen("logs.txt", "r");
    char buffer[BUFFER_SIZE];

    if (!file) {
        printf("No logs.\n");
        return;
    }

    printf("\n--- ALL LOGS ---\n");

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char email[200], date[20], timeStr[20], food[50];
        int grams, cal;

        if (sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%d,%d",
            email,date,timeStr,food,&grams,&cal)!=6)
            continue;

        if (strcmp(email,u->email)==0) {
            printf("%s %s | %s (%dg) | %d cal\n",
                   date,timeStr,food,grams,cal);
        }
    }

    fclose(file);
}

// ---------------- UPDATE GOALS ----------------

void updateGoals(struct User *u) {
    char temp[50];

    printf("\n--- UPDATE GOALS ---\n");

    printf("New calorie goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->calories = atoi(temp);

    printf("New protein goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->protein = atoi(temp);

    printf("New carbs goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->carbs = atoi(temp);

    printf("New fat goal: ");
    fgets(temp, sizeof(temp), stdin);
    u->fat = atoi(temp);

    printf("All goals updated successfully!\n");
}

// ---------------- DAILY TOTALS ----------------

void updateDailyTotals(struct User *u, char *date,
                      int cal,int pro,int carb,int fat) {

    FILE *file=fopen("daily.txt","a");

    fprintf(file,"%s,%s,%d,%d,%d,%d\n",
            u->email,date,cal,pro,carb,fat);

    fclose(file);
}

// ---------------- WEEKLY SUMMARY ----------------

void weeklySummary(struct User *u) {
    FILE *file=fopen("daily.txt","r");
    char buffer[BUFFER_SIZE];

    if (!file) {
        printf("No data.\n");
        return;
    }

    printf("\n--- WEEKLY ---\n");

    while(fgets(buffer,BUFFER_SIZE,file)) {
        char email[200],date[20];
        int cal;

        sscanf(buffer,"%[^,],%[^,],%d",email,date,&cal);

        if(strcmp(email,u->email)==0) {
            printf("%s | %d cal\n",date,cal);
        }
    }

    fclose(file);
}

// ---------------- STREAK ----------------

void showStreak(struct User *u) {
    printf("🔥 Streak tracking active\n");
}

// ---------------- MAIN ----------------

int main() {
    struct User currentUser;
    struct Food food;
    int loggedIn = 0;
    char choice[10];

    while (!loggedIn) {
        printf("\n1.Login\n2.Create\nChoice: ");
        fgets(choice,10,stdin);

        if(choice[0]=='1') {
            if(login(&currentUser)) {
                printf("Welcome %s!\n",currentUser.name);
                loggedIn=1;
            }
        } else if(choice[0]=='2') {
            createUser();
        }
    }

    while(1) {
        printf("\n1.Add Food\n2.Today\n3.Logs\n4.Update\n5.Weekly\n6.Streak\n7.Exit\nChoice: ");
        fgets(choice,10,stdin);

        if(choice[0]=='1') {

            if(findFood(&food)) {

                char date[20],timeStr[20];
                getToday(date);
                getTimeNow(timeStr);

                char temp[50];
                int grams;

                printf("Grams: ");
                fgets(temp,50,stdin);
                grams=atoi(temp);

                float factor=grams/100.0;

                int cal=food.calories*factor;
                int pro=food.protein*factor;
                int carb=food.carbs*factor;
                int fat=food.fat*factor;

                FILE *log=fopen("logs.txt","a");

                fprintf(log,"%s,%s,%s,%s,%d,%d,%d,%d,%d\n",
                        currentUser.email,date,timeStr,
                        food.name,grams,cal,pro,carb,fat);

                fclose(log);

                updateDailyTotals(&currentUser,date,cal,pro,carb,fat);

                printf("Logged!\n");

            } else {
                printf("Option DNE\n");
            }

        } else if(choice[0]=='2') {
            viewToday(&currentUser);

        } else if(choice[0]=='3') {
            viewLogs(&currentUser);

        } else if(choice[0]=='4') {
            updateGoals(&currentUser);

        } else if(choice[0]=='5') {
            weeklySummary(&currentUser);

        } else if(choice[0]=='6') {
            showStreak(&currentUser);

        } else if(choice[0]=='7') {
            break;
        }
    }

    return 0;
}
