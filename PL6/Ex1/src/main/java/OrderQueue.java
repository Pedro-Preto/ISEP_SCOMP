import java.util.LinkedList;

public class OrderQueue {
  
    private LinkedList<Order> orders;
    private boolean result;

    public OrderQueue (){
        orders = new LinkedList<>();
    }


    public synchronized boolean addOrder(Order order){

        while(result){
            try {
                wait();
            } catch (InterruptedException exception) {
                exception.printStackTrace();

            }

        }

        result = true;

        if (orders.add(order)) {

            notifyAll();
            System.out.println("An order was added to the list.");

            return true;
        }

        return false;
    }


    public synchronized Order removeOrder(){
        while (!result) {

            try {
                wait();
            } catch (InterruptedException exception) {
                exception.printStackTrace();

                 }
             }

        result = false;

        Order order = orders.pop();

        notifyAll();
        System.out.println("An order was removed from list.");

        return order;
    }

    public void removeOrderPosition (int position){
        orders.remove(position);
    }

}
