#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int ID;
int islogin = 0;
int isAdmin = 0;
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
void searchBook(){
	char searchName[250];//used
	char buffer[512];//used
	Book currentBook;//used
	bool bookFound= false;//used
	int choice;//used
	char newValue[100];//used

	printf("Nasil aramaya yapmak istersiniz?\n");
	printf("1. Kitap adina göre\n");
	printf("2. Yazar adina göre\n");
	printf("3. Kategoriye göre\n");
	printf("Seciminiz: ");
	fgets(newValue, sizeof(newValue), stdin);
	choice = atoi(newValue);
	switch (choice)
	{
	case 1:
		printf("Aramak istediginiz kitabin ismini girin: ");
		fgets(searchName,sizeof(searchName),stdin);
		searchName[strcspn(searchName,"\n")]=0;
		break;
	case 2:
		printf("Aramak istediginiz yazar ismini girin: ");
		fgets(searchName,sizeof(searchName),stdin);
		searchName[strcspn(searchName,"\n")]=0;
		break;
	case 3:
		printf("Aramak istediginiz kategori ismini girin: ");
		fgets(searchName,sizeof(searchName),stdin);
		searchName[strcspn(searchName,"\n")]=0;
		break;
	default:
		printf("Gecersiz Secim.\n");
		return;
	}		

	FILE *fp = fopen("Books.dat","r");
	if (fp == NULL) {
		printf("Dosya acma hatasi!\n");
		return; // Fonksiyondan çık
	}

	while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		char originalLine[512];
		strcpy(originalLine,buffer);

		char *bookid= strtok(buffer,",");
		char *bookname=strtok(NULL,",");
		char *author=strtok(NULL,",");
		char *ctgry=strtok(NULL,",");
		char *taken=strtok(NULL,",");
		char *userId=strtok(NULL,",");
		char *borrowDate=strtok(NULL,",");
		char *returnDate=strtok(NULL,",");

		if (bookid == NULL)	continue;
		currentBook.bookId=atoi(bookid);
		if(bookname == NULL) 
		{
			continue;
		}
		strcpy(currentBook.bookName,bookname);
		if(author) strcpy(currentBook.author,author); else strcpy(currentBook.author,"\0");
		if(ctgry) strcpy(currentBook.ctgry,ctgry); else strcpy(currentBook.ctgry,"\0");
		if(taken) currentBook.taken=atoi(taken); else currentBook.taken=false;
		if(borrowDate) strcpy(currentBook.borrowDate,borrowDate); else strcpy(currentBook.borrowDate,"\0");
		if(returnDate){
			returnDate[strcspn(returnDate,"\n")]='\0';
			strcpy(currentBook.returnDate,returnDate);
		}
		else currentBook.returnDate[0] = '\0';

		if(strcmp(currentBook.bookName,searchName)==0){
			bookFound =1;
			printf("Kitap bulundu ID:%d, Ad: %s, Yazar: %s, Kategori: %s Raftami: %d",currentBook.bookId,currentBook.bookName,currentBook.author,currentBook.ctgry,currentBook.taken);
			break;
		}
		
	}
}
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
	book.borrowDate[0] = '0';
	book.returnDate[0] = '0';
	FILE *fp;
	FILE *Idp = fopen("UniqueBookID.dat", "r");
	if (Idp == NULL)
	{
		printf("Dosya acilamadi");
		return;
	}
	else
	{
		fscanf(Idp, "%d", &ID);
			book.bookId = ID;
			ID++;
		fclose(Idp);
	}
	Idp = fopen("UniqueBookID.dat", "w");
		if (Idp == NULL)
		{
			printf("Dosya acilamadi");
			return;
		}
		fprintf(Idp, "%d", ID);
		fclose(Idp);
	fp = fopen("Books.dat", "a+");
		if (fp == NULL)
		{
			printf("Dosya acilamadi");
			return;
		}
		fprintf(fp, "%d,%s,%s,%s,%d,%d,%s,%s \n", book.bookId, book.bookName,book.author, book.ctgry,book.taken,book.userId,book.borrowDate,book.returnDate);
		fclose(fp);
		printf("Book adding successfully!\n");
}
void updateBook(){
	char searchName[250];
	char buffer[512];
	Book currentBook;
	bool bookFound= false;
	int choice;
	char newValue[100];

	printf("Guncellemek istediginiz kitabin ismini girinÇ: ");
	fgets(searchName,sizeof(searchName),stdin);
	searchName[strcspn(searchName,"\n")]=0; 

	FILE *fp = fopen("Books.dat","r");
	FILE *tempFp= fopen("temp_books.dat","w");

    if (fp == NULL || tempFp == NULL) {
        printf("Dosya acma hatasi!\n");
        if (fp) fclose(fp); // Eğer fp açıldıysa kapat
        if (tempFp) fclose(tempFp); // Eğer tempFp açıldıysa kapat
        return; // Fonksiyondan çık
    }
	while(fgets(buffer,sizeof(buffer),fp)!=NULL){
		char originalLine[512];
		strcpy(originalLine,buffer);

		char *bookid= strtok(buffer,",");
		char *bookname=strtok(NULL,",");
		char *author=strtok(NULL,",");
		char *ctgry=strtok(NULL,",");
		char *taken=strtok(NULL,",");
		char *userId=strtok(NULL,",");
		char *borrowDate=strtok(NULL,",");
		char *returnDate=strtok(NULL,",");

		if (bookid == NULL)	continue;
		currentBook.bookId=atoi(bookid);
		if(bookname == NULL) 
		{
			fprintf(tempFp,"%s",originalLine);
			continue;
		}
		strcpy(currentBook.bookName,bookname);
		if(strcmp(currentBook.bookName,searchName)==0){
			bookFound =1;
			printf("Kitap bulundu ID:%d, Ad: %s",currentBook.bookId,currentBook.bookName);
		
		if(author) strcpy(currentBook.author,author); else strcpy(currentBook.author,"\0");
		if(ctgry) strcpy(currentBook.ctgry,ctgry); else strcpy(currentBook.ctgry,"\0");
		if(taken) currentBook.taken=atoi(taken); else currentBook.taken=false;
		if(userId) currentBook.userId=atoi(userId); else currentBook.userId=-1;
		if(borrowDate) strcpy(currentBook.borrowDate,borrowDate); else strcpy(currentBook.borrowDate,"\0");
		if(returnDate){
			returnDate[strcspn(returnDate,"\n")]='\0';
			strcpy(currentBook.returnDate,returnDate);
		}
		else currentBook.returnDate[0] = '\0';

		printf("Mevcut Yazar: %s\n", currentBook.author);
        printf("Mevcut Kategori: %s\n", currentBook.ctgry);
        printf("Hangi bilgiyi guncellemek istersiniz?\n");
        printf("1. Kitap Adi\n");
        printf("2. Yazar Adi\n");
        printf("3. Kategori\n");
        printf("Seciminiz: ");
	
		char choiceStr[10];
        fgets(choiceStr, sizeof(choiceStr), stdin);
        choice = atoi(choiceStr);

		printf("Yeni degeri girin: ");
        fgets(newValue, sizeof(newValue), stdin);
    	newValue[strcspn(newValue, "\n")] = 0;

		switch (choice)
		{
		case 1:
			strcpy(currentBook.bookName,newValue);
			break;
		case 2:
			strcpy(currentBook.author,newValue);
			break;
		case 3:
			strcpy(currentBook.ctgry,newValue);
			break;
		default:
			printf("Gecersiz Secim.");
			fprintf(tempFp, "%s", originalLine);
			continue;
		}
		fprintf(tempFp, "%d,%s,%s,%s,%d,%d,%s,%s\n", 
			currentBook.bookId, currentBook.bookName, currentBook.author, 
			currentBook.ctgry, currentBook.taken, currentBook.userId, 
			currentBook.borrowDate, currentBook.returnDate);
		}
		else {
            // Aranan kitap bu değilse, orijinal satırı olduğu gibi geçici dosyaya yaz
            fprintf(tempFp, "%s", originalLine);
        }
	}
	// Dosyaları kapat
    fclose(fp);
    fclose(tempFp);

	if(bookFound){
		if(remove("Books.dat")!=0){
			printf("Eski Books.dat dosyasi silinemedi!\n");
		}
		else{			if(rename("temp_books.dat","Books.dat")!=0){
				printf("Gecici dosya Books.dat olarak yeniden adlandirilamadi!\n");
			}
			else{
				printf("Books.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else{
		printf("'%s' adinda bir kitap bulunamadi.\n", searchName);
		remove("tempBooks.dat");
	}
}
void deleteBook(){
	char searchName[250];
	char buffer[512];
	Book currentBook;
	bool bookFound= false;
	//int choice;
	//char newValue[100];

	printf("Silmek istediginiz kitabin ismini girin: ");
	fgets(searchName,sizeof(searchName),stdin);
	searchName[strcspn(searchName,"\n")]=0;

	FILE *fp=fopen("Books.dat","r");
	FILE *tempFp=fopen("temp_books.dat","w");

	if(fp==NULL|| tempFp==NULL){
		printf("Dosya acma hatasi!");
		if(fp) fclose(fp);
		if(tempFp) fclose(tempFp);
		return;
	}

	while(fgets(buffer,sizeof(buffer),fp)!=NULL)
	{
		char originalLine[512];
		strcpy(originalLine,buffer);

		char *bookid= strtok(buffer,",");
		char *bookname=strtok(NULL,",");
		char *author=strtok(NULL,",");
		char *ctgry=strtok(NULL,",");
		char *taken=strtok(NULL,",");
		char *userId=strtok(NULL,",");
		char *borrowDate=strtok(NULL,",");
		char *returnDate=strtok(NULL,",");

		if (bookid == NULL)	continue;
		currentBook.bookId=atoi(bookid);
		
		if(bookname == NULL) 
		{
			fprintf(tempFp,"%s",originalLine);
			continue;
		}
		strcpy(currentBook.bookName,bookname);
		if(author) strcpy(currentBook.author,author); else strcpy(currentBook.author,"\0");
		if(ctgry) strcpy(currentBook.ctgry,ctgry); else strcpy(currentBook.ctgry,"\0");
		if(taken) currentBook.taken=atoi(taken); else currentBook.taken=false;
		if(userId) currentBook.userId=atoi(userId); else currentBook.userId=-1;
		if(borrowDate) strcpy(currentBook.borrowDate,borrowDate); else strcpy(currentBook.borrowDate,"\0");
		if(returnDate){
			returnDate[strcspn(returnDate,"\n")]='\0';
			strcpy(currentBook.returnDate,returnDate);
		}
		else currentBook.returnDate[0] = '\0';
		
		if(strcmp(currentBook.bookName,searchName)==0){
			bookFound =1;
			printf("Kitap silindi ID:%d, Ad: %s",currentBook.bookId,currentBook.bookName);
			continue;
		}
		else{
			fprintf(tempFp, "%s", originalLine);
		}
	}
	// Dosyaları kapat
    fclose(fp);
    fclose(tempFp);
	if(bookFound){
		if(remove("Books.dat")!=0){
			printf("Eski Books.dat dosyasi silinemedi!\n");
		}
		else{	
			if(rename("temp_books.dat","Books.dat")!=0){
				printf("Gecici dosya Books.dat olarak yeniden adlandirilamadi!\n");
			}
			else{
				printf("Books.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else{
		printf("'%s' adindaki kitap silinemedi.\n", searchName);
		remove("tempBooks.dat");
	}

}
//bir süre deakftif
/*void viewOutdatedBooks(){
	char searchName[250];
	char buffer[512];
	Borrow currentBorrow;
	bool bookFound= false;
	int choice;
	char newValue[100];

	FILE *fp = fopen("Borrows.dat","r");


	if (fp == NULL) {
        printf("Dosya acma hatasi!\n");
        if (fp) fclose(fp); // Eğer fp açıldıysa kapat
        return; // Fonksiyondan çık
    }

	while(fgets(buffer,sizeof(buffer),fp)!=NULL)
	{	
		buffer[strcspn(buffer, "\n")] = 0;
		char originalLine[512];
		strcpy(originalLine,buffer);


		char *borrowId_str = strtok(buffer,",");
        char *userId_str = strtok(NULL,",");
        char *bookId_str = strtok(NULL,",");
        char *borrowDate_str = strtok(NULL,",");
        char *returnDate_str = strtok(NULL,",");
		if (borrowId_str == NULL)	continue;

		currentBorrow.borrowId=atoi(borrowId_str);
		if(userId_str) currentBorrow.userId=atoi(userId_str)	; else currentBorrow.userId=0;
		if(bookId_str) currentBorrow.bookId=atoi(bookId_str); else currentBorrow.bookId=0;
		if(borrowDate_str) strcpy(currentBorrow.borrowDate,borrowDate_str); else strcpy(currentBorrow.borrowDate,"\0");
		if(returnDate_str){
			returnDate_str[strcspn(returnDate_str,"\n")]='\0';
			strcpy(currentBorrow.returnDate,returnDate_str);
		}
		else currentBorrow.returnDate[0] = '\0';

		if (borrowId_str == NULL || userId_str == NULL || bookId_str == NULL || borrowDate_str == NULL || returnDate_str == NULL) {
            fprintf(stderr, "Hatali satir (eksik alan): %s\n", originalLine);
            continue;
        }
		
		

	}
}*/
void adminChoose(int choice)
{
	switch (choice)
	{
	case 1:
		registerUser(); 
		break;
	case 2:
		addBook();
		break;
	case 3:
		updateBook();
		break;
	case 4:
		deleteBook();
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
void userChoose(int choice)
{
	switch (choice)
	{
	case 1:
		// Borrow Book
		break;
	case 2:
		// Return Book
		break;
	case 3:
		// View Borrowed Books
		break;
	case 4:
		printf("Logging out...\n");
		break;
	default:
		printf("Invalid choice. Please try again.\n");
	}
}
int main()
{
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
					int adminChoice= 0;
					do
					{
					printf("You are an admin\n choose the admin functions\n");
					printf("1. Add User\n");
					printf("2. Add Book\n");
					printf("3. Update Book\n");
					printf("4. Delete Book\n");
					printf("5. View outDate Books\n");
					printf("6. Logout\n");
					
					fgets(choiceStr, sizeof(choiceStr), stdin);
					adminChoice = atoi(choiceStr);
					adminChoose(adminChoice);
					} while (adminChoice!=6);
					
					continue;
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