public class Order {

    private int number;
 
    public Order(){
        this.number = 0;

    }

    public Order(int number){
        this.number = number;
    }
    
    public String toString(){
        return "Order id: " + number + ".\n";
    } 
}