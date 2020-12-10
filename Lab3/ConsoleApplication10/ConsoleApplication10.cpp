/* Создайте файл каталог книг. По заданной специальности занесите в отдельный файл сведения обо всех изданиях (ФИО, название, год изда-ния).
Также указанная программа должна предоставлять пользователю ос-новные инструменты для работы с файлами. */

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct            // информация о книге
{
	char author[40];      // автор
	char name[80];        // название   
	char year_edit[5];    // год издания
	char specialty[40];   // специальность
} TBook;

char FileName[64] = "";                    // имя исходного файла
char FileName_out[64] = "";                // имя для других файлов
FILE *f, *f_out;
int item = -1;                            // переменная для выбора действия в меню
TBook empty_str = { 0 };                  // структура, эл-ты которой инициализированы и обнулены
char emp[3] = "\n";                       // строка с символом перехода на новую строку (для корректного отображения меню   
char emp1[] = "";


int InputQuery(const char *txt)              // функция для задния вопроса пользователю, ответ на который да/нет
{
	char ans;                                // переменная для ответа
	printf_s("%s", txt);
	ans = getchar();
	getchar();
	while ((ans!= 'N') && (ans!= 'n') && (ans!= 'Y') && (ans!= 'y'))        // пока ответ не будет корректным
	{
		printf_s("Error! Reenter the answer again\n");
		ans = getchar();
		getchar();
	}
	if ((ans == 'Y') || (ans == 'y'))
		return 1;
	else
		return 0;
}

int FileExist(const char *FileName)                   // функция проверки файла на существование
{
	if (fopen_s(&f, FileName, "rb") == 0)            
	{
		fclose(f);
		return 1;
	}
	else
		return 0;
}

int CorrectFileName(const char *txt)              // проверка имени файла на корректность
{
	int len;
	int res = 1;
	int i = 0;
	int j;
	char cor[8] = "/?:<>|";
	len = strlen(txt);
	if (len == 0)
		res = 0;
	else
		while ((i < len) && (res == 1))
		{
			j = 0;
			while ((j < 8) && (res == 1))
			{
				if (txt[i] != cor[j])
					j++;
				else
					res = 0;
			}
			i++;
		}
	return res;
}

int InputFileName(const char *txt, char *FileName, int IsOpen)      // ввод имени файла
{
	char name[64];
	int res=0;
	printf_s("%s\n", txt);
	gets_s(name, 64);
	while (CorrectFileName(name) == 0)
	{
		printf_s("Error! Reenter file name again \n");
		gets_s(name);
	}
	if (name == "")
		strcpy_s(FileName, 64, "");
	else
		if (IsOpen == 1)
		{
			if (FileExist(name) == 1)
			{
				res = 1;
				strcpy_s(FileName, 64, name);
			}
			else
				printf_s("File not found \n");
		}
		else
		{
			if (FileExist(name) == 0)
			{
				res = 1;
				strcpy_s(FileName, sizeof(char)*64, name);
			}
			else
			{
				if (InputQuery("File exists. Replace it? (Y/N)\n") == 1)
				{
					res = 1;
					strcpy_s(FileName, 64, name);
				}
			}
		}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////
int CheckYearMaxMin(char *year)                       // проверка на то, что год не больше и не меньше указанной даты
{
	const int MaxYear = 2020;
	const int MinYear = 1423;                        // первая печатная книга, дошедшая до нас, датирована 1423 годом
	int i = 0;
	int res = 0;
	int len = strlen(year);
	while (i < len)
	{
		res = res * 10 + (int(year[i]) - int('0'));         // перевод строки в число
		i++;
	}
	return ((res < MaxYear)&&(res > MinYear));
}


int CheckYear(char *year)
{
	int len, j;
	int i = 0;
	int res;
	char cor[11] = "0123456789";              // допустимые символы
	len = strlen(year);
	
	res = 0;
	while ((i < len) && !res)
	{
		j = 0;
		while ((j < 11) && (res == 0))
			if (year[i] == cor[j])
				res = 1;
			else
				j++;
		i++;
	}
	return (res && CheckYearMaxMin(year));
}

void InputElem(TBook *b)
{
	int ok;
	char temp_year[60];
	printf_s("Enter the author of the book \n");
	gets_s(b->author);
	printf_s("Enter the name of the book \n");
	gets_s(b->name);
	printf_s("Enter the specialty of the book \n");
	gets_s(b->specialty);
	printf_s("Enter the year of edition of the book \n");
	do
	{
		do
		{
			gets_s(temp_year, 60);
			if (strlen(temp_year) > 4)
				printf_s("Incorrect year, enter again. \n");
		} 
		while (strlen(temp_year)>4);
		strcpy_s(b->year_edit, temp_year);
		ok = CheckYear(b->year_edit);
		if (!ok)
			printf_s("Incorrect year, enter again. \n");
	} 
	while (!ok);
}

void PrintElem(TBook b)
{
	printf_s("Author: %s\n", b.author);
	printf_s("Title: %s\n", b.name);
	printf_s("Secialty: %s\n", b.specialty);
	printf_s("Year of edition: %s\n", b.year_edit);
	printf_s("\n");
}

void ChangeSomeElem(FILE *f)
{
	int n;                          // количество записей в файле
	int num = 0;                    // номер записи
	TBook b;
	int s = sizeof(TBook);
	fseek(f, 0, SEEK_END);
	n = ftell(f);                  // размер файла
	n = n / s;          // количество записей в файле   
	printf_s("Enter the number of book that you want to change \n");
	scanf_s("%d", &num);
	while (num < 1 || num > n)
	{
		printf_s("Incorrect number, please, try again (books in file %d)\n", n);
		scanf_s("%d", &num);
	}
	getchar();
	num--;
	printf_s("Enter new information about this book \n");
	fseek(f, num*s, SEEK_SET);              // устанавливаем указатель на начало записи, которую будем изменять
	InputElem(&b);
	fwrite(&b, s, 1, f);
	printf_s("Information about this book was changed \n");
	while (InputQuery("More? (Y/N)\n") == 1)
	{
		printf_s("Enter the number of book that you want to change \n");
		scanf_s("%d", &num);
		while ((num < 1) || (num > n))
		{
			printf_s("Incorrect number, please, try again (books in file %d)\n", n);
			scanf_s("%d", &num);
		}
		getchar();
		num--;
		printf_s("Enter new information about this book \n");
		fseek(f, num*s, SEEK_SET);             
		InputElem(&b);
		fwrite(&b, s, 1, f);
		printf_s("Information about this book was changed \n");
	}
}

void AddSomeElem(FILE *f)
{
	TBook b;
	int s = sizeof(TBook);
	b = empty_str;            // инициализация элементов структуры нулями         
	InputElem(&b);
	fwrite(&b, s, 1, f);
	while (InputQuery("More? (Y/N)\n") == 1)
	{
		InputElem(&b);
		fwrite(&b, s, 1, f);
	}
}

int DeleteSomeElem(FILE *f)
{
	int n;                         //количество записей в файле
	int num = 0;                   //номер записи 
	int fnewsize;
	TBook b;
	int s = sizeof(TBook);        // размер одной записи
	FILE *f_out;
	fseek(f, 0, SEEK_END);
	n = ftell(f);                //присвоение переменной размера файла
	n = n / s;                      // количество записей в файле
	fnewsize = n - s;            //размер файла, уменьшенного на одну запись
	printf_s("Enter the number of book that you want to delete \n");
	scanf_s("%d", &num);
	while ((num < 1) || (num > n)) //проверка на корректность номера записи
	{
		printf_s("Incorrect number, please, try again (books in file %d)\n", n);
		scanf_s("%d", &num);
	}
	getchar();
	if ((num == 1) && (n == 1))         // если в файле одна запись
	{
		fclose(f);
		fopen_s(&f, FileName, "wb+");      //очищение файла
		printf_s("Information about this book was deleted. Now file is empty. \n");
		return 0;
	}
	else             // если в файле больше чем одна запись 
	{
		num--;
		fseek(f, -s, SEEK_END);         // установка указателя на начало последней записи
		fread(&b, s, 1, f);             // считывание последней записи
		fseek(f, num*s, SEEK_SET);      // установка указателя на начало записи, которую необходимо удалить
		fwrite(&b, s, 1, f);            // запись последней записи на место удаляемой
		fseek(f, 0, SEEK_SET);
		fopen_s(&f_out,"1.txt", "wb+");  // открытие нового файла для записи изменённого старого файла
		for (int i = 0; i < (n - 1); i++)
		{
			fread(&b, s, 1, f);
			fwrite(&b, s, 1, f_out);
		}
		fclose(f);
		fopen_s(&f, FileName, "wb+");
		fseek(f_out, 0, SEEK_SET);
		for (int i = 0; i < (n - 1); i++)     //перезапись исходного файла
		{
			fread(&b, s, 1, f_out);
			fwrite(&b, s, 1, f);
		}
		fclose(f_out);
		remove("1.txt");                   //удаление файла с скопированными данными
		printf_s("Information about this book was deleted \n");
		return 1;
	}
}

int CheckTask(TBook b, char spec[40])              // поиск книг определённой специальности
{
	if (!strcmp(b.specialty, spec))
		return 1;
	else
		return 0;
}


///////////////////////////////////////////////////////////////////////////////////////

void CreateFile(FILE *f, const char *FileName)
{
	fopen_s(&f, FileName, "wb");                  // открытие файла для записи
	if (InputQuery("Do you want to enter new information? (Y/N) \n") == 1)      // ввод информации если необходимо
		AddSomeElem(f);
	printf_s("File was created \n");
	fclose(f);
}

void OpenFile(FILE *f, char *FileName)
{
	TBook b;
	int s = sizeof(TBook);

	errno_t err = fopen_s(&f, FileName, "r+b");
	if (err !=0)
	{
		printf_s("Cannot open the file \n");
		strcpy_s(FileName,sizeof(char)*64 ,"");
	}
	else
	{
		printf_s("File was opened \n");
		fclose(f);
	}
	
}

void PrintFile(FILE *f)
{
	fopen_s(&f, FileName, "rb");      // открытие файла для чтения
	TBook b;
	int s = sizeof(TBook);
	fseek(f, 0, SEEK_END);
	int n = ftell(f);
	if (n == 0)
		printf_s("File is empty \n");
	else
	{
		fseek(f, 0, SEEK_SET);
		if (fread(&b, s, 1, f) != 1)
			printf_s("Error. Cannot read this file. \n");
		else             // файл не пустой и его данные можно считать
		{
			n = n / s;
			fseek(f, 0, SEEK_SET);
			for (int i = 0; i < n; i++)
			{
				fread(&b, s, 1, f);
				PrintElem(b);
				fseek(f, 0, SEEK_CUR);
			}
		}
	}
	fclose(f);
}	

void PrintFileText(FILE *f)
{
	fopen_s(&f, FileName, "r");      // открытие файла для чтения
	char tmp[50];
	if (feof(f))
		printf_s("File is empty \n");
	else
	{
		while (!feof(f))
		{
			fgets(tmp, 50, f);
			printf_s("%s", tmp);
		}
	}
	fclose(f);
}

void AddElems(FILE *f)
{
	TBook b;
	int s = sizeof(TBook);
	fopen_s(&f, FileName, "r+b");        // открытие файла для добавления элементов
	fseek(f, 0, SEEK_END);
	if (fread(&b, s, 1, f) != 1)               // если данные из файла невозможно прочесть
		printf_s("Error. Cannot read this file. \n");
	else
	{
		AddSomeElem(f);
		printf_s("File was changed \n");
	}

	fclose(f);
}

void ChangeElems(FILE *f)                                     
{
	printf_s("%s\n", FileName);
	fopen_s(&f, FileName, "r+b"); // открытие файла для чтения и записи
	TBook b;
	int s = sizeof(TBook);
	fseek(f, 0, SEEK_END);
	int n = ftell(f);
	if (n == 0)
		printf_s("File is empty. \n");
	else
	{
		fseek(f, 0, SEEK_SET);
		if (fread(&b, s, 1, f) != 1)
			printf_s("Error. Cannot read this file. \n");
		else                         // файл не пустой и его данные можно считать
		{
			ChangeSomeElem(f);
			printf_s("File was changed \n");
		}
	}
	fclose(f);
}

void DeleteElems(FILE *f)
{
	TBook b;
	int res = 1;
	fopen_s(&f, FileName, "rb+");
	fseek(f, 0, SEEK_END);
	int n = ftell(f);
	int size = n % sizeof(TBook);       
	if (n == 0)            // если файл пустой
		printf_s("File is empty \n");
	else
		if (size)
			printf_s("Error. Cannot read this file.\n");
		else
		{		 // если файл не пустой и содержит корректные данные
			while ((res == 1) && (DeleteSomeElem(f) == 1))    // пока файл не пуст и требуется удалить запись
				if (InputQuery("One more? (Y/N) \n") == 0)
					res = 0;
			printf_s("File was changed \n");
		
		}
	fclose(f);                                         
}

void ConvertToText(FILE *f1, FILE *f2, const char *FileName2)
{
	TBook b;
	char tmp[30];
	fopen_s(&f1, FileName, "rb"); // открытие первого файла для чтения
	fseek(f1, 0, SEEK_END);
	int s = sizeof(TBook);
	int n = ftell(f1);
	int size = n % sizeof(TBook);
	if (n == 0)                                  // если файл пустой
		printf_s("File is empty \n");
	
	else
	{
		if (size)
			printf_s("Error. Cannot read this file.\n");
		else
		{
			strcpy_s(tmp, FileName2);
			strcat_s(tmp, ".txt");

			fopen_s(&f2, tmp, "w");        // открытие второго файла для записи
			n = n / s;                            //  количество записей в файле
			fseek(f1, 0, SEEK_SET);
			for (int i = 0; i < n; i++)
			{
				fread(&b, s, 1, f1);
				fprintf_s(f2, "Author: %s\n", b.author);           // запись автора
				fprintf_s(f2, "Title: %s\n", b.name);              // запись названия
				fprintf_s(f2, "Year of edition: %s\n", b.year_edit);    // запись года издания
				fprintf_s(f2, "Specialty : %s\n\n", b.specialty);
			}
			printf_s("Text File was created \n");
			fclose(f2);
		
		}
	}
	fclose(f1);
}

void Task(FILE *f1, FILE* f2, const char* FileName2)      // запись подходящих книг в новый файл
{
	int res;
	char spec[40];                                       // заданная специальность, по которой будут отбираться книги
	TBook b;
	fopen_s(&f1, FileName, "rb");                        // открытие файла для чтения
	fseek(f1, 0, SEEK_END);
	int s = sizeof(TBook);
	int n = ftell(f1);
	int size = n % sizeof(TBook);
	if (n == 0)                                         // если файл пустой
		printf_s("File is empty \n");
	else
	{
		if (size)
			printf_s("Error. Cannot read this file.\n");
		else
		{
			fopen_s(&f2, FileName2, "wb"); // открытие второго файла для записи
			printf_s("Enter the specialty for which you want to find the book \n");
			gets_s(spec, 40);
			res = 1;
			n = n/s;
			fseek(f1, 0, SEEK_SET);
			for (int i = 0; i < n; i++)          // проход по всем записям файла
			{
				fread(&b, s, 1, f1);

				if (CheckTask(b, spec) == 1)    // если специальность очередной книги совпадает с заданной специальностью
					fwrite(&b, s, 1, f2);                 // запись информации о книге в новый файл	
			}				
			printf_s("New file was created \n");
			fclose(f2);
		}
	}
	fclose(f1);
}

int SelectMenuItem(void)                                         // меню
{
	int res;
	int N = 8;
	int MaxItem;
	printf_s("\nSelect one of the menu items \n");
	printf_s("1 - create file\n");
	printf_s("2 - open file\n");
	MaxItem = 2;
	if (strcmp(FileName, emp) == 1)            //если используется какой-либо файл, то показываем доп. пункты меню
	{
		printf_s("3 - print binary file\n");
		printf_s("4 - print text file\n");
		printf_s("5 - add record to file\n");
		printf_s("6 - change record in file\n");
		printf_s("7 - delete record in file\n");
		printf_s("8 - conversion binary file to text file\n");
		printf_s("9 - perform the task\n");
		MaxItem = 9;
	}
	printf_s("0 - exit\n");
	scanf_s("%d", &res);                 //ввод пункта меню
	while ((res < 0) || (res > MaxItem))          //проверка на правильность выбора пункта меню
	{
		printf_s("Error! Reenter the number menu again \n");
		scanf_s("%d", &res);
	}
	getchar(); //пропуск символа конца строки
	return res;
}

int main ()
{
	while (item != 0)                 // пока не выбрано действие "выход"
	{
		item = SelectMenuItem();
		switch (item)
		{
		case 1:
			if (InputFileName("Enter name of a new file", FileName, 0) == 1)
				CreateFile(f, FileName);
			break;
		case 2:
			if (InputFileName("Enter name of the file that is already exist", FileName, 1) == 1)
				OpenFile(f, FileName);
			break;
		case 3:
			PrintFile(f);
			break;
		case 4:
			PrintFileText(f);
			break;
		case 5:
			AddElems(f);
			break;
		case 6:
			ChangeElems(f);
			break;
		case 7:
			DeleteElems(f);
			break;
		case 8:
			if (InputFileName("Enter name of a new text file", FileName_out, 0) == 1)
				ConvertToText(f, f_out, FileName_out);
			break;
		case 9:
			if (InputFileName("Enter name of a new file for the task", FileName_out, 0) == 1)
				Task(f, f_out, FileName_out);
			break;
		}
	}
	return 0;
}