import java.util.concurrent.Semaphore;

public class TicketSelling implements Runnable {

    private Semaphore semaforo;
    private String sellerName;
    
    //shared variable
    private volatile static int numberOfTickets = 100;

    public TicketSelling (String name, Semaphore sem) {
        this.sellerName = name;
        this.semaforo = sem;  
    }

    @Override
    public void run() {
        if(numberOfTickets == 0){
            System.out.println("There is no ticket available.");
        }

        do{
            System.out.println(sellerName + " --> waiting for customers.");

            try {
                //decrementa o semaforo
                semaforo.acquire();
            } catch (InterruptedException exception) {
                exception.printStackTrace();
            }

            if (numberOfTickets > 0) {
                numberOfTickets--;

                System.out.println(sellerName + " sold a ticket.\n In this moment there are " + numberOfTickets + " tickets.");
            }
            //incrementa o semaforo
            semaforo.release();

        } while (numberOfTickets > 0);
        

      
    }
}