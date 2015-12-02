#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <sstream>

using namespace std;

string fileName = "data.txt";
string fileRecord = "records.txt";
string delimiter = ";";

struct Program {
	string start, showName, desc, end;
	Program* next;
};

struct Channel {
	string chName;
	bool HD;
	Program* prog;

	Channel() { 
		prog = NULL;
		HD = false;
	}

	void insert(Program* node) {
		Program* temp = node;

		if (prog == NULL) {
			prog = temp;
			prog -> next = NULL;
		}

		else {
			temp -> next = prog;
			prog = temp;
		}
	}

	bool HDcheck(string& chName) {
		string comp = chName.substr(0,2);
		string hd = "HD";
		if (comp.compare(hd) == 0) {
			chName.erase(0,3);
			return true;
		}
		return false;
	}

	void print() {
		Program* _prog = prog;
		while (_prog != NULL) {
			cout<<"\nStart time: "<<_prog->start;
			cout<<"\nShow name: "<<_prog->showName;
			cout<<"\nChannel description: "<<_prog->desc;
			cout<<"\nEnd time: "<<_prog->end<<endl;
			_prog = _prog->next;
		}
	}

	Channel* next;
};

class device {
private:
	Channel* head;
	Channel* currChannel;
	Program* currProg;
public:
	device () {
		head = NULL;
		currChannel = NULL;
		currProg = NULL;
	}

	~device() {
		cout<<"\n\n====== SHUTTING DOWN ======\n\n";
	}

	void insert(Channel* node) {
		Channel* temp = node;

		if (head == NULL) {
			head = temp;
			head -> next = NULL;
		}

		else {
			temp -> next = head;
			head = temp;
		}
	}

	void record () {
		record(currChannel->chName, currProg->showName);
	}

	void record (string chName, string showName) {
		Channel* _head = head;
		while (_head != NULL) {
			if (_head->chName.compare(chName) == 0) {
				Program* _prog = _head->prog;
				while (_prog != NULL) {
					if (_prog->showName.compare(showName) == 0) {
						ofstream myfile;
						string filename = chName + showName + ".txt";
						myfile.open(&filename[0]);
						myfile<<"Channel: "<<chName;
						if (_head->HD) myfile<<" (HD)";
						myfile<<"\nStart time: "<<_prog->start;
						myfile<<"\nShow name: "<<_prog->showName;
						myfile<<"\nChannel description: "<<_prog->desc;
						myfile<<"\nEnd time: "<<_prog->end<<endl;
						myfile.close();
						cout<<"Program recorded\n";
					
						myfile.open(&fileRecord[0], ios::out | ios::app);
						myfile<<filename<<endl;
						myfile.close();
						return;
					}
					_prog = _prog->next;
				}
				cout<<"Error, program is not in the channel list\n";
			}
			_head = _head->next;
		}
	}
	
	void showRecords() {
		cout<<"Your recorded programs are:\n\n";
		ifstream myfile;
		myfile.open(&fileRecord[0]);
		vector <string> record;
		string line;
		while (getline (myfile,line)) record.push_back(line);
		myfile.close();
		if (record.size() == 0) {
			cout<<"No records yet\n"; 
			return;
		}
		for (int i = 0; i < record.size(); i++) {
			myfile.open(&record[i][0]);
			while (getline (myfile,line)) {
				cout<<line<<endl;
			}
			cout<<endl;
			myfile.close();
		}
	}

	bool remove(string name) {
		name += ".txt";
		ifstream myfile;
		myfile.open(&fileRecord[0]);
		vector <string> record;
		string line;
		while (getline (myfile,line)) record.push_back(line);
		myfile.close();
		if (record.size() == 0) return false;

		for (int i = 0; i < record.size(); i++) {
			if (record[i].compare(name) == 0) {
				remove(&name[0]);
				record.erase(record.begin()+i);
				ofstream myfile1;
				myfile1.open(&fileRecord[0]);
				for (int i = 0; i < record.size(); i++) {
					myfile1<<record[i]<<endl;
				}
				myfile1.close();
				return true;
			}
		}
		return false;
	}

	void watch (string chName, string showName) {
		bool channelPresent = false;
		Channel* _head = head;
		while (_head != NULL) {
			if (_head->chName.compare(chName) == 0) {
				currChannel = _head;
				Program* _prog = _head->prog;
				while (_prog != NULL) {
					if (_prog->showName.compare(showName) == 0) {
						currProg = _prog;
						channelPresent = true;
						break;
					}
					_prog = _prog->next;
				}
				if (channelPresent) break;
				cout<<"Error, program is not in the channel list\n";
				return;
			}
			if (channelPresent) break;
			_head = _head->next;
		}

		cout<<"You are watching "<<showName<<" on "<<chName<<endl;
		cout<<"Press 'r' any time to record, 'q' to return to previous menu !";
		string inp;
		while (1) {
			getline (cin,inp);
			if (inp == "q" || inp == "Q") return;
			if (inp == "r" || inp == "R") record();
			else {
				Program* _prog = currChannel->prog;
				while (_prog != NULL) {
					if (_prog->showName.compare(inp) == 0) {
						cout<<"Switching programs now\n";
						watch(chName, inp);
						return;
					}
					_prog = _prog->next;
				}
			}
		}
	}

	void printChannels() {
		Channel* _head = head;
		while (_head != NULL) {
			cout<<_head->chName;
			if (_head->HD) cout<<" (HD)";
			_head = _head->next;
			cout<<endl;
		}
	}

	void print() {
		Channel* _head = head;
		int channels = 1;
		while (_head != NULL) {
			cout<<"Channel" <<channels++<<": "<<_head->chName;
			if (_head->HD) cout<<" (HD)";
			cout<<"\nPrograms running:\n";
			_head->print();
			_head = _head->next;
			cout<<endl;
		}
	}

	bool print(string chName) {
		Channel* _head = head;
		while (_head != NULL) {
			if (_head->chName.compare(chName) == 0) {
				cout<<"Channel: "<<_head->chName;
				if (_head->HD) cout<<" (HD)";
				cout<<"\nPrograms running:\n";
				_head->print();
				return true;
			}
			_head = _head->next;
		}
		cout<<"Channel not found !\n";
		return false;
	}

	void instructions() {
		cout<<"=============================\n";
		cout<<"\tSTB SYSTEM\n=============================\n";
		cout<<"\nHow to operate device:\n";
		cout<<"List of channels will be displayed, you can choose any channel by writing";
		cout<<" the name of the channel. You can also see all the channels by inputting 'all'.";
		cout<<" Once you have selected a channel, you will be prompted";
		cout<<" to enter any program you wish to watch or record. You can record any program";
		cout<<" by entering the program name. Also if you are watching a program, you can just press";
		cout<<" 'r' to record the current program. If you are watching a program and want to switch to";
		cout<<" another program inside the same channel, you can type the name of a different program";
		cout<<" and you will be directed to a differnt program.\n";
		cout<<"Type 'records' to get a list of all the recorded programs you have recorded in the past.\n";
		cout<<"At any time, press 'q' to return to the previous menu.\n\n";
	}
};
