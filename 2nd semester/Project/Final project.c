#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>


#define BOOK_FILE "books.txt"
#define BORROW_FILE "borrowed.txt"


void admin_login();
void save_members();
void load_members();
void add_members();
void display_members();
void delete_members();
void save_books();
void load_books();
void save_borrowed();
void load_borrowed();
void borrowed_history();
void add_books();
void search_books();
void admin_panel();
void borrow_books();
void return_books();
void user_login();
void user_panel();


void admin_login()
{
    char exact_username[] = "admin";
    char exact_password[] = "admin";

    char username[50];
    char password[50];
    char ch;
    int i = 0;
    int maxLength = sizeof(password);

    printf("Enter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    while (i < maxLength - 1)
    {
        ch = getch();

        if (ch == 13)
            break;
        else if (ch == 8 && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else if (ch >= 32 && ch <= 126)
        {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';
    printf("\n");

    if (strcmp(username, exact_username) == 0 && strcmp(password, exact_password) == 0)
    {
        printf("\nLogin successful!\n");
        admin_panel();
    }
    else
    {
        printf("\nIncorrect username or password! Access denied.\n");
    }
}


typedef struct member
{
    int id;
    char member_name [50];
    char member_email [100];
    struct member *next;
} member;

member *member_list=NULL;


void save_members()
{
    FILE *fp = fopen("members.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    member *temp = member_list;
    while (temp != NULL)
    {
        fprintf(fp, "%d,%s,%s\n", temp->id, temp->member_name, temp->member_email);
        temp = temp->next;
    }
    fclose(fp);
}


void load_members()
{
    FILE *fp = fopen("members.txt", "r");
    if (!fp) return;
    while (!feof(fp))
    {
        member *new_member = (member *)malloc(sizeof(member));
        if (fscanf(fp, "%d,%49[^,],%99[^\n]\n", &new_member->id, new_member->member_name, new_member->member_email) == 3)
        {
            new_member->next = member_list;
            member_list = new_member;
        }
        else
        {
            free(new_member);
        }
    }
    fclose(fp);
}


void add_members()
{
    member *new_member = (member *)malloc(sizeof(member));

    printf("Enter The ID: ");
    scanf("%d", &new_member->id);
    printf("Enter The Member Name: ");
    scanf(" %[^\n]s", new_member->member_name);
    printf("Enter the e-mail: ");
    scanf(" %[^\n]s", new_member->member_email);

    for (member *temp = member_list; temp != NULL; temp = temp->next)
    {
        if (temp->id == new_member->id || strcmp(temp->member_email, new_member->member_email) == 0)
        {
            printf("Member with ID %d or email '%s' already exists!\n", new_member->id, new_member->member_email);
            free(new_member);
            printf("\nDo You Want to Try Adding Another Member?\n1. Yes\n2. No\n");
            int choice;
            printf("Enter Your Choice: ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                add_members();
            }
            else
            {
                admin_panel();
            }
        }
    }

    new_member->next = member_list;
    member_list = new_member;

    printf("Member '%s' (ID: %d) added successfully!\n", new_member->member_name, new_member->id);
    save_members();

    printf("\nDo You Want to Add Another Member?\n1. Yes\n2. No\n");
    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if (a == 1)
    {
        add_members();
    }
    else
    {
        admin_panel();
    }
}


void display_members()
{
    if(member_list==NULL)
    {
        printf("There is no Members to Show\n");
        main();
    }

    printf("\n========== Member List ==========\n");

    member* temp = member_list;

    while (temp)
    {
        printf("ID: %d | Name: %s | Email: %s\n", temp->id, temp->member_name, temp->member_email);
        temp = temp->next;
    }

    admin_panel();
}

void delete_members()
{
    char find[50];
    printf("Enter the name you want to delete: ");
    scanf(" %[^\n]s", find);

    member *temp = member_list;
    member *prev = NULL;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->member_name, find) == 0)
        {
            if (prev == NULL)
            {
                member_list = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }

            printf("Member '%s' (ID: %d) deleted!\n", temp->member_name, temp->id);
            free(temp);
            save_members();
            found = 1;
            break;
        }

        prev = temp;
        temp = temp->next;
    }

    if (!found)
    {
        printf("No member named '%s' found.\n", find);
    }

    int choice;
    printf("\nDelete another member?\n1. Yes\n2. No\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        delete_members();
    }
    else
    {
        admin_panel();
    }
}


typedef struct book
{

    char name[50];
    char author[50];
    int id;
    struct book *next;
} book;

typedef struct borrow
{
    char name[50];
    char author[50];
    int id;
    struct borrow *next;
} borrow;

book* start=NULL;
borrow* head=NULL;

void save_books()
{
    FILE *fp = fopen("books.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    book *temp = start;
    while (temp != NULL)
    {
        fprintf(fp, "%d,%s,%s\n", temp->id, temp->name, temp->author);
        temp = temp->next;
    }
    fclose(fp);
}


void load_books()
{
    FILE *fp = fopen("books.txt", "r");
    if (!fp) return;
    while (!feof(fp))
    {
        book *new_book = (book *)malloc(sizeof(book));
        if (fscanf(fp, "%d,%49[^,],%49[^\n]\n", &new_book->id, new_book->name, new_book->author) == 3)
        {
            new_book->next = start;
            start = new_book;
        }
        else
        {
            free(new_book);
        }
    }
    fclose(fp);
}


void save_borrowed()
{
    FILE *fp = fopen(BORROW_FILE, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    borrow *temp = head;
    while (temp != NULL)
    {
        fprintf(fp, "%d,%s,%s\n", temp->id, temp->name, temp->author);
        temp = temp->next;
    }
    fclose(fp);
}


void load_borrowed()
{
    FILE *fp = fopen(BORROW_FILE, "r");
    if (!fp) return;
    while (!feof(fp))
    {
        borrow *new_borrow = (borrow *)malloc(sizeof(borrow));
        if (fscanf(fp, "%d,%49[^,],%49[^\n]\n", &new_borrow->id, new_borrow->name, new_borrow->author) == 3)
        {
            new_borrow->next = head;
            head = new_borrow;
        }
        else
        {
            free(new_borrow);
        }
    }
    fclose(fp);
}


void borrowed_history()
{
    if (head == NULL)
    {
        printf("There is no Borrowed Books to Show\n");
        admin_panel();
    }

    printf("\n========== Borrowed History ==========\n");

    borrow* temp = head;
    while (temp != NULL)
    {
        printf("ID: %d | Book Name: %s | Author: %s\n", temp->id, temp->name, temp->author);
        temp = temp->next;
    }

    admin_panel();
}



void add_books()
{
    book* newbook;
    newbook = (book*)malloc(sizeof(book));

    if (newbook == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &newbook->id);
    printf("Enter Book Name: ");
    scanf(" %[^\n]s", newbook->name);
    printf("Enter Author Name: ");
    scanf(" %[^\n]s", newbook->author);

    int duplicate = 0;

    for (book* temp = start; temp != NULL; temp = temp->next)
    {
        if (temp->id == newbook->id)
        {
            printf("A book with ID %d already exists!\n", newbook->id);
            duplicate = 1;
            break;
        }
        if (strcmp(temp->name, newbook->name) == 0)
        {
            printf("A book with the name '%s' already exists!\n", newbook->name);
            duplicate = 1;
            break;
        }
    }

    if (duplicate == 0)
    {
        newbook->next = start;
        start = newbook;
        printf("Book '%s' by %s (ID: %d) added successfully!\n", newbook->name, newbook->author, newbook->id);
        save_books();
    }

    printf("\n\n");

    printf("Do You Want to Add Another Book?\n");
    printf("1. Yes\n2. No\n");

    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if (a == 1)
    {
        add_books();
    }
    else
    {
        admin_panel();
    }
}



void search_books()
{
    char find[50];
    printf("Enter The Name of Book, You Want to See Details: ");
    scanf(" %[^\n]s", find);

    if (start == NULL)
    {
        printf("There is no Book to Show\n");
        admin_panel();
        return;
    }

    book* temp = start;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->name, find) == 0)
        {
            if (found == 0)
            {
                printf("\n========== Found Item ==========\n");
            }
            printf("ID: %d | Name: %s | Author: %s\n", temp->id, temp->name, temp->author);
            found = 1;
        }
        temp = temp->next;
    }

    if (found == 0)
    {
        printf("Book '%s' not found in the library.\n", find);
    }


    printf("\nDo You Want to Search Another Book?\n");
    printf("1. Yes\n2. No\n");

    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if (a == 1)
    {
        search_books();
    }
    else
    {
        printf("\n1. Admin Panel\n2. User Panel\n");
        int x;
        printf("Enter Your Choice: ");
        scanf("%d", &x);

        if (x == 1)
        {
            admin_panel();
        }
        else if (x == 2)
        {
            user_panel();
        }
    }
}


void admin_panel()
{
    printf("==============================\n");
    printf("=          Admin Panel       =\n");
    printf("==============================\n");
    printf("1. Add Members\n");
    printf("2. Delete Members\n");
    printf("3. Display Members\n");
    printf("4. Add Books\n");
    printf("5. Search Books\n");
    printf("6. View Borrowed Books\n");
    printf("7. Main Menu\n");

    int a;
    printf("Enter your Choice: ");
    scanf("%d", &a);

    if(a==1)
    {
        add_members();
    }
    if(a==2)
    {
        delete_members();
    }
    if(a==3)
    {
        display_members();
    }
    if(a==4)
    {
        add_books();
    }
    if(a==5)
    {
        search_books();
    }
    if(a==6)
    {
        borrowed_history();
    }
    if(a==7)
    {
        main();
    }
}


void borrow_books()
{
    if (start == NULL)
    {
        printf("No books available in the library to borrow.\n");
        user_panel();
    }

    char find[60];
    printf("Enter the name of the book you want to borrow: ");
    scanf(" %[^\n]s", find);

    book *temp = start;
    while (temp != NULL)
    {
        if (strcmp(temp->name, find) == 0)
        {
            borrow *new_borrow;
            new_borrow = (borrow*)malloc(sizeof(borrow));
            if (!new_borrow)
            {
                printf("Memory allocation failed!\n");
                user_panel();
            }

            new_borrow->id = temp->id;
            strcpy(new_borrow->name, temp->name);
            strcpy(new_borrow->author, temp->author);

            new_borrow->next = head;
            head = new_borrow;

            printf("You have borrowed the book: '%s' by %s (ID: %d)\n", temp->name, temp->author, temp->id);
            save_borrowed();
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Book '%s' not found in the library.\n", find);
    }

    printf("Do You Want to Borrow Another Book?\n");
    printf("1. Yes\n2. No\n");

    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if (a == 1)
    {
        borrow_books();
    }
    else
    {
        user_panel();
    }
}



void return_books()
{
    if (head == NULL)
    {
        printf("No borrowed books to return.\n");

        user_panel();
    }

    char find[60];
    printf("Enter the name of the book to return: ");
    scanf(" %[^\n]s", find);

    borrow *temp = head;
    borrow *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->name, find) == 0)
        {

            if (prev == NULL)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }

            free(temp);
            printf("The Book '%s' returned successfully.\n", find);
            save_borrowed();

            break;
        }
        prev = temp;
        temp = temp->next;
    }

    //printf("Book '%s' not found in borrowed books.\n", find);

    printf("Do You Want to Return Another Book\n");
    printf("1. Yes\n2. No\n");

    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if(a==1)
    {
        return_books();
    }
    if(a==2)
    {
        user_panel();
    }
}

void user_login()
{
    char create_username[50];
    char create_password[50];

    printf("Create a New Account\n");
    printf("---------------------\n");
    printf("Enter new username: ");
    scanf("%s", create_username);

    char ch;
    int i = 0;
    int maxLength = sizeof(create_password);

    printf("Enter new password: ");
    while (i < maxLength - 1)
    {
        ch = getch();
        if (ch == 13)
            break;
        else if (ch == 8 && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else if (ch >= 32 && ch <= 126)
        {
            create_password[i++] = ch;
            printf("*");
        }
    }
    create_password[i] = '\0';
    printf("\n\nAccount Created Successfully!\n");

    char username[50];
    char password[50];

    printf("\n====================\n");
    printf("=      Login       =\n");
    printf("====================\n");

    int attempts = 3;
    while (attempts--)
    {
        printf("Enter Username: ");
        scanf("%s", username);

        printf("Enter Password: ");
        i = 0;
        while (i < sizeof(password) - 1)
        {
            ch = getch();
            if (ch == 13)
                break;
            else if (ch == 8 && i > 0)
            {
                i--;
                printf("\b \b");
            }
            else if (ch >= 32 && ch <= 126)
            {
                password[i++] = ch;
                printf("*");
            }
        }
        password[i] = '\0';
        printf("\n");

        if (strcmp(username, create_username) == 0 && strcmp(password, create_password) == 0)
        {
            printf("\nLogin successful!\n");
            user_panel();
            break;
        }
        else
        {
            printf("\nIncorrect username or password. Attempts left: %d\n", attempts);
        }
    }

    if (attempts < 0)
    {
        printf("\nToo many failed attempts. Try again later.\n");
    }
}






void user_panel()
{
    printf("==============================\n");
    printf("=          User Panel        =\n");
    printf("==============================\n");
    printf("1. Borrow Books\n");
    printf("2. Return Books\n");
    printf("3. Search Books\n");
    printf("4. Main Menu\n");

    printf("Enter Your Choice: ");
    int a;
    scanf("%d", &a);

    if(a==1)
    {
        borrow_books();
    }
    if(a==2)
    {
        return_books();
    }
    if(a==3)
    {
        search_books();
    }
    if(a==4)
    {
        main();
    }

}

int main()
{

    load_members();
    load_books();
    load_borrowed();

    printf("**********Welcome To Our Project**********\n\n");
    printf("==============================\n");
    printf("  Library Management System\n");
    printf("==============================\n");
    printf("1. Admin Panel\n");
    printf("2. User Panel\n");
    printf("3. Exit Program\n");

    int input;
    printf("Enter Your Choice: ");
    scanf("%d", &input);
    if(input==1)
    {
        printf("1. Login\n");
        printf("2. Main menu\n");
        int a;
        printf("Enter Your Choice: ");
        scanf("%d", &a);
        if(a==1)
        {
            admin_login();
        }
        if(a==2)
        {
            main();
        }
    }

    if(input==2)
    {
        printf("1. Create Account\n");
        printf("2. Main Menu\n");

        int a;
        printf("Enter Your Choice: ");
        scanf("%d", &a);

        if(a==1)
        {
            user_login();
        }
        if(a==2)
        {
            main();
        }
    }

    if(input==3)
    {
        printf("\nExiting..........\n");
        return;
    }
    else
    {
        printf("Invalid Choice........\nProgram Exiting.......\n");
    }
}

