import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;


public class mainSum {

    static Scanner sc = new Scanner(System.in);
    public static void main(String args[]) {

        System.out.println("Choose two different numbers in the range [0,9].");
        System.out.print("First: ");
        int first = sc.nextInt();
        System.out.print("Second: ");
        int second = sc.nextInt();
        System.out.println();


        List<int[]> list1 = new LinkedList<>();
        int[] numberData = new int[10];

        int index = 0;
        int count = 0;
        for (int i = 0; i < 50; i++) {
            if (count < 10) {
                numberData[index] = i;
                count++;
                index++;
            } else {
                list1.add(numberData);
                numberData = new int[10];
                count = 0;
                index = 0;
                numberData[index] = i;
                index++;
                count++;
            }
        }
        list1.add(numberData);



        ExecutorService executor = Executors.newFixedThreadPool(5);


        for (int[] ints : list1) {
            //create a list to hold the Future object associated with Callable
            List<Future<Integer>> list = new ArrayList<Future<Integer>>();
            Callable<Integer> ca = new Sum(ints, first, second);


            //submit Callable tasks to be executed by thread pool
            Future<Integer> future = executor.submit(ca);
            //add Future to the list, we can get return value using Future
            list.add(future);

            int sum = 0;
            for (Future<Integer> fut : list) {

                try {
                    //print the return value of Future, notice the output delay in console
                    // because Future.get() waits for task to get completed
                    sum = fut.get();

                    System.out.println("Total between the first given position " + first + " and second position " + second + " is " + sum);

                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

        }

        executor.shutdown();


        System.out.println("Executor Finish!");

    }
}
