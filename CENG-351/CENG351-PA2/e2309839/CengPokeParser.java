import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Scanner;

public class CengPokeParser {

    public static ArrayList<CengPoke> parsePokeFile(String filename) {
        ArrayList<CengPoke> pokeList = new ArrayList<>();

        Scanner s = null;
        try {
            File input_file = new File(filename);
            s = new Scanner(input_file);
        } catch (FileNotFoundException e) {

            e.printStackTrace();
        }
        String myLine = null;
        while (s.hasNextLine()) {
            myLine = s.nextLine();
            String[] array = myLine.split("[\\t]");
            Integer key = Integer.parseInt(array[1]);
            String name = array[2];
            String power = array[3];
            String type = array[4];

            CengPoke ceng_p = new CengPoke(key, name, power, type);
            pokeList.add(ceng_p);
        }
        return pokeList;
    }

    public static void startParsingCommandLine() throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        boolean running = true;
        String inpLine = null;
        while (running) {
            inpLine = reader.readLine();
            String[] array = inpLine.split("[\\t]");
            String command = array[0];
            if (command.equalsIgnoreCase("quit")) {
                running = false;
            } else if (command.equalsIgnoreCase("add")) {
                Integer key = Integer.parseInt(array[1]);
                String name = array[2];
                String power = array[3];
                String type = array[4];

                CengPokeKeeper.addPoke(new CengPoke(key, name, power, type));
            } else if (command.equalsIgnoreCase("search")) {
                int key = Integer.parseInt(array[1]);
                CengPokeKeeper.searchPoke(key);
            } else if (command.equalsIgnoreCase("delete")) {
                int key = Integer.parseInt(array[1]);
                CengPokeKeeper.deletePoke(key);
            } else if (command.equalsIgnoreCase("print")) {
                CengPokeKeeper.printEverything();
            }
        }
    }
}
