import java.util.Random;

public class OrderTaker implements Runnable{
    private OrderQueue orderQ;
    private Order order;

    public OrderTaker(OrderQueue oq){
        this.orderQ = oq;
    }


    @Override
    public void run() {
        
        Random rand = new Random();

        for (int i = 0; i < 10; i++) {
            order = new Order(rand.nextInt(10));
            orderQ.addOrder(order);
        }

        System.out.println("Thread: " + Thread.currentThread().getId() + " exiting." );
    }

}