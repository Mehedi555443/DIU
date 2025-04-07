#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void admin_login();
void admin_panel();
void add_members();
void display_members();
void delete_members();
void add_books();
void search_books();
void borrow_books();
void return_books();
void view_borrowed_books();
void user_login();
void user_panel();
void save_data();
void load_data();
int main();

#define MEMBERS_FILE "members.dat"
#define BOOKS_FILE "books.dat"
#define BORROWED_FILE "borrowed.dat"

typedef struct member {
    int id;
    char member_name[50];
    char member_email[100];
    struct member *next;
} member;

member *member_list = NULL;

typedef struct book {
    char name[50];
    char author[50];
    int id;
    struct book *next;
} book;

typedef struct borrow {
    char name[50];
    char author[50];
    int id;
    struct borrow *next;
} borrow;

book* start = NULL;
borrow* head = NULL;

void save_data() {
    FILE *fp = fopen(MEMBERS_FILE, "wb");
    if (fp) {
        member *temp = member_list;
        while (temp) {
            fwrite(temp, sizeof(member), 1, fp);
            temp = temp->next;
        }
        fclose(fp);
    }

    fp = fopen(BOOKS_FILE, "wb");
    if (fp) {
        book *temp = start;
        while (temp) {
            fwrite(temp, sizeof(book), 1, fp);
            temp = temp->next;
        }
        fclose(fp);
    }

    fp = fopen(BORROWED_FILE, "wb");
    if (fp) {
        borrow *temp = head;
        while (temp) {
            fwrite(temp, sizeof(borrow), 1, fp);
            temp = temp->next;
        }
        fclose(fp);
    }
}

void load_data() {
    FILE *fp = fopen(MEMBERS_FILE, "rb");
    if (fp) {
        member m;
        while (fread(&m, sizeof(member), 1, fp)) {
            member *new_member = (member*)malloc(sizeof(member));
            *new_member = m;
            new_member->next = member_list;
            member_list = new_member;
        }
        fclose(fp);
    }

    fp = fopen(BOOKS_FILE, "rb");
    if (fp) {
        book b;
        while (fread(&b, sizeof(book), 1, fp)) {
            book *new_book = (book*)malloc(sizeof(book));
            *new_book = b;
            new_book->next = start;
            start = new_book;
        }
        fclose(fp);
    }

    fp = fopen(BORROWED_FILE, "rb");
    if (fp) {
        borrow br;
        while (fread(&br, sizeof(borrow), 1, fp)) {
            borrow *new_borrow = (borrow*)malloc(sizeof(borrow));
            *new_borrow = br;
            new_borrow->next = head;
            head = new_borrow;
        }
        fclose(fp);
    }
}

void admin_login() {
    char exact_username[] = "admin";
    char exact_password[] = "admin";

    char username[50];
    char password[50];

    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    if (strcmp(username, exact_username) == 0 && strcmp(password, exact_password) == 0) {
        printf("\nLogin successful!\n");
        admin_panel();
    } else {
        printf("\nIncorrect credentials! Access denied.\n");
        main();
    }
}

void add_members() {
    member *new_member = (member*)malloc(sizeof(member));
    if (!new_member) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter The ID: ");
    scanf("%d", &new_member->id);
    printf("Enter The Member Name: ");
    scanf(" %[^\n]", new_member->member_name);
    printf("Enter the e-mail: ");
    scanf(" %[^\n]", new_member->member_email);

    new_member->next = member_list;
    member_list = new_member;

    printf("Member '%s' (ID: %d) added successfully!\n",
           new_member->member_name, new_member->id);
    save_data();
}

void display_members() {
    if (member_list == NULL) {
        printf("There are no Members to Show\n");
        return;
    }

    printf("\n========== Member List ==========\n");
    member* temp = member_list;
    while (temp) {
        printf("ID: %d | Name: %s | Email: %s\n",
               temp->id, temp->member_name, temp->member_email);
        temp = temp->next;
    }
}

void delete_members() {
    if (member_list == NULL) {
        printf("No members to delete.\n");
        return;
    }

    char find[50];
    printf("Enter the name you want to delete: ");
    scanf(" %[^\n]", find);

    member *temp = member_list;
    member *prev = NULL;
    int found = 0;

    while (temp != NULL) {
        if (strcmp(temp->member_name, find) == 0) {
            found = 1;
            if (prev == NULL) {
                member_list = temp->next;
            } else {
                prev->next = temp->next;
            }

            printf("Member '%s' (ID: %d) deleted successfully!\n",
                   temp->member_name, temp->id);
            free(temp);
            save_data();
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found) {
        printf("Member '%s' not found.\n", find);
    }
}

void add_books() {
    book* newbook = (book*)malloc(sizeof(book));
    if (!newbook) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &newbook->id);
    printf("Enter Book Name: ");
    scanf(" %[^\n]", newbook->name);
    printf("Enter Author Name: ");
    scanf(" %[^\n]", newbook->author);

    newbook->next = start;
    start = newbook;
    printf("Book '%s' by %s (ID: %d) added successfully!\n",
           newbook->name, newbook->author, newbook->id);
    save_data();
}

void search_books() {
    if (start == NULL) {
        printf("There are no books to show\n");
        return;
    }

    char find[50];
    printf("Enter The Name of Book You Want to Search: ");
    scanf(" %[^\n]", find);

    printf("\n========== Search Results ==========\n");
    book* temp = start;
    int found = 0;

    while (temp != NULL) {
        if (strstr(temp->name, find) != NULL) {
            printf("ID: %d | Name: %s | Author: %s\n",
                   temp->id, temp->name, temp->author);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No books found matching '%s'\n", find);
    }
}

void borrow_books() {
    if (start == NULL) {
        printf("No books available to borrow.\n");
        return;
    }

    char find[60];
    printf("Enter the name of the book you want to borrow: ");
    scanf(" %[^\n]", find);

    book *temp = start;
    while (temp != NULL) {
        if (strcmp(temp->name, find) == 0) {
            borrow *new_borrow = (borrow*)malloc(sizeof(borrow));
            if (!new_borrow) {
                printf("Memory allocation failed!\n");
                return;
            }

            new_borrow->id = temp->id;
            strcpy(new_borrow->name, temp->name);
            strcpy(new_borrow->author, temp->author);
            new_borrow->next = head;
            head = new_borrow;

            printf("You have borrowed: '%s' by %s (ID: %d)\n",
                   temp->name, temp->author, temp->id);
            save_data();
            return;
        }
        temp = temp->next;
    }
    printf("Book '%s' not found.\n", find);
}

void return_books() {
    if (head == NULL) {
        printf("No books to return.\n");
        return;
    }

    char find[60];
    printf("Enter the name of the book to return: ");
    scanf(" %[^\n]", find);

    borrow *temp = head;
    borrow *prev = NULL;
    int found = 0;

    while (temp != NULL) {
        if (strcmp(temp->name, find) == 0) {
            found = 1;
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            printf("Book '%s' returned successfully.\n", temp->name);
            free(temp);
            save_data();
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found) {
        printf("You haven't borrowed '%s'.\n", find);
    }
}

void view_borrowed_books() {
    if (head == NULL) {
        printf("No books currently borrowed.\n");
        return;
    }

    printf("\n========== Borrowed Books ==========\n");
    borrow* temp = head;
    while (temp != NULL) {
        printf("ID: %d | Name: %s | Author: %s\n",
               temp->id, temp->name, temp->author);
        temp = temp->next;
    }
}

void admin_panel() {
    int choice;
    do {
        printf("\n==============================\n");
        printf("=          Admin Panel       =\n");
        printf("==============================\n");
        printf("1. Add Members\n");
        printf("2. Delete Members\n");
        printf("3. Display Members\n");
        printf("4. Add Books\n");
        printf("5. Search Books\n");
        printf("6. Main Menu\n");
        printf("Enter your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_members(); break;
            case 2: delete_members(); break;
            case 3: display_members(); break;
            case 4: add_books(); break;
            case 5: search_books(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

void user_panel() {
    int choice;
    do {
        printf("\n==============================\n");
        printf("=          User Panel        =\n");
        printf("==============================\n");
        printf("1. Borrow Books\n");
        printf("2. Return Books\n");
        printf("3. View Borrowed Books\n");
        printf("4. Search Books\n");
        printf("5. Main Menu\n");
        printf("Enter your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: borrow_books(); break;
            case 2: return_books(); break;
            case 3: view_borrowed_books(); break;
            case 4: search_books(); break;
            case 5: return;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);
}

void user_login() {
    char create_username[50];
    char create_password[50];

    printf("Enter new username: ");
    scanf("%s", create_username);
    printf("Enter new password: ");
    scanf("%s", create_password);

    printf("Account Created Successfully!\n");

    char username[50];
    char password[50];
    int attempts = 3;

    printf("====================\n");
    printf("=      Login       =\n");
    printf("====================\n");

    while (attempts > 0) {
        printf("Enter Username: ");
        scanf("%s", username);
        printf("Enter Password: ");
        scanf("%s", password);

        if (strcmp(username, create_username) == 0 &&
            strcmp(password, create_password) == 0) {
            printf("\nLogin successful!\n");
            user_panel();
            return;
        } else {
            attempts--;
            printf("\nIncorrect credentials! %d attempts remaining.\n", attempts);
        }
    }
    printf("Login failed. Returning to main menu.\n");
}

int main() {
    load_data();

    int input;
    do {
        printf("\n==============================\n");
        printf("  Library Management System\n");
        printf("==============================\n");
        printf("1. Admin Panel\n");
        printf("2. User Panel\n");
        printf("3. Exit Program\n");
        printf("Enter Your Choice: ");
        scanf("%d", &input);

        switch (input) {
            case 1:
                printf("1. Login\n");
                printf("2. Main menu\n");
                printf("Enter Your Choice: ");
                scanf("%d", &input);
                if (input == 1) admin_login();
                break;
            case 2:
                printf("1. Create Account\n");
                printf("2. Main Menu\n");
                printf("Enter Your Choice: ");
                scanf("%d", &input);
                if (input == 1) user_login();
                break;
            case 3:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (input != 3);

    member *m_temp;
    while (member_list != NULL) {
        m_temp = member_list;
        member_list = member_list->next;
        free(m_temp);
    }

    book *b_temp;
    while (start != NULL) {
        b_temp = start;
        start = start->next;
        free(b_temp);
    }

    borrow *br_temp;
    while (head != NULL) {
        br_temp = head;
        head = head->next;
        free(br_temp);
    }

    return 0;
}
