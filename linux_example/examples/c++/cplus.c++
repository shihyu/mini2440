#include <iostream>
#include <cstring>
using namespace std;

class String
{
	private:
   	    char *str;
	public:
	    String(char *s)
	    {
	        int lenght=strlen(s);
	        str = new char[lenght+1];
	        strcpy(str, s);	
	    }
	    ~String()
	    {
		cout << "Deleting str.\n";
		delete[] str;	    
	    }
	    void display()
	    {
		cout << str <<endl;
	    }
};

int main(void)
{
	String s1="I like FriendlyARM.";
	cout << "s1=";
	s1.display();
	return 0;
	double num, ans;

	cout << "Enter num:";
}
