import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws Exception {
        File f = new File("insert.txt");
        Scanner reader = new Scanner(f);

        assert(reader.hasNextInt());
        reader.nextInt();

        RBTree<Integer> tree = new RBTree<>();
        while (reader.hasNextInt()) {
            tree.add(reader.nextInt());
        }
        reader.close();

        Traverse traverse = new Traverse(tree);
        PrintWriter writer;

        writer = new PrintWriter("LNR.txt");
        for (var node : traverse.inOrder()) {
            writer.println(node);
        }
        writer.close();

        writer = new PrintWriter("NLR.txt");
        for (var node : traverse.preOrder()) {
            writer.println(node);
        }
        writer.close();

        writer = new PrintWriter("LOT.txt");
        for (var node : traverse.levelOrder()) {
            writer.println(node);
        }
        writer.close();
    }
}
