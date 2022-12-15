import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class mainTicket {
    public static void main(String args[]) {
        
        int i;
        ArrayList<String> nameData = new ArrayList<>();
        Semaphore sem = new Semaphore(1);
        
        nameData.add("Lara");
        nameData.add("Gonçalo");
        nameData.add("Luis");
        nameData.add("Érica");
        nameData.add("Cecília");
        nameData.add("António");
        nameData.add("Laureana");
        nameData.add("Jeogirna");
        nameData.add("Joaquim");
        nameData.add("Fernandes");
        
        //number of threads == number of seller
        Thread[] thread_id = new Thread[nameData.size()];
        
        for (i = 0; i < nameData.size(); i++) {
           
            TicketSelling ticket = new TicketSelling(nameData.get(i), sem);
            
            thread_id[i] = new Thread(ticket, "Thread #" + i);
            thread_id[i].start(); //run of the thread
        }
        
        for (i = 0; i < nameData.size(); i++) {
            try {
                thread_id[i].join();
            } catch (InterruptedException exception) {
                exception.printStackTrace();
            }
        }
    }
}
