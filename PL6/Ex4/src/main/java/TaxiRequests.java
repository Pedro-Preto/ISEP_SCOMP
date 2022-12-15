public class TaxiRequests {

    private int requestId;
    private int numberOfPassengers;
    private String address;

    public TaxiRequests(int requestId, int numberOfPassengers, String address) {
        this.requestId = requestId;
        this.numberOfPassengers = numberOfPassengers;
        this.address = address;
    }

    public int getRequestId() {
        return requestId;
    }

    public int getNumberOfPassengers() {
        return numberOfPassengers;
    }

    public void setNumberOfPassengers(int numberOfPassengers) {
        this.numberOfPassengers = numberOfPassengers;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }
}
