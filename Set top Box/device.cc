#include "device.h"

using namespace std;

int main ()
{
	cout << "\033[2J\033[1;1H"; // Screen clear
	ifstream myfile;
	myfile.open (&fileName[0]); // myfile.open("1.txt");

	int count = 0;
	string line;
	device Device;

	while (getline (myfile,line)) {

		Channel* channel = new Channel;
		istringstream ss(line);
		vector <string> record;

		while (ss)
		{
			string str;
			if (!getline(ss, str, ';')) break;
			record.push_back(str);
		}

		int shows = (record.size() - 1) /4;

		channel->chName = record[0];
		if (channel->HDcheck(channel->chName)) channel->HD = true;

		size_t pos = channel->chName.find(" ");
		channel->chName.erase(0, pos + 1);

		for (int i = 0; i < shows; i++) {
			Program* _p = new Program;
			string str = record[i*4 + 1];
			size_t pos = str.find(' ', 1);
			str.erase(0, pos + 1);
			_p->start = str;

			str = record[i*4 + 2];
			pos = str.find(' ', 1);
			str.erase(0, pos + 1);
			_p->showName = str;

			str = record[i*4 + 3];
			pos = str.find(' ', 1);
			str.erase(0, pos + 1);
			_p->desc = str;

			str = record[i*4 + 4];
			pos = str.find(' ', 1);
			str.erase(0, pos + 1);
			_p->end = str;
		
			channel->insert(_p);
		}

		Device.insert(channel);
	}

	myfile.close();
	Device.instructions();
	
	cout<<"List of channels:\n";
	Device.printChannels();

	string inpCh, inpRec;
	
	while (1) {
		cout<<"\nWhich channel do you want to see ?";
		cout<<"\nType channel name or type 'all' to print all channels and all programs.\n";
		cout<<"(Type 'records' to preview your recorded programs)";
		cout<<"(Press 'q' or 'Q' anytime to turn off the TV)\n";
		getline (cin, inpCh);
		if (inpCh == "q" || inpCh == "Q") return 0;
		if (inpCh == "records") {
			Device.showRecords();
			cout<<"\nDo you want to delete any recorded program from memory? (y/n): ";
			string y_n;
			getline(cin,y_n);
			if (y_n == "y" or y_n == "Y") {
				cout<<"Enter show name and program name to delete the record ";
				cout<<"(no spaces between the names): ";
				getline(cin,y_n);
				if (Device.remove(y_n)) cout<<"Record removed successfully\n";
				else cout<<"Record doesn't exist or you must have typed in the wrong name\n";
			}
		}
		else {
			if (inpCh == "all") {
				Device.print();
				cout<<"Enter channel name: ";
				getline (cin,inpCh);
			}
			else Device.print(inpCh);

			
			cout<<"Do you want to record a show(r) or watch a show(w) ? ";
			getline (cin,inpRec);
			if (inpRec == "q" || inpRec == "Q") return 0;
			if (inpRec == "r" || inpRec == "R") {
				cout<<"Which show do you want to record? ";
				getline (cin,inpRec);
				if (inpRec == "q" || inpRec == "Q") return 0;
				Device.record(inpCh, inpRec);
			}
			
			if (inpRec == "w" || inpRec == "W") {
				cout<<"Which show do you want to watch? ";
				getline (cin,inpRec);
				if (inpRec == "q" || inpRec == "Q") return 0;
				Device.watch(inpCh, inpRec);
			}
		}
	}

	cout<<endl;
	return 0;
}
