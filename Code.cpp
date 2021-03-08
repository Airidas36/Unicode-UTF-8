#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
void decToHexa(int n, string& x);
void determinte_bytes(string& utf8, string x, int& bits);
string bitwork(int bytes, string unicode);
void BinaryStringToText(ostream& output, string binaryString);

int main()
{
	char choice;
	string unicode;
	string utf;
	int bits = 0;
	do
	{
		system("CLS");
		cout << "Pasirinkite veiksma: \n";
		cout << "1. Unicode ir UTF-8 kodo gavimas, pagal desimtaini skaiciu.\n";
		cout << "2. intel386.txt failo konvertavimas pagal CP437 koduote ir perrasymas i nauja ConvertedUTF.txt faila.\n";
		cout << "3. Baigti programa.\n";
		cin >> choice;
		if (choice == '1')
		{
			unicode = "";
			int number;
			cout << "Iveskite desimtaini skaiciu: ";
			cin >> number;
			cout << "\nUNICODE: U+";
			decToHexa(number, unicode);
			cout << unicode << endl;
			unsigned char byte1 = NULL;
			byte1 = (char)(number);
			determinte_bytes(utf, unicode, bits);
			cout << "UTF-8: " << hex << uppercase << (int)strtol(utf.c_str(), NULL, 2) << endl;
			cout << "Symbol: " << (char)(number) << endl;
			system("pause");
		}
		else if (choice == '2')
		{
			char c;
			unsigned char c1;
			int num;
			string cp;
			vector <string> cp437;
			cp437.reserve(255);
			ifstream input("386intel.txt");
			ifstream code("codepage.txt");
			while (code)
			{
				code >> num >> cp;
				cp437.push_back(cp);
			}
			code.close();
			ofstream output("ConvertedUTF.txt");
			int it;
			while (input)
			{
				input.get(c);
				c1 = c;
				it = (int)(c1);
				if (it == 16) it = 249;
				if (it > 127)
				{
					determinte_bytes(utf, cp437[it], bits);
					BinaryStringToText(output, utf);;
				}
				else output << c;
			}
			input.close();
			output.close();
			system("pause");
		}
		else if (choice != '3')
		{
			cout << "Neteisingas pasirinkimas! Bandykite is naujo\n";
			system("pause");
		}
	} while (choice != '3');
	return 0;
}

void decToHexa(int n, string& x)
{
	int i = 1, temp;
	char hex[100];
	while (n != 0)
	{
		temp = n % 16;
		if (temp < 10)
			temp = temp + 48;
		else
			temp = temp + 55;
		hex[i++] = temp;
		n = n / 16;
	}
	for (int j = 1; j < 6 - i; j++)
		cout << "0";
	for (int j = i - 1; j > 0; j--)
		x += hex[j];
}

string bitwork(int bytes, string unicode)
{
	string base;
	if (bytes == 1)
		base = "0xxxxxxx";
	else if (bytes == 2)
		base = "110xxxxx10xxxxxx";
	else if (bytes == 3)
		base = "1110xxxx10xxxxxx10xxxxxx";
	else base = "11110xxx10xxxxxx10xxxxxx10xxxxxx";
	int size = base.length();
	int len = unicode.length() - 1;
	for (int i = size - 1; i >= 0; i--)
	{
		if (base[i] == 'x')
			base[i] = unicode[len--];
	}
	return base;
}

void determinte_bytes(string& utf8, string x, int& bits)
{
	const char* hexstring = x.c_str();
	int number = (int)strtol(hexstring, NULL, 16);
	if (number >= 0x0000 && number <= 0x007f)
	{
		string unicode = bitset<8>(number).to_string();
		utf8 = bitset<8>(bitwork(1, unicode)).to_string();
		bits = 8;
	}
	else if (number >= 0x0080 && number <= 0x07ff)
	{
		string unicode = bitset<16>(number).to_string();
		utf8 = bitset<16>(bitwork(2, unicode)).to_string();
		bits = 16;
	}
	else if (number >= 0x0800 && number <= 0xffff)
	{
		string unicode = bitset<24>(number).to_string();
		utf8 = bitset<24>(bitwork(3, unicode)).to_string();
		bits = 24;
	}
	else if (number >= 0x1000 && number <= 0x10ffff)
	{
		string unicode = bitset<32>(number).to_string();
		utf8 = bitset<32>(bitwork(4, unicode)).to_string();
		bits = 32;
	}
}

void BinaryStringToText(ostream& output, string binaryString)
{
	string text;
	for (int i = 0; i < binaryString.length(); i++)
	{
		text += binaryString[i];
		if (i != 0 && ((i + 1) % 8 == 0))
		{
			char c = bitset<8>(text).to_ulong();
			output << c;
			text.clear();
		}
	}
}
