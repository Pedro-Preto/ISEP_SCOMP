public class Taxi {
  private String taxiName;
    private int numberOfSits;

    public Taxi(String taxiName, int numberOfSits) {
        this.taxiName = taxiName;
        this.numberOfSits = numberOfSits;
    }

    public String getTaxiName() {
        return taxiName;
    }

    public int getNumberOfSits() {
        return numberOfSits;
    }
}
