/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*
-> Project      : ADDRESS BOOK MANAGEMENT SYSTEM.
-> File         : contact.c / contact.h / main.c / data.txt
-> Name         : A. Bharath Simha Reddy.
-> Date         : 31-Aug-2025.

-> Description  :
->    This project is a console-based Address Book Management System,implemented in C.
        It allows users to efficiently manage contact.
->    Information including Name, Mobile Number, and Mail ID. The system is designed to be simple,
    user-friendly, and robust with proper user input.

->    validation and persistent File storage.

->    Main functionalities include:

        -> Adding new contacts: Users can add multiple contacts at a time, with automatic validation to ensure
            correct format and uniqueness.

        -> Listing contacts: Display all stored contacts in a well-formatted table for easy readability.

    -> Searching contacts: Search by Name, Mobile Number, or Mail ID with case-insensitive matching.
        Handles multiple results and allows user selection.

    -> Editing contacts: Update any individual field or all fields of a contact. Ensures input validation after editing.

    -> Deleting contacts: Remove contacts from the address book with confirmation to avoid accidental deletions.

    -> Saving contacts: Automatically or manually save contacts to a file (data.txt) for persistent storage,
    allowing data retrieval on program restart.

    Input validation:

    -> Name: Only alphabets and spaces are allowed. First character must be an alphabet.

    -> Mobile Number: Must be exactly 10 digits, contain only numbers,and be unique.

    -> Mail ID: Must follow standard email format with one '@', valid local and domain parts, and uniqueness ensured.

Features     :

    -> Interactive menu-driven interface for easy use.

    -> Alphabetical sorting of contacts by Name for organized display.

    -> Handles multiple search results and allows selection of correct contact for editing or deleting.

    -> Supports up to 100 contacts with persistent file storage.

    -> Detailed success and error messages for user guidance.

Usage Notes :

    -> Ensure the data file (data.txt) exists in the same directory or the program will start with an empty address book.
    -> Use valid and unique data to avoid errors or duplicates.
    -> Menu options guide the user through all available operations.

******************************************************************************/
#include <stdio.h>      // Include standard input/output functions (printf, scanf, etc.)
#include <string.h>     // Include string handling functions (strcpy, strcmp, strlen, etc.)
#include <ctype.h>      // Include character functions (tolower, isalpha, etc.)
#include <stdlib.h>     // Include standard library functions (exit, atoi, malloc, etc.)
#include "contact.h"    // Include custom header file with structure definitions and function prototypes

int compare_names(const char *a, const char *b)  // Function to compare two strings case-insensitively
{
    while (*a && *b) {                            // Loop until either string reaches null character
        char ca = tolower(*a);                    // Convert current character of string a to lowercase
        char cb = tolower(*b);                    // Convert current character of string b to lowercase
        if (ca != cb)                             // Check if characters are different
            return ca - cb;                       // Return difference if not equal (used for sorting)
        a++;                                      // Move pointer to next character in string a
        b++;                                      // Move pointer to next character in string b
    }
    return tolower(*a) - tolower(*b);            // If loop ends, return difference of final characters (handles strings of different lengths)
}

/*---------------- Sort contacts by Name (dictionary order) ----------------*/
void sort_contacts_by_name(struct Address_book *addressbook) // Sort contacts alphabetically by Name
{
    for (int i = 0; i < addressbook->contact_count - 1; i++) // Outer loop through each contact
    {
        for (int j = i + 1; j < addressbook->contact_count; j++) // Inner loop to compare with subsequent contacts
        {
            if (compare_names(addressbook->contact_details[i].Name,
                              addressbook->contact_details[j].Name) > 0) // Compare names case-insensitively
            {
                struct Contact_data temp = addressbook->contact_details[i]; // Store current contact in temp
                addressbook->contact_details[i] = addressbook->contact_details[j]; // Swap with next contact
                addressbook->contact_details[j] = temp; // Complete swap
            }
        }
    }
}


void load_contact(FILE *fp, struct Address_book *addressbook) // Load contacts from file
{
    fscanf(fp,"#%d\n",&addressbook->contact_count);
    for(int i=0;i<addressbook->contact_count;i++)
    {
        fscanf(fp,"%[^,],%[^,],%[^\n]\n", // Parse Name, Mail ID, Mobile
            addressbook->contact_details[i].Name,
            addressbook->contact_details[i].Mobile_number,
            addressbook->contact_details[i].Mail_ID);
    
    }

    sort_contacts_by_name(addressbook); // Sort contacts alphabetically by Name
}


/*------------------- Validate Name -------------------*/
void valid_name(char *name) // Validate user's input for Name
{
    int repeat;
    do
    {
        repeat = 0;
        scanf(" %[^\n]", name); // Read input including spaces until newline
        if (!isalpha(name[0])) // Check first character is alphabet
        {
            printf("First letter must be an alphabet. Try again: ");
            repeat = 1; // Repeat input if invalid
        }
        for (int i = 0; name[i] != '\0'; i++) // Loop through each character
        {
            if (!isalpha(name[i]) && name[i] != ' ') // Allow only letters and spaces
            {
                printf("Name can contain only alphabets and spaces. Try again: ");
                repeat = 1; // Repeat input if invalid
                break; // Exit inner loop
            }
        }
    } while (repeat); // Repeat until valid input

    // Display validation success in formatted table
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║               VALIDATION SUCCESS           ║\n");
    printf("╠════════════════════════════════════════════╣\n");

    printf("║ Name   : %-34s║\n", name); // Display name with padding
    printf("╚════════════════════════════════════════════╝\n\n");
}


/*------------------- Validate Mobile Number -------------------*/
void valid_mobile_number(char *mobile_number, struct Address_book *addressbook) // Validate user's Mobile Number
{
    int duplicate;
    do
    {
        duplicate = 0;
        scanf(" %s", mobile_number); // Read mobile number input

        // Length check
        if (strlen(mobile_number) != 10) // Must be exactly 10 digits
        {
            printf("Mobile number must be exactly 10 digits. Try again: ");
            duplicate = 1; // Repeat input
            continue;
        }

        // Digit check
        for (int i = 0; i < 10; i++)
        {
            if (!isdigit(mobile_number[i])) // Ensure all characters are digits
            {
                printf("Mobile number must contain only digits.\n Enter Mobile Number:");
                duplicate = 1; // Repeat input
                break;
            }
        }

        // Uniqueness check
        for (int i = 0; i < addressbook->contact_count; i++)
        {
            if (strcmp(addressbook->contact_details[i].Mobile_number, mobile_number) == 0) // Check duplicate
            {
                printf("Mobile number already exists.\nEnter a different one: ");
                duplicate = 1; // Repeat input
                break;
            }
        }
    } while (duplicate); // Repeat until valid

    // Display validation success
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║               VALIDATION SUCCESS           ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║ Mobile : %-32s  ║\n", mobile_number); // Show validated mobile
    printf("╚════════════════════════════════════════════╝\n\n");
}

/*------------------- Validate Mail ID -------------------*/
void valid_mail_id(char *mail_id, struct Address_book *addressbook) // Validate user's Mail ID
{
    int repeat;
    do
    {
        repeat = 0;
        scanf(" %s", mail_id); // Read mail input

        // Must contain exactly one '@'
        int at_count = 0, at_pos = -1;
        for (int i = 0; mail_id[i] != '\0'; i++)
        {
            if (mail_id[i] == '@') // Count '@'
            {
                at_count++;
                at_pos = i; // Store position
            }
        }
        if (at_count != 1) // Check single '@'
        {
            printf("Mail must contain exactly one '@'. Try again: ");
            repeat = 1; // Repeat input
            continue;
        }

        // At least 5 characters before '@'
        if (at_pos < 5) // Minimum local part length
        {
            printf("At least 5 characters required before '@'. Try again: ");
            repeat = 1;
            continue;
        }

        // First letter lowercase
        if (!islower(mail_id[0])) // Must start with lowercase
        {
            printf("First letter must be lowercase. Try again: ");
            repeat = 1;
            continue;
        }

        // Validate local part
        for (int i = 0; i < at_pos; i++)
        {
            if (!islower(mail_id[i]) && !isdigit(mail_id[i])) // Only lowercase letters/digits
            {
                printf("Local part can have only lowercase letters or digits. Try again.\n Enter Mail Id: ");
                repeat = 1;
                break;
            }
        }
        if (repeat)
            continue;

        // Domain validation
        int has_letter = 0, has_dot = 0;
        for (int i = at_pos + 1; mail_id[i] != '\0'; i++)
        {
            if (isalpha(mail_id[i])) // Must have at least one letter
                has_letter = 1;
            if (mail_id[i] == '.') // Must contain '.'
                has_dot = 1;

            if (!isalnum(mail_id[i]) && mail_id[i] != '.' && mail_id[i] != '-') // Only allowed chars
            {
                printf("Domain can have only letters, digits, '.' or '-'. Try again: ");
                repeat = 1;
                break;
            }
        }
        if (!has_letter) // Letter check
        {
            printf("Domain must have at least one letter. Try again: ");
            repeat = 1;
            continue;
        }
        if (!has_dot) // Dot check
        {
            printf("Domain must contain '.'. Try again: ");
            repeat = 1;
            continue;
        }

        // Duplicate check
        for (int i = 0; i < addressbook->contact_count; i++)
        {
            if (strcmp(addressbook->contact_details[i].Mail_ID, mail_id) == 0) // Existing email
            {
                printf("Mail ID already exists. Enter a different one: ");
                repeat = 1;
                break;
            }
        }

    } while (repeat); // Repeat until valid

    // Display validation success
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║               VALIDATION SUCCESS           ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║ Mail   : %-32s  ║\n", mail_id); // Show validated mail
    printf("╚════════════════════════════════════════════╝\n\n");
}

/*------------------- Create Contact -------------------*/
void create_contact(struct Address_book *addressbook) // Function to create new contacts
{
    int repeat = 1; // Control loop for adding multiple contacts

    while (repeat) // Repeat if user wants to add more
    {
        int n;
        printf("How many contacts do you want to add? ");
        scanf("%d", &n); // Read number of contacts to add

        if (addressbook->contact_count + n > 100) // Check maximum limit
        {
            printf("Cannot add %d contacts. Limit exceeded (max 100).\n", n);
            return; // Exit if limit exceeded
        }

        for (int i = 0; i < n; i++) // Loop for each new contact
        {
            // Display contact header
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║               ADD CONTACT                  ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║ Contact Number:%-28d║\n", addressbook->contact_count + 1);
            printf("╚════════════════════════════════════════════╝\n\n");

            char name[32], mobile_number[11], mail_id[35]; // Temporary storage

            printf("Enter Name: ");
            valid_name(name); // Validate name input
            strcpy(addressbook->contact_details[addressbook->contact_count].Name, name); // Store name

            printf("Enter Mobile Number: ");
            valid_mobile_number(mobile_number, addressbook); // Validate mobile number
            strcpy(addressbook->contact_details[addressbook->contact_count].Mobile_number, mobile_number); // Store mobile

            printf("Enter Mail ID: ");
            valid_mail_id(mail_id, addressbook); // Validate mail ID
            strcpy(addressbook->contact_details[addressbook->contact_count].Mail_ID, mail_id); // Store mail ID

            addressbook->contact_count++; // Increment contact count

            // Display success message
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║          CONTACT ADDED SUCCESSFULLY        ║\n");
            printf("╚════════════════════════════════════════════╝\n\n");
        }

        // Ask user to add more contacts
        printf("Do you want to add more contacts? (1 = Yes, 0 = No): ");
        scanf("%d", &repeat); // Read user choice
        printf("\n");
    }
}


/*------------------- List Contacts -------------------*/
void list_contacts(struct Address_book *addressbook) // Function to display all contacts
{
    if (addressbook->contact_count == 0) // No contacts
    {
        printf("\nNo contacts available to display.\n");
        return;
    }

    // Print table header
// Table header
printf("\n╔══════╦════════════════════╦══════════════╦═══════════════════════════════════════════╗\n");
printf("║ %-4s ║ %-18s ║ %-12s ║ %-41s ║\n", "No.", "Name", "Mobile", "Mail ID");
printf("╠══════╬════════════════════╬══════════════╬═══════════════════════════════════════════╣\n");

// Table rows
for (int i = 0; i < addressbook->contact_count; i++)
{
    printf("║ %-4d ║ %-18.18s ║ %-12.12s ║ %-41.41s ║\n",
        i + 1,                                       // No.
        addressbook->contact_details[i].Name,       // Name
        addressbook->contact_details[i].Mobile_number, // Mobile
        addressbook->contact_details[i].Mail_ID     // Mail ID
    );
}

// Table footer
printf("╚══════╩════════════════════╩══════════════╩═══════════════════════════════════════════╝\n");
}

/*------------------- Search Contact by Name -------------------*/
int search_name(struct Address_book *addressbook) // Function to search contact(s) by name
{
    char name[35];
    printf("Enter Name to search: ");
    scanf(" %[^\n]", name); // Input search name

    int count = 0; // Counter for matches
    int index[100]; // Array to store indices of matching contacts

    // Collect ALL matches (case-insensitive, full match)
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (strcasecmp(addressbook->contact_details[i].Name, name) == 0) // Compare ignoring case
        {
            index[count++] = i;  // Store matching index
        }
    }

    if (count == 0) // No match found
    {
        printf("\n╔════════════════════════════════════════════╗\n");
        printf("║        NO CONTACT FOUND WITH THIS NAME     ║\n");
        printf("╚════════════════════════════════════════════╝\n\n");
        return -1; // Return -1 if not found
    }

    // Print all matches in a table
// Table header
printf("\n╔═══════╦════════════════════╦══════════════╦════════════════════════════════════════════╗\n");
printf("║ %-5s ║ %-18s ║ %-12s ║ %-42s ║\n", "No.", "Name", "Mobile", "Mail ID");
printf("╠═══════╬════════════════════╬══════════════╬════════════════════════════════════════════╣\n");

// Table rows
for (int k = 0; k < count; k++)
{
    int i = index[k];
    printf("║ %-5d ║ %-18.18s ║ %-12.12s ║ %-42.42s ║\n",
        k + 1,                                       // No.
        addressbook->contact_details[i].Name,       // Name
        addressbook->contact_details[i].Mobile_number, // Mobile
        addressbook->contact_details[i].Mail_ID     // Mail ID
    );
}

// Table footer
printf("╚═══════╩════════════════════╩══════════════╩════════════════════════════════════════════╝\n");

    if (count == 1)
    return index[0];  // Only one match → return its index
    int choice;
    printf("\nMultiple contacts found. Select a contact (1-%d): ", count);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > count) { // Validate input
        printf("Invalid choice!\n");
        return -1;
    }

    return index[choice - 1];  // Return actual index in addressbook
}


/*------------------- Search Contact by Mobile -------------------*/
int search_Mobile_Number(struct Address_book *addressbook) // Function to search contact by mobile number
{
    char mobile[11];
    printf("Enter Mobile Number to search: ");
    scanf(" %s", mobile); // Input mobile number

    for (int i = 0; i < addressbook->contact_count; i++) // Loop through contacts
    {
        if (strcmp(addressbook->contact_details[i].Mobile_number, mobile) == 0) // Match found
        {
            // Display contact details in formatted box
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║              CONTACT FOUND                 ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║ Name   : %-33s ║\n", addressbook->contact_details[i].Name);
            printf("║ Mail   : %-33s ║\n", addressbook->contact_details[i].Mail_ID);
            printf("║ Mobile : %-33s ║\n", addressbook->contact_details[i].Mobile_number);
            printf("╚════════════════════════════════════════════╝\n\n");
            return i; // Return index of found contact
        }
    }

    // No match found
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║        NO CONTACT FOUND WITH THIS NUMBER   ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    return -1; // Return -1 if not found
}


/*------------------- Search Contact by Mail -------------------*/
int search_mail_id(struct Address_book *addressbook) // Function to search contact by Mail ID
{
    char mail[35]; // Buffer to store input Mail ID
    printf("Enter Mail ID to search: ");
    scanf(" %s", mail); // Input Mail ID

    for (int i = 0; i < addressbook->contact_count; i++) // Loop through all contacts
    {
        if (strcmp(addressbook->contact_details[i].Mail_ID, mail) == 0) // Check for match
        {
            // Display contact details in formatted box
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║              CONTACT FOUND                 ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║ Name   : %-33s ║\n", addressbook->contact_details[i].Name); // Print Name
            printf("║ Mail   : %-33s ║\n", addressbook->contact_details[i].Mail_ID); // Print Mail ID
            printf("║ Mobile : %-33s ║\n", addressbook->contact_details[i].Mobile_number); // Print Mobile
            printf("╚════════════════════════════════════════════╝\n\n");
            return i; // Return index of found contact
        }
    }

    // If no match found, display message
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║        NO CONTACT FOUND WITH THIS MAIL ID  ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    return -1; // Return -1 if not found
}


/*------------------- Search Contacts Menu -------------------*/
void search_contacts(struct Address_book *addressbook) // Function to provide a menu for searching contacts
{
    while (1) // Infinite loop to keep menu active until user exits
    {
        // Display search menu options
        printf("\nSearch Contacts Menu:\n1.Name\n2.Mobile Number\n3.Mail ID\n4.Exit\nEnter choice: ");
        int choice;
        scanf("%d", &choice); // Input user choice

        if (choice == 1) // If user chooses Name
            search_name(addressbook); // Call function to search by name
        else if (choice == 2) // If user chooses Mobile Number
            search_Mobile_Number(addressbook); // Call function to search by mobile
        else if (choice == 3) // If user chooses Mail ID
            search_mail_id(addressbook); // Call function to search by mail
        else if (choice == 4) // If user chooses Exit
            return; // Exit the function
        else // Invalid input
            printf("Invalid choice. Try again.\n"); // Prompt invalid choice
    }
}


/*------------------- Edit Contact -------------------*/
void edit_contact(struct Address_book *addressbook) // Function to edit an existing contact
{
    int res = -1; // Store index of contact to edit
    int repeat_search = 1; // Flag to allow multiple edits

    while (repeat_search) // Loop for searching contacts repeatedly
    {
        // Step 1: Search for contact
        while (1) // Loop until a contact is found or user exits
        {
            // Display search options
            printf("\nSearch Contact for editing : \n");
            printf("1. Name \n2. Mobile number\n3. Mail ID\n4. Exit\nEnter the option : ");
            int n;
            scanf("%d", &n); // Input user choice

            if (n == 1) // Search by Name
                res = search_name(addressbook);
            else if (n == 2) // Search by Mobile Number
                res = search_Mobile_Number(addressbook);
            else if (n == 3) // Search by Mail ID
                res = search_mail_id(addressbook);
            else if (n == 4) // Exit search
            {
                printf("Exiting Search edit menu.\n");
                return;
            }
            else
            {
                printf("Invalid choice. Try again.\n"); // Invalid input
            }

            if (res != -1) // If contact found, exit loop
                break;
            else
                printf("Contact not found, try again...\n"); // Not found, retry
        }

        // Step 2: Edit Menu
        int edit_repeat = 1; // Flag for edit menu loop
        while (edit_repeat) // Loop until user exits edit menu
        {
            // Display edit menu
            printf("\n╔════════════════════════════════════════════╗\n");
            printf("║              EDIT CONTACT                  ║\n");
            printf("╠════════════════════════════════════════════╣\n");
            printf("║ 1. Name                                    ║\n");
            printf("║ 2. Mobile Number                           ║\n");
            printf("║ 3. Mail ID                                 ║\n");
            printf("║ 4. All                                     ║\n");
            printf("║ 5. Exit                                    ║\n");
            printf("╚════════════════════════════════════════════╝\n");
            printf("Enter your choice: ");

            int edit_choice;
            scanf("%d", &edit_choice); // Input edit choice

            char temp_name[32], temp_mobile[11], temp_mail[35]; // Temporary storage for inputs

            switch (edit_choice) // Handle edit options
            {
                case 1: // Edit Name
                    printf("Enter new Name: ");
                    valid_name(temp_name); // Validate name
                    strcpy(addressbook->contact_details[res].Name, temp_name); // Update name
                    printf("Name updated successfully!\n");
                    break;

                case 2: // Edit Mobile Number
                    printf("Enter new Mobile Number: ");
                    valid_mobile_number(temp_mobile, addressbook); // Validate mobile
                    strcpy(addressbook->contact_details[res].Mobile_number, temp_mobile); // Update mobile
                    printf("Mobile number updated successfully!\n");
                    break;

                case 3: // Edit Mail ID
                    printf("Enter new Mail ID: ");
                    valid_mail_id(temp_mail, addressbook); // Validate mail
                    strcpy(addressbook->contact_details[res].Mail_ID, temp_mail); // Update mail
                    printf("Mail ID updated successfully!\n");
                    break;

                case 4: // Edit all fields
                    printf("Enter new Name: ");
                    valid_name(temp_name);
                    strcpy(addressbook->contact_details[res].Name, temp_name);

                    printf("Enter new Mobile Number: ");
                    valid_mobile_number(temp_mobile, addressbook);
                    strcpy(addressbook->contact_details[res].Mobile_number, temp_mobile);

                    printf("Enter new Mail ID: ");
                    valid_mail_id(temp_mail, addressbook);
                    strcpy(addressbook->contact_details[res].Mail_ID, temp_mail);

                    printf("All fields updated successfully!\n");
                    break;

                case 5: // Exit edit menu
                    edit_repeat = 0;
                    break;

                default: // Invalid input
                    printf("Invalid choice. Try again.\n");
            }
        }

        // Step 3: Ask if user wants to edit another contact
        printf("Do you want to edit another contact? (1 = Yes, 0 = No): ");
        scanf("%d", &repeat_search); // Input for repeating search
    }
}


/*------------------- Delete Contact -------------------*/
void delete_contact(struct Address_book *addressbook) // Function to delete a contact
{
    if (addressbook->contact_count == 0)   // Check if any contacts exist
    {
        printf("No contacts available to delete.\n");
        return; // Exit if none
    }

    int repeat = 1; // Flag for deleting multiple contacts

    while (repeat) // Loop to allow multiple deletions
    {
        int index = -1; // Store index of contact to delete

        // Step 1: Search Contact
        while (1) // Loop until contact is found or user exits
        {
            // Display search menu
            printf("\nSearch Contact for deleting : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : ");
            int n;
            scanf("%d", &n); // Input search option

            if (n == 1) // Search by Name
                index = search_name(addressbook);
            else if (n == 2) // Search by Mobile Number
                index = search_Mobile_Number(addressbook);
            else if (n == 3) // Search by Mail ID
                index = search_mail_id(addressbook);
            else if (n == 4) // Exit delete menu
            {
                printf("Exiting delete menu.\n");
                return;
            }
            else
            {
                printf("Invalid choice. Try again.\n"); // Invalid input
            }

            if (index != -1) // Contact found, exit loop
                break;
            else
                printf("Contact not found, try again...\n"); // Not found, retry
        }

        // Step 2: Confirm Deletion
        char confirm;
        printf("\nAre you sure you want to delete this contact? (y/n): ");
        scanf(" %c", &confirm); // Input confirmation
        if (confirm != 'y' && confirm != 'Y') // Cancel deletion
        {
            printf("Deletion cancelled.\n");
        }
        else
        {
            // Step 3: Delete Contact
            for (int i = index; i < addressbook->contact_count - 1; i++) // Shift contacts
            {
                addressbook->contact_details[i] = addressbook->contact_details[i + 1]; // Overwrite deleted contact
            }
            addressbook->contact_count--; // Reduce count

            // Display success message
        printf("\n╔════════════════════════════════════════════════════════════════════════════╗\n");
printf("║                    CONTACT DELETED SUCCESSFULLY!                           ║\n");
printf("╚════════════════════════════════════════════════════════════════════════════╝\n\n");

        }

        // Step 4: Ask if user wants to delete another contact
        printf("Do you want to delete another contact? (1 = Yes, 0 = No): ");
        scanf("%d", &repeat); // Input for repeating deletion
    }
}


/*------------------- Save Contacts to File -------------------*/
void save_contacts(struct Address_book *addressbook)
{
    FILE *fp = fopen("data.txt", "w");            // Open file in write mode
    if (!fp) return;                             // Exit if file can't be opened

    fprintf(fp, "#%d\n", addressbook->contact_count);  // Write total contacts

    for (int i = 0; i < addressbook->contact_count; i++)
    {
        // Write each contact's Name, Mail ID, Mobile Number separated by commas
        fprintf(fp, "%s,%s,%s\n",
                addressbook->contact_details[i].Name,
                addressbook->contact_details[i].Mobile_number,
                addressbook->contact_details[i].Mail_ID);
    }

    fclose(fp);                                   // Close the file
}
