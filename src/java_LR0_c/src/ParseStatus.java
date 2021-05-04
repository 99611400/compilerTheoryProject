import java.util.LinkedList;

public class ParseStatus {
    private final   LinkedList<Integer> statusStack;
    private final  LinkedList<String> symbolStack;
    private final  LinkedList<String> leftTokenStack;

    public LinkedList<Integer> getStatusStack() {
        return statusStack;
    }

    public LinkedList<String> getSymbolStack() {
        return symbolStack;
    }

    public LinkedList<String> getLeftTokenStack() {
        return leftTokenStack;
    }

    public ParseStatus(LinkedList<Integer> statusStack, LinkedList<String> symbolStack, LinkedList<String> leftTokenStack) {
        this.statusStack = statusStack;
        this.symbolStack = symbolStack;
        this.leftTokenStack = leftTokenStack;
    }
}
