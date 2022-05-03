
#include<iostream>
#include<cctype>
#include<cstring>
#include<iomanip>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<vector>
using namespace std;


int menu(int&, struct Company&, bool&);
int employeeMenu(int&, struct Company&, bool&);
void adddeleteMenu(int&, struct Company&, string, bool&);
void impor(int&, struct Company&);
void validate(int&);
void validate(int&, int);
int employeeList(int&, struct Company&, bool&);
void modifyMenu(int&, struct Company&, bool&);
void ynValidate(char&);
void validateEmpNum(string&);
void changes(int&, struct Company&, string, int, bool&);
void clearAndDisplayList(int&, struct Company&, bool&);
char correctInfo();
int validateListSelect(int&, struct Company&, string, bool&);
bool isNumber(const string);

struct Employee
{
	string firstName;
	string lastName;
	string empNumber;
};
struct Company
{
	Employee employee;
	vector<Employee> employees;
};



int main()
{
	Company myCompany;
	ofstream changes;

	changes.open("NewImportfile.txt");

	bool fast_and_loose = false;
	char empfileimport;
	int currentEmployees = 0;
	cout << "Before menu, do you have an employee file to import? (Y/N)\n";
	cin >> empfileimport;
	empfileimport = toupper(empfileimport);
	ynValidate(empfileimport);

	if (empfileimport == 'Y')
	{
		impor(currentEmployees, myCompany);
	}

	menu(currentEmployees, myCompany, fast_and_loose);

	for (int count = 0; count < currentEmployees; count++)
	{
		changes << myCompany.employees[count].firstName << " " << myCompany.employees[count].lastName << " " << myCompany.employees[count].empNumber << endl;
	}
	changes.close();
	cout << "Thank you for using this program.\nYour new import file has been created. Goodbye.";

	system("pause>0");
}
int menu(int& current, struct Company& company,  bool& fandl)
{
	system("CLS");

	char choice;
	cout << "Welcome to employee menu";
	cout << "\nPlease select an option by entering the () letter.\n";
	cout << "(E)mployees (P)references E(X)it\n";
	cin >> choice;
	choice = toupper(choice);
	while (!(choice == 'E' || choice == 'P' || choice == 'X'))
	{
		cout << "Invalid input, try again:";
		cin >> choice;
		choice = toupper(choice);
	}
	if (choice == 'P')
	{
		char yn;
		cout << "Would you like to enable fast and loose mode? (Y/N)\n";
		cout << "This essentially doesn't ask if you meant what you typed.\n";
		cout << "Only for the fearless, or the person who doesn't mind correcting themself.\n";
		cin >> yn;
		ynValidate(yn);
		if (yn == 'Y')
		{
			fandl = true;
			menu(current, company, fandl);
		}

		if (yn == 'N')

			menu(current, company, fandl);
	}
	if (choice == 'E')
		if (employeeMenu(current, company, fandl))
			menu(current, company, fandl);
	if (choice == 'X')
		return 1;
}
int employeeMenu(int& current, struct Company& company, bool& fandl)
{
	system("CLS");
	char choice;
	cout << "Employees\nPlease select operation.\n";
	cout << "(V)iew current employees. (M)odify current employee. (A)dd employee. (D)elete employee.\n";
	cout << "Or (R)eturn to the main screen.\n";
	cin >> choice;
	choice = toupper(choice);
	while (!(choice == 'V' || choice == 'M' || choice == 'A' || choice == 'D' || choice == 'R'))
	{
		cout << "Invalid input, try again:";
		cin >> choice;
	}
	if (choice == 'V')
	{
		employeeList(current, company, fandl);
	}
	if (choice == 'M')
	{
		modifyMenu(current, company, fandl);
		employeeList(current, company, fandl);
	}
	if (choice == 'A')
		adddeleteMenu(current, company, "Add", fandl);
	if (choice == 'D')
		adddeleteMenu(current, company, "Delete", fandl);
	if (choice == 'R')
		return 1;
}
int employeeList(int& current, struct Company& company, bool& fandl)
{
	system("CLS");
	char choice;
	cout << "Employee list\n";
	cout << "Options:(M)odify a current employee. (A)dd employee. (D)elete employee. (R)eturn to main screen. \n";
	for (int count = 0; count < current; count++)
	{
		cout << (count + 1) << ": " << company.employees[count].firstName << " " << 
			company.employees[count].lastName << " " << company.employees[count].empNumber << endl;
	}
	cout << "Please enter option choice:";
	cin >> choice;
	choice = toupper(choice);
	while (!(choice == 'M' || choice == 'A' || choice == 'D' || choice == 'R'))
	{
		cout << "Invalid input, try again:";
		cin >> choice;
	}
	if (choice == 'M')
	{
		modifyMenu(current, company, fandl);
		employeeList(current, company, fandl);
	}
	if (choice == 'A')
		adddeleteMenu(current, company, "Add", fandl);
	if (choice == 'D')
		adddeleteMenu(current, company, "Delete", fandl);
	if (choice == 'R')
		return 1;
}
void modifyMenu(int& current, struct Company& company, bool& fandl)
{
	int listNum, modNum;
	char decision = 'N';
	clearAndDisplayList(current, company, fandl);
	do
	{

		listNum = validateListSelect(current, company, "Modify", fandl);
		cout << "What would you like to modify? 1.Name, or 2.Employee Number or 3.Return.\nPlease select the corresponding number:";
		cin >> modNum;
		while (!(modNum == 1 || modNum == 2 || modNum == 3))
		{
			cout << "Invalid input, please re-enter selection:";
			cin >> modNum;
		}
		if (modNum == 3)
			employeeList(current, company, fandl);
		else if (modNum == 2)
		{
			changes(current, company, "Number", listNum, fandl);
			decision = correctInfo();
		}
		else if (modNum == 1)
		{
			changes(current, company, "Name", listNum, fandl);
			decision = correctInfo();
		}
	} while (decision == 'Y');

}
void changes(int& current, struct Company& company, string title, int counter, bool& fandl)
{
	int listNum = counter;
	char yncorrect = 'Y';
	string newFName, newLName, newNum;
	do
	{
		cout << "Please enter updated Employee " + title;
		if (title == "Number")
		{
			cout << +"\n""It must be 6 digits:";
			cin >> newNum;
			validateEmpNum(newNum);
			cout << "Employee " + title + " updated to : " << newNum << " for Employee";
			cout << ": " << company.employees[listNum - 1].firstName << " " << company.employees[listNum - 1].lastName << endl;
		}
		if (title == "Name")
		{
			cout << ", (first *space* last): ";
			cin >> newFName >> newLName;
			cout << "Employee " + title + " updated to : " << newFName << " " << newLName << " for Employee Number: ";
			cout << company.employees[listNum - 1].empNumber << endl;
		}
		if (!(fandl)) {
			cout << "Is this correct? (Y/N)\n";
			cin >> yncorrect;
			ynValidate(yncorrect);
		}
	} while (yncorrect == 'N');
	if (title == "Number")
		company.employees[listNum - 1].empNumber = newNum;
	if (title == "Name")
	{
		company.employees[listNum - 1].firstName = newFName;
		company.employees[listNum - 1].lastName = newLName;
	}
	clearAndDisplayList(current, company, fandl);
}
void adddeleteMenu(int& current, struct Company& company, string title, bool& fandl)
{

	clearAndDisplayList(current, company, fandl);
	string lName, fName, eNum;
	int addAmount;
	char decision = 'Y';
	if (title == "Add")
	{
		cout << "How many employees do you need to add?";
		cin >> addAmount;
		current += addAmount;
		cout << "Please enter employee first name, last name, and their Employee Number all seperated by a space.\n";
		for (int count = 0; count < addAmount; count++)
		{
			do {
				cout << "Employee " << (count + 1) << ":";
				cin >> fName >> lName >> eNum;
				validateEmpNum(eNum);
				if (!(fandl)) {
					cout << "You have entered " + lName + " " + eNum + " Is this correct? (Y/N)\n";
					cin >> decision;
					ynValidate(decision);
				}
			} while (decision == 'N');
			company.employee.firstName = fName;
			company.employee.lastName = lName;
			company.employee.empNumber = eNum;
			company.employees.push_back(company.employee);
			
		}
		employeeList(current, company, fandl);
	}
	if (title == "Delete")
	{
		int listNum;
		listNum = validateListSelect(current, company, "Delete", fandl);
		company.employees.erase(company.employees.begin() + (listNum - 1));
		current--;
	}
	employeeList(current, company, fandl);
}
void ynValidate(char& decision)
{
	decision = toupper(decision);
	while (!(decision == 'N' || decision == 'Y'))
	{
		cout << "Invalid input, try again:";
		cin >> decision;
		decision = toupper(decision);
	}
}
void validateEmpNum(string& newNum)
{
	while ((!(isNumber(newNum))) || ((newNum.length()) != 6))
	{
		cout << "Invalid input, please renter Employee Number:";
		cin >> newNum;
	}
}
void clearAndDisplayList(int& current, struct Company& company, bool& fandl)
{
	system("CLS");
	for (int count = 0; count < current; count++)
	{
		cout << (count + 1) << ": " << company.employees[count].firstName << " " << company.employees[count].lastName << " " << company.employees[count].empNumber << endl;
	}
}
char correctInfo()
{
	char yn;
	cout << "Would you like to make any more changes? Enter N to return to employee list. (Y/N)\n";
	cin >> yn;
	ynValidate(yn);
	return yn;

}
void impor(int& current, struct Company& company)
{
	ifstream build;
	build.open("employee.txt");
	bool empty = false;
	while (!(build))
	{
		char retry;
		cout << "File did not open correctly, please check file and retry.\n";
		cout << "Retry? (Y/N):";
		cin >> retry;
		retry = toupper(retry);
		while (!(retry == 'Y' || retry == 'N'))
		{
			cout << "Incorrect input, try again.\n";
			cin >> retry;
		}
		if ((retry == 'Y'))
		{
			build.open("employee.txt");
			continue;
		}
	}
	while (!empty)
	{
		string fname, lname, empnum;
		build >> fname >> lname >> empnum;
		if (!(build.good()))
			empty = true;
		
		company.employee.firstName = fname;
		company.employee.lastName = lname;
		company.employee.empNumber = empnum;
		company.employees.push_back(company.employee);
		current++;
	}

	build.close();

}
void validate(int& number)
{
	while (number < 1)
	{
		cout << "Invalid input, please re-enter amount:";
		cin >> number;
	}
}
void validate(int& number, int total)
{
	while (number < 1 || number>total)
	{
		cout << "Invalid input, please re-enter list number:";
		cin >> number;
	}
}
int validateListSelect(int& current, struct Company& company, string title, bool& fandl)
{
	int listNum;
	char yn = 'Y';
	do
	{
		cout << "Which employee would you like to " + title + "? \nPlease enter their list number : ";
		cin >> listNum;
		validate(listNum, current);
		if (!(fandl)) {
			cout << "You have selected " << company.employees[listNum - 1].firstName << " " << company.employees[listNum-1].lastName << ". Is this correct? (Y/N)";
			cin >> yn;
			ynValidate(yn);
		}
	} while (yn == 'N');
	return listNum;
}
bool isNumber(const string str)
{
	for (char  c : str) {
		if (!isdigit(c))
			return false;
	}
	return true;
}