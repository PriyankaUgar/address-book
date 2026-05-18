#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.txt", "w"); // open the file in a write mode to write contacts present in addressbook db

    // Validation
    if (fptr == NULL)
    {
        printf("%s Error opening file!\n.%s\n", RED, RESET);
        addressBook->contactCount = 0;
        return;
    }

    // print total contact count in first line with #
    fprintf(fptr, "# Total Contacts : %d\n", addressBook->contactCount);

    // Storing contacts from addressbook db

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s, %s, %s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }

    // close the file
    fclose(fptr);

    printf("%sContact saved successfully%s\n", GREEN, RESET);
}

void loadContactsFromFile(AddressBook *addressBook)
{

    FILE *fptr = fopen("contacts.txt", "r"); // open the file in a read mode to write contacts present in addressbook db

    // Validation
    if (fptr == NULL)
    {
        printf("%sFile doesn't exist!\n.%s\n", RED, RESET);
        addressBook->contactCount = 0;
        return;
    }

    char line[200];

    // Reads first line

    if (fgets(line, sizeof(line), fptr) != NULL)
    {
        // Extract contactCount and avoid # Total Contacts :
        sscanf(line, "# Total Contacts : %d", &addressBook->contactCount); // stores value in contactCount
    }

    // Read contacts from file and store it in addressBook->contacts structure

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fptr,
               " %[^,], %[^,], %[^\n]",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    // close the file
    fclose(fptr);

    printf("%sContact loaded successfully%s\n", GREEN, RESET);
}