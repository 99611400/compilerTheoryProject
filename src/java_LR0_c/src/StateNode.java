import java.util.Objects;

public class StateNode {


    private final Integer originalState;
    private final String  passBy;
    private final Integer target;

    public StateNode(Integer originalState, String passBy, Integer target) {
        this.originalState = originalState;
        this.passBy = passBy;
        this.target = target;
    }

    public Integer getOriginalState() {
        return originalState;
    }

    public String  getPassBy() {
        return passBy;
    }

    public Integer getTarget() {
        return target;
    }

    @Override
    public String toString() {
        return  "I" + originalState +
                "---" + passBy +"I" + target;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof StateNode)) return false;
        StateNode stateNode = (StateNode) o;
        return Objects.equals(getOriginalState(), stateNode.getOriginalState()) && Objects.equals(getPassBy(), stateNode.getPassBy()) && Objects.equals(getTarget(), stateNode.getTarget());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getOriginalState(), getPassBy(), getTarget());
    }
}
