#include <stdio.h>
#include <time.h>
#include <string.h>

struct Class {
    char day[20];
    char subject[50];
    char type[20];
};

void greet_user();
void print_routine(struct Class routine[], int size);
void display_day_routine(struct Class routine[], int size);
void count_classes_per_week(struct Class routine[], int size);
void average_classes_per_day(struct Class routine[], int size);
void modify_routine(struct Class routine[], int *size);

int main() {
    greet_user();
    struct Class routine[] = {
        {"Monday", "Mathematics", "Theoretical"},
        {"Monday", "Programming", "Practical"},
        {"Tuesday", "Physics", "Theoretical"},
        {"Tuesday", "Computer Architecture", "Practical"},
        {"Wednesday", "English", "Theoretical"},
        {"Wednesday", "Electronics", "Theoretical"},
        {"Thursday", "IPR Law", "Theoretical"},
        {"Thursday", "Operating Systems", "Practical"},
        {"Friday", "Database Management Systems", "Theoretical"},
        {"Friday", "Computer Networks in C", "Practical"}
    };
    int size = sizeof(routine) / sizeof(routine[0]);
    int choice;
    do {
        printf("\nWelcome to Class Routine Management System\n");
        printf("1. Print entire routine\n");
        printf("2. Display routine for a specific day\n");
        printf("3. Count number of theoretical and practical classes per week\n");
        printf("4. Calculate average number of classes per day\n");
        printf("5. Modify routine\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                print_routine(routine, size);
                break;
            case 2:
                display_day_routine(routine, size);
                break;
            case 3:
                count_classes_per_week(routine, size);
                break;
            case 4:
                average_classes_per_day(routine, size);
                break;
            case 5:
                modify_routine(routine, &size);
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);
    return 0;
}

void greet_user() {
    time_t current_time = time(NULL);
    struct tm *timeinfo = localtime(&current_time);
    int hour = timeinfo->tm_hour;
   
    if (hour < 12)
        printf("Good morning!\n");
    else if (hour < 18)
        printf("Good afternoon!\n");
    else
        printf("Good evening!\n");
}

void print_routine(struct Class routine[], int size) {
    printf("Class Routine:\n");
    for (int i = 0; i < size; i++) {
        printf("%s: %s (%s)\n", routine[i].day, routine[i].subject, routine[i].type);
    }
}

void display_day_routine(struct Class routine[], int size) {
    char day[20];
    printf("Enter the day (e.g., Monday, Tuesday): ");
    scanf("%s", day);

    printf("Routine for %s:\n", day);
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(routine[i].day, day) == 0) {
            printf("%s: %s (%s)\n", routine[i].day, routine[i].subject, routine[i].type);
            found = 1;
        }
    }
    if (!found)
        printf("No classes scheduled for %s\n", day);
}

void count_classes_per_week(struct Class routine[], int size) {
    int theoretical = 0, practical = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(routine[i].type, "Theoretical") == 0)
            theoretical++;
        else if (strcmp(routine[i].type, "Practical") == 0)
            practical++;
    }
    printf("Number of theoretical classes per week: %d\n", theoretical);
    printf("Number of practical classes per week: %d\n", practical);
}

void average_classes_per_day(struct Class routine[], int size) {
    int days = 0;
    char prev_day[20] = "";
    for (int i = 0; i < size; i++) {
        if (strcmp(routine[i].day, prev_day) != 0) {
            days++;
            strcpy(prev_day, routine[i].day);
        }
    }
    printf("Average number of classes per day: %d\n", size / days);
}

void modify_routine(struct Class routine[], int *size) {
    printf("Enter the new class routine in the format 'Day: Subject (Type)'\n");
    printf("For example: Monday: Mathematics (Theoretical)\n");
    printf("Enter 'done' to finish.\n");
    char input[100];
    while (1) {
        scanf("%s", input);
        if (strcmp(input, "done") == 0)
            break;
        sscanf(input, "%s %[^()] (%[^)])", routine[*size].day, routine[*size].subject, routine[*size].type);
        (*size)++;
    }
}

	

