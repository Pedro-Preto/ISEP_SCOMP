import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class Main {
    public static void main(String args[]) {
        List<Taxi> availableTaxis = new LinkedList<>();
        availableTaxis.add(new Taxi("Taxi 1", 6));
        availableTaxis.add(new Taxi("Taxi 2", 4));//14
        availableTaxis.add(new Taxi("Taxi 3", 4));
        availableTaxis.add(new Taxi("Taxi 4", 4));
        availableTaxis.add(new Taxi("Taxi 5", 4));
        availableTaxis.add(new Taxi("Taxi 6", 4));

        List<TaxiRequests> requests = new LinkedList<>();
        requests.add(new TaxiRequests(1, 8, "Street 1"));
        requests.add(new TaxiRequests(2, 4, "Street 2"));//14
        requests.add(new TaxiRequests(3, 2, "Street 3"));

        List<Taxi> usedTaxis = new LinkedList<>();

        Semaphore sem = new Semaphore(1);
        Thread[] threads = new Thread[availableTaxis.size()];

        for (int i = 0; i < availableTaxis.size(); i++) {
            if (!requests.isEmpty()) {
                Taxi taxi = availableTaxis.get(i);
                usedTaxis.add(taxi);

                TaxiRequests r = requests.get(0);

                TripSimulation trip = new TripSimulation(taxi, r, sem);

                if (taxi.getNumberOfSits() >= r.getNumberOfPassengers()) {
                    requests.remove(0);
                    threads[i] = new Thread(trip, "Thread #" + i);
                    threads[i].start();
                } else if (taxi.getNumberOfSits() < r.getNumberOfPassengers()) {
                    requests.get(0).setNumberOfPassengers(r.getNumberOfPassengers() - taxi.getNumberOfSits());
                    threads[i] = new Thread(trip, "Thread #" + i);
                    threads[i].start();
                }
            }
        }
        //Esperar que as threads terminem a sua execução
        for (int i = 0; i < usedTaxis.size(); i++) {
            try {
                threads[i].join();
            } catch (InterruptedException exception) {
                exception.printStackTrace();
            }
        }
    }
}

