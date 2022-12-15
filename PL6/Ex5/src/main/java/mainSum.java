
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class mainSum {

    static Scanner sc = new Scanner(System.in);

    public static void main(String args[]) {

        System.out.println("Choose two different numbers in the range [0,9].");
        System.out.print("First: ");
        int first = sc.nextInt();
        System.out.print("Second: ");
        int second = sc.nextInt();
        System.out.println();


        List<int[]> list = new LinkedList<>();
        int[] numberData = new int[10];

        int index = 0;
        int count = 0;
        for (int i = 0; i < 50; i++) {
            if (count < 10) {
                numberData[index] = i;
                count++;
                index++;
            } else {
                list.add(numberData);
                numberData = new int[10];
                count = 0;
                index = 0;
                numberData[index] = i;
                index++;
                count++;
            }
        }
        list.add(numberData);

        //number
        Thread[] thread_id = new Thread[list.size()];

        ExecutorService executor = Executors.newSingleThreadExecutor();

        executor.submit(() -> {
            for (int i = 0; i < list.size(); i++) {
                Sum s = new Sum(list.get(i), first, second);

                thread_id[i] = new Thread(s, "Thread #" + i);
                thread_id[i].start(); //run of the thread

            }

            for (int j = 0; j < list.size(); j++) {
                try {
                    thread_id[j].join();
                } catch (InterruptedException exception) {
                    exception.printStackTrace();
                }
            }

        });

        executor.shutdown();


        System.out.println("Executor Finish!");

    }

}
