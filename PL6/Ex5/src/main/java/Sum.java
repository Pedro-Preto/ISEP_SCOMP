
public class Sum implements Runnable {

    private int[] data;
    // shared variable
    private volatile static int first = 0;
    private volatile static int second = 0;

    public Sum(int[] array, int first, int second) {
        this.data = array;
        Sum.first = first;
        Sum.second = second;
    }



    @Override
    public void run() {
        if (data.length == 0) {
            System.out.println("The array is empty.");
        }

        if(first > 10 || first < 0){
            System.out.println("The first number should be between 0 and 10.");
        }

        if(second > 10 || second < 0){
            System.out.println("The second number should be between 0 and 10.");
        }

        int i;
        int total = 0;
        for (i = first; i < second + 1; i++) {
            total = total + data[i];
        }
        System.out.println( "Total between the first given position " + first + " and second position " + second + " is " + total);

    }
}
