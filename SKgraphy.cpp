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
const string algoritmTemp = "Temp//AlgTemp";//temp
const string audioTemp = "Temp//AudioTemp";//temp
const string alfPass = "alfass.txt"; //Symbols for password

//Global
const int lenPass = 62;//len of alfPass
int position[lenPass];

//Functions
void Code(string originalAudioFile, string newAudioFile, string message, string password);
void Encode(string newAudioFile, string password);
int  Transform(string origAfile, string Atemp);
void Algoritm(string alfPass, string password, int numAudTemp);
void ApplyAlg(int numAudTemp, string audioTemp, string algoritmTemp, string message, string password);
void AltTransform(string newAudioFile, string audioTemp, int numAudTemp);
string ApplyAltAlg(string audioTemp, string password, int numAudTemp);


void Choice();

int main() {
	Choice();
	return 0;
}

void Code(string originalAudioFile, string newAudioFile, string message, string password)
{
		cout << "Transform file to bites" << endl;
	int numAudTemp = Transform(originalAudioFile, audioTemp);	//get file lenght
	
		cout<< "Calculate" << endl;
	Algoritm(alfPass, password, numAudTemp);
	
		cout << "Apply algoritm" << endl;
	ApplyAlg(numAudTemp, audioTemp, algoritmTemp, message, password);
	
		cout << "Transform file back" << endl;
	AltTransform(newAudioFile, algoritmTemp, numAudTemp);//Transform back
}

void Encode(string newAudioFile, string password)
{
		cout << "Transform file to bites" << endl;
	int numAudTemp = Transform(newAudioFile, audioTemp);	//get file lenght

		cout << "Apply algoritm" << endl;
	Algoritm(alfPass, password, numAudTemp);		

		cout << "Get message" << endl;
	string m = ApplyAltAlg(audioTemp, password, numAudTemp);		
	cout << "Message: " << m << endl;		
}

void Choice(){
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
			exit(0); 
		}

		case '1': { 
			cout << "Enter file path:";
			cin  >> newAudioFile;
			 cout << endl << "Enter the password(from the Latin alphabet and Arabic numerals):";
			 cin >> password;

			Encode(newAudioFile, password); 
			exit(0);
		}

		case '2': {cout << endl << "My work here is done"; exit(0); }
		default: { cout << endl << endl << "Incorrect in, try again" << endl << endl; Choice(); }
	}
}


int Transform(string AudioFile, string audioTemp) 
{
	ifstream infile(AudioFile, ios::binary);
	ofstream outfile(audioTemp, ios::binary);

	char bt;
	int numAudTemp = 0;//bite lenght of file

	//File to array
	while (!infile.eof()) {
		infile >> bt;
		numAudTemp++;
	}

	//File size rounding(Because Transform changed size, we need same size for future)
	string str = to_string(numAudTemp); int h = str.length()-1;
	int g=1;
	for (int i = 0; i < h; i++)
		 g= g*10;
	int gg = (numAudTemp / g) + 1;
	numAudTemp = g * gg;

	infile.clear();
	infile.seekg(0);


	//Transform and write
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

	return numAudTemp;
}

void Algoritm(string alfPass, string password, int numAudTemp)
{

	//AlfPass to array
	ifstream alfPassAlg(alfPass, ios::binary); 

	int h = 0; char arrPass[lenPass];
	while (h<lenPass)
	{
		alfPassAlg >> arrPass[h]; 
		h++;
	}
	alfPassAlg.close();

	int algnum = (numAudTemp / lenPass) / 2; //1 password symbol on symbols in file
	//Null array
	memset(position, 0, (sizeof(int)*lenPass) + 1);

	//Swap bites
	for (int i = 0, j = 0; i < password.length();)
	{
		if (password[i] == arrPass[j]) {
			position[i] = (j+1) * algnum; i++; j = 0;
		}
		else if (position[i] == 0) {
			for (int jj = 0; jj < (lenPass); jj++)//!!!
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

	for (int i = 0; i < lenPass; i++)
	{
		for (int ii = 0; ii < lenPass; ii++)
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
}

void ApplyAlg(int numAudTemp, string audioTemp, string algoritmTemp, string message, string password) {

	ifstream AudioTempApp(audioTemp, ios::binary);
	ofstream AlgoritmTempApp(algoritmTemp, ios::binary);

	//Bubble sort
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
	
	//Apply Algoritm to file
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
}



void AltTransform(string newAudioFile, string algoritmTemp, int numAudTemp)
{
	ifstream infile(algoritmTemp, ios::binary);
	ofstream outfile(newAudioFile, ios::binary);

	//Transform
	int integear; char character; 
	while (!infile.eof()) {
		infile >> integear;
		character = (byte)integear;
		outfile << character; 
	}

	infile.close();
	outfile.close();

}

string ApplyAltAlg(string audioTemp, string password, int numAudTemp)
{
	string mainMess = "";
	
	ifstream AudioTempApp(audioTemp, ios::binary);
	
	//Check password
	int N;
	for (int i = 0; i < numAudTemp; i++) {
		AudioTempApp >> N;
		if (i == ((position[0])))
		{
			if (N != 64) { //64(@)
			return "Error";
			}
		}
	}

	char pos; char first;
	for (int i = 0, pos = 0; i < numAudTemp; i++)
	{
		AudioTempApp >> N;
		if (pos < lenPass)
			if (i == (position[pos]+1))
			{
				if (N == 61) { i = numAudTemp; break; }//61('=')

				mainMess += N;
				pos++; i = 0;

				AudioTempApp.clear();
				AudioTempApp.seekg(0);
			}
	}
	
	AudioTempApp.close();

	
	mainMess[0] = ' ';
	return mainMess;
}