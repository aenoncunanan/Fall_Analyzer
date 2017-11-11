package dlsu;

import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import javafx.event.ActionEvent;

import java.io.IOException;

/**
 * Created by aenon on 11/11/2017.
 */

public class changeScene {

    public void setScene(String fxmlFile, String cssFile, ActionEvent actionEvent) throws IOException {
        Parent parent = FXMLLoader.load(getClass().getResource(fxmlFile));
        parent.getStylesheets().addAll(this.getClass().getResource(cssFile).toExternalForm());
        Scene scene = new Scene(parent);
        Stage stage = (Stage) ((Node) actionEvent.getSource()).getScene().getWindow();
        stage.setScene(scene);
        stage.show();
    }
}
