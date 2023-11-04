package oguzhantopaloglu.data;


import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;


public class DataManager {

    private final Gson gson;
    private final Path path;
    private Data data;


    public DataManager() {
        this.gson = new GsonBuilder()
                .serializeNulls()
                .setPrettyPrinting()
                .create();
        this.path = Path.of("c19052025.json");

        // Read this.data from file
        try {
            if(Files.exists(this.path)) {
                String jsonStr = Files.readString(this.path);
                this.data = gson.fromJson(jsonStr, Data.class);
            }
            else {
                this.data = new Data();
            }
        }
        catch (IOException e) {
            throw new RuntimeException(e);
        }

    }



    public void addQuestionSetOption(QuestionSetOption questionSetOption) {
        this.data.addQuestionSetOption(questionSetOption);
    }

    public void addPreviousPlay(QuestionSet questionSet) {
        this.data.addPreviousPlay(questionSet);
    }


    public ArrayList<QuestionSetOption> getQuestionSetOptions() {
        return data.getQuestionSetOptions();
    }


    public ArrayList<QuestionSet> getPreviousPlays() {
        return data.getPreviousPlays();
    }


    /*
     * Used to save the changes to the json file so that they can be read once the app restarts.
     */
    public void save() {
        try {
            String jsonStr = gson.toJson(this.data);
            if(Files.notExists(this.path)) {
                Files.createFile(this.path);
            }

            Files.writeString(this.path, jsonStr, StandardOpenOption.WRITE);

        }
        catch (IOException e) {
//            System.out.println("An IOException has happened!");
            throw new RuntimeException(e);
        }
    }



}
