#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Student {
	char name[256];
	Student* ptrNext;
	Student* ptrPrev;
};

Student* head = NULL; 
Student* tail = NULL;
int n = 0;
bool flag1 = false;

Student* createRight(int);								// указатель на след элемент
Student* createLeft();									// указатель на пред элемент
void insertNode(const int, int&);						// Добавление элемента
void deleteNode(const int, int&);						// удалить элемент из структуры
void fill();										// заполнение списка (полностью)
void output();											// вывод списка
void outputRight();										// вывод с конца (для откладки)
void destroy();											// уничтожить список
void writeToFile(FILE*);								// запись в файл
void addDoubles(const int, const int);					// Добавление элементов по K справа и слева от элемента
void restoreFromFile(FILE* file);						// вывод из файла
void menu(const int c);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true) {
		system("pause");
		system("cls");
		cout << "Здравствуйте, что вы хотите сделать?" << endl;
		cout << "1. Создать список" << endl;
		cout << "2. Добавить по K элементов к элементу на позиции pos слева и справа" << endl;
		cout << "3. Удалить элемент из списка" << endl;
		cout << "4. Напечатать список" << endl;
		cout << "5. Записать список в файл" << endl;
		cout << "6. Уничтожить список" << endl;
		cout << "7. Восстановить список из файла" << endl; cout << endl;
		cout << "Введите c" << endl;
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}

Student* createRight(const int n) {		// первоначальное создание списка вправо
	Student* first = NULL;
	Student* ptr;
	for (int i = n; i > 0; i--) {
		ptr = new(Student);
		ptr->ptrNext = first;
		first = ptr;
	}
	return first;
}
Student* createLeft() {		// создание списка влево на основе созданного
	Student* second = NULL;
	for (Student* ptr = head; ptr != NULL;) {
		ptr->ptrPrev = second;
		second = ptr;
		ptr = ptr->ptrNext;
	}
	return second;
}
void deleteNode(const int pos, int& n) {
	int c;
	if (pos > n) { cout << "Ошибка 1"; exit(1); }
	else if (pos == 1 && pos == n) { c = 0; }					// если один элемент
	else if (pos == 1) { c = 1; }								// если первый элемент
	else if (pos == n) { c = 2; }								// если последний элемент
	else if (pos > 1 && pos < n && pos * 2 <= n) c = 3;			// если центральный элемент и элемент в левой половине (заходит через первый элемент)
	else if (pos > 1 && pos < n && pos * 2 > n) c = 4;			// если центральный элемент и элемент в правой половине (заходит через последний элемент)
	else exit(2);
	Student* ptr = head;
	Student* ptrLast = tail;

	switch (c) {
	case 0:
	{
		delete ptr; ptr = NULL;
		ptrLast = NULL;
		head = NULL; tail = NULL;
		break;
	}
	case 1:
	{
		head = ptr->ptrNext;
		delete ptr; ptr = NULL;
		head->ptrPrev = NULL;
		break;
	}
	case 2:
	{
		tail = ptrLast->ptrPrev;		// указатель на последний элемент
		tail->ptrNext = NULL;
		delete ptrLast; ptrLast = NULL;
		break;
	}
	case 3:
	{
		for (int i = 1; i < pos - 1; i++) {		// операции до элемента предшествующего данному
			ptr = ptr->ptrNext;
		}
		Student* ptr1 = ptr->ptrNext->ptrNext;	//элемент после данного
		delete ptr->ptrNext;					// удаление выбранного элемента
		ptr->ptrNext = ptr1;
		ptr1->ptrPrev = ptr;
		break;
	}
	case 4:
	{
		for (int i = n; i > pos + 1; i--) {			// операции до элемента после данного
			ptrLast = ptrLast->ptrPrev;
		}
		Student* ptr1 = ptrLast->ptrPrev->ptrPrev;	// элемент перед данным
		delete ptrLast->ptrPrev;
		ptrLast->ptrPrev = ptr1;
		ptr1->ptrNext = ptrLast;
		break;
	}
	}
	n--;
}
void insertNode(const int pos, int& n) {	
	int c;
	if (pos > n) { c = 1; }					// Добавление элемента в конец
	else if (pos == 1) { c = 0; }			// если вставка на первый элемент
	else if (pos * 2 <= n) { c = 2; }		// вставляет на позицию элемента, отодвигая его
	else if (pos * 2 > n) { c = 3; }		// вставляет на позицию элемента, отодвигая его
	
	char name[256];
	cout << "Имя?" << endl;
	gets_s(name, 256);
	
	Student* ptr = head;
	Student * ptrLast = tail;
	Student* temp = new(Student);		// вводимая в список структура
	strcpy_s(temp->name, name);

	switch (c) {
	case 0:
	{
		ptr->ptrPrev = temp;
		head = temp;
		temp->ptrNext = ptr;
		temp->ptrPrev = NULL;
		break;
	}
	case 1:
	{
		ptrLast->ptrNext = temp;		
		temp->ptrPrev = ptrLast;
		temp->ptrNext = NULL;
		tail = temp;					// переместил указатель конца
		break;
	}
	case 2:					
	{
		int i = 1;
		while (i < pos - 1) {				// нахожусь на элемент раньше нужного
			ptr = ptr->ptrNext;
			i++;
		}
		Student* tempValue = ptr->ptrNext;	// получает ссылку на элемент на место которого вставляем
		ptr->ptrNext = temp;				// ptr теряет ссылку на следующий элемент и получает новую ссылку
		tempValue->ptrPrev = temp;			// записываем в значение элемента ссылку на предыдущий
		temp->ptrNext = tempValue;			// в веденную структуру записывается ссылка следующего за ней элемента
		temp->ptrPrev = ptr;
		break;
	}
	case 3:
	{
		int i = n;
		while (i > pos) {					// нахожусь на элемент позже нужного
			ptrLast = ptrLast->ptrPrev;
			i--;
		}

		Student* tempValue = ptrLast->ptrPrev;		// получает ссылку на элемент на место которого вставляем
		ptrLast->ptrPrev = temp;					// ptr теряет ссылку на следующий элемент и получает новую ссылку
		temp->ptrNext = ptrLast;					// в веденную структуру записывается ссылка следующего за ней элемента
		temp->ptrPrev = tempValue;
		tempValue->ptrNext = temp;
		if (pos == n) ptrLast->ptrNext = NULL;
		break;
	}

	}
	++n;

}
void fill() {
	cout << "Введите индетификатор n пользователей: " << endl; cin.ignore(); 
	for (Student *ptr = head; ptr != NULL;) {
		char nameT[255 + 1];
		cin.getline(nameT, 255);
		strcpy_s(ptr->name, nameT);
		ptr = ptr->ptrNext;
	}
}
void output() {
	cout << "Ваш список: " << endl;
	int counter = 1;
	for (Student* ptr = head; ptr != NULL;) {
		cout << counter++ << " " << ptr->name << endl;
		ptr = ptr->ptrNext;
	}
}
void outputRight() {
	cout << "Ваш список: " << endl;
	int counter = n;
	for (Student* ptr = tail; ptr != NULL;) {
		cout << counter-- << " " << ptr->name << endl;
		ptr = ptr->ptrPrev;
	}
}
void addDoubles(const int pos, int K) {
	int c = pos;
	int i = 0;
	while (i < K) {
		insertNode(c, n);
		c++;
		i++;
	}

	c += 1;
	i = 0;
	while (i < K) {
		insertNode(c, n);
		c++;
		i++;
	}
}
void writeToFile(FILE* file) {
	if ((file = fopen("myFile.txt", "w+")) == NULL) {
		cout << "Ошибка при записи файла" << endl; exit(3);
	}
	Student* ptr = head;
	while (ptr != NULL) {
		fwrite(ptr->name, sizeof(ptr), 1, file);
		ptr = ptr->ptrNext;
	}
	fclose(file);
}
void destroy() {
	Student* ptr = head;
	while (ptr != NULL) {
		Student* ptr1 = ptr;
		ptr = ptr->ptrNext;
		delete ptr1;
	}
	head = NULL;
	tail = NULL;
	n = 0;
}
void restoreFromFile(FILE* file) {
	if ((file = fopen("myFile.txt", "r+")) == NULL) {
		cout << "Ошибка при открытии файла" << endl; exit(4);
	}

	Student* ptr = new(Student);
	fread(ptr->name, sizeof(ptr), 1, file);
	head = ptr;
	ptr->ptrPrev = NULL;
	Student* last = ptr;
	if (!feof(file)) {
		while (!feof(file)) {
			last = ptr;
			ptr->ptrNext = new(Student);

			ptr = ptr->ptrNext;
			ptr->ptrPrev = last;
			fread(ptr->name, sizeof(ptr), 1, file);
			n++;
		}

		delete last->ptrNext; last->ptrNext = NULL;
		tail = last;
	}
	else {
		delete ptr; head = NULL; tail = NULL;
	}
	fclose(file);
}
void menu(const int c) {
	FILE* file = NULL;

	if ((c == 1 || c == 7) && flag1) {					// для случая, когда список уже создан
		cout << "Вы уже создали список" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !flag1) {				// для случая, когда список еще не создан
		cout << "Создайте/ восстановите список" << endl;
		return;
	}
	else if (c >= 2 && c <= 5 && n == 0) { cout << "Список пуст." << endl; return; }
	switch (c) {
	case 1:
	{
		flag1 = true;
		cout << "Введите количество структур списка: ";
		cin >> n;
		while (n <= 0) {
			cout << "Нельзя ввести столько структур" << endl;
			cin >> n;
		}
		head = createRight(n);	// Создание и заполнение
		tail = createLeft();
		fill();
		break;
	}
	case 2:
	{
		cout << "Введите позицию для добавления и количество элементов для добавления" << endl;
		int pos, K;
		cin >> pos >> K;
		cin.get();									// нормализация буфера для работы без перебоев
		while (pos > n) { cout << "В списке всего " << n << "позиций" << endl; cin >> pos; }
		addDoubles(pos, K);			// добавление элементов с двух сторон
		output();
		break;
	}
	case 3:
	{
		int pos;
		cout << "Введите позицию для удаления" << endl;
		cin >> pos;
		while (pos > n) { cout << "В списке всего " << n << " позиций" << endl; cin >> pos; }
		deleteNode(pos, n);
		output();
		break;
	}
	case 4:
	{
		output();
		cout << endl;
		outputRight();
		break;
	}
	case 5:
	{
		writeToFile(file);
		break;
	}
	case 6:
	{
		destroy();
		cout << "Список удален" << endl;
		flag1 = false;
		break;
	}

	case 7:
	{
		cout << "Восстановление списка из файла" << endl;
		int n1 = n;
		restoreFromFile(file);
		if (n - n1 == 0) cout << "Нечего восстанавливать" << endl;
		else flag1 = true;
		break;
	}
	default:
	{
		cout << "Выход из меню" << endl;
		destroy();
		exit(777);
	}
	}
}