#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <unistd.h>

using namespace std;

string fileName = "log.txt";
string delimiter = "|";

struct log {
	string _roomNumb;
	string _date;
	string _time;
	string _cardNumb;

	log* next;

	void print() {
		cout<<"\nRoom Number: "<<_roomNumb;
		cout<<"\nDate: "<<_date;
		cout<<"\nTime: "<<_time;
		cout<<"\nCard Number: "<<_cardNumb<<endl;
	}
};

class adss {
private:
	log* head;

public:
	adss () {
		head = NULL;
	}

	~adss() {
		remove( &fileName[0] );
		ofstream myfile;
		myfile.open(&fileName[0]);
		while (head != NULL) {
			myfile<<head->_roomNumb<<delimiter;
			myfile<<head->_date<<delimiter;
			myfile<<head->_time<<delimiter;
			myfile<<head->_cardNumb;
			head = head->next;
			if (head != NULL) myfile<<endl;
		}
	}

	void print() {
		log* _head = head;
		while (_head != NULL) {
			_head->print();
			_head = _head->next;
		}
	}

	void insert(log* node) {
		log* temp = node;

		if (head == NULL) {
			head = temp;
			head -> next = NULL;
		}

		else {
			temp -> next = head;
			head = temp;
		}
	}

	void _add() {

		log* node = new log;
		cout<<"\nRoom Number: ";
		cin>>node->_roomNumb;

		cout<<"\nCard Number: ";
		cin>>node->_cardNumb;

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];
		char buffer1[80];

		time (&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer,80,"%m/%d/%Y",timeinfo);
		string date(buffer);
		node->_date = date;

		strftime (buffer1,80,"%I:%M:%S",timeinfo);
		string time(buffer1);
		node->_time = time;

		//node->_cardNumb = "admin";

		insert(node);
	}
	
	bool IsThere(string key) {
		log* _head = head;
		while (_head != NULL) {
			if (_head->_roomNumb.compare(key) == 0) return true;
			_head = _head->next;
		}	
		return false;
	}

	
	void _remove(string key) {
		if (IsThere(key)) {
			log* _head = head;
			
			if (head->_roomNumb.compare(key) == 0) {
				if (head->next == NULL) head = NULL;
				else head = head->next;
			}
			
			else {
				log* prev = _head;
				_head = _head->next;
				while (1) {
					if (_head->_roomNumb.compare(key) == 0) break;
					_head = _head->next;
					prev = prev->next;
				}
				if (_head->next == NULL) prev->next = NULL;
				else prev->next = _head->next;
			}

			cout<<"Room number "<<key<<" is removed\n";
		}
		else
			cout<<"Room Number is not in data, can't remove\n";
	}

	bool exists(string userID) {
		log* _head = head;
		while (_head != NULL) {
			if (_head->_cardNumb.compare(userID) == 0) {
				cout<<"You have entered your room "<<_head->_roomNumb<<" for 5 seconds only\n";
				sleep(5);
				cout<<"Key removed now !!!\n";
				return true;
			}
			_head = _head->next;
		}
	return false;
	}

};

string secrectPwd = "admin";
