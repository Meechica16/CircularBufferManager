
#include"CircularBufferManager.h"

int main(){
	int arr[2]; 
	void *p, *q, *r;
	bool res;
	CircularBufferManager B((void*)arr,2*sizeof(int), &f);
	q = B.Acquire(3,1);
	B.Free(q);
	cout << "Came here"<<endl;
	res = B.Free(q);
	cout<< res<<endl;
	cout<<"111111111111"<<q<<endl;
	r = B.Acquire(2,2);
	cout<<"2222222222222"<<r<<endl;
	p = B.Acquire(1,3);
	//cout<<"3333333333333"<<p<<endl;
	//res = B.Free(p);
	//cout << "@@@@@@@@@running again@@@@@@@@@@@" << endl;
	//cout << res <<endl;
	
	B.Acquire(2,4);
	
    B.Acquire(1,5);
    B.Acquire(1,0);
    B.Acquire(1,6);
    B.Acquire(2,7);
    B.Acquire(2,5);
    B.Acquire(2,0);
    //B.Acquire(4,0);

	//cout<< sizeof(size_t)<<endl;

}