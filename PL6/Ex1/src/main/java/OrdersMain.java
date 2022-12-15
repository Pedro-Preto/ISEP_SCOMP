public class OrdersMain {


    public static void main(String args[]) {
        OrderQueue queue = new OrderQueue();
        Thread[] taker = new Thread[10];
        Thread[] handler = new Thread[10];
        
        int i;
        
        for (i = 0; i < 10; i++) {
            OrderHandler oh = new OrderHandler(queue);
            OrderTaker ot = new OrderTaker(queue);

            handler[i] = new Thread(oh, "Handler_Thread #" + i);
            handler[i].start();

            taker[i] = new Thread(ot, "Taker_Thread #" + i);
            taker[i].start();
        }

        for (i = 0; i < 10; i++) {//esperar que todas as threads acabem
            try {
                handler[i].join();
            } catch (InterruptedException exception) {
                exception.printStackTrace();
            }

            try {
                taker[i].join();
            } catch (InterruptedException exception) {
                exception.printStackTrace();

            }
        }   
      
     } 
        
        
    }

