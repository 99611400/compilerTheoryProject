
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class LR0 {

    private static final String SPOT = "･";
    private static final String NONE = "@";
    private static final String END = "$";


    private int acceptNodeNumber;
    private String tobeResolvedInput;  // 存储要解析的字符串
    private ProductionForm startProductionForm;
    private final Set<ProductionForm> reduceProductionFormSet;
    ArrayList<String> lr0TableHeadList;
    String[][] lr0Table;
    ArrayList<ProductionForm> productionFormList;
    ArrayList<ParseStatus> parseStatusArrayList;


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
                // 如果当前没越界 且点后边的token是非终结符 那么 需要 将这个字符加入到afterSpotNonTerminatorSet中
                if(locationOfSpot +1 <pRightList.size() && noneTerminatorSet.contains(pRightList.get(locationOfSpot+1))){
                    afterSpotNonTerminatorSet.add(pRightList.get(locationOfSpot+1));
                }
            }
            // 如果存在 afterSpotNonTerminatorSet 中的元素是 某一产生式的左侧 那么 这个产生式需要加入到CLOSURE闭包中
            // 比如  求 E->･S 的闭包   而且还知道  S->BB   上边的for循环找到了点后边的非终结符 S 那么S ->･BB 也应该加入到闭包中 以此类推 B->･xxxxxx也应该添加
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
                        // 这种情况是可能存在的 比如 当前currentNodeProductionFormSet 是 S->･AA S->･AB
                        nextINodeProductionFormSet.add(p.rightMoveSpotOneStep());
                    }
                }else if(locationOfSpot + 1 == rightArrayList.size()) {
                    //这说明 到达了临终节点  比如 B->BB･ A->a･  等等
                    reduceProductionFormSet.add(p);
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
                    if(nextINodeProductionFormSet.size() ==1 &&
                            nextINodeProductionFormSet.contains(startProductionForm.insertSpot().rightMoveSpotOneStep())){
                        acceptNodeNumber = INodeList.size() -1;
                    }
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
                        mergeTerAndNoneVector.add(SPOT);
                        mergeTerAndNoneVector.add(NONE);
                        mergeTerAndNoneVector.add(END);
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
        reduceProductionFormSet = new HashSet<>();
        INodeList = new ArrayList<Set<ProductionForm>>(); // 节点的集合 里边的一个Set<ProductionForm> 的所有产生式都是同状态的
        IEdgeList = new ArrayList<StateNode>();  //  状态到状态转变 这个list维护了一个节点到另一个节点边的信息
        getInputFromFile(path);
        Set<ProductionForm> startStateSet = new HashSet<>(); // 开始集的set 里边
        startStateSet.add(new ProductionForm(startProductionForm.getLeft(),SPOT+startProductionForm.getRight(),splitByTerminatorAndNoneTer(SPOT+startProductionForm.getRight())));
        calculationCLOSURE(startStateSet);
        // 第一个节点是 计算了起始产生式的闭包后  得到的set  内容是 startStateSet
        INodeList.add(startStateSet);
        // calculationAllCLOSURE 会根据INodeList中的内容计算所有的闭包  直至所有的状态被求出来 而且 IEdgeList还会存储不同状态集之间的关系
        calculationAllCLOSURE();
        createLR0Table();
        parseToken();
    }

    private void parseToken() {
         parseStatusArrayList = new ArrayList<>();
        /*
        0
        #     baaaaaab$
        */


        LinkedList<Integer> startStatusStack = new LinkedList<>();
        startStatusStack.add(0);
        LinkedList<String> symbolStack = new LinkedList<>();
        symbolStack.add("$");
        LinkedList<String> leftTokenStack = new LinkedList<>();
        leftTokenStack.addAll(splitByTerminatorAndNoneTer(tobeResolvedInput));
        ParseStatus singleParseStatus = new ParseStatus(startStatusStack,symbolStack,leftTokenStack);
        while (singleParseStatus != null){
            parseStatusArrayList.add(singleParseStatus);
            singleParseStatus = getSingleParseStatus();
        }


    }

    private ParseStatus getSingleParseStatus() {
        ParseStatus currentParseStatus = parseStatusArrayList.get(parseStatusArrayList.size() - 1);
        String nextInstruct = "";
        String next = "";
        Integer jumpNumber =-1;

        LinkedList<Integer> startStatusStack = null;
        LinkedList<String> symbolStack = null;
        LinkedList<String> leftTokenStack = null;

        startStatusStack = new LinkedList<>();
        startStatusStack.addAll(currentParseStatus.getStatusStack());
        symbolStack = new LinkedList<>();
        symbolStack.addAll(currentParseStatus.getSymbolStack());
        leftTokenStack = new LinkedList<>();
        leftTokenStack.addAll(currentParseStatus.getLeftTokenStack());
        if(currentParseStatus.getStatusStack().size() == currentParseStatus.getSymbolStack().size()){
            nextInstruct = lr0Table[currentParseStatus.getStatusStack().getLast()][lr0TableHeadList.indexOf(currentParseStatus.getLeftTokenStack().getFirst())];
            if(nextInstruct == "acc"){
                return null;
            }
            if(nextInstruct == null)
                return null;

            next = nextInstruct.substring(0,1);
            jumpNumber = Integer.parseInt(nextInstruct.substring(1,2));
            if(next.equals("s")){
                startStatusStack.add(jumpNumber);
                symbolStack.add(leftTokenStack.getFirst());
                leftTokenStack.remove(0);
            }else if(next.equals("r")){
                ProductionForm currentProductionForm = productionFormList.get(jumpNumber);
                for(String ele:currentProductionForm.getRightList()){
                    startStatusStack.remove(startStatusStack.size()-1);
                    symbolStack.remove(symbolStack.size()-1);
                }
                symbolStack.add(currentProductionForm.getLeft());
            }
            return new ParseStatus(startStatusStack,symbolStack,leftTokenStack);
        }else {
            nextInstruct = lr0Table[currentParseStatus.getStatusStack().getLast()][lr0TableHeadList.indexOf(currentParseStatus.getSymbolStack().getLast())];
            jumpNumber = Integer.parseInt(nextInstruct);
            startStatusStack.add(jumpNumber);
            return new ParseStatus(startStatusStack,symbolStack,leftTokenStack);
        }
    }

    private Boolean createLR0Table() {
        lr0TableHeadList = new ArrayList<>();
        productionFormList = new ArrayList<>();
        productionFormList.addAll(productionFormSet);
        lr0TableHeadList.addAll(terminatorSet);
        lr0TableHeadList.add("$");
        lr0TableHeadList.addAll(noneTerminatorSet);
        lr0TableHeadList.remove(startProductionForm.getLeft());

        // 创建goto表
        int[][] gotoTable = new int[INodeList.size()][lr0TableHeadList.size()];
        lr0Table = new String[INodeList.size()][lr0TableHeadList.size()];
        for(int i = 0 ;i<IEdgeList.size();i++){
            StateNode currentEdge = IEdgeList.get(i);
            int locationLr0TableHead= lr0TableHeadList.indexOf(currentEdge.getPassBy());
            int currentNodeNumber = currentEdge.getOriginalState();
            if(gotoTable[currentNodeNumber][locationLr0TableHead] == 0){
                gotoTable[currentNodeNumber][locationLr0TableHead] = currentEdge.getTarget();
            }else {
                return false;
            }
        }


        //填写action表
        for(int i =0;i<INodeList.size();i++ ){
            Set<ProductionForm> currentNode = INodeList.get(i);
            if(i == acceptNodeNumber){
                lr0Table[i][terminatorSet.size()] = "acc";
            }
            for(int j=0;j<lr0TableHeadList.size();j++){
                if(gotoTable[i][j] != 0 ) {
                    if(j < terminatorSet.size() + 1){
                        lr0Table[i][j] = "s" + gotoTable[i][j];
                    }else if(j >= terminatorSet.size() + 1){
                        lr0Table[i][j] =gotoTable[i][j] + "";
                    }
                    continue;
                }

                if(currentNode.size() == 1 && i!=acceptNodeNumber){
                    ProductionForm aa = currentNode.iterator().next();
                    if (j < terminatorSet.size() + 1  ){
                        int locationInProductionFormList = productionFormList.indexOf(aa.removeSpot());
                        lr0Table[i][j] = "r"  + locationInProductionFormList;
                    }
                }

            }
        }


        return true;
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
        for(ProductionForm p : productionFormList){
            System.out.print(++i +". ");
            System.out.println(p.getLeft() + "=>"+ p.getRightList());
        }
        System.out.println("Lr0分析表是");
        System.out.format("%-10s","");
        for(String str:lr0TableHeadList)
            System.out.format("%-10s",str);
        System.out.println();
        for(int p=0;p<INodeList.size();p++){
            System.out.format("%-10s","I" +p);
            for(int q=0;q<lr0TableHeadList.size();q++){
                if(lr0Table[p][q] != null)
                    System.out.format("%-10s",lr0Table[p][q]);
                else
                    System.out.format("%-10s","");
            }

            System.out.println("\t"+INodeList.get(p));
        }

        System.out.println("要解析的串是" + tobeResolvedInput);

        Boolean flag = true;
        ParseStatus lastStatus = parseStatusArrayList.get(parseStatusArrayList.size() - 1);
        String temp = lr0Table[lastStatus.getStatusStack().getLast()][lr0TableHeadList.indexOf(lastStatus.getLeftTokenStack().getFirst())];

        if(temp !=null && temp.equals("acc")  ){
            System.out.println("解析成功");
        }else {
            System.out.println("解析失败,该字符串不是文法的语言");
            flag = false;
        }
        if(flag){
            int j = 0;
            for(ParseStatus p : parseStatusArrayList){
                System.out.println("状态" + ++j +":");
                System.out.println(p.getStatusStack() );
                System.out.println(p.getSymbolStack()+"\t"+ p.getLeftTokenStack());
            }
            System.out.println("解析成功");
        }






    }
}
