#include <stdlib.h>
#include <iostream>
#include <time.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;

struct Node
{
	int count; //������� ��� ������������ ������
	int timeofentering; // ����� ����������� ������ � �������
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
	int countofendings = 0; // ���������� ����������� �� ������ ������
	int k,m;
	cout<<"������� �������� �1:"<<endl;
	cin>>k;
	cout<<"������� �������� �2:"<<endl;
	cin>>m;
	int T1=rand()%k;
	int timegone = 0;// ����� �� ������ ������, ��� ������ - ����� �������������
	long maxtimeofwaiting = -1; // ������������ ����� ��������
	int amountofrenovating = 0; // ���������� ���� ����������� ����� ��
	unsigned long timeofwaitingOA = 0; // ����� ����� �������� ��
	unsigned long timeofrenovating = 0; // ����� ����� �������� ������� ���������� ����� �����
	unsigned long timeofT1 = T1; // ����� ����� �������� �1
	Node *maxi = NULL; // ������ � ����. �������� ��������;
	int downtime=-T1;
	int numberofoffers = 0; // ����� ������
	int T2 = -1; // ��� ������ ���������.
	while (countofendings<1000)// ������ ������ ����� �� ������ �������, ��� ������ ������� ��������, ��� ��� � ��� ����� ��������� T1, T2
	{
		while (T1 == 0)
		{
			// ���������� � ���������� ����� � ����� ������, ������ ���������� T1
			list.Add(timegone, numberofoffers);
			numberofoffers++;
			T1 = rand()%k;
			timeofT1 += T1;
		}
		if (T2 == -1 && list.Amount() > 0) // ���� ������ �� ������, � ������ ��� ������, �� ������ ���������� T2(T2 == -1 - �������, ��� �����-�� ������ ������ ����������)
			{
				T2 = rand()%m;
				timeofwaitingOA += T2;
			}
		while (T2 == 0) // ������� ������ � �� � ����� � ����� ������ � ������ ���������� �2
		{
			if (timegone - list.Head->timeofentering > maxtimeofwaiting) // �������� �� ������������ ��������
			{
				maxi = list.Head;
				maxtimeofwaiting = timegone - list.Head->timeofentering;
			}
			amountofrenovating++;
			timeofrenovating += timegone - list.Head->timeofentering;
			if(list.Head->count < 3)
				list.HeadToTail(timegone);
			else if(list.Head->count == 3) // ���� ��� ����������� ��������� �� ��� ������(��������� �� �����)
			{
				countofendings++; // ��������� ������� ���������� ��������
				if (timegone - list.Head->timeofentering > maxtimeofwaiting)
				{
					maxi = list.Head;
					maxtimeofwaiting = timegone - list.Head->timeofentering;
				}
				list.DeleteHead(); // ������� ������ �� ������(��� ��� 4 ���� ������)
				if (countofendings % 100 == 0)
				{
					cout << countofendings << "!" << endl;
					cout << "���-�� �������� ������:" << numberofoffers << " ����� �������:" << list.Amount() << " " << endl; // ����� ���������� ������ � ������� �� ������ ������

					cout << "����� ������ � ������������ �������� ��������:" << maxi->numberofoffer << " ���� ������������:" << maxi->count << endl << "������������ ����� ���������� � �������:" << maxtimeofwaiting << " ������� ����� ����������:" << timeofrenovating / amountofrenovating << endl;
				}
			}
			if (list.Amount() == 0) // �������� �� ��, ��� ������ ����.
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
    cout << "������� ����� �1:" << timeofT1 / numberofoffers << endl;
    downtime+=timegone-timeofwaitingOA-timeofT1 / numberofoffers;
    downtime=abs(downtime);
	cout << "����� �������������:" << timegone << " ����� �������" << downtime << " ���-�� ������������ ������:" << amountofrenovating;
	cout << " ���-�� ��������" << numberofoffers << " ���-�� ��������" << countofendings << endl;


}


