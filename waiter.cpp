// Work Distribution: Kevin Mateo 50%, Patrick Hennis 50%
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
int Waiter::getNext(ORDER &anOrder)
{
  return myIO.getNext(anOrder);
}

void Waiter::beWaiter()
{
    ORDER order;
    b_WaiterIsFinished = false;

    lock_guard<mutex> lock(mutex_order_inQ);

    while(getNext(order) == SUCCESS) // while there are orders left
        order_in_Q.push(order);

    b_WaiterIsFinished = true; // waiter is finished, notify baker
    cv_order_inQ.notify_all();
}
