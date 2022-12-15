public class EurosAmount {

    private double amount;

    public EurosAmount(double amount) {
        this.amount = amount;
    }

    public double getAmount() {
        return amount;
    }

    @Override
    public String toString() {
        return String.valueOf(amount);

    }
}
