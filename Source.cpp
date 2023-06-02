#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Equation {
public:
	double left_border;
	double right_border;
	double step; //шаг вычисления(точность)
	double (*f_ptr)(double) { nullptr }; // указатель на функцию f(x) в уравнении

	Equation() {}

	Equation(double left_border_, double right_border_, double step_, double (*f_ptr_)(double)) :
		left_border(left_border_), right_border(right_border_), step(step_), f_ptr(f_ptr_) {}

	Equation(const Equation& e) {
		left_border = e.left_border;
		right_border = e.right_border;
		step = e.step;
		f_ptr = e.f_ptr;
	}
	Equation& operator=(const Equation& e) {
		left_border = e.left_border;
		right_border = e.right_border;
		step = e.step;
		f_ptr = e.f_ptr;
		return *this;
	}

	friend ostream& operator<<(ostream& os, const Equation& e);
	friend istream& operator>>(istream& os, Equation& e);

	//решение уравнения
	double getDes() const {
		for (double i = left_border; i <= right_border; i += step)
			if (f_ptr(i) < step)
				return i;
	}
	
	~Equation() {}
};
ostream& operator<<(ostream& os, const Equation& e) {
	return os <<fixed << setprecision(2) << "Левая граница: " << e.left_border << " Правая граница: "
		<< e.right_border << " Шаг:" << e.step <<" Решение: "<< e.getDes() << '\n'; // как эти гении хотят вывести функцию в стандратный ввод / вывод я не знаю)
}
istream& operator>>(istream& is, Equation& e) {
	return is >> e.left_border >> e.right_border >> e.step; //как эти гении хотят ввести функцию в стандратный ввод/вывод я не знаю)
}

double f(double x) { //пример функции f(x)
	return x * x;
}

template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:
	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}
	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}
	T getInfo() { return info; }
	void setInfo(T value) { info = value; }
	Element<T>* getNext() { return next; }
	void setNext(Element<T>* value) { next = value; }
	Element<T>* getPrev() { return prev; }
	void setPrev(Element<T>* value) { prev = value; }
	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el);
};
template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}

//Исключение неверного индекса
class IndexOutOfRangeException : public exception {
protected:
	string error_info;
public:
	IndexOutOfRangeException(string in) : exception() {
		error_info = in;
	}
	void print() const {
		cout << this->error_info << endl;
	}
};

template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}
	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(T value) = 0;
	Element<T>& operator[](int index)
	{
		if (index < 0 || index >= count) throw IndexOutOfRangeException(string("index = " + to_string(index) + ", count of elements = " + to_string(count)));
		Element<T>* current = head;
		for (int i = 0; current != NULL && i < index; current = current->getNext(), i++);
			return *current;
	}
	virtual bool isEmpty() {
		return (LinkedList<T>::count == 0);
	}
	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);
	virtual ~LinkedList()
	{
		cout << "\nBase class destructor";
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}
	}
};
template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current;
	for (current = el.head; current != NULL; current = current->getNext())
		s << *current << "; ";
	return s;
}
template<class T>
bool operator ==(const T& t1, const T& t2) {
	return t1.getDes() == t2.getDes();
}
template<class T>
class ReversedStack : public LinkedList<T>
{
public:
	ReversedStack<T>() : LinkedList<T>() {}
	virtual ~ReversedStack() { cout << "\nReversedReversedStack class destructor"; }
	virtual Element<T>* pop()
	{
		if (LinkedList<T>::head == NULL) return NULL;
		Element<T>* res = LinkedList<T>::head;
		if (LinkedList<T>::tail == LinkedList<T>::head)
		{
			LinkedList<T>::count = 0;
			res = LinkedList<T>::head;
			LinkedList<T>::tail = LinkedList<T>::head = NULL;
			return res;
		}
		Element<T>* current;
		for (current = LinkedList<T>::tail; current->getPrev()
			!= LinkedList<T>::head; current = current->getPrev());
		current->setPrev(NULL);
		LinkedList<T>::count--;
		LinkedList<T>::head = current;
		return res;
	}
	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		if (LinkedList<T>::head != NULL)
		{
			LinkedList<T>::head->setPrev(newElem);
			LinkedList<T>::head = LinkedList<T>::head->getPrev();
		}
		else
			LinkedList<T>::head = LinkedList<T>::tail = newElem;
		LinkedList<T>::count++;
		return LinkedList<T>::head;
	}
	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		if (previous == NULL) return push(value);
		Element<T>* inserted = new Element<T>(value);
		Element<T>* next = previous->getNext();
		previous->setNext(inserted);
		inserted->setNext(next);
		LinkedList<T>::count++;
		return inserted;
	}
	virtual void remove(T value, Element<T>* elem = NULL)
	{
		if (elem == NULL) return;
		Element<T>* next = elem->getNext();
		Element<T>* prev = elem->getPrev();
		if (next != NULL)
			next->setPrev(prev);
		if (next != NULL)
			next->setNext(next);
		LinkedList<T>::count--;
	}
	virtual void search(T value) //6.2
	{
		for (Element<T>* current = LinkedList<T>::head; current != NULL;
			current = current->getNext())
		{
			if (value == current->getInfo())
				cout << value<<'\n';
		}
	}
	void save_to(ofstream& s) { //6.3
		s << this->count << endl;
		Element<T>* cur = this->head;
		for (int i = 0; i < this->count; ++i) {
			s << (*cur) << endl;
			cur = cur->next;
		}
	}

	void load_from(ifstream& s) {
		this->clear();

		int count_of_elements;
		string line;
		getline(s, line);
		std::istringstream iss(line);
		iss >> count_of_elements;
		for (int i = 0; i < count_of_elements; ++i) {
			T cur;
			s >> cur;
			this->push(cur);
		}

	}
};
class my_class
{
public:
	int data;
	my_class(int v = 0) {
		data = v; cout << "\nmy_class constructor";
	}
	my_class(const my_class& m) {
		data = m.data; cout <<
			"\nmy_class copy constructor";
	}
	~my_class() { cout << "\nmy_class destructor"; }
	friend ostream& operator<<(ostream& s, my_class& value);
};
ostream& operator<<(ostream& s, my_class& value)
{
	s << value.data;
	return s;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	// ДРУЗЬЯ, У ВАС БЕДЫ С ВВОДОМ ФУНКЦИИ В ФАЙЛ ИЛИ СТАНДАРТНЫЙ ВЫВОД!!!!!!!!!

	cout << "Equation object\n";
	ReversedStack<Equation> RS;
	Equation e1(0, 100, 0.5, f);
	Equation e2(-100, 100, 0.5, f);
	Equation e3(-100, 0, 0.5, f);
	RS.push(e1);
	RS.push(e2);
	RS.push(e3);
	//cout << "\n" << e1 << "\n" << e2 << "\n" << e3 << '\n';
	for (; !RS.isEmpty(); cout << RS.pop()->getInfo() << ",");
	cout << "\n";

	return 0;


}
