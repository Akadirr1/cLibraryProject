#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int ID;
typedef struct
{
	int userId;
	char name[100];
	bool isAdmin;
	char email[100];
	char password[100];
} User;
typedef struct
{
	int bookId;
	char bookName[100];
	char author[100];
	char ctgry[100];
	bool taken;
	int userId;
	char borrowDate[100];
	char returnDate[100];

} Book;
typedef struct
{
	int borrowId;
	int userId;
	int bookId;
	char borrowDate[100];
	char returnDate[100];
} Borrow;
bool isEmailRegistered(const char *email)
{
	const char delimiter[2] = ",";
	char buffer[512];
	char emailToCheck[100];
	strcpy(emailToCheck, email);
	emailToCheck[strcspn(emailToCheck, "\n")] = 0;

	FILE *fp = fopen("Users.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return false;
	}
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;
		char *id = strtok(buffer, delimiter);
		char *name = strtok(NULL, delimiter);
		(void)*name;
		(void)*id;
		char *storedEmail = strtok(NULL, delimiter);
		if (storedEmail != NULL)
		{
			storedEmail[strcspn(storedEmail, "\n")] = 0;
			if (strcmp(storedEmail, emailToCheck) == 0)
			{
				fclose(fp);
				return true;
			}
		}
	}
	fclose(fp);
	return false;
}
void registerUser()
{
	User user;
	printf("Enter your name: \n");
	fgets(user.name, sizeof(user.name), stdin);
	printf("Enter your email: \n");
	fgets(user.email, sizeof(user.email), stdin);
	if (isEmailRegistered(user.email) == true)
	{
		printf("Email already registered\n");
	}
	else
	{
		printf("Enter your password: ");
		fgets(user.password, sizeof(user.password), stdin);

		user.name[strcspn(user.name, "\n")] = 0;
		user.email[strcspn(user.email, "\n")] = 0;
		user.password[strcspn(user.password, "\n")] = 0;
		user.isAdmin = false;

		FILE *fp;
		FILE *Idp = fopen("UniqueUserID.dat", "r");
		if (Idp == NULL)
		{
			printf("Dosya acilamadi");
			return;
		}
		else
		{
			fscanf(Idp, "%d", &ID);
			user.userId = ID;
			ID++;
			fclose(Idp);
		}
		Idp = fopen("UniqueUserID.dat", "w");
		if (Idp == NULL)
		{
			printf("Dosya acilamadi");
			return;
		}
		fprintf(Idp, "%d", ID);
		fclose(Idp);
		fp = fopen("Users.dat", "a+");
		if (fp == NULL)
		{
			printf("Dosya acilamadi");
			return;
		}
		fprintf(fp, "%d,%s,%s,%s,%d \n", user.userId, user.name, user.email, user.password, user.isAdmin);
		fclose(fp);
		printf("User registered successfully!\n");
	}
}
void loginUser(int *islogin, int *isAdmin)
{
	char email[250];
	char password[250];
	bool emailFound = false;
	bool isAdmin = false;
	printf("Enter your email: ");
	fgets(email, sizeof(email), stdin);

	printf("Enter your password: ");
	fgets(password, sizeof(password), stdin);

	email[strcspn(email, "\n")] = 0;
	password[strcspn(password, "\n")] = 0;

	const char delimiter[2] = ",";
	char buffer[512];

	FILE *fp = fopen("Users.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return;
	}
	*islogin = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;

		char *id = strtok(buffer, delimiter);
		char *name = strtok(NULL, delimiter);
		(void)*name;
		(void)*id;

		char *storedEmail = strtok(NULL, delimiter);
		char *storedPassword = strtok(NULL, delimiter);
		char *StoredAdmin = strtok(NULL, delimiter);

		if (storedEmail != NULL && storedPassword != NULL)
		{
			storedEmail[strcspn(storedEmail, "\n")] = 0;
			storedPassword[strcspn(storedPassword, " \n")] = 0;

			if (strcmp(storedEmail, email) == 0)
			{
				emailFound = true;
				if (strcmp(storedPassword, password) == 0)
				{
					*islogin = 1;
					*isAdmin = atoi(StoredAdmin);
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
	fclose(fp);
	if (!emailFound)
	{
		printf("Email not found\n");
	}
	else if (*islogin == 0)
	{
		printf("Password is incorrect\n");
	}
}
void addBook(){
	Book book;
	printf("Enter book name: ");
	fgets(book.bookName, sizeof(book.bookName), stdin);
	printf("Enter author name: ");
	fgets(book.author, sizeof(book.author), stdin);
	printf("Enter category: ");
	fgets(book.ctgry, sizeof(book.ctgry), stdin);
	book.bookName[strcspn(book.bookName, "\n")] = 0;
	book.author[strcspn(book.author, "\n")] = 0;
	book.ctgry[strcspn(book.ctgry, "\n")] = 0;
	book.taken = false;
	book.userId = -1;
	book.borrowDate[0] = '\0';
	book.returnDate[0] = '\0';
	FILE *fp;
	FILE *Idp = fopen("UniqueBookID.dat", "r");
	if (Idp == NULL)
	{
		printf("Dosya acilamadi");
		return;
	}
	else
	{
		book.bookId = ID;
		ID++;
		fclose(Idp);
	}
	
}
void adminChoose(int choice)
{
	switch (choice)
	{
	case 1:
		registerUser(); 
		break;
	case 2:
		// Add Book
		break;
	case 3:
		// Update Book
		break;
	case 4:
		// Delete Book
		break;
	case 5:
		// View outDate Books
		break;
	case 6:
		printf("Logging out...\n");
		break;
	default:
		printf("Invalid choice. Please try again.\n");
	}
}

int main()
{
	int islogin = 0;
	int isAdmin = 0;
	int choice = 0;
	char choiceStr[10];
	while (choice != 3)
	{
		printf("Welcome to the Library System\n");
		printf("1. Register\n");
		printf("2. Login\n");
		printf("3. Exit\n");
		fgets(choiceStr, sizeof(choiceStr), stdin);
		choice = atoi(choiceStr);
		if (choice == 1)
		{
			registerUser();
			continue;
		}
		else if (choice == 2)
		{
			loginUser(&islogin, &isAdmin);
			if (islogin == 1)
			{
				printf("Login successful!\n");
				printf("Welcome to the library system!\n");
				if (isAdmin == 1)
				{
					printf("You are an admin\n choose the admin functions\n");
					printf("1. Add User\n");
					printf("2. Add Book\n");
					printf("3. Update Book\n");
					printf("4. Delete Book\n");
					printf("5. View outDate Books\n");
					printf("6. Logout\n");
					int adminChoice;
					fgets(choiceStr, sizeof(choiceStr), stdin);
					adminChoice = atoi(choiceStr);
					adminChoose(adminChoice);
				}
				else
				{
					printf("You are a user\n");
					// User functions can be called here
				}
			}
			else
			{
				printf("Login failed!\n");
			}
		}
		else if (choice == 3)
		{
			printf("Exiting...\n");
			return 0;
		}
		else
		{
			printf("Invalid choice. Please try again.\n");
		}
	}
}