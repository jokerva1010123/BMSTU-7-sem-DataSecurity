#include <bits/stdc++.h>

using namespace std;

struct rotor
{
    vector<unsigned char> alphabet;
    int start_state, state;
    int click;
    vector<unsigned char> get_from_file(string filename)
    {
        vector<unsigned char> res;
        return res;
    }
    void init(vector<unsigned char> alpha, int type, string filename = "")
    {
        if(type == 1)
        {
            alphabet = alpha;
        }
        else
            alphabet = get_from_file(filename);
        click = 0;
    }
    void rotate()
    {
        state += 1;
        click += 1;
    }
};

struct Enima
{
    vector<unsigned char> alphabet;
    void init()
    {

    }
};

void menu()
{
    cout << "1 - Зашифровать файл\n2 - Расшифровать файл\n3 - Зашифровать сообщение\n4 - Расшифровать сообщение\n0 - Выход\n";
}

char encodingLetter(char letter)
{
    
}

void encodingMes()
{
    cout << "Input message: ";
    string mes;
    cin >> mes;
    string res = "";
    for (int i = 0; i < mes.length(); i++)
        res += encodingLetter(mes[i]);
    cout << "Result: " << res << '\n';
}

int main()
{
    cout << "Welcome.\n";
    menu();
    cout << "Choose: ";
    int command;
    cin >> command;
    while(command)
    {
        switch (command)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                encodingMess();
                break;
            case 4:
                break;
            case 0:
                cout << "Goodbye.\n";
                return 0;
            default:
                cout << "Choose again\n";
                break;
        }
        menu();
        cout << "Choose: ";
        cin >> command;
    }
    return 0;
}