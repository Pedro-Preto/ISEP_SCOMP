import java.util.concurrent.Callable;

public class Sum implements Callable<Integer>{

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
    public Integer call() throws Exception{
        if (data.length == 0) {
            System.out.println("The array is empty.");
        }

        if(first > 10 || first < 0){
            throw new Exception("The first number should be between 0 and 10.");
        }

        if(second > 10 || second < 0){
            throw new Exception("The second number should be between 0 and 10.");
        }

        int i;
        int total = 0;
        for (i = first; i < second + 1; i++) {
            total = total + data[i];
        }

        return total;
    }

}
