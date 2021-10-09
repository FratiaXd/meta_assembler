
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include<string.h>
#include <vector>
#include <sstream>
using namespace std;

class Instruction
{
private:
    string mnemonic;
    string opcode;
public:
    Instruction(string mnem, string op);
    Instruction();
    void list(ifstream& file);
    void read_file(Instruction list[], int max);
    string create_code(string a, string b, Instruction list[], int max);
};

Instruction::Instruction(string mnem, string op)        //Parametrized constructor
{
    mnemonic = mnem;
    opcode = op;
}
Instruction::Instruction()          //Default constructor
{
    mnemonic = "No data";
    opcode = "No data";
}

void Instruction::list(ifstream& file)          //Puts mnemonic and instruction code in variables
{
    file >> mnemonic >> opcode;
}

void Instruction::read_file(Instruction list[], int max)
{
    ifstream instructions("example.txt");
    if (!instructions)
    {
        cout << "Instuctions definitions file invalid." << endl;
    }
    for (int i = 0; i < max; i++)
    {
        list[i].list(instructions);
    }
    instructions.close();
}

string Instruction::create_code(string a, string b, Instruction list[], int max)
{
    int res;
    string code;
    for (int i = 0; i < max; i++)
    {
        int res = a.compare(list[i].mnemonic);          //Changes mnemonic to instruction code
        if (res == 0)
        {
            code = list[i].opcode + b;
        }
        else
        {
            continue;
        }
    }
    return code;
}

int vector_off(vector<string>& vect, int st);
int check_org(int start);
void create_cdm_file(vector<string>& vect, int x, int y, int vec_size);
int count_strings(string T);
int org_dir(string org, string num);
string dc_dir(string dc, string num);
string ds_dir(string ds, string num);

const int MAXINSTRUCTIONS = 15;

int main()
{
    string f;
    string fname;
    Instruction instList[MAXINSTRUCTIONS];

    vector<string> cdm;

    instList->read_file(instList, MAXINSTRUCTIONS);         //Saves instructions in the array

    cout << "META ASSEMBLER" << endl;
    cout << "Enter the file name that contains the assembly program: (If you want to use my example enter 'assembly')" << endl;
    cin >> f;
    fname = f + ".txt";
    ifstream file(fname.c_str());
    string line;
    string content;
    string con;
    string end;
    int start_org = 0;
    int vector_size;

    while (getline(file, line))
    {
        if (count_strings(line) == 2)           //If normal instruction
        {
            stringstream ss(line);
            while (ss >> content >> con)
            {
                if (org_dir(content, con) > 0)              //Checks for org directory
                {
                    start_org = org_dir(content, con);
                    continue;
                }
                else if (dc_dir(content, con).length() > 1)         //Checks for dc directory
                {
                    end = dc_dir(content, con);
                }
                else if (ds_dir(content, con).length() > 1)             //Checks for ds directory
                {
                    end = ds_dir(content, con);
                }
                else
                {
                    start_org += 0;
                    end = instList->create_code(content, con, instList, MAXINSTRUCTIONS);
                }
            }
        }
        else                                               //If branch instruction (no code)
        {
            end = instList->create_code(line, "", instList, MAXINSTRUCTIONS);
        }

        cdm.push_back(end);
    }

    vector_size = vector_off(cdm, start_org);           //Updates vector size according to org
    create_cdm_file(cdm,start_org, check_org(start_org), vector_size);
}

int vector_off(vector<string>& vect, int st)
{
    stringstream tt(st);
    int size = vect.size() + st;
    return size;
}

int check_org(int start)
{
    if (start > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void create_cdm_file(vector<string>& vect, int x, int y, int vec_size)
{
    ofstream cdm_file;
    cdm_file.open("cdmf.cdm");
    int a = y;
    string dir;
    int val;
    for (int i = x; i < vec_size; i++)              
    {
        if (vect[a].find("DS") != string::npos)         //Defines space
        {
            stringstream vv(vect[a]);
            vv >> dir >> hex >> val;
            val -= 1;
            i += val;
            vec_size += val;
            if (y == 1)
            {
                vec_size -= 1;
            }
            a += 1;
            continue;
        }
        cdm_file << hex << i << ":" << vect[a] << endl;
        if ((y == 1) && (a < (vect.size() - 1)))            //When org
        {
            a += 1;
        }
        if (y == 0)                 //When no org
        {
            a += 1;
        }
    }
}

int count_strings(string T)
{
    size_t n_str = T.empty() || T.back() == ' ' ? 0 : 1;
    for (size_t s = T.size(); s > 0; --s)
        if (T[s] == ' ' && T[s - 1] != ' ') ++n_str;
    return n_str;
}

int org_dir(string org, string num)
{
    if (org.compare("ORG") == 0)
    {
        stringstream ll(num);
        int x = 0;
        ll >> hex >> x;
        return x;
    }
    else
    {
        int x = 0;
        return 0;
    }
}

string dc_dir(string dc, string num)
{
    if (dc.compare("DC") == 0)
    {
        return num;
    }
    else
    {
        int x = 0;
        return "";
    }
}

string ds_dir(string ds, string num)
{
    string s;
    if (ds.compare("DS") == 0)
    {
        s = ds + " " + num;
        return s;
    }
    else
    {
        int x = 0;
        return "";
    }
}

