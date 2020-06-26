//Fragment of Vard(v.1.2)

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

//Audio files
string originalAudioFile;
string newAudioFile;

//Programm files
const string AlgoritmTemp = "Temp//ALGTEMP.txt";//temp
const string AudioTemp = "Temp//AudioTemp.txt";//temp
const string alfPass = "Temp//alfpass.txt"; //Symbols for password

//Global
const int lenpass = 62;//len of alfPass
int position[lenpass];

void Code(string originalAudioFile, string newAudioFile, string message, string password);
void Encode(string newAudioFile, string password);
int transform(string origAfile, string Atemp);
void algoritm(string alfPass, string password, int numAudTemp);
void appAlg(int numAudTemp, string AudioTemp, string AlgoritmTemp, string message, string password);
void alttransform(string newAudioFile, string AudioTemp, int numAudTemp);
void appAltAlg(string AudioTemp, string password, int numAudTemp);


int main() {
	string Choise; char ch;
	cout << "Choise option: Code(0); Encode(1); Exit(2)" << endl << "Option:";
	cin >> Choise; 

	if (Choise.length() == 1)
		ch = Choise[0];
	else
		ch = '#'; //default

	string message, password;
	switch (ch)
	{
		case '0': { 
			cout << "Enter original file path:";
			cin  >> originalAudioFile;

			cout << "Enter new file path:";
			cin  >> newAudioFile;

			cout << endl << "Enter a message (@ and = characters are not recommended):";
			cin >> message;
			message = "@" + message + "="; 

			cout << endl << "Enter the password(from the Latin alphabet and Arabic numerals):";
			cin >> password;
			
			Code(originalAudioFile, newAudioFile, message, password); 
			cout << endl << endl << "Code successfully" << endl << endl;  main(); }

		case '1': { 
			cout << "Enter file path:";
			cin  >> newAudioFile;

			cout << endl << "Enter the password(from the Latin alphabet and Arabic numerals):";
			cin >> password;

			Encode(newAudioFile, password); 
			cout << endl << endl << "Encode successfully" << endl << endl; main(); }

		case '2': {cout << endl << "My work here is done"; return 0; }
		default: { cout << endl << endl << "Incorrect in, try again" << endl << endl; main(); }
	}
}

void Code(string originalAudioFile, string newAudioFile, string message, string password)
{
	int numAudTemp = transform(originalAudioFile, AudioTemp);
	algoritm(alfPass, password, numAudTemp);
	appAlg(numAudTemp, AudioTemp, AlgoritmTemp, message, password);
	alttransform(newAudioFile, AlgoritmTemp, numAudTemp);
}

void Encode(string newAudioFile, string password)
{
	int numAudTemp = transform(newAudioFile, AudioTemp);
	algoritm(alfPass, password, numAudTemp);
	appAltAlg(AudioTemp, password, numAudTemp);
}


int transform(string AudioFile, string AudioTemp) 
{
	ifstream infile(AudioFile, ios::binary);
	ofstream outfile(AudioTemp, ios::binary);

	char bt;
	int numAudTemp = 0;

	while (!infile.eof()) {
		infile >> bt;
		numAudTemp++;
	}

	string str = to_string(numAudTemp); int h = str.length()-1;
	int g=1;
	for (int i = 0; i < h; i++)
		 g= g*10;
	int gg = (numAudTemp / g) + 1;
	numAudTemp = g * gg;

	infile.clear();
	infile.seekg(0);

	char* buffer = new char[numAudTemp];

	infile.read(buffer, numAudTemp);

	int H; 
	for (int i = 0; i < numAudTemp; i++) {//while (!infile.eof()) {
		H = (byte)buffer[i];
		outfile << H << endl;
	}

	delete[] buffer;
	
	infile.close();
	outfile.close();

	cout << endl << "[##########....................]" << endl;
	return numAudTemp;
}

void algoritm(string alfPass, string password, int numAudTemp)
{
	ifstream alfPassAlg(alfPass, ios::binary); 

	int h = 0; char arrPass[lenpass];
	while (h<lenpass)
	{
		alfPassAlg >> arrPass[h]; 
		h++;
	}

	alfPassAlg.close();

	int algnum = (numAudTemp / lenpass) / 2;

	memset(position, 0, (sizeof(int)*lenpass) + 1);

	for (int i = 0, j = 0; i < password.length();)
	{
		if (password[i] == arrPass[j]) {
			position[i] = (j+1) * algnum; i++; j = 0;
		}
		else if (position[i] == 0) {
			for (int jj = 0; jj < (lenpass); jj++)//!!!
			{
				if (500 + position[jj + i] < numAudTemp) position[jj + i] += 500;
				else if (50 + position[jj + i] < numAudTemp) position[jj + i] += 50;
				else if (5 + position[jj + i] < numAudTemp) position[jj + i] += 5;
				else if (1 + position[jj + i] < numAudTemp) position[jj + i] += 1;
				else { cout << endl << "Too big message for this file" << endl; return; }
			}
		}
		else j++;
	}

	for (int i = 0; i < lenpass; i++)
	{
		for (int ii = 0; ii < lenpass; ii++)
		{
			if (position[i] != 0 && position[ii] != 0)
				if (i != ii)
					if (position[i] == position[ii])
					{
						if (1000 + position[ii] < numAudTemp) position[ii] += 1000;
						else if (100 + position[ii] < numAudTemp) position[ii] += 100;
						else if (10 + position[ii] < numAudTemp) position[ii] += 10;
						else if (2 + position[ii] < numAudTemp) position[ii] += 2;
						else { cout << endl << "Too big message for this file" << endl; return; }
					}
		}
	}
	cout << endl << "[####################..........]" << endl;
}

void appAlg(int numAudTemp, string AudioTemp, string AlgoritmTemp, string message, string password) {

	ifstream AudioTempApp(AudioTemp, ios::binary);
	ofstream AlgoritmTempApp(AlgoritmTemp, ios::binary);

	int tmp = 0; char zz;
	for (int i = 0; i < message.length(); i++) { 
		for (int j = (message.length() - 1); j >= (i + 1); j--) {
			if (position[j] < position[j - 1]) {
				tmp = position[j];
				position[j] = position[j - 1];
				position[j - 1] = tmp;

				if (j < message.length()) {
					zz = message[j-1];
					message[j-1] = message[j];
					message[j] = zz;
				}
			} 
		} 
	}
	
	bool booln = true;
	int N = 0;
	int B = 0;

	int i = 0, pos = 0;
	while (!AudioTempApp.eof())
	{
			booln = true;
			
			if (pos < (message.length())) 
				if (i == position[pos]) {
					B = message[pos]; // (pos[i - 1])
					booln = false;  AlgoritmTempApp << B << endl;
					AudioTempApp >> N; 
					pos++; i++;
				}
				
			if (booln = true) {
				AudioTempApp >> N;
				AlgoritmTempApp << N << endl;
				i++;
			}
	}

	AudioTempApp.close();
	AlgoritmTempApp.close();
	cout << endl << "[#########################.....]" << endl;
}



void alttransform(string newAudioFile, string AlgoritmTemp, int numAudTemp)
{
	ifstream infile(AlgoritmTemp, ios::binary);
	ofstream outfile(newAudioFile, ios::binary);

	int integear; char character; 
	while (!infile.eof()) {
		infile >> integear;
		character = (byte)integear;
		outfile << character; 
	}

	infile.close();
	outfile.close();

	cout << endl << "[##############################]" << endl;
}

void appAltAlg(string AudioTemp, string password, int numAudTemp)
{
	string mainMess = "";
	
	ifstream AudioTempApp(AudioTemp, ios::binary);
	
	int N;
	for (int i = 0; i < numAudTemp; i++) {
		AudioTempApp >> N;
		if (i == ((position[0])))
		{
			if (N != 64) { cout << "Error"; //64(@)
			return;
			}
		}
	}

	char pos; char first;

	for (int i = 0, pos = 0; i < numAudTemp; i++)
	{
		AudioTempApp >> N;
		if (pos < lenpass)
			if (i == (position[pos]+1))
			{
				if (N == 61) { i = numAudTemp;break; }//61('=')

				mainMess += N;
				pos++; i = 0;

				AudioTempApp.clear();
				AudioTempApp.seekg(0);
			}
	}
	
	AudioTempApp.close();

	cout << endl << "[##############################]" << endl;
	
	mainMess[0] = ' ';
	cout << endl << endl << "Message:" << endl << mainMess << endl << endl;
}