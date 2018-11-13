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
void Baker::bake_and_box(ORDER &anOrder) {
	int num_dons = anOrder.number_donuts;
	//create new box
	Box box = Box();
	Box box2 = Box();
	//add donuts
		for(int i = 0; i < num_dons; i++){
			DONUT temp_don = DONUT();
			bool added_donut = box.addDonut(temp_don);
			if(added_donut == false){
				anOrder.boxes.push_back(box);
				box2.addDonut(temp_don);
				anOrder.boxes.push_back(box2);
			}
		}
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
void Baker::beBaker() {
	unique_lock<mutex> locker(mutex_order_inQ);
	//baker is waiting for waiter
	//check if watier is finished
	//then see if orders are in queue
	while((b_WaiterIsFinished == false)){
		if(order_in_Q.empty()){
			cv_order_inQ.wait(locker);
		}
	}
	//lock  mutex order outQ
	mutex_order_outQ.lock();
	// bake and box for next order
	ORDER temp = order_in_Q.front();
	bake_and_box(temp);
	order_out_Vector.push_back(temp);
	order_in_Q.pop();
	
	//unlock mutex
	mutex_order_outQ.unlock();
}
