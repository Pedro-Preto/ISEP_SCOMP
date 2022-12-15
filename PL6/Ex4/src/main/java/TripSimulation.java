import java.util.List;
import java.util.concurrent.Semaphore;

public class TripSimulation implements Runnable {

    private Taxi assignedTaxi;
    private TaxiRequests requests;
    private Semaphore semaphore;

    public TripSimulation(Taxi assignedTaxi, TaxiRequests requests, Semaphore semaphore) {
        this.assignedTaxi = assignedTaxi;
        this.requests = requests;
        this.semaphore = semaphore;
    }

    public void makeTrip(Taxi assignedTaxi, TaxiRequests requests) {
        try {
            //decrementa o semaforo
            semaphore.acquire();
        } catch (InterruptedException exception) {
            exception.printStackTrace();
        }
        int pickUpPeople=0;
        if(assignedTaxi.getNumberOfSits()<requests.getNumberOfPassengers()){
            pickUpPeople=requests.getNumberOfPassengers()-assignedTaxi.getNumberOfSits();
        }else if(assignedTaxi.getNumberOfSits()==requests.getNumberOfPassengers()){
            pickUpPeople=assignedTaxi.getNumberOfSits();
        }else if(assignedTaxi.getNumberOfSits()>requests.getNumberOfPassengers()){
            pickUpPeople=assignedTaxi.getNumberOfSits()-requests.getNumberOfPassengers();
        }
        System.out.println("==============");
        System.out.println(assignedTaxi.getTaxiName()+" with "+assignedTaxi.getNumberOfSits()+ " sits available "+" pick up "+pickUpPeople+" with request Id: "+requests.getRequestId());
        System.out.println("==============");

        //incrementa o semaforo
        semaphore.release();
    }


    @Override
    public void run() {
        makeTrip(assignedTaxi, requests);
    }
}
