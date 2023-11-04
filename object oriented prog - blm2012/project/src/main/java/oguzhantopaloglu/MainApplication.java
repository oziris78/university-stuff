package oguzhantopaloglu;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;
import oguzhantopaloglu.MainScene;


public class MainApplication extends Application {

    @Override
    public void start(Stage stage) {
        stage.setScene(new MainScene().getScene());
        stage.setWidth(600);
        stage.setHeight(600);
        stage.setTitle("OOP Project, Oguzhan Topaloglu, C19052025");
        stage.show();
    }

    @Override
    public void stop() {
        Platform.exit();
        System.exit(0);
    }

}
