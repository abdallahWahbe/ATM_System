// ATM_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<string>
#include<vector>
#include<fstream>


using namespace std;

const string ClientsFileName = "Clients.txt";


struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

enum enMainMenueOptions
{

	eQucikWithdraw = 1,
	eNormalWithDraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eExit = 5

};

void ShowMainMenue();
void GoBackToMainMenue();
short ReadMainMenueOption();
void ShowNormalWithDrawScreen();
void Login();



sClient CurrentClient;

vector <string> Splitstring(string Line, string Delim)
{

	vector <string> vWord;

	string sWord;

	short pos = 0;

	while ((pos = Line.find(Delim) )!= std::string::npos)
	{


		sWord = Line.substr(0, pos);

		if (sWord != "")
		{

			vWord.push_back(sWord);

		}

		Line.erase(0, pos + Delim.length());

	}

	if (Line != "")
	{

		vWord.push_back(Line);


	}

	return vWord;

}

string ConvertRecodToLine(sClient Client, string Delim = "#//#")
{

	
	string Line;

	Line += Client.AccountNumber + Delim;
	Line += Client.PinCode + Delim;
	Line += Client.Name + Delim;
	Line += Client.Phone + Delim;
	Line += to_string(Client.AccountBalance);

	return Line;

}

sClient ConvertLineToRecord(string Line , string Delim="#//#")
{

	vector <string> vWord;

	vWord = Splitstring(Line, Delim);

	sClient Client;

	Client.AccountNumber = vWord[0];
	Client.PinCode = vWord[1];
	Client.Name = vWord[2];
	Client.Phone = vWord[3];
	Client.AccountBalance = stod(vWord[4]);


	return Client;

}

vector <sClient> LoadDataFromFileToVector(string FileName)
{

	vector <sClient> vClients;
	
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{

		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{

			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);

		}

		MyFile.close();

	}
	
	return vClients;

}

bool FindClientByAccountNumberAndPineCode(string AccountNumber , string PinCode, sClient &Client)
{

	vector <sClient> vClients;

	vClients = LoadDataFromFileToVector(ClientsFileName);

	for (sClient &c : vClients)
	{
		if (c.AccountNumber == AccountNumber && c.PinCode == PinCode)
		{
			Client = c;
			return true;

		}

	}

	return false;

}

bool LoadClientInfo(string AccountNumber ,string PinCode )
{
	return FindClientByAccountNumberAndPineCode(AccountNumber, PinCode,CurrentClient);
}

short ReadMainMenueOption()
{
	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Choice;

	return Choice;


}

bool FindClientByAccountNumber(string AccountNumber ,vector <sClient> &vClinets , sClient &Client )
{
	for (sClient &c :vClinets)
	{
		if (c.AccountNumber == AccountNumber)
		{
			Client = c;

			return true;
		}
	}

	return false;

}

void SaveClientDataToFile(string FileName, vector <sClient>& vClients)
{

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{

		string Line;

		for (sClient& c : vClients)
		{

			Line = ConvertRecodToLine(c);
			MyFile << Line << endl;

		}

		MyFile.close();

	}

}
bool DepositBalanceByAccountNumber(string AccountNumber, int Amount, vector <sClient>& vClients)
{

	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction?  y / n ? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{

		for (sClient& c : vClients)
		{
			if (c.AccountNumber == AccountNumber)
			{
				c.AccountBalance += Amount;
				SaveClientDataToFile(ClientsFileName, vClients);

				cout << "\n\nDone Successfully. New balance is: "
					<< c.AccountBalance;
				return true;
			}
		}
	}

	return false;


}
short ReadQuickWithdrawOption()
{
	short Choice = 0;
	while (Choice < 1 || Choice>9)
	{
		cout << "\nChoose what to do from [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PerfromQuickWithdrawOption(short QuickWithDrawAmount)
{
	if (QuickWithDrawAmount == 9 )
	{
		return;
	}

	int WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawAmount);

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		return;
	}

	vector <sClient> vClients;

	vClients = LoadDataFromFileToVector(ClientsFileName);


	if (DepositBalanceByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients))
	{
		CurrentClient.AccountBalance -= WithDrawBalance;
	}

}

void ShowQuickWithdrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tQucik Withdraw\n";
	cout << "===========================================\n";
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "===========================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance;
	PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

void PerfromNormalWithdrawOption()
{

	int Amount = 0;

	do
	{
		cout << "\nEnter Amount Multiple 5’s : ";
		cin >> Amount;


	} while (Amount % 5 != 0 || Amount <=0);


	if (Amount > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
			cout << "Press Anykey to continue...";
		system("pause>0");
		ShowNormalWithDrawScreen();
		return;
	}

	vector <sClient> vClients;

	vClients = LoadDataFromFileToVector(ClientsFileName);

	if(DepositBalanceByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients))
	{
		CurrentClient.AccountBalance -= Amount;
	}

}


void ShowNormalWithDrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tNormal Withdraw Screen\n";
	cout << "===========================================\n";

	PerfromNormalWithdrawOption();

}

void PerfromDepositOption()
{
	int Amount=0;


	do
	{
		cout << "\nEnter a Positive Deposit Amount : ";
		cin >> Amount;
	} while (Amount < 0);

	vector <sClient> vClient;

	vClient = LoadDataFromFileToVector(ClientsFileName);
	if(DepositBalanceByAccountNumber(CurrentClient.AccountNumber, Amount, vClient))
	{
		CurrentClient.AccountBalance += Amount;
	}

	}

void ShowDepositDalancescreen()
{
	cout << "===========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "===========================================\n";

	PerfromDepositOption();
}




void ShowCheckBalanceScreen()
{
	cout << "===========================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "===========================================\n";

	cout << "\nYour Balance Is : " << CurrentClient.AccountBalance << endl;
}


void PerfromMainMenueOption(enMainMenueOptions MainMenueOptions )
{


	switch (MainMenueOptions)
	{



	case enMainMenueOptions::eQucikWithdraw:

		{
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToMainMenue();

		break;

		}


	case enMainMenueOptions::eNormalWithDraw:
	{
		system("cls");
		ShowNormalWithDrawScreen();
		GoBackToMainMenue();

		break;

	}

	case enMainMenueOptions::eDeposit:
	{
		system("cls");
		ShowDepositDalancescreen();
		GoBackToMainMenue()	;

		break;
	}

	case enMainMenueOptions::eCheckBalance:
	{

		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenue();

		break;

	}

	case enMainMenueOptions::eExit:
	{
		system("cls");
		Login();

		break;
	}


	}
}
void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===========================================\n";
	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void Login()
{
	bool LoginFaild = false;
	string AccountNumer, PinCode;
	do 
	{
		system("cls");

		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";


		if (LoginFaild)
		{

			cout << "Invlaid Username/Password!\n";

		}

		cout <<"\nEnter AccountNumber : ";
		cin >> AccountNumer;

		cout << "\nEnter PinCode       : ";
		cin >> PinCode;

		LoginFaild = !LoadClientInfo(AccountNumer, PinCode);

	} while (LoginFaild);

	ShowMainMenue();

}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}
int main()
{
	Login();
}