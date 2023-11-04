package oguzhantopaloglu;


import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.*;
import oguzhantopaloglu.data.DataManager;

import java.util.ArrayList;

import static oguzhantopaloglu.Utils.*;


public class MainScene {

    private final Scene scene;
    private VBox root;
    private DataManager dataManager;

    public MainScene() {
        this.dataManager = new DataManager();

        root = new VBox();
        root.setSpacing(50);
        fillRoot();

        this.scene = new Scene(root);
    }

    public void fillRoot(Node... nodes) {
        this.root.getChildren().setAll(nodes);
    }

    public void fillRoot(ArrayList<Node> nodes) {
        this.root.getChildren().setAll(nodes);
    }

    public void fillRoot() {
        Button adultButton = btn("Enter as an adult", 400, 100, 35);
        adultButton.setOnAction((e) -> new MainMenu(this, true));

        Button childButton = btn("Enter as a child", 400, 100, 35);
        childButton.setOnAction((e) -> new MainMenu(this, false));

        fillRoot(reg(),
                new HBox(reg(), adultButton, reg()),
                new HBox(reg(), childButton, reg()),
                reg());
    }

    public DataManager getDataManager() {
        return dataManager;
    }

    public Scene getScene() {
        return scene;
    }

}
