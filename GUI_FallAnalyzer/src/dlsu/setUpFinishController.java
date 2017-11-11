package dlsu;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.control.Button;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by aenon on 11/11/2017.
 */
public class setUpFinishController implements Initializable {

    @FXML
    private Button onFinish;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        onFinish.setOnMouseEntered(event -> onFinish.setCursor(Cursor.HAND));
        onFinish.setOnMousePressed(event -> onFinish.setCursor(Cursor.CLOSED_HAND));
        onFinish.setOnMouseReleased(event -> onFinish.setCursor(Cursor.HAND));
        onFinish.setOnMouseExited(event -> onFinish.setCursor(Cursor.DEFAULT));
    }

    @FXML
    private void onFinish(ActionEvent actionEvent) throws IOException {
        Stage stage = (Stage) onFinish.getScene().getWindow();
        stage.close();
    }

}
