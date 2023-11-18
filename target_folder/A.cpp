#include<iostream>
using namespace std;
class x{
	public:
 	virtual void print(){
		cout<<"i am x\n";
	}
};
class y:public x{
	public:
	void print()
	{
		cout << "i am y \n ";
	}
};
int main()
{
	x* t = new y();

	t->print();

}
