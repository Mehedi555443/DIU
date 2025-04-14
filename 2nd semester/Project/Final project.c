#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

#define BOOK_FILE "books.txt"
#define BORROW_FILE "borrowed.txt"
#define MEMBER_FILE "members.txt"
#define USER_CREDENTIALS_FILE "user_credentials.txt"

// Admin Functions
void admin_login();
void admin_panel();

// Member Management
void add_members();
void display_members();
void delete_members();
void save_members();
void load_members();

// Book Management
void add_books();
void search_books();
void save_books();
void load_books();

// Borrow/Return Management
void borrow_books();
void return_books();
void borrowed_history();
void save_borrowed();
void load_borrowed();

// User Account
void create_user_account();
void user_login();
void user_panel();

// System Functions
void initialize_system();
void display_welcome_message();
void display_main_menu();
void handle_admin_selection();
void handle_user_selection();
void exit_program();

// Main
int main();

typedef struct member
{
    int id;
    char member_name[50];
    char member_email[100];
    struct member *next;
} member;

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

member *member_list = NULL;
book *start = NULL;
borrow *head = NULL;
char stored_username[50] = "";
char stored_password[50] = "";

void admin_login()
{
    char exact_username[] = "admin";
    char exact_password[] = "admin";

    char username[50];
    char password[50];
    char ch;
    int i = 0;

    printf("Enter Username: ");
    scanf("%49s", username);

    printf("Enter Password: ");
    while (i < 49)
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
        main();
    }
}

void admin_panel()
{
    printf("\n==============================\n");
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

    switch (a)
    {
    case 1:
        add_members();
        break;
    case 2:
        delete_members();
        break;
    case 3:
        display_members();
        break;
    case 4:
        add_books();
        break;
    case 5:
        search_books();
        break;
    case 6:
        borrowed_history();
        break;
    case 7:
        main();
        break;
    default:
        printf("Invalid choice!\n");
        admin_panel();
    }
}

void add_members()
{
    member *new_member = (member *)malloc(sizeof(member));
    if (new_member == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter The ID: ");
    scanf("%d", &new_member->id);
    printf("Enter The Member Name: ");
    scanf(" %49[^\n]", new_member->member_name);
    printf("Enter the e-mail: ");
    scanf(" %99[^\n]", new_member->member_email);

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
            return;
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
    if (member_list == NULL)
    {
        printf("There are no Members to Show\n");
        admin_panel();
        return;
    }

    printf("\n========== Member List ==========\n");

    member* temp = member_list;

    while (temp != NULL)
    {
        printf("ID: %d | Name: %s | Email: %s\n", temp->id, temp->member_name, temp->member_email);
        temp = temp->next;
    }

    admin_panel();
}

void delete_members()
{
    if (member_list == NULL)
    {
        printf("No members to delete.\n");
        admin_panel();
        return;
    }

    char find[50];
    printf("Enter the name you want to delete: ");
    scanf(" %49[^\n]", find);

    member *temp = member_list;
    member *prev = NULL;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->member_name, find) == 0)
        {
            found = 1;
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

void save_members()
{
    FILE *fp = fopen(MEMBER_FILE, "w");
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
    FILE *fp = fopen(MEMBER_FILE, "r");
    if (!fp) return;

    member *current = member_list;
    while (current != NULL)
    {
        member *temp = current;
        current = current->next;
        free(temp);
    }
    member_list = NULL;

    while (1)
    {
        member *new_member = (member *)malloc(sizeof(member));
        if (fscanf(fp, "%d,%49[^,],%99[^\n]\n", &new_member->id, new_member->member_name, new_member->member_email) != 3)
        {
            free(new_member);
            break;
        }
        new_member->next = member_list;
        member_list = new_member;
    }
    fclose(fp);
}

void add_books()
{
    book* newbook = (book*)malloc(sizeof(book));
    if (newbook == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &newbook->id);
    printf("Enter Book Name: ");
    scanf(" %49[^\n]", newbook->name);
    printf("Enter Author Name: ");
    scanf(" %49[^\n]", newbook->author);

    book* temp = start;
    while (temp != NULL)
    {
        if (temp->id == newbook->id)
        {
            printf("A book with ID %d already exists!\n", newbook->id);
            free(newbook);
            goto add_another;
        }
        if (strcmp(temp->name, newbook->name) == 0)
        {
            printf("A book with the name '%s' already exists!\n", newbook->name);
            free(newbook);
            goto add_another;
        }
        temp = temp->next;
    }

    newbook->next = start;
    start = newbook;
    printf("Book '%s' by %s (ID: %d) added successfully!\n", newbook->name, newbook->author, newbook->id);
    save_books();

add_another:
    printf("\nDo You Want to Add Another Book?\n");
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
    if (start == NULL)
    {
        printf("There are no books in the library.\n");
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
        return;
    }

    char find[50];
    printf("Enter The Name of Book, You Want to See Details: ");
    scanf(" %49[^\n]", find);

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

void save_books()
{
    FILE *fp = fopen(BOOK_FILE, "w");
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
    FILE *fp = fopen(BOOK_FILE, "r");
    if (!fp) return;

    book *current = start;
    while (current != NULL)
    {
        book *temp = current;
        current = current->next;
        free(temp);
    }
    start = NULL;

    while (1)
    {
        book *new_book = (book *)malloc(sizeof(book));
        if (fscanf(fp, "%d,%49[^,],%49[^\n]\n", &new_book->id, new_book->name, new_book->author) != 3)
        {
            free(new_book);
            break;
        }
        new_book->next = start;
        start = new_book;
    }
    fclose(fp);
}

void borrow_books()
{
    if (start == NULL)
    {
        printf("No books available in the library to borrow.\n");
        user_panel();
        return;
    }

    char find[60];
    printf("Enter the name of the book you want to borrow: ");
    scanf(" %59[^\n]", find);

    book *temp = start;
    book *prev = NULL;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->name, find) == 0)
        {
            found = 1;

            borrow *b_temp = head;
            while (b_temp != NULL)
            {
                if (b_temp->id == temp->id)
                {
                    printf("This book is already borrowed!\n");
                    user_panel();
                    return;
                }
                b_temp = b_temp->next;
            }

            borrow *new_borrow = (borrow*)malloc(sizeof(borrow));
            if (!new_borrow)
            {
                printf("Memory allocation failed!\n");
                user_panel();
                return;
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
        prev = temp;
        temp = temp->next;
    }

    if (!found)
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
        return;
    }

    char find[60];
    printf("Enter the name of the book to return: ");
    scanf(" %59[^\n]", find);

    borrow *temp = head;
    borrow *prev = NULL;
    int found = 0;

    while (temp != NULL)
    {
        if (strcmp(temp->name, find) == 0)
        {
            found = 1;
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

    if (!found)
    {
        printf("Book '%s' not found in borrowed books.\n", find);
    }

    printf("Do You Want to Return Another Book\n");
    printf("1. Yes\n2. No\n");

    int a;
    printf("Enter Your Choice: ");
    scanf("%d", &a);

    if (a == 1)
    {
        return_books();
    }
    else
    {
        user_panel();
    }
}

void borrowed_history()
{
    if (head == NULL)
    {
        printf("There are no Borrowed Books to Show\n");
        admin_panel();
        return;
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

    borrow *current = head;
    while (current != NULL)
    {
        borrow *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;

    while (1)
    {
        borrow *new_borrow = (borrow *)malloc(sizeof(borrow));
        if (fscanf(fp, "%d,%49[^,],%49[^\n]\n", &new_borrow->id, new_borrow->name, new_borrow->author) != 3)
        {
            free(new_borrow);
            break;
        }
        new_borrow->next = head;
        head = new_borrow;
    }
    fclose(fp);
}

void create_user_account()
{
    FILE *fp = fopen(USER_CREDENTIALS_FILE, "a+");
    if (fp == NULL)
    {
        printf("Error: Cannot open credentials file!\n");
        return;
    }

    printf("Create a New Account\n");
    printf("---------------------\n");

    char username[50];
    char password[50];
    char ch;
    int i = 0;

    printf("Enter new username: ");
    scanf("%49s", username);

    rewind(fp);
    char line[150];
    int username_exists = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char saved_username[50];
        if (sscanf(line, "%49[^,]", saved_username) == 1)
        {
            if (strcmp(saved_username, username) == 0)
            {
                username_exists = 1;
                break;
            }
        }
    }

    if (username_exists)
    {
        printf("Username already exists! Please choose another.\n");
        fclose(fp);
        printf("\n1. Try again\n2. Back to main menu\n");
        int choice;
        printf("Enter Your Choice: ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            create_user_account();
        }
        else
        {
            main();
        }
        return;
    }

    printf("Enter new password: ");
    i = 0;
    while (i < 49)
    {
        ch = getch();
        if (ch == 13) break;
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

    fprintf(fp, "%s,%s\n", username, password);
    fclose(fp);

    strcpy(stored_username, username);
    strcpy(stored_password, password);

    printf("\n\nAccount Created Successfully!\n");
    user_login();
}

void user_login()
{
    if (strlen(stored_username) == 0)
    {
        FILE *fp = fopen(USER_CREDENTIALS_FILE, "r");
        if (fp == NULL)
        {
            printf("No account exists. Please create an account first.\n");
            create_user_account();
            return;
        }

        char line[150];
        if (fgets(line, sizeof(line), fp))
        {
            char saved_username[50], saved_password[50];
            if (sscanf(line, "%49[^,],%49[^\n]", saved_username, saved_password) == 2)
            {
                strcpy(stored_username, saved_username);
                strcpy(stored_password, saved_password);
            }
        }
        fclose(fp);
    }

    char username[50];
    char password[50];

    printf("\n====================\n");
    printf("=      Login       =\n");
    printf("====================\n");

    int attempts = 3;
    while (attempts > 0)
    {
        printf("Enter Username: ");
        scanf("%49s", username);

        printf("Enter Password: ");
        int i = 0;
        char ch;
        while (i < 49)
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

        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0)
        {
            printf("\nLogin successful!\n");
            user_panel();
            return;
        }
        else
        {
            attempts--;
            printf("\nIncorrect username or password. Attempts left: %d\n", attempts);
        }
    }

    if (attempts == 0)
    {
        printf("\nToo many failed attempts. Try again later.\n");
        main();
    }
}

void user_panel()
{
    printf("\n==============================\n");
    printf("=          User Panel        =\n");
    printf("==============================\n");
    printf("1. Borrow Books\n");
    printf("2. Return Books\n");
    printf("3. Search Books\n");
    printf("4. Main Menu\n");

    printf("Enter Your Choice: ");
    int a;
    scanf("%d", &a);

    switch (a)
    {
    case 1:
        borrow_books();
        break;
    case 2:
        return_books();
        break;
    case 3:
        search_books();
        break;
    case 4:
        main();
        break;
    default:
        printf("Invalid choice!\n");
        user_panel();
    }
}

void initialize_system()
{
    load_members();
    load_books();
    load_borrowed();
}

void display_welcome_message()
{
    printf("\n**********Welcome To Our Project**********\n\n");
    printf("==============================\n");
    printf("  Library Management System\n");
    printf("==============================\n");
}

void display_main_menu()
{
    printf("1. Admin Panel\n");
    printf("2. User Panel\n");
    printf("3. Exit Program\n");
    printf("Enter Your Choice: ");
}

void handle_admin_selection()
{
    printf("\n1. Login\n2. Main menu\nEnter Your Choice: ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        admin_login();
    }
    else
    {
        main();
    }
}

void handle_user_selection()
{
    printf("\n1. Create Account\n2. User Login\n3. Main Menu\nEnter Your Choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        create_user_account();
    }
    else if (choice == 2)
    {
        user_login();
    }
    else
    {
        main();
    }
}

void exit_program()
{
    printf("\nExiting..........\n");
    exit(0);
}

int main()
{
    initialize_system();

    while (1)
    {
        display_welcome_message();
        display_main_menu();

        int input;
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            handle_admin_selection();
            break;
        case 2:
            handle_user_selection();
            break;
        case 3:
            exit_program();
        default:
            printf("Invalid Choice! Try again.\n");
            continue;
        }
    }

    return 0;
}
