import java.util.ArrayList;
import java.util.Objects;

public class ProductionForm {
    private static final String SPOT = "･";
    private static final String NONE = "@";
    private final String fullString;
    private final String left;
    private final String right;
    private ArrayList<String> rightList;


    public String getFullString() {
        return fullString;
    }

    public String getLeft() {
        return left;
    }

    public String getRight() {
        return right;
    }

    public ArrayList<String> getRightList() {
        return rightList;
    }





    public ProductionForm(String left,String right,ArrayList<String> rightList){
        this.left = left;
        this.right = right;
        this.fullString = left + "=>" + right;
        this.rightList = rightList;
    }

    ProductionForm insertSpot(){
        ProductionForm p;
        ArrayList<String> tempArraylist;
        if(right.equals(NONE)){
            tempArraylist = new ArrayList<>();
            tempArraylist.add(SPOT);
            p = new ProductionForm(left,SPOT,tempArraylist);
        }else {
            tempArraylist = (ArrayList<String>) getRightList().clone();
            tempArraylist.add(0,SPOT);
            p = new ProductionForm(left,SPOT+getRight(),tempArraylist);
        }
        return p;
    }

    ProductionForm rightMoveSpotOneStep(){
        String rightStr = this.right;
        int locationOfSpotInString = rightStr.indexOf(SPOT);
        StringBuffer temp = new StringBuffer(rightStr);
        temp.deleteCharAt(locationOfSpotInString);
        temp.insert(locationOfSpotInString+1,SPOT);

        ArrayList<String> cloneRightArrayList = (ArrayList<String>) getRightList().clone();
        int locationOfSpotInArraylist = cloneRightArrayList.indexOf(SPOT);
        cloneRightArrayList.remove(locationOfSpotInArraylist);
        cloneRightArrayList.add(locationOfSpotInArraylist+1,SPOT);
        ProductionForm p = new ProductionForm(this.getLeft(),temp.toString(),cloneRightArrayList);
        return p;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof ProductionForm)) return false;
        ProductionForm that = (ProductionForm) o;
        return Objects.equals(fullString, that.fullString) && Objects.equals(left, that.left) && Objects.equals(right, that.right);
    }

    @Override
    public int hashCode() {
        return Objects.hash(fullString, left, right);
    }

    @Override
    public String toString() {
        return fullString;
    }
}
