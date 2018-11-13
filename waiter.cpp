#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/datastructs.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
    ORDER order;

    lock_guard<mutex> lock(mutex_order_inQ);

    while(getNext(order) == SUCCESS)
        order_in_Q.push(order);


    b_WaiterIsFinished = true;
    cv_order_inQ.notify_all();

}

