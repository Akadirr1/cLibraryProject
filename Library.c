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
int userHowManyHaveBooks(int userID)
{
	Borrow currentBorrow;
	FILE *fp;
	char buffer[512];
	currentBorrow.userId = userID;
	int number = 0;
	fp = fopen("Borrows.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return -1;
	}
	while (fgets(buffer, sizeof(buffer), fp))
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *borrowId = strtok(buffer, ",");
		char *userId = strtok(NULL, ",");
		char *bookId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");
		returnDate[strcspn(returnDate, "\n")] = '0';

		currentBorrow.borrowId = atoi(borrowId);
		currentBorrow.userId = atoi(userId);
		currentBorrow.bookId = atoi(bookId);
		strcpy(currentBorrow.borrowDate, borrowDate);
		strcpy(currentBorrow.returnDate, returnDate);

		if (currentBorrow.userId == userID)
		{
			number += 1;
			continue;
		}
		else
			continue;
	}
	fclose(fp);
	return number;
}
int howManyUserRegistered()
{
	FILE *fp;
	char buffer[512];
	int number = 0;

	fp = fopen("Users.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return -1;
	}
	while (fgets(buffer, sizeof(buffer), fp))
	{
		number++;
	}
	fclose(fp);
	return number;
}
int howManyBooksAdded()
{
	FILE *fp;
	char buffer[512];
	int number = 0;

	fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return -1;
	}
	while (fgets(buffer, sizeof(buffer), fp))
	{
		number++;
	}
	fclose(fp);
	return number;
}
void todaysDate(char *buffer, size_t size)
{
	time_t t;
	time(&t);

	struct tm *zaman_bilgisi;	   // Bu time.h icerisinde tanimli bir structmis.
	zaman_bilgisi = localtime(&t); // 1900den bu gune gecen saniyeyi structtaki yil gun saat formatina ceviriyor.

	strftime(buffer, size, "%Y-%m-%d", zaman_bilgisi);
}
void later15Day(char *buffer, size_t size)
{

	time_t t;
	time(&t);
	t += 1296000;				   // 15 gun kac saniye ise onu ekledim
	struct tm *zaman_bilgisi;	   // Bu time.h icerisinde tanimli bir structmis.
	zaman_bilgisi = localtime(&t); // 1970 ten bu gune gecen saniyeyi structtaki yil gun saat formatina ceviriyor.

	strftime(buffer, size, "%Y-%m-%d", zaman_bilgisi);
}
int isOutdate(char *returnDate, char *currentDate, long int *howmany)
{
	struct tm tm1 = {0}, tm2 = {0};
	int year1, mon1, day1, year2, mon2, day2;

	sscanf(returnDate, "%d-%d-%d", &year1, &mon1, &day1);
	sscanf(currentDate, "%d-%d-%d", &year2, &mon2, &day2);

	tm1.tm_year = year1 - 1900; // bu yapi yili 1900den sonra tuttugu icin 2025 yerine 2025-1900 ile 125 olarak tutmak gerek.
	tm1.tm_mon = mon1 - 1;
	tm1.tm_mday = day1;
	tm2.tm_year = year2 - 1900;
	tm2.tm_mon = mon2 - 1; // bu yapi aylari 0-11 arasi tutuyor biz strignten 1-12 arasi cekiyoruz o yuzden -1 ile ceviriyoruz.
	tm2.tm_mday = day2;

	time_t time1 = mktime(&tm1); // Son teslim tarihi
	time_t time2 = mktime(&tm2); // Bugün

	long int diffDate = difftime(time2, time1) / (24 * 60 * 60);
	*howmany = diffDate;
	if (diffDate > 0)
	{
		return 1; // Son teslim tarihinden sonra (gecikmiş)
	}
	else
	{
		return 0; // Son teslim tarihinden önce veya aynı gün (gecikme yok)
	}
}
int isTaken(int ID)
{
	char buffer[512];		// used
	bool bookFound = false; // used

	FILE *fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		return -1; // Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		(void)*bookid;
		(void)*bookname;
		(void)*author;
		(void)*ctgry;
		(void)bookFound;
		if (bookid == NULL || bookname == NULL)
			continue;
		if (ID == atoi(bookid))
		{
			bookFound = true;
			if (atoi(taken) && taken != NULL)
			{
				fclose(fp);
				return 1;
			}
			else
			{
				fclose(fp);
				return 0;
			}
		}
	}
	fclose(fp);
	return -1;
}
void searchBookByName()
{
	char searchName[250]; // used
	char buffer[512];	  // used
	Book currentBook = {0};
	bool bookFound = false; // used

	printf("Aramak istediginiz kitabin ismini girin: ");
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strcspn(searchName, "\n")] = 0;

	FILE *fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		return; // Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		if (bookname == NULL)
			continue;

		if (strstr(bookname, searchName) != NULL)
		{
			bookFound = true;

			currentBook.bookId = atoi(bookid);
			strcpy(currentBook.bookName, bookname);
			if (author)
				strcpy(currentBook.author, author);
			else
				strcpy(currentBook.author, "");
			if (ctgry)
				strcpy(currentBook.ctgry, ctgry);
			else
				strcpy(currentBook.ctgry, "");
			if (taken)
				currentBook.taken = atoi(taken);
			else
				currentBook.taken = false;
			if (userId)
				currentBook.userId = atoi(userId);
			else
				currentBook.userId = -1;
			if (borrowDate)
				strcpy(currentBook.borrowDate, borrowDate);
			else
				strcpy(currentBook.borrowDate, "");
			if (returnDate)
			{
				returnDate[strcspn(returnDate, "\n")] = '\0';
				strcpy(currentBook.returnDate, returnDate);
			}
			else
				currentBook.returnDate[0] = '\0';

			printf("ID: %d, Kitap Adi: %s, Yazar: %s, Kategori: %s\n", currentBook.bookId, currentBook.bookName, currentBook.author, currentBook.ctgry);
			if (currentBook.taken)
			{
				printf("Durum: Odunc Alinmis\n");
			}
			else
			{
				printf("Durum: Mevcut \n");
			}
		}
	}
	fclose(fp);
	if (!bookFound)
	{
		printf("'%s' adinda veya iceren bir kitap bulunamadi.\n", searchName);
	}
}
void searchBookByAuthor()
{
	char searchName[250]; // used
	char buffer[512];	  // used
	Book currentBook = {0};
	bool bookFound = false; // used

	printf("Aramak istediginiz yazarin ismini girin: ");
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strcspn(searchName, "\n")] = 0;

	FILE *fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		return; // Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		if (bookname == NULL)
			continue;

		if (strstr(author, searchName) != NULL)
		{
			bookFound = true;

			currentBook.bookId = atoi(bookid);
			strcpy(currentBook.bookName, bookname);
			if (author)
				strcpy(currentBook.author, author);
			else
				strcpy(currentBook.author, "");
			if (ctgry)
				strcpy(currentBook.ctgry, ctgry);
			else
				strcpy(currentBook.ctgry, "");
			if (taken)
				currentBook.taken = atoi(taken);
			else
				currentBook.taken = false;
			if (userId)
				currentBook.userId = atoi(userId);
			else
				currentBook.userId = -1;
			if (borrowDate)
				strcpy(currentBook.borrowDate, borrowDate);
			else
				strcpy(currentBook.borrowDate, "");
			if (returnDate)
			{
				returnDate[strcspn(returnDate, "\n")] = '\0';
				strcpy(currentBook.returnDate, returnDate);
			}
			else
				currentBook.returnDate[0] = '\0';

			printf("ID: %d\n", currentBook.bookId);
			printf("Kitap Adi: %s\n", currentBook.bookName);
			printf("Yazar: %s\n", currentBook.author);
			printf("Kategori: %s\n", currentBook.ctgry);
			if (currentBook.taken)
			{
				printf("Durum: Odunc Alinmis\n");
			}
			else
			{
				printf("Durum: Mevcut \n");
			}
		}
	}
	fclose(fp);
	if (!bookFound)
	{
		printf("'%s' yazarina ait bir kitap bulunamadi.\n", searchName);
	}
}
void searchBookByCategory()
{
	char searchName[250]; // used
	char buffer[512];	  // used
	Book currentBook = {0};
	bool bookFound = false; // used

	printf("Aramak istediginiz kategorinin ismini girin: ");
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strcspn(searchName, "\n")] = 0;

	FILE *fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		return; // Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		if (bookname == NULL)
			continue;

		if (strstr(ctgry, searchName) != NULL)
		{
			bookFound = true;

			currentBook.bookId = atoi(bookid);
			strcpy(currentBook.bookName, bookname);
			if (author)
				strcpy(currentBook.author, author);
			else
				strcpy(currentBook.author, "");
			if (ctgry)
				strcpy(currentBook.ctgry, ctgry);
			else
				strcpy(currentBook.ctgry, "");
			if (taken)
				currentBook.taken = atoi(taken);
			else
				currentBook.taken = false;
			if (userId)
				currentBook.userId = atoi(userId);
			else
				currentBook.userId = -1;
			if (borrowDate)
				strcpy(currentBook.borrowDate, borrowDate);
			else
				strcpy(currentBook.borrowDate, "");
			if (returnDate)
			{
				returnDate[strcspn(returnDate, "\n")] = '\0';
				strcpy(currentBook.returnDate, returnDate);
			}
			else
				currentBook.returnDate[0] = '\0';

			printf("ID: %d\n", currentBook.bookId);
			printf("Kitap Adi: %s\n", currentBook.bookName);
			printf("Yazar: %s\n", currentBook.author);
			printf("Kategori: %s\n", currentBook.ctgry);
			if (currentBook.taken)
			{
				printf("Durum: Odunc Alinmis\n");
			}
			else
			{
				printf("Durum: Mevcut \n");
			}
		}
	}
	fclose(fp);
	if (!bookFound)
	{
		printf("'%s' kategorisinde bir kitap bulunamadi.\n", searchName);
	}
}
void searchBook()
{
	int choice;
	char choiceStr[10];
	printf("Arama yapmak istediginiz alani secin:\n");
	printf("1. Kitap Adi\n");
	printf("2. Yazar Adi\n");
	printf("3. Kategori\n");
	printf("Seciminiz: ");
	fgets(choiceStr, sizeof(choiceStr), stdin);
	choice = atoi(choiceStr);
	switch (choice)
	{
	case 1:
		searchBookByName();
		break;
	case 2:
		searchBookByAuthor();
		break;
	case 3:
		searchBookByCategory();
		break;
	default:
		printf("Gecersiz secim.\n");
		break;
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
	int ManyUser = howManyUserRegistered();
	if (ManyUser >= 100)
	{
		printf("100 Kullanicidan fazla kayit olamaz\n");
		return;
	}
	User user;
	printf("Enter your name: \n");
	fgets(user.name, sizeof(user.name), stdin);
	user.name[strcspn(user.name, "\n")] = 0;
	if (strlen(user.name) == 0)
	{
		printf("Isim bos olamaz!!!");
		printf("Kayit basarisiz!!!");
		return;
	}
	printf("Enter your email: \n");
	fgets(user.email, sizeof(user.email), stdin);
	user.email[strcspn(user.email, "\n")] = 0;
	if (strlen(user.email) == 0)
	{
		printf("Email bos olamaz!!!");
		printf("Kayit basarisiz!!!");
		return;
	}
	if (isEmailRegistered(user.email) == true)
	{
		printf("Email already registered\n");
	}
	else
	{
		printf("Enter your password: ");
		fgets(user.password, sizeof(user.password), stdin);
		user.password[strcspn(user.password, "\n")] = 0;
		if (strlen(user.password) == 0)
		{
			printf("Kayit basarisiz!!!");
			printf("Parola bos olamaz!!!");
			return;
		}
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
int loginUser(int *islogin, int *isAdmin)
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
		return -1;
	}
	*islogin = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;

		char *id = strtok(buffer, delimiter);
		char *name = strtok(NULL, delimiter);
		(void)*name;

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
					return atoi(id);
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

	return -1;
}
void addBook()
{
	int ManyBooks = howManyBooksAdded();
	if (ManyBooks >= 500)
	{
		printf("500 kitaptan fazlasi eklenemez!!\n");
		return;
	}
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
	fprintf(fp, "%d,%s,%s,%s,%d,%d,%d,%d \n", book.bookId, book.bookName, book.author, book.ctgry, book.taken, book.userId, 0, 0);
	fclose(fp);
	printf("Book adding successfully!\n");
}
void updateTakenBook(int ID, int userID, int take)
{
	char buffer[512];
	Book currentBook;
	bool bookFound = false;

	FILE *fp = fopen("Books.dat", "r");
	FILE *tempFp = fopen("temp_books.dat", "w");

	if (fp == NULL || tempFp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		if (fp)
			fclose(fp); // Eğer fp açıldıysa kapat
		if (tempFp)
			fclose(tempFp); // Eğer tempFp açıldıysa kapat
		return;				// Fonksiyondan çık
	}
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		currentBook.bookId = atoi(bookid);
		if (bookname == NULL)
		{
			fprintf(tempFp, "%s", originalLine);
			continue;
		}
		strcpy(currentBook.bookName, bookname);
		if (currentBook.bookId == ID)
		{
			bookFound = true;
			if (author)
				strcpy(currentBook.author, author);
			else
				strcpy(currentBook.author, "\0");
			if (ctgry)
				strcpy(currentBook.ctgry, ctgry);
			else
				strcpy(currentBook.ctgry, "\0");
			if (taken)
				currentBook.taken = take;
			else
				currentBook.taken = false;
			if (userId)
				currentBook.userId = userID;
			else
				currentBook.userId = -1;
			if (borrowDate)
				strcpy(currentBook.borrowDate, borrowDate);
			else
				strcpy(currentBook.borrowDate, "\0");
			if (returnDate)
			{
				returnDate[strcspn(returnDate, "\n")] = '\0';
				strcpy(currentBook.returnDate, returnDate);
			}
			else
				currentBook.returnDate[0] = '\0';

			fprintf(tempFp, "%d,%s,%s,%s,%d,%d,%s,%s\n",
					currentBook.bookId, currentBook.bookName, currentBook.author,
					currentBook.ctgry, currentBook.taken, currentBook.userId,
					currentBook.borrowDate, currentBook.returnDate);
		}
		else
		{
			// Aranan kitap bu değilse, orijinal satırı olduğu gibi geçici dosyaya yaz
			fprintf(tempFp, "%s", originalLine);
		}
	}
	// Dosyaları kapat
	fclose(fp);
	fclose(tempFp);

	if (bookFound)
	{
		if (remove("Books.dat") != 0)
		{
			printf("Eski Books.dat dosyasi silinemedi!\n");
		}
		else
		{
			if (rename("temp_books.dat", "Books.dat") != 0)
			{
				// printf("Gecici dosya Books.dat olarak yeniden adlandirilamadi!\n");
			}
			else
			{
				// printf("Books.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else
	{
		printf("'%d' ID'sine sahip bir kitap bulunamadi.\n", ID);
		if (remove("temp_books.dat") != 0)
		{
			perror("Gecici dosya silme hatasi");
		}
	}
}
void makeBorrow(int bookID, int userID)
{
	Borrow currentBorrow;
	FILE *fp;
	FILE *Idp = fopen("UniqueBorrowID.dat", "w");
	if (Idp == NULL)
	{
		printf("Dosya acilamadi");
		return;
	}
	else
	{
		fscanf(Idp, "%d", &ID);
		currentBorrow.borrowId = ID;
		ID++;
		fprintf(Idp, "%d", ID);
		fclose(Idp);
	}
	currentBorrow.userId = userID;
	currentBorrow.bookId = bookID;
	todaysDate(currentBorrow.borrowDate, sizeof(currentBorrow.borrowDate));
	later15Day(currentBorrow.returnDate, sizeof(currentBorrow.borrowDate));
	fp = fopen("Borrows.dat", "a+");
	if (fp == NULL)
	{
		printf("Dosya acilamadi");
		return;
	}
	fprintf(fp, "%d,%d,%d,%s,%s \n", currentBorrow.borrowId, currentBorrow.userId, currentBorrow.bookId, currentBorrow.borrowDate, currentBorrow.returnDate);
	fclose(fp);
}
void removeBorrow(int bookID, int userID)
{
	char buffer[512];
	Borrow currentBorrow;
	bool bookFound = false;

	FILE *fp = fopen("Borrows.dat", "r");
	FILE *tempFp = fopen("temp_borrows.dat", "w");

	if (fp == NULL || tempFp == NULL)
	{
		printf("Dosya acma hatasi!");
		if (fp)
			fclose(fp);
		if (tempFp)
			fclose(tempFp);
		return;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *borrowId = strtok(buffer, ",");
		char *userId = strtok(NULL, ",");
		char *bookId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (borrowId == NULL)
			continue;
		currentBorrow.borrowId = atoi(borrowId);
		currentBorrow.userId = atoi(userId);
		currentBorrow.bookId = atoi(bookId);

		if (borrowDate)
			strcpy(currentBorrow.borrowDate, borrowDate);
		else
			strcpy(currentBorrow.borrowDate, "\0");
		if (returnDate)
		{
			returnDate[strcspn(returnDate, "\n")] = '\0';
			strcpy(currentBorrow.returnDate, returnDate);
		}
		else
			currentBorrow.returnDate[0] = '\0';

		if (userID == currentBorrow.userId && bookID == currentBorrow.bookId)
		{
			bookFound = true;
			printf("Kitap iade edildi ID:%d\n", currentBorrow.bookId);
			continue;
		}
		else
		{
			fprintf(tempFp, "%s", originalLine);
		}
	}
	// Dosyaları kapat
	fclose(fp);
	fclose(tempFp);
	if (bookFound)
	{
		if (remove("Borrows.dat") != 0)
		{
			printf("Eski Borrows.dat dosyasi silinemedi!\n");
		}
		else
		{
			if (rename("temp_borrows.dat", "Borrows.dat") != 0)
			{
				// printf("Gecici dosya Borrows.dat olarak yeniden adlandirilamadi!\n");
			}
			else
			{
				// printf("Borrows.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else
	{
		printf("'%d' ID degerine sahip kitap iade edilemedi.\n", bookID);
		remove("temp_borrows.dat");
	}
}
void updateBook()
{
	char searchName[250];
	char buffer[512];
	Book currentBook;
	bool bookFound = false;
	int choice;
	char newValue[100];

	printf("Guncellemek istediginiz kitabin ismini girin: ");
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strcspn(searchName, "\n")] = 0;

	FILE *fp = fopen("Books.dat", "r");
	FILE *tempFp = fopen("temp_books.dat", "w");

	if (fp == NULL || tempFp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		if (fp)
			fclose(fp); // Eğer fp açıldıysa kapat
		if (tempFp)
			fclose(tempFp); // Eğer tempFp açıldıysa kapat
		return;				// Fonksiyondan çık
	}
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		currentBook.bookId = atoi(bookid);
		if (bookname == NULL)
		{
			fprintf(tempFp, "%s", originalLine);
			continue;
		}
		strcpy(currentBook.bookName, bookname);
		if (strcmp(currentBook.bookName, searchName) == 0)
		{
			bookFound = 1;
			printf("Kitap bulundu ID:%d, Ad: %s", currentBook.bookId, currentBook.bookName);

			if (author)
				strcpy(currentBook.author, author);
			else
				strcpy(currentBook.author, "\0");
			if (ctgry)
				strcpy(currentBook.ctgry, ctgry);
			else
				strcpy(currentBook.ctgry, "\0");
			if (taken)
				currentBook.taken = atoi(taken);
			else
				currentBook.taken = false;
			if (userId)
				currentBook.userId = atoi(userId);
			else
				currentBook.userId = -1;
			if (borrowDate)
				strcpy(currentBook.borrowDate, borrowDate);
			else
				strcpy(currentBook.borrowDate, "\0");
			if (returnDate)
			{
				returnDate[strcspn(returnDate, "\n")] = '\0';
				strcpy(currentBook.returnDate, returnDate);
			}
			else
				currentBook.returnDate[0] = '\0';

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
				strcpy(currentBook.bookName, newValue);
				break;
			case 2:
				strcpy(currentBook.author, newValue);
				break;
			case 3:
				strcpy(currentBook.ctgry, newValue);
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
		else
		{
			// Aranan kitap bu değilse, orijinal satırı olduğu gibi geçici dosyaya yaz
			fprintf(tempFp, "%s", originalLine);
		}
	}
	// Dosyaları kapat
	fclose(fp);
	fclose(tempFp);

	if (bookFound)
	{
		if (remove("Books.dat") != 0)
		{
			// printf("Eski Books.dat dosyasi silinemedi!\n");
		}
		else
		{
			if (rename("temp_books.dat", "Books.dat") != 0)
			{
				// printf("Gecici dosya Books.dat olarak yeniden adlandirilamadi!\n");
			}
			else
			{
				// printf("Books.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else
	{
		printf("'%s' adinda bir kitap bulunamadi.\n", searchName);
		remove("temp_books.dat");
	}
}
void deleteBook()
{
	char searchName[250];
	char buffer[512];
	Book currentBook;
	bool bookFound = false;
	// int choice;
	// char newValue[100];

	printf("Silmek istediginiz kitabin ID girin: ");
	fgets(searchName, sizeof(searchName), stdin);
	searchName[strcspn(searchName, "\n")] = 0;

	FILE *fp = fopen("Books.dat", "r");
	FILE *tempFp = fopen("temp_books.dat", "w");

	if (fp == NULL || tempFp == NULL)
	{
		printf("Dosya acma hatasi!");
		if (fp)
			fclose(fp);
		if (tempFp)
			fclose(tempFp);
		return;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *bookid = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *userId = strtok(NULL, ",");
		char *borrowDate = strtok(NULL, ",");
		char *returnDate = strtok(NULL, ",");

		if (bookid == NULL)
			continue;
		currentBook.bookId = atoi(bookid);

		if (bookname == NULL)
		{
			fprintf(tempFp, "%s", originalLine);
			continue;
		}
		strcpy(currentBook.bookName, bookname);
		if (author)
			strcpy(currentBook.author, author);
		else
			strcpy(currentBook.author, "\0");
		if (ctgry)
			strcpy(currentBook.ctgry, ctgry);
		else
			strcpy(currentBook.ctgry, "\0");
		if (taken)
			currentBook.taken = atoi(taken);
		else
			currentBook.taken = false;
		if (userId)
			currentBook.userId = atoi(userId);
		else
			currentBook.userId = -1;
		if (borrowDate)
			strcpy(currentBook.borrowDate, borrowDate);
		else
			strcpy(currentBook.borrowDate, "\0");
		if (returnDate)
		{
			returnDate[strcspn(returnDate, "\n")] = '\0';
			strcpy(currentBook.returnDate, returnDate);
		}
		else
			currentBook.returnDate[0] = '\0';

		if (strcmp(bookid, searchName) == 0)
		{
			bookFound = 1;
			removeBorrow(currentBook.bookId, currentBook.userId);
			printf("Kitap silindi ID:%d, Ad: %s\n", currentBook.bookId, currentBook.bookName);
			continue;
		}
		else
		{
			fprintf(tempFp, "%s", originalLine);
		}
	}
	// Dosyaları kapat
	fclose(fp);
	fclose(tempFp);
	if (bookFound)
	{
		if (remove("Books.dat") != 0)
		{
			printf("Eski Books.dat dosyasi silinemedi!\n");
		}
		else
		{
			if (rename("temp_books.dat", "Books.dat") != 0)
			{
				// printf("Gecici dosya Books.dat olarak yeniden adlandirilamadi!\n");
			}
			else
			{
				// printf("Books.dat dosyasi basariyla guncellendi.\n");
			}
		}
	}
	else
	{
		printf("'%s' adindaki kitap silinemedi.\n", searchName);
		remove("temp_books.dat");
	}
}
void borrowBook(int userId)
{
	int bookId;
	char bookIdStr[100];
	int istaken;
	int userhowmanyhave = 0;
	printf("Odunc almak istediginiz kitabin ID degerini girin:");
	fgets(bookIdStr, sizeof(bookIdStr), stdin);
	bookId = atoi(bookIdStr);

	istaken = isTaken(bookId);
	userhowmanyhave = userHowManyHaveBooks(userId);
	if (istaken == 0)
	{
		if (userhowmanyhave < 3)
		{
			// alinabilir
			updateTakenBook(bookId, userId, 1);
			makeBorrow(bookId, userId);
			printf("Kitap odunc alindi teslim icin 15 gununuz var!!!\n");
		}
		else
		{
			printf("Kitap odunc alamazsiniz 3 Adet kitap odunc almissiniz!\n");
		}
	}
	else if (istaken == 1)
	{
		printf("Bu kitap odunc alinamaz\n");
	}
	else if (istaken == -1)
	{
		printf("Bu kitap bulunamadi\n");
		// KITAP BULUNAMADI
	}
}
void returnBook(int userId)
{
	int bookId;
	char bookIdStr[100];
	int istaken;
	bool isUserBook = false;

	printf("Iade etmek istediginiz kitabin ID degerini girin:");
	fgets(bookIdStr, sizeof(bookIdStr), stdin);
	bookId = atoi(bookIdStr);

	istaken = isTaken(bookId);

	if (istaken == 1)
	{
		FILE *fp = fopen("Books.dat", "r");
		if (fp == NULL)
		{
			printf("Dosya acma hatasi!\n");
			return;
		}
		char buffer[512];
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			char *bookid_str = strtok(buffer, ",");
			char *bookname = strtok(NULL, ",");
			char *author = strtok(NULL, ",");
			char *ctgry = strtok(NULL, ",");
			char *taken = strtok(NULL, ",");
			char *currentUserId = strtok(NULL, ",");
			(void)*bookid_str;
			(void)*bookname;
			(void)*author;
			(void)*ctgry;
			(void)*taken;
			if (bookid_str == NULL || currentUserId == NULL)
				continue;

			int currentBookId = atoi(bookid_str);
			int userIdFromFile = atoi(currentUserId);

			if (currentBookId == bookId && userIdFromFile == userId)
			{
				isUserBook = true;
				break;
			}
		}

		fclose(fp);
		if (isUserBook)
		{
			// iade edilebilir
			removeBorrow(bookId, userId);
			updateTakenBook(bookId, -1, 0);
		}
		else
		{
			printf("Bu kitap sizin tarafinizdan odunc alinmamis!\n");
		}
	}
	else if (istaken == 0)
	{
		printf("Bu kitap zaten kutuphanede, iade edilemez\n");
	}
	else if (istaken == -1)
	{
		printf("Bu kitap bulunamadi\n");
		// KITAP BULUNAMADI
	}
}
void viewBorrowedBook(int userId)
{
	// int isUserBook= 0; KULLANILACAK
	FILE *fp = fopen("Books.dat", "r");
	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		return;
	}
	char buffer[512];
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		char *bookid_str = strtok(buffer, ",");
		char *bookname = strtok(NULL, ",");
		char *author = strtok(NULL, ",");
		char *ctgry = strtok(NULL, ",");
		char *taken = strtok(NULL, ",");
		char *currentUserId = strtok(NULL, ",");
		(void)*bookid_str;
		(void)*bookname;
		(void)*author;
		(void)*ctgry;
		(void)*taken;
		if (bookid_str == NULL || currentUserId == NULL)
			continue;

		int currentBookId = atoi(bookid_str);
		int userIdFromFile = atoi(currentUserId);

		if (userIdFromFile == userId)
		{
			printf("ID: %d, Kitap Adi: %s, Yazar: %s, Kategori: %s\n", currentBookId, bookname, author, ctgry);
			// isUserBook = true; KULLANILACAK
			continue;
		}
	}
	fclose(fp);
}
void viewOutdatedBooks()
{
	char buffer[512];
	Borrow currentBorrow;

	FILE *fp = fopen("Borrows.dat", "r");

	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		if (fp)
			fclose(fp); // Eğer fp açıldıysa kapat
		return;			// Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *borrowId_str = strtok(buffer, ",");
		char *userId_str = strtok(NULL, ",");
		char *bookId_str = strtok(NULL, ",");
		char *borrowDate_str = strtok(NULL, ",");
		char *returnDate_str = strtok(NULL, ",");
		if (borrowId_str == NULL)
			continue;

		currentBorrow.borrowId = atoi(borrowId_str);
		if (userId_str)
			currentBorrow.userId = atoi(userId_str);
		else
			currentBorrow.userId = 0;
		if (bookId_str)
			currentBorrow.bookId = atoi(bookId_str);
		else
			currentBorrow.bookId = 0;
		if (borrowDate_str)
			strcpy(currentBorrow.borrowDate, borrowDate_str);
		else
			strcpy(currentBorrow.borrowDate, "\0");
		if (returnDate_str)
		{
			returnDate_str[strcspn(returnDate_str, "\n")] = '\0';
			strcpy(currentBorrow.returnDate, returnDate_str);
		}
		else
			currentBorrow.returnDate[0] = '\0';
		if (borrowId_str == NULL || userId_str == NULL || bookId_str == NULL || borrowDate_str == NULL || returnDate_str == NULL)
		{
			printf("Hatali alan");
			continue;
		}
		else
		{
			char Today[100];
			todaysDate(Today, sizeof(Today));
			long int howmanydate;
			int isoutdate = isOutdate(returnDate_str, Today, &howmanydate);
			if (isoutdate)
			{
				// FILE *atf = fopen("Attention.dat", "a+");
				// if (atf)
				//{
				//	fprintf(atf, "%d,%d,%d,%s,%s,%li\n", currentBorrow.bookId, currentBorrow.userId, currentBorrow.borrowId, currentBorrow.borrowDate, currentBorrow.returnDate, howmanydate);

				//}
				// else
				//{
				//	printf("Uyari dosyasi acilamadi!!");
				//}
				// fclose(atf);

				printf("BookID: %d, UserID: %d, BorrowID: %d, BorrowDate: %s, ReturnDate: %s, How many days outdate: %li\n", currentBorrow.bookId, currentBorrow.userId, currentBorrow.borrowId, currentBorrow.borrowDate, currentBorrow.returnDate, howmanydate);
			}
		}
	}
	fclose(fp);
}
void saveOutDatedBooks()
{
	char buffer[512];
	Borrow currentBorrow;

	FILE *fp = fopen("Borrows.dat", "r");
	FILE *odb = fopen("OutDatedBooks.dat", "w");
	if (fp == NULL || odb == NULL)
	{
		printf("Dosya acma hatasi!\n");
		if (fp)
			fclose(fp); // Eğer fp açıldıysa kapat
		if (odb)
			fclose(odb); // Eğer odb açıldıysa kapat
		return;			 // Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *borrowId_str = strtok(buffer, ",");
		char *userId_str = strtok(NULL, ",");
		char *bookId_str = strtok(NULL, ",");
		char *borrowDate_str = strtok(NULL, ",");
		char *returnDate_str = strtok(NULL, ",");
		if (borrowId_str == NULL)
			continue;

		currentBorrow.borrowId = atoi(borrowId_str);
		if (userId_str)
			currentBorrow.userId = atoi(userId_str);
		else
			currentBorrow.userId = 0;
		if (bookId_str)
			currentBorrow.bookId = atoi(bookId_str);
		else
			currentBorrow.bookId = 0;
		if (borrowDate_str)
			strcpy(currentBorrow.borrowDate, borrowDate_str);
		else
			strcpy(currentBorrow.borrowDate, "\0");
		if (returnDate_str)
		{
			returnDate_str[strcspn(returnDate_str, "\n")] = '\0';
			strcpy(currentBorrow.returnDate, returnDate_str);
		}
		else
			currentBorrow.returnDate[0] = '\0';

		if (borrowId_str == NULL || userId_str == NULL || bookId_str == NULL || borrowDate_str == NULL || returnDate_str == NULL)
		{
			printf("Hatali alan");
			continue;
		}
		else
		{
			char Today[100];
			todaysDate(Today, sizeof(Today));
			long int howmanydate;
			int isoutdate = isOutdate(returnDate_str, Today, &howmanydate);
			if (isoutdate)
			{
				// FILE *atf = fopen("Attention.dat", "a+");
				// if (atf)
				//{
				//	fprintf(atf, "%d,%d,%d,%s,%s,%li\n", currentBorrow.bookId, currentBorrow.userId, currentBorrow.borrowId, currentBorrow.borrowDate, currentBorrow.returnDate, howmanydate);

				//}
				// else
				//{
				//	printf("Uyari dosyasi acilamadi!!");
				//}
				// fclose(atf);

				fprintf(odb, "%d,%d,%d,%s,%s\n", currentBorrow.bookId, currentBorrow.userId, currentBorrow.borrowId, currentBorrow.borrowDate, currentBorrow.returnDate);
			}
		}
	}
	fclose(fp);
	fclose(odb);
}
void GiveAttention(int userID)
{
	char buffer[512];
	Borrow currentBorrow;

	FILE *fp = fopen("OutDatedBooks.dat", "r");

	if (fp == NULL)
	{
		printf("Dosya acma hatasi!\n");
		if (fp)
			fclose(fp); // Eğer fp açıldıysa kapat
		return;			// Fonksiyondan çık
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;
		char originalLine[512];
		strcpy(originalLine, buffer);

		char *borrowId_str = strtok(buffer, ",");
		char *userId_str = strtok(NULL, ",");
		char *bookId_str = strtok(NULL, ",");
		char *borrowDate_str = strtok(NULL, ",");
		char *returnDate_str = strtok(NULL, ",");
		returnDate_str[strcspn(returnDate_str, "\n")] = '\0';
		if (borrowId_str == NULL)
			continue;

		currentBorrow.borrowId = atoi(borrowId_str);
		if (userId_str)
			currentBorrow.userId = atoi(userId_str);
		else
			currentBorrow.userId = 0;
		if (bookId_str)
			currentBorrow.bookId = atoi(bookId_str);
		else
			currentBorrow.bookId = 0;
		if (borrowDate_str)
			strcpy(currentBorrow.borrowDate, borrowDate_str);
		else
			strcpy(currentBorrow.borrowDate, "\0");
		if (returnDate_str)
		{
			strcpy(currentBorrow.returnDate, returnDate_str);
		}
		else
			currentBorrow.returnDate[0] = '\0';
		if (borrowId_str == NULL || userId_str == NULL || bookId_str == NULL || borrowDate_str == NULL || returnDate_str == NULL)
		{
			printf("Hatali alan");
			continue;
		}
		else
		{
			char Today[100];
			todaysDate(Today, sizeof(Today));
			long int howmanydate;
			int isoutdate = isOutdate(returnDate_str, Today, &howmanydate);
			if (isoutdate)
			{
				if (userID == currentBorrow.userId)
				{
					printf("%d ID degerine sahip kitabin teslim suresi %li gun gecti!!\n", currentBorrow.bookId, howmanydate);
				}
			}
		}
	}
	fclose(fp);
}
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
		viewOutdatedBooks();
		break;
	case 6:
		saveOutDatedBooks();
		break;
	case 7:
		printf("Logging out...\n");
		break;
	default:
		printf("Invalid choice. Please try again.\n");
	}
}
void userChoose(int userid)
{
	int choice = 0;
	char choiceStr[10];

	while (choice != -1)
	{
		GiveAttention(userid);
		printf("1. Search Book\n");
		printf("2. Borrow Book\n");
		printf("3. Return Book\n");
		printf("4. View Borrowed Books\n");
		printf("5. Logout\n");
		fgets(choiceStr, sizeof(choiceStr), stdin);
		choice = atoi(choiceStr);
		switch (choice)
		{
		case 1:
			searchBook();
			break;
		case 2:
			borrowBook(userid);
			break;
		case 3:
			returnBook(userid);
			break;
		case 4:
			viewBorrowedBook(userid);
			break;
		case 5:
			choice = -1;
			printf("Logging out...\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}
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
			int userid = loginUser(&islogin, &isAdmin);
			if (islogin == 1)
			{
				printf("Login successful!\n");
				printf("Welcome to the library system!\n");
				if (isAdmin == 1)
				{
					int adminChoice = 0;
					do
					{
						printf("You are an admin\n choose the admin functions\n");
						printf("1. Add User\n");
						printf("2. Add Book\n");
						printf("3. Update Book\n");
						printf("4. Delete Book\n");
						printf("5. View outDate Books\n");
						printf("6. Attempt Users\n");
						printf("7. Logout\n");

						fgets(choiceStr, sizeof(choiceStr), stdin);
						adminChoice = atoi(choiceStr);
						adminChoose(adminChoice);
					} while (adminChoice != 7);

					continue;
				}
				else
				{
					printf("You are a user\n");
					userChoose(userid);
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