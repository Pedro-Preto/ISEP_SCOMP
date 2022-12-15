import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class MainEx3 {
    public static void main(String args[]) {
        List<String> sellers=new LinkedList<>();

        sellers.add("Seller 1");
        sellers.add("Seller 2");
        sellers.add("Seller 3");

        List<Account> accounts=new LinkedList<>();
        for(int i=0;i<sellers.size();i++){
            accounts.add(new Account(sellers.get(i),new EurosAmount(100)));
        }


        Semaphore sem = new Semaphore(1);
        Thread[] threads = new Thread[sellers.size()];
        //Cada vendedor deve corresponder a 1 thread
        Random random=new Random();

        for (int i = 0; i < sellers.size(); i++) {

            int sellerGiver=random.nextInt(sellers.size());
            int sellerReceiver=-1;
            while(sellerReceiver==-1||sellerReceiver==sellerGiver){
                sellerReceiver=random.nextInt(sellers.size());
            }

            Account transation=new Account(accounts.get(sellerGiver),accounts.get(sellerReceiver),new EurosAmount(Double.parseDouble(String.valueOf(random.nextInt(100)))),sem);

            threads[i] = new Thread(transation, "Thread #" + i);
            threads[i].start();
        }

        //Esperar que as threads terminem a sua execução
        for (int i = 0; i < sellers.size(); i++) {
            try {
                threads[i].join();
            } catch (InterruptedException exception) {
                exception.printStackTrace();
            }
        }
    }
}