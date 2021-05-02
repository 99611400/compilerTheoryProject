
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class LR0 {

    private static final String SPOT = "･";
    private static final String NONE = "@";
    private static final String END = "$";


    private String tobeResolvedInput;  // 存储要解析的字符串
    private ProductionForm startProductionForm;

    private final Vector<String> mergeTerAndNoneVector; // 合并终结符符和非终结符的Vector
    private final HashSet<String> terminatorSet;  // 终结符的set
    private final HashSet<String> noneTerminatorSet; // 非终结符set
    private final HashSet<ProductionForm> productionFormSet; // 所有产生式的合集 里边都是单独的  A->aA|bB 会被拆成两个产生式

    private final ArrayList<Set<ProductionForm>> INodeList; // 节点列表
    private final ArrayList<StateNode> IEdgeList; // 边的集合   两点之间的联系



    private ArrayList<String>  splitByTerminatorAndNoneTer(String str){
        ArrayList<String> res = new ArrayList<>();
        int point = 0;
        int strLength = str.length();
        while(point <= strLength -1){
            for(String token: mergeTerAndNoneVector){
                int tokenLength = token.length();
                if (point + tokenLength <= strLength && str.substring(point,point+tokenLength).equals(token)){
                    point += tokenLength;
                    res.add(token);
                    break;
                }
            }
        }
        return res;
    }
    private void calculationCLOSURE(Set<ProductionForm> ISet){
        Set<String> afterSpotNonTerminatorSet = new HashSet<>();
        int ISize;
        do{
            ISize = ISet.size();
            for(ProductionForm  p:ISet){
                ArrayList<String> pRightList = p.getRightList();
                int locationOfSpot = pRightList.indexOf(SPOT);
                if(locationOfSpot +1 <pRightList.size() && noneTerminatorSet.contains(pRightList.get(locationOfSpot+1))){
                    afterSpotNonTerminatorSet.add(pRightList.get(locationOfSpot+1));
                }
            }

            for(ProductionForm p:productionFormSet){
                if(afterSpotNonTerminatorSet.contains(p.getLeft())){
                    ISet.add(p.insertSpot());
                }
            }
        }while (ISize !=ISet.size());

    }
    private void calculationAllCLOSURE() {
        Queue<Set<ProductionForm>> unfinishedQueue = new LinkedList<>();
        unfinishedQueue.add(INodeList.get(0));
        Set<ProductionForm> nextINodeProductionFormSet;
        Map<String,Set<ProductionForm>> currentINodeEdgeMap;
        while (!unfinishedQueue.isEmpty()){
            Set<ProductionForm>  currentNodeProductionFormSet= unfinishedQueue.poll();
            currentINodeEdgeMap =  new HashMap<>();
            for(ProductionForm p:currentNodeProductionFormSet){
                ArrayList<String> rightArrayList = p.getRightList();
                int locationOfSpot = rightArrayList.indexOf(SPOT);
                if(locationOfSpot + 1< rightArrayList.size()){
                    String afterSpotToken = rightArrayList.get(locationOfSpot + 1);
                    nextINodeProductionFormSet = currentINodeEdgeMap.get(afterSpotToken);
                    if(nextINodeProductionFormSet == null){
                        nextINodeProductionFormSet = new HashSet<>();
                        nextINodeProductionFormSet.add(p.rightMoveSpotOneStep());
                        currentINodeEdgeMap.put(afterSpotToken,nextINodeProductionFormSet);
                    }else {
                        nextINodeProductionFormSet.add(p.rightMoveSpotOneStep());
                    }
                }
            }

            // 求闭包
            int i = INodeList.indexOf(currentNodeProductionFormSet);
            for(String currentINodeEdgeMapKey:currentINodeEdgeMap.keySet()){
                nextINodeProductionFormSet = currentINodeEdgeMap.get(currentINodeEdgeMapKey);
                calculationCLOSURE(nextINodeProductionFormSet);

                int j = INodeList.indexOf(nextINodeProductionFormSet);
                if(j == -1){
                    unfinishedQueue.add(nextINodeProductionFormSet);
                    INodeList.add(nextINodeProductionFormSet);
                    j = INodeList.size() - 1;
                }
                IEdgeList.add(new StateNode(i,currentINodeEdgeMapKey,j));
            }
            currentINodeEdgeMap.clear();

        }
    }

    private void getInputFromFile(String path) {
        File file = new File(path);
        BufferedReader reader = null;
        Boolean flag = true;
        try {
            reader = new BufferedReader(new FileReader(file));
            String lineString = null;
            // 一次读入一行，直到读入null为文件结束
            int i = 0;
            while ((lineString = reader.readLine()) != null) {
                if(i==0){
                    String[] noneTerminatorStrArray = lineString.split(" ");
                    terminatorSet.addAll(Arrays.asList(noneTerminatorStrArray));
                    terminatorSet.add(SPOT);
                    terminatorSet.add(NONE);
                    terminatorSet.add(END);
                }else if(i==1) {
                    String[] terminatorStrArray = lineString.split(" ");
                    noneTerminatorSet.addAll(Arrays.asList(terminatorStrArray));
                }else if(i>=2){
                    String trimStr = lineString.replace(" ","");
                    // 例子  A->aA|bB  mm存的是 A  和 aA|bB|cC
                    // nn存的是 aA 和 bB 和 cC
                    String[] mm = trimStr.split("=>");
                    if(i ==2){
                        mergeTerAndNoneVector.addAll(noneTerminatorSet);
                        mergeTerAndNoneVector.addAll(terminatorSet);
                        mergeTerAndNoneVector.sort((o1, o2) -> {
                            if (o1.length() < o2.length()){
                                return 1;
                            }else {
                                return o1.compareTo(o2);
                            }
                        }); // 排序 倒序排序  字符数多的在前边 因为 贪婪匹配
                        startProductionForm = new ProductionForm(mm[0],mm[1],splitByTerminatorAndNoneTer(mm[1]));
                    }

                    if(!flag){
                        tobeResolvedInput = trimStr;
                        break;
                    }

                    if(!trimStr.equals("end") && !trimStr.contains("|")){

                        productionFormSet.add(new ProductionForm(mm[0],mm[1],splitByTerminatorAndNoneTer(mm[1])));
                    }else if(!trimStr.equals("end") &&trimStr.contains("|")) {
                        String[] nn = mm[1].split("\\|");
                        for(String ele:nn){
                            productionFormSet.add(new ProductionForm(mm[0],ele,splitByTerminatorAndNoneTer(ele)));
                        }
                    }else if(trimStr.equals("end")){
                        flag = false;
                    }
                }
                i++;
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e1) {
                }
            }
        }
    }


    public LR0(String path) {
        terminatorSet = new HashSet<>();
        noneTerminatorSet = new HashSet<>();
        productionFormSet = new HashSet<>();
        mergeTerAndNoneVector = new Vector<>();

        INodeList = new ArrayList<Set<ProductionForm>>(); // 节点的集合 里边的一个Set<ProductionForm> 的所有产生式都是同状态的
        IEdgeList = new ArrayList<StateNode>();  //  状态到状态转变 这个list维护了一个节点到另一个节点边的信息

        getInputFromFile(path);

        Set<ProductionForm> startStateSet = new HashSet<>(); // 开始集的set 里边
        startStateSet.add(new ProductionForm(startProductionForm.getLeft(),SPOT+startProductionForm.getRight(),splitByTerminatorAndNoneTer(SPOT+startProductionForm.getRight())));
        calculationCLOSURE(startStateSet);
        // 第一个节点是 计算了起始产生式的闭包后  得到的set  内容是 startStateSet
        INodeList.add(startStateSet);
        calculationAllCLOSURE();
        int a = 0;
    }



    public void printDetail(){
        System.out.print("所有的终结符有： ");
        for(String ele:terminatorSet){
            System.out.print(ele + " ");
        }

        System.out.print("\n所有的非终结符有：");
        for(String ele:noneTerminatorSet){
            System.out.print(ele + " ");
        }
        System.out.println("\n总共有" + productionFormSet.size()+"个产生式" );
        System.out.println("起始非终结符是：" + startProductionForm.getLeft());
        int i = 0;
        for(ProductionForm p : productionFormSet){
            System.out.print(++i +". ");
            System.out.println(p.getLeft() + "=>"+ p.getRightList());
        }
        System.out.println("要解析的串是" + tobeResolvedInput);
    }
}
