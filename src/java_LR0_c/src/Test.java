import java.util.LinkedList;

public class Test {
    public static void main(String[] args) {
//        String str =  "a|b";
//        String[] split = str.split("\\|");
//        System.out.println();
//
//        Vector<String> testVector =  new Vector<>();
//        testVector.add("abc");
//        testVector.add("aa");
//        testVector.add("b");
//        testVector.add("c");
//        testVector.sort(new Comparator<String>() {
//            @Override
//            public int compare(String o1, String o2) {
//                if (o1.length() < o2.length()){
//                    return 1;
//                }else {
//                    return o1.compareTo(o2);
//                }
//            }
//        });
//        System.out.println(testVector);
        LinkedList<String> testLinkedList = new LinkedList<>();
        testLinkedList.add("a");
        testLinkedList.add("b");
        testLinkedList.add("c");
//        System.out.println(testLinkedList);
        System.out.println(testLinkedList.get(1));
    }
}
