
#include <iostream>
#include "E101.h"

using namespace std;

int main()
{
  int err;
  cout<<" Hello"<<endl;
  err = init(0);
  cout<<"After init() error="<<err<<endl;
  
  int count = 0;
  open_screen_stream();
  while(count < 10)
  {
	 set_motors(1,48);
	 set_motors(5,48);
	 hardware_exchange();
	 	
	 sleep1(200);
       count++;
    }
    //save_picture("file1");
    close_screen_stream();
    stoph();
	
	return 0;
}
