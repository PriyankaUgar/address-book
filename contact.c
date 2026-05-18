#include <stdio.h>          // Standard input/output library
#include <stdlib.h>         // Standard library (exit, memory functions)
#include <ctype.h>          // Character handling functions (isdigit, isalpha, tolower)
#include <string.h>         // String handling functions (strcmp, strcpy, strstr)
#include "contact.h"        // Custom header for Contact and AddressBook structure
#include "file.h"           // File handling functions (save/load contacts)



/* ================================== FUNCTION PROTOTYPES ========================================== */
// Declaring search functions before use
void search_by_name(AddressBook *addressBook, char *searchKey);   //Search by name
void search_by_phone(AddressBook *addressBook, char *searchKey);  //Search ny phone
void search_by_email(AddressBook *addressBook, char *searchKey);  //Search by email

/* =================================== LIST CONTACTS============================== ================= */
void listContacts(AddressBook *addressBook)
{
    // Sorting contacts alphabetically by name (bubble sort)
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = i + 1; j < addressBook->contactCount; j++)
        { 
            // Compare names using strcmp function
            if (strcmp(addressBook->contacts[i].name, addressBook->contacts[j].name) > 0)
            {
                // Swap contacts
                Contact t = addressBook->contacts[i];
                addressBook->contacts[i] = addressBook->contacts[j];
                addressBook->contacts[j] = t;
            }
        }
    }

    // Print table header
    printf(YELLOW_BOLD "==========================================================================================\n" RESET);
    printf( "||      %-20s           |         %-15s          |         %-30s      ||\n", BLUE_BOLD "Name" RESET, BLUE_BOLD "Phone" RESET, BLUE_BOLD "Email" RESET) ;
    printf(YELLOW_BOLD "==========================================================================================\n" RESET);

    // Print contacts inside table
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf("||      %-20s|     %-15s    |    %-30s||\n",
               addressBook->contacts[i].name,            // Name
               addressBook->contacts[i].phone,           // Phone
               addressBook->contacts[i].email);          // Email
        printf(YELLOW_BOLD "-------------------------------------------------------------------------------------------\n" RESET);
    }

    // If no contacts exist
    if (addressBook->contactCount == 0)
    {
        printf(RED "No contacts available\n" RESET);   // Print text No contact found
        return;
    }

}

/* ======================================== INITIALIZE =============================================== */
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;        // initialize contact count
}
     


/* ========================================SAVE & EXIT=============================================== */
void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook);     // Save contacts to file
    exit(EXIT_SUCCESS);
    
}

/* ========================================CREATE CONTACT============================================ */

void createContact(AddressBook *addressBook)
{
    // Temporary contact storage
    Contact newContact;
    int attempts=0;

        /*-------------------------------------- NAME ------------------------------------ */
    // Set attempts 3
    while (attempts < 3)
    {
        printf("Enter name: ");             //Asking user to enter name
        scanf(" %[^\n]", newContact.name);  //Reading input from user

        int len = strlen(newContact.name);  //Length of name using strlen function
        int valid = 1;

        // Minimum length of name
        if (len < 4)
        {
            //Print error msg
            printf(YELLOW_BOLD "Error: Name must contain at least 4 characters\n" RESET);
            valid = 0;
        }

        // Only alphabets (no numbers/symbols)
        for (int i = 0; i < len; i++)
        {
            if (!isalpha(newContact.name[i]) && newContact.name[i] != ' ')
            { 
                //Print error msg
                printf("Error: Name should contain only alphabets (A-Z, a-z)\n");
                valid = 0;
                break;
            }
        }

        if (valid)
            break;

        attempts++;
    }

    // Stop after 3 invalid attempts
    if (attempts == 3)
    {
        printf("Too many invalid attempts.\n");
        {
            return;
        }
        
    }


            /* ----------------------------------- PHONE ------------------------------------- */
    int phoneAttempts = 0;

    //set phone attempts 3
    while (phoneAttempts < 3)
    {
        printf("Enter phone number: ");         // Asking user to enter phone number
        scanf(" %[^\n]", newContact.phone);     //Reading input from

        int valid = 1;
         // Check phone length and starting digit
        if (strlen(newContact.phone) == 10 && newContact.phone[0] >= '6' && newContact.phone[0] <= '9')
        {
            for (int i = 0; i < 10; i++)
            {
                if (!isdigit(newContact.phone[i]))
                {
                    valid = 0;
                    break;
                }
            }

            // Check duplicate phone number
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, newContact.phone) == 0)
                {
                    printf("Phone number already exists\n");
                    valid = 0;
                    break;
                }
            }
        }
        else
        {
            valid = 0;
        }

        if (valid) break;

        printf(YELLOW "Invalid phone number\n" RESET);
        phoneAttempts++;
    }

    // Stop after 3 invalid attempts
    if (phoneAttempts == 3) 
    {
        return;
    }
            /* ------------------------------------- Email ---------------------------------------*/
    int emailAttempts = 0;
    
    //Set email attempts 3
    while (emailAttempts < 3)
    {
        printf("Enter email: ");            //Asking user to enter email
        scanf(" %[^\n]", newContact.email); //Reading input from user

        int valid = 1;
        int len = strlen(newContact.email);  //Using strlen find the length of string

        int atCount = 0, dotCount = 0;
        int atIndex = -1, dotIndex = -1;

        // convert to lowercase (your original logic kept)
        for (int i = 0; newContact.email[i]; i++)
            newContact.email[i] = tolower(newContact.email[i]);

        // scan email
        for (int i = 0; i < len; i++)
        {
            if (newContact.email[i] == '@')     // @ once
            {
                atCount++;
                atIndex = i;
            }
            else if (newContact.email[i] == '.')  // . once
            {
                dotCount++;
                dotIndex = i;
            }
            else if (!isalnum(newContact.email[i]))
            {
                printf(YELLOW_BOLD "Error: Only @ and . are allowed as symbols\n" RESET);
                valid = 0;
                break;
            }
        }

        
        if (valid && atCount != 1)
        {
             printf(YELLOW_BOLD "Error: Email must contain exactly one @ symbol\n" RESET);
            valid = 0;
        }

    
        if (valid && dotCount < 1)
        {
            printf(YELLOW_BOLD "Error: Email must contain at least one dot (.)\n" RESET);
            valid = 0;
        }

    
        if (valid && dotIndex < atIndex)
        {
            printf(YELLOW_BOLD "Error: Dot must appear after @\n" RESET);
            valid = 0;
        }

        
        if (valid && dotIndex == atIndex + 1)
        {
            printf(YELLOW_BOLD "Error: At least one character required between @ and .\n" RESET);
            valid = 0;
        }

        // (no extra chars after domain like .com123)
        if (valid && dotIndex != -1 && dotIndex < len - 1)
        {
            for (int i = dotIndex + 1; i < len; i++)
            {
                if (!isalpha(newContact.email[i]))
                {
                    printf(YELLOW_BOLD "Error: Extra characters after domain are not allowed\n" RESET);
                    valid = 0;
                    break;
                }
            }
        }

    

        // minimum 1 char before @
        if (valid && atIndex < 1)
        {
            printf(YELLOW_BOLD "Error: Minimum 1 character required before '@'\n" RESET);
            valid = 0;
        }

        // duplicate check (your logic kept)
        if (valid)
        {
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, newContact.email) == 0)
                {
                    printf(RED_BOLD "Error: Email already exists\n" RESET);
                    valid = 0;
                    break;
                }   
            }
        }

        if (valid)
            break;

        printf("Invalid email\n");
        emailAttempts++;
    }

    // Stop after 3 invalid attempts
    if (emailAttempts == 3)
        return;

 
    // Store contact in array
    addressBook->contacts[addressBook->contactCount++] = newContact;
    saveContactsToFile(addressBook);

    printf(GREEN_BOLD "Contact added successfully!\n" RESET);
}

  /* ========================================== SEARCH MENU============================================= */
void searchContact(AddressBook *addressBook)
{
    int choice;          // Store user choice
    char searchKey[50];  // Store search input

    printf("Search By:\n1. Name\n2. Phone\n3. Email\n");
    printf("Enter your choice : ");
    scanf("%d", &choice);

    getchar();         //Clear input buffe

    printf("Enter search value: ");
    scanf(" %[^\n]", searchKey);

    // Call appropriate search function
    switch (choice)
    {
        case 1:
            printf("Enter the name :\n ");            //Enter the name
            search_by_name(addressBook, searchKey);   // Call the search_by_name function
            break;
        case 2:
            printf("Enter the phone number :\n");     // Enter phone number
            search_by_phone(addressBook, searchKey);  // Call the search_by_phone function
            break;
        case 3:
            printf("Enter the email id :\n ");        // Enter email id
            search_by_email(addressBook, searchKey);  // Call the search_by_email function
            break;
        default:
            printf("Invalid choice\n");              // Invalid choice
    }
}

            /* ------------------------------ SEARCH BY NAME ------------------------------- */

void search_by_name(AddressBook *addressBook, char *searchKey)
{
    int found = 0;      // Flag for match


    for (int i = 0; i < addressBook->contactCount; i++)
    {
        char temp[50];
        strcpy(temp, addressBook->contacts[i].name);

        // Convert to lowercase
        for (int k = 0; temp[k]; k++)
            temp[k] = tolower(temp[k]);

        for (int k = 0; searchKey[k]; k++)
            searchKey[k] = tolower(searchKey[k]);

        // Check substring match
        if (strstr(temp, searchKey))
        {
            if (!found)
            {
                printf(GREEN_BOLD "\nCONTACT FOUND:\n" RESET);
            }

            printf("Name  : %s\n", addressBook->contacts[i].name);        // Name
            printf("Phone : %s\n", addressBook->contacts[i].phone);       // Phone number
            printf("Email : %s\n\n", addressBook->contacts[i].email);     // Email

            found = 1;
        }
    }

    if (!found)
    {
        printf(RED_BOLD "NO CONTACT FOUND!\n" RESET);
    }

}

                 /* ------------------------------ SEARCH BY PHONE -------------------------------- */
void search_by_phone(AddressBook *addressBook, char *searchKey)
{
    int found = 0;

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        // strstr function user to search contacts
        if (strstr(addressBook->contacts[i].phone, searchKey))
        {
            if (!found)
            {
                printf(GREEN_BOLD "\nCONTACT FOUND:\n" RESET);
            }

            printf("Name  : %s\n", addressBook->contacts[i].name);     // Name
            printf("Phone : %s\n", addressBook->contacts[i].phone);    // Phone
            printf("Email : %s\n\n", addressBook->contacts[i].email);  //Email

            found = 1;
        }
    }

    if (!found)
    {
        printf(RED_BOLD "NO CONTACT FOUND!\n" RESET);
    }
}

             /* ------------------------------ SEARCH BY EMAIL ------------------------------ */
void search_by_email(AddressBook *addressBook, char *searchKey)
{
    int found = 0;     // Flag for match


    for (int i = 0; i < addressBook->contactCount; i++)
    {
        char temp[50];
        strcpy(temp, addressBook->contacts[i].email);

        // Convert to lowercase
        for (int k = 0; temp[k]; k++)
        {
            temp[k] = tolower(temp[k]);
        }

        for (int k = 0; searchKey[k]; k++)
        {
            searchKey[k] = tolower(searchKey[k]);
        }

        // Check substring match
        if (strstr(temp, searchKey))
        { 
            // Show contact details
            if (!found)
            {
                printf(GREEN_BOLD "\nCONTACT FOUND:\n" RESET);
            }

            printf("Name  : %s\n", addressBook->contacts[i].name);       // Name
            printf("Phone : %s\n", addressBook->contacts[i].phone);      // Phone number
            printf("Email : %s\n\n", addressBook->contacts[i].email);    // Email

            found = 1;
        }
    }

    if (!found)
    {
        printf(RED_BOLD "NO CONTACT FOUND!\n" RESET);
    }
}

    /* ============================================ EDITCONTACT =============================================== */ 
void editContact(AddressBook *addressBook) 
{
    char key[50];     // Store search input
    int choice;       // Store user choice

    printf("Enter Name / Phone / Email to search: ");  // Ask to user
    scanf(" %[^\n]", key);                             //Read from user

    int foundIndexes[100];
    int count = 0;

    /* ---------- SEARCH ---------- */
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasecmp(addressBook->contacts[i].name, key) == 0 ||
            strcmp(addressBook->contacts[i].phone, key) == 0 ||
            strcasecmp(addressBook->contacts[i].email, key) == 0)
        {
            foundIndexes[count++] = i;
        }
    }

    if (count == 0)
    {
        printf("CONTACT NOT FOUND!\n");
        return;
    }

    int index = foundIndexes[0];

               /* ------------------------ IF MULTIPLE CONTACTS ------------------------------- */
    if (count > 1)
    {
        printf("\nMultiple contacts found:\n");

        for (int i = 0; i < count; i++)
        {
            printf("%d. %s | %s | %s\n",
                   i + 1,
                   addressBook->contacts[foundIndexes[i]].name,
                   addressBook->contacts[foundIndexes[i]].phone,
                   addressBook->contacts[foundIndexes[i]].email);
        }

        int select;
        printf("Select contact: ");
        scanf("%d", &select);

        if (select < 1 || select > count)
        {
            printf("Invalid selection\n");
            return;
        }

        index = foundIndexes[select - 1];
    }

           /* ------------------------------ EDIT MENU ---------------------------------- */
    while (1)
    {
        // Edit menu
        printf("\nEdit Menu:\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Exit\n");

        printf("Enter choice: ");   //Asking user to enter choice
        scanf("%d", &choice);       //Read input from user

        /* ================= NAME ================= */
        if (choice == 1)
        {
            char newName[50]; // Store newname
            int valid = 1;

            printf("Enter new name: ");   //Asking user to enter new name
            scanf(" %[^\n]", newName);    //Reading new name from user

            //Using strlen function to find length of string and check the condition name must be 4 character 
            if (strlen(newName) < 4)
            {
                printf("Error: Name must be at least 4 characters\n");   // Print error msg
                valid = 0;
            }

            for (int i = 0; newName[i]; i++)
            {
                //Using isalpha function check condition only alphabets allowed
                if (!isalpha(newName[i]))
                {
                    printf("Error: Only alphabets allowed\n");         // Print error msg
                    valid = 0;
                    break;
                }
            }

            if (valid)
            {
                strcpy(addressBook->contacts[index].name, newName);
                printf("Name updated successfully\n");
            }
        }

        /* ================= PHONE ================= */
        else if (choice == 2)
        {
            char newPhone[20];   // Store newphone number
            int valid = 1;

            printf("Enter new phone number: ");    // Asking user to enter new phone number
            scanf(" %[^\n]", newPhone);            //Reading phone number from user

            // Using if statement check Phone must be 10 digits
            if (strlen(newPhone) != 10)
            {
                printf("Error: Phone must be 10 digits\n");
                valid = 0;
            }

            // Using if statement chech phone number start from 6 to 9.
            if (newPhone[0] < '6' || newPhone[0] > '9')
            {
                printf("Error: First digit must be 6-9\n");
                valid = 0;
            }

            for (int i = 0; i < 10; i++)
            {
                // Using if statement check only digits are allowed
                if (!isdigit(newPhone[i]))
                {
                    printf("Error: Only digits allowed\n");
                    valid = 0;
                    break;
                }
            }

            if (valid)
            {
                strcpy(addressBook->contacts[index].phone, newPhone);
                printf("Phone updated successfully\n");
            }
        }

        /* ================= EMAIL ================= */
        else if (choice == 3)
        {
            char newEmail[50];  // Store new email
            int valid = 1;

            printf("Enter new email: ");    // Asking user to enter new email
            scanf(" %[^\n]", newEmail);     // Reading new email fromuser

            int len = strlen(newEmail);
            int atCount = 0, dotCount = 0;
            int atIndex = -1, dotIndex = -1;

            /* ----------------------- SCAN EMAIL ----------------------------- */
            for (int i = 0; i < len; i++)
            {
                if (newEmail[i] == '@')       // @ once
                {
                    atCount++;
                    atIndex = i;
                }
                else if (newEmail[i] == '.')  // .once
                {
                    dotCount++;
                    dotIndex = i;
                }
                else if (!isalnum(newEmail[i]))
                {
                    printf("Error: Only alphabets and digits allowed except @ and .\n");
                    valid = 0;
                    break;
                }
            }

            
            if (atCount != 1)
            {
                 printf("Error: Email must contain exactly one @\n");
                valid = 0;
            }

            if (dotCount < 1)
            {
                printf("Error: Email must contain .com\n");
                valid = 0;
            }

            /* ----------------------- BEFORE @ ----------------------- */
            if (valid && atIndex < 1)
            {
                 printf("Error: At least 1 character required before @\n");
                valid = 0;
            }

            /* ------------------5 CHARS BETWEEN @ AND . ---------------- */
            int domainLen = dotIndex - atIndex - 1;

            if (valid && domainLen != 5)
            {
                 printf("Error: Exactly 5 characters required between @ and .com\n");
                valid = 0;
            }

            /* ---------------------- AFTER .COM ------------------------- */
            if (valid)
            {
                // ensure .com is at end
                if (len < 4 || strcmp(&newEmail[len - 4], ".com") != 0)
                {
                    printf("Error: Email must end with .com\n");
                    valid = 0;
                }

                // no digits after .com (strict rule)
                if (valid)
                {
                    for (int i = dotIndex; i < len; i++)
                    {
                        if (isdigit(newEmail[i]))
                        {
                            printf("Error: No digits allowed after .com\n");
                            valid = 0;
                            break;
                        }
                    }
                }   
            }

            /* ------------------ DUPLICATE CHECK --------------------- */
            if (valid)
            {
                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (strcmp(addressBook->contacts[i].email, newEmail) == 0)
                    {
                        printf("Error: Email already exists\n");
                        valid = 0;
                        break;
                    }
                }   
            }

            /* --------------------- UPDATE -------------------------- */
            if (valid)
            {
                strcpy(addressBook->contacts[index].email, newEmail);
                printf("Email updated successfully\n");
            }
        }

        /* -------------------------EXIT ------------------------------*/
        else if (choice == 4)
        {
            printf("Exiting edit menu...\n");
            break;
        }
        else
        {
            printf("Invalid choice\n");
        }
    }

    saveContactsToFile(addressBook);
}
      /*====================================== DELETE CONTACT ================================================*/
void deleteContact(AddressBook *addressBook) 
{
    char name[20];
    printf("Enter first 2 characters of name to delete: ");  // Print text Enter first 2 characters of name to delete
    scanf(" %2s", name);   //  only take 2 characters

    int matchedIndices[100];
    int matchCount = 0;

    /* ---------- SEARCH ---------- */
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strncasecmp(addressBook->contacts[i].name, name, 2) == 0)
        {
            matchedIndices[matchCount] = i;
            matchCount++;
        }
    }

    if (matchCount == 0)
    {
        printf(RED_BOLD "Contact not found!\n" RESET);
        return;
    }

    int index;

    /* ------------------------- MULTIPLE MATCH ------------------------------ */
    if (matchCount > 1)
    {
        printf("\nMultiple contacts found:\n");
        printf("\n");

        for (int i = 0; i < matchCount; i++)
        {
            int idx = matchedIndices[i];

            printf("%d. %s|%s| %s\n |", i + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }

        int choice;
        printf("Select contact number to delete: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > matchCount)
        {
            printf("Invalid choice\n");
            return;
        }

        index = matchedIndices[choice - 1];
    }
    else
    {
        index = matchedIndices[0];
    }

    /* ------------------------ DELETE (SHIFT LEFT) --------------------------------- */
    for (int i = index; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--;

    /* ------------------------------- SAVE --------------------------------------- */
    saveContactsToFile(addressBook);

    printf(GREEN_BOLD "Contact deleted successfully!\n" RESET);
}