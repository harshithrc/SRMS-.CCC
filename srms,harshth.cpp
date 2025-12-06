#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "student.txt"
#define CRE_FILE "credentials.txt"

char currentUser[50];
char currentRole[20];

void readLine(char *str, int size)
{
    if (fgets(str, size, stdin))
    {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
            str[len - 1] = '\0';
    }
    else str[0] = '\0';
}

void createDefaultCredentials()
{
    FILE *fp = fopen(CRE_FILE, "w");
    fprintf(fp, "admin admin123 admin\n");
    fprintf(fp, "staff staff123 staff\n");
    fprintf(fp, "guest guest123 guest\n");
    fclose(fp);
}

int login()
{
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp)
    {
        createDefaultCredentials();
        fp = fopen(CRE_FILE, "r");
    }

    printf("USERNAME: ");
    scanf("%49s", inUser);
    printf("PASSWORD: ");
    scanf("%49s", inPass);

    while (fscanf(fp, "%49s %49s %19s", u, p, r) == 3)
    {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0)
        {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addStudent()
{
    int roll;
    char name[50];
    float mark;

    printf("Roll: ");
    scanf("%d", &roll);
    getchar();
    printf("Name: ");
    readLine(name, sizeof(name));
    printf("Mark: ");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    fprintf(fp, "%d|%s|%.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student added!\n");
}

void displayStudents()
{
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp)
    {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("Roll\tName\t\tMark\n");
    printf("----\t----\t\t----\n");

    while (fscanf(fp, "%d|%49[^|]|%f\n", &roll, name, &mark) == 3)
    {
        printf("%d\t%s\t\t%.2f\n", roll, name, mark);
    }
    fclose(fp);
}

void searchStudent()
{
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp)
    {
        printf("No student file!\n");
        return;
    }

    while (fscanf(fp, "%d|%49[^|]|%f\n", &roll, name, &mark) == 3)
    {
        if (roll == find)
        {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

void deleteStudent()
{
    int delRoll;
    printf("Enter roll to delete: ");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp)
    {
        printf("No student file!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d|%49[^|]|%f\n", &roll, name, &mark) == 3)
    {
        if (roll != delRoll)
            fprintf(temp, "%d|%s|%.2f\n", roll, name, mark);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found)
        printf("Student deleted!\n");
    else
        printf("Roll not found!\n");
}

void updateStudent()
{
    int updateRoll;
    printf("Enter roll to update: ");
    scanf("%d", &updateRoll);
    getchar();

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp)
    {
        printf("No student file!\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d|%49[^|]|%f\n", &roll, name, &mark) == 3)
    {
        if (roll == updateRoll)
        {
            found = 1;
            char newName[50];
            float newMark;

            printf("New Name: ");
            readLine(newName, sizeof(newName));
            printf("New Mark: ");
            scanf("%f", &newMark);

            fprintf(temp, "%d|%s|%.2f\n", roll, newName, newMark);
        }
        else
        {
            fprintf(temp, "%d|%s|%.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found)
        printf("Student updated!\n");
    else
        printf("Roll not found!\n");
}

void adminMenu()
{
    int c;
    while (1)
    {
        printf("\nADMIN MENU\n1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Logout\nEnter: ");
        scanf("%d", &c);

        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else return;
    }
}

void staffMenu()
{
    int c;
    while (1)
    {
        printf("\nSTAFF MENU\n1.Add\n2.Display\n3.Search\n4.Update\n5.Logout\nEnter: ");
        scanf("%d", &c);

        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else return;
    }
}

void guestMenu()
{
    int c;
    while (1)
    {
        printf("\nGUEST MENU\n1.Display\n2.Search\n3.Logout\nEnter: ");
        scanf("%d", &c);

        if (c == 1) displayStudents();
        else if (c == 2) searchStudent();
        else return;
    }
}

int main()
{
    if (!login())
    {
        printf("Invalid login!\n");
        return 0;
    }

    printf("Logged in as: %s\n", currentRole);

    if (strcmp(currentRole, "admin") == 0)
        adminMenu();
    else if (strcmp(currentRole, "staff") == 0)
        staffMenu();
    else
        guestMenu();

    return 0;
}
