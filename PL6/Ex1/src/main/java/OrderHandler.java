public class OrderHandler implements Runnable {

    private OrderQueue orderQ;
    private Order order;


    public OrderHandler(OrderQueue queue) {
        this.orderQ = queue;
    }

    @Override
    public void run() {
        
            for (int i = 0; i < 10; i++) {
                order = orderQ.removeOrder();
                System.out.println(order.toString());
            }

            System.out.println("Thread: " + Thread.currentThread().getId() + " exiting." );


        

    }

}