package oguzhantopaloglu.data;


import java.util.ArrayList;

class Data {

    private ArrayList<QuestionSetOption> questionSetOptions;
    private ArrayList<QuestionSet> previousPlays;

    Data() {
        this.questionSetOptions = new ArrayList<>();
        this.previousPlays = new ArrayList<>();
    }

    public void addQuestionSetOption(QuestionSetOption questionSetOption) {
        this.questionSetOptions.add(questionSetOption);
    }

    public void addPreviousPlay(QuestionSet questionSet) {
        this.previousPlays.add(questionSet);
    }

    @Override
    public String toString() {
        return String.format("DEFINED QUESTIONS:\n%s\n\nDEFINED PLAYS:\n%s\n\n",
                questionSetOptions.toString(), previousPlays.toString());
    }

    public ArrayList<QuestionSetOption> getQuestionSetOptions() {
        return questionSetOptions;
    }

    public ArrayList<QuestionSet> getPreviousPlays() {
        return previousPlays;
    }

}
