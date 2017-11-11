package dlsu;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by aenon on 11/11/2017.
 */
public class setUpHomeController implements Initializable {

    @FXML
    private Button onNext;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        onNext.setOnMouseEntered(event -> onNext.setCursor(Cursor.HAND));
        onNext.setOnMousePressed(event -> onNext.setCursor(Cursor.CLOSED_HAND));
        onNext.setOnMouseReleased(event -> onNext.setCursor(Cursor.HAND));
        onNext.setOnMouseExited(event -> onNext.setCursor(Cursor.DEFAULT));
    }

    @FXML
    private void onNext(ActionEvent actionEvent) throws IOException {
        dlsu.changeScene changeScene = new changeScene();
        changeScene.setScene("Setup1.fxml", "style.css", actionEvent);
    }

}
