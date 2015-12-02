#include "adss.h"

using namespace std;

int main ()
{
	ifstream myfile;
	myfile.open (&fileName[0]);

	adss ADSS;
	ADSS.instructions();
	int count = 1;
	string line;
	
	while (getline (myfile,line)) {
		log* allData = new log;
		size_t pos = 0;
		string token = "";

		pos = line.find(delimiter);
		token = line.substr(0, pos);
		allData -> _roomNumb = token;
		line.erase(0, pos + delimiter.length());

		pos = line.find(delimiter);
		token = line.substr(0, pos);
		allData -> _date = token;
		line.erase(0, pos + delimiter.length());

		pos = line.find(delimiter);
		token = line.substr(0, pos);
		allData -> _time = token;
		line.erase(0, pos + delimiter.length());

		allData -> _cardNumb = line;
		allData->next = NULL;

		ADSS.insert(allData);
		if (myfile.eof()) break;		
	}
	myfile.close();
	string adminPwd;	
	cout<<"If you are an admin, enter admin password: ";
	cin>>adminPwd;
	if (adminPwd.compare(secrectPwd) == 0) {
		cout<<"\n===========\nADMIN MODE\n===========\n\n";
		
		char c;
		cout<<"Print log: p\nView log: v\nAdd Door Lock: a\nRemove Door Lock: r\nQuit program: q\n\n";
		
		while (1) {
			cin >>c;
			switch (c) {
				case 'p':	
					ADSS.print();
					break;
				case 'v':
					ADSS.print();
					break;
				case 'a':
					ADSS._add();
					break;
				case 'r': {
					cout<<"Which room number to remove: ";
					string key;
					cin>>key;
					ADSS._remove(key);
					break;
					}
				case 'q':
					return 0;
			}
		}

	}

	else {
		cout<<"\nYou are in a user mode\n";
		cout<<"Enter user id to get access to your room: ";
		string userID;
		cin>>userID;

		if (userID.compare("admin") == 0) cout<<"You are not admin\n";		
		else {
			if (!ADSS.exists(userID))
			cout<<"Invalid user, tatty khao\n";
		}
	}

	return 0;
}
