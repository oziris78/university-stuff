package oguzhantopaloglu.data;



public class QuestionSetOption {

    public final int min, max, N;

    public QuestionSetOption(int minimumInclusive, int maximumInclusive, int questionCount) {
        this.min = minimumInclusive;
        this.max = maximumInclusive;
        this.N = questionCount;
    }

    @Override
    public String toString() {
        return "QuestionSetOption{" +
                "min=" + min +
                ", max=" + max +
                ", N=" + N +
                '}';
    }

}


