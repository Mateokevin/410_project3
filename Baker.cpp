// Work Distribution: Kevin Mateo 50%, Patrick Hennis 50%
#include <mutex>
#include <thread>

#include "../includes/baker.h"
#include "../includes/datastructs.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
#include "../includes/logger.h"
#include <vector>
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder)
{
    // create new box
    Box box;

    // add donuts
    for(int i = 0; i < anOrder.number_donuts; i++)
    {
        DONUT temp_don;
        box.addDonut(temp_don);
        if((i + 1) % DOZEN == 0)
        { // if just added 12th donut
            anOrder.boxes.push_back(box);
            box.clear();
        }
    }

    if (box.size() > 0) // doesnt give customer empty box
        anOrder.boxes.push_back(box);
}
//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker()
{
  unique_lock<mutex> locker(mutex_order_inQ);
  // baker is waiting for waiter
  // check if watier is finished
  // then see if orders are in queue
  cv_order_inQ.wait(locker);

  ORDER temp;

  while(!order_in_Q.empty() || !b_WaiterIsFinished)
  {
      // lock  mutex order outQ
      mutex_order_outQ.lock();

      // remove front of order_in_Q
      temp = order_in_Q.front();
      order_in_Q.pop();

      // bake and box for next order
      bake_and_box(temp);
      order_out_Vector.push_back(temp);

      // unlock mutex
      mutex_order_outQ.unlock();
  }
}
