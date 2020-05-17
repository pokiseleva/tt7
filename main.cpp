#include <stdlib.h>
#include <iostream>
#include <time.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;

struct Node
{
	int count; //сколько раз прокрутилась заявка
	int timeofentering; // время поступления заявки в очередь
	int numberofoffer;
	Node *Next, *Prev;
};

class List
{
public: Node *Head, *Tail;
public:
	List() :Head(NULL), Tail(NULL) {};
	~List();
	void Show();
	void Add(int timeofenter, int numberofoffer);
	void HeadToTail(int timeofenter);
	void DeleteHead();
	int Amount();
	long CountAmount();
};

List::~List()
{
	while (Head)
	{
		Tail = Head->Next;
		delete Head;
		Head = Tail;
	}
}

void List::Add(int timeofenter, int numberofoffer)
{
	Node *temp = new Node;
	temp->Next = NULL;
	temp->timeofentering = timeofenter;
	temp->count = 0;
	temp->numberofoffer = numberofoffer;

	if (Head != NULL)
	{
		temp->Prev = Tail;
		Tail->Next = temp;
		Tail = temp;
	}
	else
	{
		temp->Prev = NULL;
		Head = Tail = temp;
	}
}

void List::HeadToTail(int timeofenter)
{
	Tail->Next = Head;
	Head = Head->Next;
	Tail = Tail->Next;
	Tail->Next = NULL;
	Tail->count++;
	Tail->timeofentering = timeofenter;
}

void List::DeleteHead()
{
	Head->count++;
	if (Head->Next != NULL)
		Head = Head->Next;
	else {
		Head = NULL;
		Tail = NULL;
	}
}

int List::Amount()
{
	int amount = 1;
	if (Head == NULL)
		return 0;
	Node *temp = Head;
	while (temp != Tail && temp != NULL)
	{
		temp = temp->Next;
		amount++;
	}
	return amount;
}

long List::CountAmount()
{
	Node *temp = Head;
	long countamount = Head->count;
	while (temp != Tail)
	{
		temp = temp->Next;
		countamount += temp->count;
	}
	return countamount;
}



void List::Show()
{
	Node *temp = Head;
	while (temp != NULL)
	{
		cout << temp->numberofoffer << " ";
		temp = temp->Next;
	}
	cout << "\n";
}

int main()
{
	setlocale(0, "Russian");
	List list;
	//srand(time(0));
	int countofendings = 0; // количество завершенных на данный момент
	int k,m;
	cout<<"Введите диапозон Т1:"<<endl;
	cin>>k;
	cout<<"Введите интервал Т2:"<<endl;
	cin>>m;
	int T1=rand()%k;
	int timegone = 0;// время на данный момент, при выходе - время моделирования
	long maxtimeofwaiting = -1; // максимальное время ожидания
	int amountofrenovating = 0; // количество всех прохождений через ОА
	unsigned long timeofwaitingOA = 0; // общее время ожидания ОА
	unsigned long timeofrenovating = 0; // общее время ожидания каждого отдельного блока листа
	unsigned long timeofT1 = T1; // общее время ожидания Т1
	Node *maxi = NULL; // ячейка с макс. временем ожидания;
	int downtime=-T1;
	int numberofoffers = 0; // номер заявки
	int T2 = -1; // Для первой генерации.
	while (countofendings<1000)// НЕЛЬЗЯ делать выход по пустой очереди, ибо пустая очередь возможна, так как у нас стоят случайные T1, T2
	{
		while (T1 == 0)
		{
			// Генерируем и записываем номер в новую ячейку, заново генерируем T1
			list.Add(timegone, numberofoffers);
			numberofoffers++;
			T1 = rand()%k;
			timeofT1 += T1;
		}
		if (T2 == -1 && list.Amount() > 0) // Если список не пустой, а раньше был пустым, то заново генерируем T2(T2 == -1 - условие, что когда-то раньше список закончился)
			{
				T2 = rand()%m;
				timeofwaitingOA += T2;
			}
		while (T2 == 0) // Пускаем ячейки в ОА и затем в конец списка и заново генерируем Т2
		{
			if (timegone - list.Head->timeofentering > maxtimeofwaiting) // Проверка на максимальное ожидание
			{
				maxi = list.Head;
				maxtimeofwaiting = timegone - list.Head->timeofentering;
			}
			amountofrenovating++;
			timeofrenovating += timegone - list.Head->timeofentering;
			if(list.Head->count < 3)
				list.HeadToTail(timegone);
			else if(list.Head->count == 3) // если это завершающая обработка ОА для ячейки(четвертая по счету)
			{
				countofendings++; // обновляем счетчик количества вышедших
				if (timegone - list.Head->timeofentering > maxtimeofwaiting)
				{
					maxi = list.Head;
					maxtimeofwaiting = timegone - list.Head->timeofentering;
				}
				list.DeleteHead(); // выводим ячейку из списка(она уже 4 раза прошла)
				if (countofendings % 100 == 0)
				{
					cout << countofendings << "!" << endl;
					cout << "Кол-во вошедших заявок:" << numberofoffers << " Длина очереди:" << list.Amount() << " " << endl; // вывод количества заявок в очереди на данный момент

					cout << "Номер заявки с максимальным временем ожидания:" << maxi->numberofoffer << " Цикл обслуживания:" << maxi->count << endl << "Максимальное время пребывания в очереди:" << maxtimeofwaiting << " Среднее время пребывания:" << timeofrenovating / amountofrenovating << endl;
				}
			}
			if (list.Amount() == 0) // Проверка на то, что список пуст.
				T2 = -1;
			else
			{
				T2 = rand()%m;
				timeofwaitingOA += T2;
			}

		}
		if (T2 != -1)
			T2--;
		if(numberofoffers < 1000)
			T1--;
		timegone++;
	}
    cout << "Среднее время Т1:" << timeofT1 / numberofoffers << endl;
    downtime+=timegone-timeofwaitingOA-timeofT1 / numberofoffers;
    downtime=abs(downtime);
	cout << "Время моделирования:" << timegone << " Время простоя" << downtime << " Кол-во обработанных заявок:" << amountofrenovating;
	cout << " кол-во вошедших" << numberofoffers << " кол-во вышедших" << countofendings << endl;


}


