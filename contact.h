#ifndef CONTACT_H           // Header guard start, prevents multiple inclusion
#define CONTACT_H

/*------------------ Structure Declarations ------------------*/

struct Contact_data       // Structure to store individual contact information
{
    char Name[32];        // Name of contact (max 31 chars + null terminator)
    char Mobile_number[11]; // Mobile number (10 digits + null terminator)
    char Mail_ID[35];     // Email ID of contact (max 34 chars + null terminator)
};

struct Address_book       // Structure to store multiple contacts
{
    struct Contact_data contact_details[100]; // Array to store up to 100 contacts
    int contact_count;      // Number of contacts currently stored
};

/*------------------ Function Declarations ------------------*/

/* Load contacts from file */
void load_contact(FILE *fp, struct Address_book *addressbook); // Reads data from file and stores in address book

/* Validate user input */
void valid_name(char *name); // Validate name (only alphabets and spaces)
void valid_mobile_number(char *mobile_number, struct Address_book *addressbook); // Validate mobile number (digits, length, uniqueness)
void valid_mail_id(char *mail_id, struct Address_book *addressbook); // Validate mail ID (format and uniqueness)

/* Create, List, Search, Edit, Delete, Save */
void create_contact(struct Address_book *addressbook); // Add new contact(s) to address book
void list_contacts(struct Address_book *addressbook); // Display all contacts in a formatted table
void search_contacts(struct Address_book *addressbook); // Menu for searching contacts by Name, Mobile, or Mail
int search_name(struct Address_book *addressbook); // Search contact by name, return index or -1 if not found
int search_Mobile_Number(struct Address_book *addressbook); // Search contact by mobile number
int search_mail_id(struct Address_book *addressbook); // Search contact by mail ID
void edit_contact(struct Address_book *addressbook); // Edit contact fields
void delete_contact(struct Address_book *addressbook); // Delete contact from address book
void save_contacts(struct Address_book *addressbook); // Save all contacts to file

/* Sort contacts alphabetically by Name */
void sort_contact_by_name(struct Address_book *addressbook); // Sort contacts in dictionary order by name

#endif // CONTACT_H          // End of header guard
