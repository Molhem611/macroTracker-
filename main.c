#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define NAME_SIZE 100

// ---------------- STRUCTS ----------------
struct User
{
    char email[200];
    char name[50];
    char password[12];
    int calories;
    int protein;
    int carbs;
    int fat;
};
// Food struct for future use, not currently used in the program
struct Food
{
    char name[50];
    int calories;
    int protein;
    int carbs;
    int fat;
};


// ---------------- FIND FOOD ----------------
int findFood()
{
    FILE *file_ptr;
    char buffer[BUFFER_SIZE];
    char search_name[NAME_SIZE];
    char filename[] = "Foods.txt";

    printf("Food name: ");
    if (fgets(search_name, NAME_SIZE, stdin) == NULL)
    {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }

    search_name[strcspn(search_name, "\n")] = 0;

    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Searching for \"%s\" in %s...\n\n", search_name, filename);

    int found = 0;

    while (fgets(buffer, BUFFER_SIZE, file_ptr) != NULL)
    {
        if (strstr(buffer, search_name) != NULL)
        {
            printf("Match found: %s", buffer);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("%s does not exist in file\n", search_name);
    }

    fclose(file_ptr);
    return EXIT_SUCCESS;
}

// ---------------- CREATE USER ----------------
void createUser()
{
    struct User p;
    FILE *file_ptr;
    printf("Enter email: ");
    fgets(p.email, sizeof(p.email), stdin);
    p.email[strcspn(p.email, "\n")] = 0;

    char buffer[BUFFER_SIZE];
    char search_name[NAME_SIZE];
    char filename[] = "Users.txt";
    int found = 0;

    p.email[strcspn(p.email, "\n")] = 0;

    file_ptr = fopen(filename, "r");
    printf("Searching for \"%s\" in %s...\n", p.email, filename);
    printf("Not found: %s does not exist in file\n\n", search_name);

    char file_name[50];

    while (fgets(buffer, BUFFER_SIZE, file_ptr) != NULL)
    {
        sscanf(buffer, "%[^,]", file_name);

        if (strcmp(file_name, p.email) == 0)
        {
            printf("User already exists: %s\n", buffer);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("Enter name: ");
        fgets(p.name, sizeof(p.name), stdin);
        p.name[strcspn(p.name, "\n")] = 0;

        printf("Enter password: ");
        fgets(p.password, sizeof(p.password), stdin);
        p.password[strcspn(p.password, "\n")] = 0;

        printf("What is your Calories goal? ");
        scanf("%d", &p.calories);

        printf("What is your Protein goal? ");
        scanf("%d", &p.protein);

        printf("What is your Carb goal? ");
        scanf("%d", &p.carbs);

        printf("What is your Fat goal? ");
        scanf("%d", &p.fat);

        // clear buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        file_ptr = fopen("Users.txt", "a");

        if (file_ptr == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        fprintf(file_ptr, "%s, %s, %s, %d, %d, %d, %d\n",
                p.email, p.name, p.password,
                p.calories, p.protein, p.carbs, p.fat);

        fclose(file_ptr);

        printf("User saved successfully!\n");
    }
}

// ---------------- MAIN ----------------
int main()
{
    int choice;

    createUser();

    while (1)
    {
        printf("\n1. Add Food\n2. View Today\n3. View Logs\n4. Exit\nChoice: ");
        scanf("%d", &choice);

        // clear buffer BEFORE fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        if (choice == 1)
        {
            findFood();
        }
        else if (choice == 2)
        {
            printf("View Today not implemented yet\n");
        }
        else if (choice == 3)
        {
            printf("View Logs not implemented yet\n");
        }
        else if (choice == 4)
        {
            printf("Goodbye!\n");
            break;
        }
        else
        {
            printf("Not a valid option!\n");
        }
    }

    return 0;
}