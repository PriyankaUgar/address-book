#ifndef CONTACT_H
#define CONTACT_H
#define RESET       "\033[0m"

#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define CYAN        "\033[36m"

#define BOLD        "\033[1m"

#define GREEN_BOLD  "\033[1;32m"
#define RED_BOLD    "\033[1;31m"
#define YELLOW_BOLD "\033[1;33m"
#define BLUE_BOLD   "\033[1;34m"

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);

#endif
