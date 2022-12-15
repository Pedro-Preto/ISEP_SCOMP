import java.util.concurrent.Semaphore;

public class Account implements Runnable {

    private String nameGiver;
    private EurosAmount balance;


    private Account accountGiver;
    private Account accountReceiver;
    private EurosAmount payment;
    private Semaphore semaphore;

    public Account(String nameGiver, EurosAmount amount) {
        this.nameGiver = nameGiver;
        this.balance = amount;
    }

    public Account(Account accountGiver, Account accountReceiver, EurosAmount payment, Semaphore semaphore) {
        this.accountGiver = accountGiver;
        this.accountReceiver = accountReceiver;
        this.payment = payment;
        this.semaphore = semaphore;
    }

    public String getNameGiver() {
        return nameGiver;
    }


    public EurosAmount getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = new EurosAmount(balance);
    }

    public boolean transferMoney(Account fromAcct, Account toAcct, EurosAmount amount) {


        try {
            //decrementa o semaforo
            semaphore.acquire();
        } catch (InterruptedException exception) {
            exception.printStackTrace();
        }
        System.out.println("==========================");
        System.out.println(fromAcct.getNameGiver()+" pagou: "+amount.getAmount()+"€"+" ao "+toAcct.getNameGiver());

        fromAcct.setBalance(fromAcct.getBalance().getAmount()-amount.getAmount());
        toAcct.setBalance(toAcct.getBalance().getAmount()+ amount.getAmount());

        System.out.println("--------------------------");
        System.out.println(fromAcct.getNameGiver()+" balanço atual: "+fromAcct.getBalance()+"€");
        System.out.println("--------------------------");
        System.out.println(toAcct.getNameGiver()+" balanço atual: "+toAcct.getBalance()+"€");
        System.out.println("==========================\n");

        //incrementa o semaforo
        semaphore.release();
        return true;
    }


    @Override
    public void run() {
        transferMoney(accountGiver, accountReceiver, payment);
    }
}
