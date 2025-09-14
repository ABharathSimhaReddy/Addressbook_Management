#include <stdio.h>      // Include standard input/output functions
#include <string.h>     // Include string handling functions
#include "contact.h"    // Include user-defined header for contact structure and functions

int main()
{
    /* Variable and structure definition */
    int option;                         // Variable to store user menu choice
    struct Address_book addressbook;    // Define a structure variable for storing contacts
    addressbook.contact_count = 0;      // Initialize number of contacts to zero

    /* Load contacts from file if available */
    FILE *fp = fopen("data.txt", "r");  // Open file "data.txt" in read mode
    if (fp == NULL)                     // Check if file does not open
    {
        printf("Error: could not open file\n");  // Print error message
        return 1;                                // Exit program with error code
    }
    load_contact(fp, &addressbook);     // Load contacts from file into addressbook

    while (1)                           // Infinite loop for menu until user exits
    {
        /* Display menu */
        printf("\n╔════════════════════════════════════════════╗\n");
        printf("║               ADDRESS BOOK MENU            ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║ 1. Add contact                             ║\n");
        printf("║ 2. Edit contact                            ║\n");
        printf("║ 3. Delete contact                          ║\n");
        printf("║ 4. Search contact                          ║\n");
        printf("║ 5. Display all contacts                    ║\n");
        printf("║ 6. Save and Exit                           ║\n");
        printf("╚════════════════════════════════════════════╝\n");

        printf("Enter your option: ");     // Ask user to enter menu choice
        scanf("%d", &option);             // Read user's choice

        switch (option)                   // Check user's choice
        {
            case 1:
                create_contact(&addressbook); // Call function to add new contact(s)
                break;

            case 2:
                edit_contact(&addressbook);   // Call function to edit existing contact
                break;

            case 3:
                delete_contact(&addressbook); // Call function to delete contact(s)
                break;

            case 4:
                search_contacts(&addressbook); // Call function to search contact
                break;

            case 5:
                printf("\nList of contacts:\n");
                list_contacts(&addressbook);    // Call function to display all contacts
                break;

            case 6:
                printf("\nSaving contacts and exiting...\n");
                save_contacts(&addressbook);    // Call function to save contacts to file
                return 0;                       // Exit program successfully

            default:
                printf("\nInvalid option! Try again.\n");  // If user enters wrong choice
                break;
        }
    }

    return 0;   // Return 0 (program ends, though this line is never reached)
}
