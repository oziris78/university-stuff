package oguzhantopaloglu.data;


import java.util.Arrays;
import java.util.Random;

public class QuestionSet {

    private final String answeredBy;
    private final int min, max, N;

    private final int[] leftValues, rightValues;

    private final boolean[] correctness;
    private long scoreMilliseconds;

    public QuestionSet(String answeredBy, int minimumInclusive, int maximumInclusive, int questionCount) {
        this.answeredBy = answeredBy;
        this.min = minimumInclusive;
        this.max = maximumInclusive;
        this.N = questionCount;

        Random random = new Random();

        leftValues = new int[questionCount];
        for (int i = 0; i < questionCount; i++)
            leftValues[i] = min + random.nextInt(max - min + 1);

        rightValues = new int[questionCount];
        for (int i = 0; i < questionCount; i++)
            rightValues[i] = min + random.nextInt(max - min + 1);

        scoreMilliseconds = 0;

        correctness = new boolean[questionCount];
        for (int i = 0; i < questionCount; i++)
            correctness[i] = false;
    }

    public void submitAnswer(int questionIndex, int enteredResult) {
        correctness[questionIndex] = (enteredResult == leftValues[questionIndex] * rightValues[questionIndex]);
    }

    public void finish(long timeTaken) {
        this.scoreMilliseconds = timeTaken;
    }


    @Override
    public String toString() {
        return "QuestionSet{" +
                "answeredBy='" + answeredBy + '\'' +
                ", min=" + min +
                ", max=" + max +
                ", N=" + N +
                ", leftValues=" + Arrays.toString(leftValues) +
                ", rightValues=" + Arrays.toString(rightValues) +
                ", correctness=" + Arrays.toString(correctness) +
                ", scoreMilliseconds=" + scoreMilliseconds +
                '}';
    }

    public boolean[] getCorrectness() {
        return correctness;
    }

    public int getMax() {
        return max;
    }

    public int getMin() {
        return min;
    }

    public int getN() {
        return N;
    }

    public int[] getLeftValues() {
        return leftValues;
    }

    public int[] getRightValues() {
        return rightValues;
    }

    public long getScoreMilliseconds() {
        return scoreMilliseconds;
    }

    public String getAnsweredBy() {
        return answeredBy;
    }
}
