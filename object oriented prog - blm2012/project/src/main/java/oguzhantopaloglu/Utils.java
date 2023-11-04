package oguzhantopaloglu;

import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;


/**
 * Statically import this class to use the utility funcs.
 */
public final class Utils {

    private Utils() {}  // no object init

    public static String formatTime(long millis) {
        long hours = millis / (1000 * 60 * 60);
        millis -= hours * (1000 * 60 * 60);
        long minutes = millis / (1000 * 60);
        millis -= minutes * (1000 * 60);
        long seconds = millis / 1000;
        millis -= seconds * 1000;
        return String.format("%02d:%02d:%02d:%d", hours, minutes, seconds, millis);
    }


    public static Region reg() {
        Region reg = new Region();
        VBox.setVgrow(reg, Priority.ALWAYS);
        HBox.setHgrow(reg, Priority.ALWAYS);
        return reg;
    }

    public static TextField tf(String promptText, int w, int h, int fontSize) {
        TextField tf = new TextField();
        tf.setPromptText(promptText);
        tf.setPrefSize(w, h);
        tf.setFont(Font.font(fontSize));
        return tf;
    }

    public static Label lb(String title, int w, int h, int fontSize) {
        Label lb = new Label(title);
        lb.setPrefSize(w, h);
        lb.setFont(Font.font(fontSize));
        return lb;
    }

    public static Button btn(String title, int w, int h, int fontSize) {
        Button btn = new Button(title);
        btn.setPrefSize(w, h);
        btn.setFont(Font.font(fontSize));
        VBox.setVgrow(btn, Priority.NEVER);
        HBox.setHgrow(btn, Priority.NEVER);
        return btn;
    }

}
