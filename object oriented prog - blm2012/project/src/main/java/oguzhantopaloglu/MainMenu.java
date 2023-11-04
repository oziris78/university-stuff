package oguzhantopaloglu;

import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.StageStyle;
import oguzhantopaloglu.data.DataManager;
import oguzhantopaloglu.data.QuestionSet;
import oguzhantopaloglu.data.QuestionSetOption;

import java.util.*;

import static oguzhantopaloglu.Utils.*;


public class MainMenu {

    protected VBox root;
    private final MainScene mainScene;
    private boolean adult;


    public MainMenu(MainScene mainScene, boolean adult) {
        this.mainScene = mainScene;
        this.adult = adult;

        fillMenu(null);
    }


    private void handleDefineQuestions(ActionEvent e) {
        TextField minTf = tf("Min: ", 200, 50, 20);
        TextField maxTf = tf("Max: ", 200, 50, 20);
        TextField countTf = tf("Count: ", 200, 50, 20);

        Button defineBtn = btn("Define Question", 200, 50, 20);
        defineBtn.setOnAction((e2) -> {
            try {
                int min = Integer.parseInt(minTf.getText());
                int max = Integer.parseInt(maxTf.getText());
                int N = Integer.parseInt(countTf.getText());

                if(max < min || N <= 0)
                    throw new RuntimeException();

                DataManager dataManager = mainScene.getDataManager();
                dataManager.addQuestionSetOption(new QuestionSetOption(min, max, N));
                dataManager.save();
                defineBtn.setText("Define successful!");
                defineBtn.setTextFill(Color.GREEN);
            }
            catch (Exception ignored) {
                defineBtn.setText("Define FAILED!");
                defineBtn.setTextFill(Color.RED);
            }
            minTf.setText("");
            maxTf.setText("");
            countTf.setText("");

            Timer myTimer = new Timer();
            myTimer.schedule(new TimerTask(){
                @Override
                public void run() {
                    Platform.runLater(() -> {
                        defineBtn.setText("Define Question");
                        defineBtn.setTextFill(Color.BLACK);
                    });
                }
            }, 2000);

        });

        Button goBack = btn("Go Back", 200, 50, 20);
        goBack.setOnAction(this::fillMenu);

        HBox row1 = new HBox(reg(), minTf, reg());
        HBox row2 = new HBox(reg(), maxTf, reg());
        HBox row3 = new HBox(reg(), countTf, reg());
        HBox row4 = new HBox(reg(), defineBtn, reg());
        HBox row5 = new HBox(reg(), goBack, reg());
        mainScene.fillRoot(reg(), row1, row2, row3, row4, row5, reg());
    }




    private void handleCheckScores(ActionEvent e) {
        ArrayList<Node> nodes = new ArrayList<>();
        nodes.add(reg());

        Label datasLb = lb("SCORES", 300, 75, 30);
        datasLb.setAlignment(Pos.CENTER);
        nodes.add(new HBox(reg(), datasLb, reg()));


        Button goBack = btn("GO BACK", 300, 75, 25);
        goBack.setOnAction(this::fillMenu);
        nodes.add(new HBox(reg(), goBack, reg()));

        DataManager dataManager = mainScene.getDataManager();
        ArrayList<QuestionSet> questionSets = dataManager.getPreviousPlays();

        HashMap<String, ArrayList<Double>> childrenAverageTime = new HashMap<>();
        HashMap<String, ArrayList<Double>> childrenRatio = new HashMap<>();

        for(QuestionSet q : questionSets){
            boolean[] correctness = q.getCorrectness();
            double time = q.getScoreMilliseconds();
            double total = q.getN();
            int correct = 0;
            for(boolean b : correctness)
                if(b) correct++;

            String name = q.getAnsweredBy();
            double averageTime = time / total;
            double ratio = correct / total;

            if(!childrenAverageTime.containsKey(name)) {
                ArrayList<Double> list = new ArrayList<>();
                list.add(averageTime);
                childrenAverageTime.put(name, list);
            }
            else {
                childrenAverageTime.get(name).add(averageTime);
            }

            if(!childrenRatio.containsKey(name)) {
                ArrayList<Double> list = new ArrayList<>();
                list.add(ratio);
                childrenRatio.put(name, list);
            }
            else {
                childrenRatio.get(name).add(ratio);
            }
        }

        ScrollPane scrollPane = new ScrollPane();
        VBox scContent = new VBox();
        scContent.getChildren().add(reg());
        for(String child : childrenAverageTime.keySet()) {
            ArrayList<Double> averageTimes = childrenAverageTime.get(child);
            ArrayList<Double> ratios = childrenRatio.get(child);

            // Time Chart
            CategoryAxis xAxis = new CategoryAxis();
            xAxis.setLabel("Plays");

            NumberAxis yAxis = new NumberAxis();
            yAxis.setLabel("Average Time");

            BarChart<String,Number> timeChart = new BarChart<>(xAxis, yAxis);
            timeChart.setTitle(String.format("%s's Average Time", child));

            XYChart.Series series = new XYChart.Series();
            int i = 0;
            for(double time : averageTimes)
                series.getData().add(new XYChart.Data(String.valueOf(i++), time));

            timeChart.getData().addAll(series);


            // Ratio Chart
            CategoryAxis xAxis2 = new CategoryAxis();
            xAxis2.setLabel("Plays");

            NumberAxis yAxis2 = new NumberAxis();
            yAxis2.setLabel("Correctness");

            BarChart<String,Number> ratioChart = new BarChart<>(xAxis2, yAxis2);
            ratioChart.setTitle(String.format("%s's Correctness Ratio", child));

            XYChart.Series series2 = new XYChart.Series();
            i = 0;
            for(double r : ratios)
                series2.getData().add(new XYChart.Data(String.valueOf(i++), r));

            ratioChart.getData().addAll(series2);

            scContent.getChildren().add(new HBox(reg(), timeChart, ratioChart, reg()));
        }
        scContent.getChildren().add(reg());
        scrollPane.setContent(scContent);
        nodes.add(new HBox(reg(), scrollPane, reg()));
        nodes.add(reg());

        mainScene.fillRoot(nodes);
    }


    private void handlePlay(ActionEvent e) {
        TextField usernameTf = tf("Username: ", 200, 50, 20);

        ListView<String> definedGames = new ListView<>();
        definedGames.setPrefWidth(500);
        DataManager dataManager = mainScene.getDataManager();
        ArrayList<QuestionSetOption> questionSetOptions = dataManager.getQuestionSetOptions();
        if(questionSetOptions.size() > 0) {
            for(QuestionSetOption option : questionSetOptions) {
                definedGames.getItems().add(
                        String.format("Min: %s, Max: %s, Count: %s", option.min, option.max, option.N)
                );
            }
        }
        else {
            definedGames.getItems().add("NO GAMES WERE DEFINED, PLEASE ASK AN ADULT TO DEFINE GAMES FOR YOU :)");
        }

        Button btnStart = btn("Start", 200, 50, 25);
        btnStart.setOnAction(event -> startGameScreen(definedGames, usernameTf));

        Button goBack = btn("Go Back", 200, 50, 20);
        goBack.setOnAction(this::fillMenu);


        mainScene.fillRoot(reg(),
                new HBox(reg(), usernameTf, reg()),
                new HBox(reg(), definedGames, reg()),
                new HBox(reg(), btnStart, reg()),
                new HBox(reg(), goBack, reg()),
                reg());
    }



    private void startGameScreen(ListView<String> definedGames, TextField usernameTf) {
        ObservableList<String> items = definedGames.getSelectionModel().getSelectedItems();
        if(items == null)
            return;
        if(items.size() <= 0)
            return;

        String selected = items.get(0);

        if(!selected.startsWith("Min: "))
            return;

        selected = selected.replace("Min: ", "");
        selected = selected.replace(" Max: ", "");
        selected = selected.replace(" Count: ", "");
        String[] numbersAsStr = selected.split(",");
        int min = Integer.parseInt(numbersAsStr[0]);
        int max = Integer.parseInt(numbersAsStr[1]);
        int N = Integer.parseInt(numbersAsStr[2]);

        String username = usernameTf.getText();
        if(username.strip() == "")
            username = "Anonymous";

        QuestionSet questionSet = new QuestionSet(username, min, max, N);
        final int[] questionIndex = {0};
        final long[] millisTaken = { 0 };

        Label timerLb = lb("", 200, 50, 20);
        timerLb.setAlignment(Pos.CENTER);
        TextField inputTf = tf("Answer: ", 200, 50, 20);

        String text = String.format("%d * %d = ?", questionSet.getLeftValues()[questionIndex[0]],
                questionSet.getRightValues()[questionIndex[0]]);
        Label questionLb = lb(text, 200, 50, 20);
        questionLb.setAlignment(Pos.CENTER);

        Timer timeTimer = new Timer();
        timeTimer.schedule(new TimerTask(){
            @Override
            public void run() {
                millisTaken[0] += 5;
                Platform.runLater(() -> {
                    timerLb.setText(formatTime(millisTaken[0]));
                });
            }
        }, 100, 5);

        Button btnNext = btn("NEXT", 200, 50, 20);
        btnNext.setOnAction(e2 -> {
            int input = -1;
            try {
                input = Integer.parseInt(inputTf.getText());
            }
            catch (Exception ignored) {}

            questionSet.submitAnswer(questionIndex[0], input);
            questionIndex[0]++;

            if(questionIndex[0] == N) {
                timeTimer.cancel();
                long time = millisTaken[0];
                questionSet.finish(time);

                DataManager dataManager = mainScene.getDataManager();
                dataManager.addPreviousPlay(questionSet);
                dataManager.save();

                fillMenu(null);

                Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
                alert.initStyle(StageStyle.UTILITY);
                alert.setHeaderText("Congratulations!");
                alert.setTitle("You finished the game!");
                alert.setContentText(String.format("Time taken: %d", time));

                alert.showAndWait();
                return;
            }

            String newText = String.format("%d * %d = ?", questionSet.getLeftValues()[questionIndex[0]],
                    questionSet.getRightValues()[questionIndex[0]]);
            questionLb.setText(newText);
        });

        mainScene.fillRoot(reg(),
                new HBox(reg(), timerLb, reg()),
                new HBox(reg(), questionLb, reg()),
                new HBox(reg(), inputTf, reg()),
                new HBox(reg(), btnNext, reg()),
                reg());
    }



    private void fillMenu(ActionEvent unused) {
        Button btn11 = btn("Define Questions", 300, 75, 25);
        btn11.setOnAction(this::handleDefineQuestions);

        Button btn12 = btn("Play", 300, 75, 25);
        btn12.setOnAction(this::handlePlay);

        Button btn2 = btn("Check Scores", 300, 75, 25);
        btn2.setOnAction(this::handleCheckScores);

        Button btn3 = btn("Go Back", 300, 75, 25);
        btn3.setOnAction((e) -> mainScene.fillRoot());

        HBox row11 = new HBox(reg(), btn11, reg());
        HBox row12 = new HBox(reg(), btn12, reg());
        HBox row2 = new HBox(reg(), btn2, reg());
        HBox row3 = new HBox(reg(), btn3, reg());

        if(adult) { // Switch to a menu with btn11
            mainScene.fillRoot(reg(), row11, row2, row3, reg());
        }
        else { // Switch to a menu with btn12
            mainScene.fillRoot(reg(), row12, row2, row3, reg());
        }
    }



}
